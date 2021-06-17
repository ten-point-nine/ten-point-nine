/*-------------------------------------------------------
 * 
 * nonvol.ino
 * 
 * Nonvol storage managment
 * 
 * ----------------------------------------------------*/
#include "nonvol.h"
#include "freeTarget.h"
#include "json.h"
 
/*----------------------------------------------------------------
 * 
 * function: init_nonvol
 * 
 * brief: Initialize the NONVOL back to factory settings
 * 
 * return: None
 *---------------------------------------------------------------
 *
 * The variable NONVOL_INIT is corrupted and the NONVOL read back
 * in and initialized.
 * 
 *------------------------------------------------------------*/
void init_nonvol(int v)
{
  unsigned int nonvol_init;               // Initialization token
  unsigned int serial_number;             // Board serial number
  char ch;
  
  nonvol_init = 0;                        // Corrupt the init location
  serial_number = 0;
  EEPROM.put(NONVOL_INIT, nonvol_init);
  
  Serial.print("\r\nReset to factory defaults\r\n");
  
  gen_position(0); 
  EEPROM.put(NONVOL_DIP_SWITCH,  0);   // No, set up the defaults
  EEPROM.put(NONVOL_SENSOR_DIA,  230.0); 
  EEPROM.put(NONVOL_PAPER_TIME,  0);    // Duration of paper pulse (LSB = 10ms)
  EEPROM.put(NONVOL_PAPER_STEP,  1);    // Number of paper steps
  EEPROM.put(NONVOL_TEST_MODE,   0);
  EEPROM.put(NONVOL_CALIBRE_X10, 45);
  EEPROM.put(NONVOL_LED_PWM,     50);
  EEPROM.put(NONVOL_POWER_SAVE,  30);
  EEPROM.put(NONVOL_NAME_ID,      1);
  EEPROM.put(NONVOL_1_RINGx10, 1555);
  EEPROM.put(NONVOL_SEND_MISS,    0);
  EEPROM.put(NONVOL_SERIAL_NO,    0);
  EEPROM.put(NONVOL_DIP_SWITCH,   0);     // Read the nonvol settings
  EEPROM.put(NONVOL_SENSOR_DIA, 230.0);
  EEPROM.put(NONVOL_TEST_MODE,    0);
  
  EEPROM.get(NONVOL_PAPER_TIME, json_paper_time);
  EEPROM.put(NONVOL_PAPER_TIME,   0);   // and limit motor on time
  EEPROM.put(NONVOL_CALIBRE_X10, 45);   // Default to a 4.5mm pellet
  EEPROM.put(NONVOL_SENSOR_ANGLE,45);   // Default to a 45 degree offset
  EEPROM.put(NONVOL_NAME_ID,      0);   // Default to no name
  
  EEPROM.put(NONVOL_NORTH_X, 0);  
  EEPROM.put(NONVOL_NORTH_Y, 0);  
  EEPROM.put(NONVOL_EAST_X,  0);  
  EEPROM.put(NONVOL_EAST_Y,  0);  
  EEPROM.put(NONVOL_SOUTH_X, 0);  
  EEPROM.put(NONVOL_SOUTH_Y, 0);  
  EEPROM.put(NONVOL_WEST_X,  0);  
  EEPROM.put(NONVOL_WEST_Y,  0);  

  EEPROM.put(NONVOL_1_RINGx10,  json_1_ring_x10);

  EEPROM.put(NONVOL_POWER_SAVE, 0);
  EEPROM.put(NONVOL_LED_PWM,    0);
  EEPROM.put(NONVOL_SEND_MISS,  0);

/*
 * Ask for the serial number.  Exit when you get !
 */
  ch = 0;
  serial_number = 0;
  while ( Serial.available() )    // Eat any pending junk
  {
    Serial.read();
  }
  
  Serial.print("\r\nSerial Number? (number! or x)");
  while (i)
  {
    if ( Serial.available() != 0 )
    {
      ch = Serial.read();
      if ( ch == '!' )
      {
        Serial.print(" Confirm: "); Serial.print(serial_number);
        break;
      }
      if ( ch == 'x' )
      {
        return;
      }
      Serial.print(serial_number);
      serial_number *= 10;
      serial_number += ch - '0';
      Serial.print(serial_number);
    }
  }
  EEPROM.put(NONVOL_SERIAL_NO, serial_number);
  
  nonvol_init = INIT_DONE;
  EEPROM.put(NONVOL_INIT, INIT_DONE);
/*
 * Read the NONVOL and print the results
 */
  read_nonvol();                          // Force in new values
  show_echo(0);                           // Display these settings
  set_trip_point(0);                      // And stay forever in the trip mode
  
/*
 * All done, return
 */    

  return;
}

