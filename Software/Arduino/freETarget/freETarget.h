
/*----------------------------------------------------------------
 * 
 * freETarget.h
 * 
 * Software to run the Air-Rifle / Small Bore Electronic Target
 * 
 *----------------------------------------------------------------
 *
 */
#ifndef _FREETARGET_H
#define _FREETARGET_H

#define SOFTWARE_VERSION "\"3.02.0 June 14, 2021\""
#define REV_100    100
#define REV_210    210
#define REV_220    220
#define REV_290    290
#define REV_300    300

#define INIT_DONE       0xabcd        // Initialization complete signature

/*
 * Compilation Flags
 */
#define SAMPLE_CALCULATIONS false     // Trace the COUNTER values
#define AUX_SERIAL         Serial3    // Auxilary Connector
#define DISPLAY_SERIAL     Serial2    // Serial port for slave display

#define PRINT(x)  {Serial.print(x); AUX_SERIAL.print(x); DISPLAY_SERIAL.print(x);}
#define GET(ch)   {if ( Serial.available() )              ch = Serial.read();         \
                   else if ( esp01_available() )          ch = esp01_read();          \
                   else if ( DISPLAY_SERIAL.available() ) ch = DISPLAY_SERIAL.read(); \
                   else                                   ch = 0;}
                   
#define AVAILABLE ( Serial.available() + AUX_SERIAL.available() + DISPLAY_SERIAL.available() )

#define PORT_SERIAL   1
#define PORT_AUX      2
#define PORT_DISPLAY  4
#define PORT_ALL      (PORT_SERIAL + PORT_AUX + PORT_DISPLAY)

/*
 * Oscillator Features
 */
#define OSCILLATOR_MHZ   8.0                          // 8000 cycles in 1 ms
#define CLOCK_PERIOD  (1.0/OSCILLATOR_MHZ)            // Seconds per bit
#define ONE_SECOND      1000                          // 1000 ms delay
#define SHOT_TIME     ((int)(json_sensor_dia / 0.33)) // Worst case delay Sensor diameter / speed of sound)
 
/*
 * DIP Switch enabled tests.  Set (0<<x) to (1<<x) to enable always
 */
//                      From DIP    From Software
#define CALIBRATE       ((1 << 0) + (0 << (4 + 0)))     // 1 Go to Calibration Mode
#define CAL_LOW         ((1 << 1) + (0 << (4 + 1)))     // 2 When CALIBRATE is asserted, use lower trip point
#define CAL_HIGH        ((1 << 2) + (0 << (4 + 2)))     // 4 When CALIBRATE is asserted, use higher trip point
#define VERBOSE_TRACE   ((1 << 3) + (0 << (4 + 3)))     // 8 Show the verbose software trace

#define HI(x) (((x) >> 8 ) & 0x00ff)
#define LO(x) ((x) & 0x00ff)

#define N 0
#define E 1
#define S 2
#define W 3

struct history
{
  unsigned int shot;    // Current shot number
  double       x;       // X location of shot
  double       y;       // Y location of shot
};

typedef struct history history_t;

extern double     s_of_sound;

extern const char* names[];
extern bool  face_strike;
extern bool  is_trace;                // True if tracing is enabled 
extern char* nesw;                    // Cardinal Points

/*
 *  Factory settings via Arduino monitor
 */
/*
#define FACTORY        {"NAME_ID":1, "TRGT_1_RINGx10":1550, "ECHO":2}
#define FACTORY_BOSS   {"NAME_ID":1, "TRGT_1_RINGx10":1550, "ECHO":2}
#define FACTORY_MINION {"NAME_ID":2, "TRGT_1_RINGx10":1550, "ECHO":2}
#define SERIAL_NUMBER  {"NAME_ID":1, "TRGT_1_RINGx10":1550, "SN":1234, "ECHO":2}
*/
#endif
