/**********************************************************************************************
 *
 * Module: GPIO
 *
 * File Name: GPIO.h
 *
 * Description: Header file for the TM4C123GH6PM DIO driver for TivaC Built-in Buttons and LEDs
 *
 * Author: Edges for Training Team
 *
 ***********************************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "std_types.h"

#define GPIO_PORTF_PRIORITY_MASK      0xFF1FFFFF
#define GPIO_PORTF_PRIORITY_BITS_POS  21
#define GPIO_PORTF_INTERRUPT_PRIORITY 5

#define PRESSED                ((uint8)0x00)
#define RELEASED               ((uint8)0x01)

void GPIO_ButtonsLedsPoteintiometerInit(void);

void GPIO_DriverRedLedOn(void);
void GPIO_DriverBlueLedOn(void);
void GPIO_DriverGreenLedOn(void);

void GPIO_PassengerRedLedOn(void);
void GPIO_PassengerBlueLedOn(void);
void GPIO_PassengerGreenLedOn(void);


void GPIO_DriverRedLedOff(void);
void GPIO_DriverBlueLedOff(void);
void GPIO_DriverGreenLedOff(void);

void GPIO_PassengerRedLedOff(void);
void GPIO_PassengerBlueLedOff(void);
void GPIO_PassengerGreenLedOff(void);

void GPIO_RedLedToggle(void);
void GPIO_BlueLedToggle(void);
void GPIO_GreenLedToggle(void);

uint8 GPIO_SW1GetState(void);
uint8 GPIO_SW2GetState(void);

void GPIO_SW1EdgeTriggeredInterruptInit(void);
void GPIO_SW2EdgeTriggeredInterruptInit(void);
void GPIO_SW3EdgeTriggeredInterruptInit(void);

#endif /* GPIO_H_ */
