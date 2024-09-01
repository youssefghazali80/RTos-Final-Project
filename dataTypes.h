/*
 * dataTypes.h
 *
 *  Created on: Aug 20, 2024
 *      Author: youssef
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#define SENSOR_LOW_RANGE       (5U)    // MACRO TO describe the lowest value that the sensor can read
#define SENSOR_HIGH_RANGE      (40U)   // MACRO that has the highest value that the sensor can read
/*
 * Enum to Represent heating level that we want
 */
typedef enum {
    OFF , LOW = 25 , MEDIUM =30 ,HIGH = 35
} HeatLevel;
typedef enum {
    SENSOR_DRIVER_EXCEED_VALID , SENSOR_DRIVER_BELOW_VALID , SENSOR_PASSENGER_EXCEED_VALID , SENSOR_PASSENGER_BELOW_VALID
}Failure;

typedef struct {

    Failure failure;
    uint32 TimeStamp;

}FailureDiagnostics;
typedef struct {
    HeatLevel driver_heat_level;
    HeatLevel Passenger_heat_level;
    uint32 TimeStamp;
}HeatLevelDiagnostics;

#endif /* DATATYPES_H_ */