/*----------------------------------------------------------------
 * 
 * funciton: read_nonvol
 * 
 * brief: Read nonvol and set up variables
 * 
 * return: Nonvol values copied to RAM
 * 
 *---------------------------------------------------------------
 *
 * Read the nonvol into RAM.  
 * 
 * If the results is uninitalized then force the factory default.
 * Then check for out of bounds and reset those values
 *
 *------------------------------------------------------------*/
void read_nonvol(void)
{
  unsigned int nonvol_init;
  unsigned int  i;              // Iteration Counter
  unsigned int  x;              // 16 bit number
  double       dx;              // Floating point number
  
  if ( is_trace )
  {
    Serial.print("\r\nReading NONVOL");
  }
  
/*
 * Read the nonvol marker and if uninitialized then set up values
 */
  EEPROM.get(NONVOL_INIT, nonvol_init);
  
  if ( nonvol_init != INIT_DONE)                       // EEPROM never programmed
  {
    init_nonvol(0);                                    // Force in good values
  }

/*
 * Use the JSON table to initialize the local variables
 */
 i=0;
 while ( JSON[i].token != 0 )
 {
   if ( JSON[i].non_vol != 0 )                          // There is a value stored in memory
   {
     switch ( JSON[i].convert )
     {
        case IS_VOID:
          break;
        
        case IS_INT16:
          EEPROM.get(JSON[i].non_vol, x);                    // Read in the value
          *JSON[i].value = x;
          break;

        case IS_FLOAT:
        case IS_DOUBLE:
          EEPROM.get(JSON[i].non_vol, dx);                    // Read in the value
          *JSON[i].d_value = dx;
          break;
      }
   }
   i++;
 }

/*
 * Go through and verify that the special cases are taken care of
 */
  if ( (json_paper_time * PAPER_STEP) > (PAPER_LIMIT) )
  {
    json_paper_time = 0;                              // Check for an infinit loop
    EEPROM.put(NONVOL_PAPER_TIME, json_paper_time);   // and limit motor on time
  }
  
  if ( (json_paper_step == 0xffff) || (json_paper_step == 0) )
  {
    json_paper_step = 1;                              // Check for undefined time
    EEPROM.put(NONVOL_PAPER_STEP, json_paper_step);   // Set to 1
  }
  
  if ( json_calibre_x10 > 100 )
  {
    json_calibre_x10 = 45;                            // Check for an undefined pellet
    EEPROM.put(NONVOL_CALIBRE_X10, json_calibre_x10); // Default to a 4.5mm pellet
  }
  json_calibre_x10 = 0;                               // AMB
  
  if ( json_sensor_angle == 0xffff )
  {
    json_sensor_angle = 45;                             // Check for an undefined Angle
    EEPROM.put(NONVOL_SENSOR_ANGLE, json_sensor_angle);// Default to a 4.5mm pellet
  }

  if ( json_name_id == 0xffff )
  {
    json_name_id = 0;                                 // Check for an undefined Name
    EEPROM.put(NONVOL_NAME_ID, json_name_id);         // Set to zero
  }
  
  json_sensor_angle = 45;                            // AMB Kluge
  EEPROM.put(NONVOL_SENSOR_ANGLE, json_sensor_angle);

/*
 * All done, begin the program
 */
  return;
}

/*----------------------------------------------------------------
 *
 * function: gen_postion
 * 
 * brief: Generate new position varibles based on new sensor diameter
 * 
 * return: Position values stored in NONVOL
 * 
 *---------------------------------------------------------------
 *
 *  This function resets the offsets to 0 whenever a new 
 *  sensor diameter is entered.
 *  
 *------------------------------------------------------------*/
void gen_position(int v)
{
 /*
  * Work out the geometry of the sensors
  */
  json_north_x = 0;
  json_north_y = 0;
  
  json_east_x = 0;
  json_east_y = 0;

  json_south_x = 0;
  json_south_y = 0;
  
  json_west_x = 0;
  json_west_y = 0;

 /*
  * Save to persistent storage
  */
  EEPROM.put(NONVOL_NORTH_X, json_north_x);  
  EEPROM.put(NONVOL_NORTH_Y, json_north_y);  
  EEPROM.put(NONVOL_EAST_X,  json_east_x);  
  EEPROM.put(NONVOL_EAST_Y,  json_east_y);  
  EEPROM.put(NONVOL_SOUTH_X, json_south_x);  
  EEPROM.put(NONVOL_SOUTH_Y, json_south_y);  
  EEPROM.put(NONVOL_WEST_X,  json_west_x);  
  EEPROM.put(NONVOL_WEST_Y,  json_west_y);  
   
 /* 
  *  All done, return
  */
  return;
}
