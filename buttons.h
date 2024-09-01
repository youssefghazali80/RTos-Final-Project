/*
 * buttons.h
 *
 *  Created on: Aug 21, 2024
 *      Author: youssef
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_



// define the pins
#define PF0     0U
#define PF4     4U
#define PA5     5U


// define the pins which is connected to the buttons
#define DRIVER_BUTTON_1        PF0
#define DRIVER_BUTTON_2        PF4
#define PASSENGER_BUTTON_1     PA5

// define the masks for the events for each button
#define DRIVER_BUTTON1_MASK               0x01
#define DRIVER_BUTTON2_MASK               0x02
#define PASSENGER_BUTTON_MASK             0x04
#define DRIVER_HEATER_MASK                0x08
#define PASSENGER_HEATER_MASK             0x10
#define DIAGNOSTIC_INPUT_MASK             0x20
#define DIAGNOSTIC_DRIVER_FAILURE_MASK    0x40
#define DIAGNOSTIC_PASSENGER_FAILURE_MASK 0x80



// handler for interrupts generated from pressing the buttons
void DriverButtonPressedHandler (void);
void PassengerButtonPressedHandler (void);

#endif /* BUTTONS_H_ */
