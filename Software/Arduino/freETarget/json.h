/*
 * Global functions
 */
#ifndef _JSON_H_
#define _JSON_H_

typedef struct  {
  char*             token;    // JSON token string, ex "RADIUS": 
  int*              value;    // Where value is stored 
  double*         d_value;    // Where value is stored 
  unsigned int    convert;    // Conversion type
  void         (*f)(int x);   // Function to execute with message
  unsigned int    non_vol;    // Storage in NON-VOL
  unsigned int init_value;    // Initial Value
} json_message;

#define IS_VOID    0        // Value is a void
#define IS_INT16   1        // Value is a 16 bit int
#define IS_FLOAT   2        // Value is a floating point number
#define IS_DOUBLE  3        // Value is a double


bool read_JSON(void);             // Scan the serial port looking for JSON input
void show_echo(int v);            // Display the settings

extern int    json_dip_switch;    // DIP switch overwritten by JSON message
extern double json_sensor_dia;    // Sensor radius overwitten by JSON message
extern int    json_sensor_angle;  // Angle sensors are rotated through
extern int    json_paper_time;    // Time to turn on paper backer motor
extern int    json_echo;          // Value to ech
extern int    json_test;          // Self test to be performed
extern int    json_calibre_x10;   // Pellet Calibre
extern int    json_north_x;       // North Adjustment
extern int    json_north_y;
extern int    json_east_x;        // East Adjustment
extern int    json_east_y;
extern int    json_south_x;       // South Adjustment
extern int    json_south_y;
extern int    json_west_x;        // WestAdjustment
extern int    json_west_y;
extern int    json_spare_1;       // Not used
extern int    json_name_id;       // Name Identifier
extern int    json_1_ring_x10;    // Size of 1 ring in mmx10
extern int    json_LED_PWM;       // PWM Setting (%)
extern int    json_power_save;    // How long to run target before turning off LEDs
extern int    json_send_miss;     // Sent the miss message when TRUE
extern int    json_serial_number; // EIN 
extern int    json_paper_step;    // Number of times paper motor is stepped
extern int    json_multifunction; // Multifunction switch operation

#endif _JSON_H_
