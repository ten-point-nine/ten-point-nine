/*----------------------------------------------------------------
 *
 * esp-01.h
 *
 * WiFi Driver for esp-01
 *
 *---------------------------------------------------------------*/

#ifndef _ESP_01_H_
#define _ESP_01_H_

/*
 * Function Prototypes
 */
void esp01_init(void);                      // Initialize the device
bool esp01_restart(void);                   // Take control and reset the device

char         esp01_read(void);              // Read a character from the queuue
unsigned int esp01_available(void);         // Return the number of available characters
void         esp01_send(bool start);        // Start or end a send sequence
void         esp01_receive(void);           // Take care of receiving characters from the IP channel
#endif
