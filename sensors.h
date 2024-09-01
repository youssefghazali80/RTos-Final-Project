/*
 * sensors.h
 *
 *  Created on: Aug 21, 2024
 *      Author: youssef
 */

#ifndef SENSORS_H_
#define SENSORS_H_
//define the pin numbers
#define PE1     1U
#define PE2     2U
// define pins that are connected to the temperature sensor (potentiometer in test cases)
#define DRIVER_SENSOR_PIN       PE2   //AIN1
#define PASSENGER_SENSOR_PIN    PE1   //AIN2

// define sensor valid range
#define SENSOR_MAX_VALUE   45U
#define SENSOR_MIN_VALUE   0U
#define SENSOR_MAX_VALID   40U
#define SENSOR_MIN_VALID   5U

#endif /* SENSORS_H_ */
