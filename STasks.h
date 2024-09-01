/*
 * tasks.h
 *
 *  Created on: Aug 24, 2024
 *      Author: youssef
 */

#ifndef STASKS_H_
#define STASKS_H_

#include "gpio.h"
#include "buttons.h"
#include "datatypes.h"
#include "task.h"
#include "semphr.h"



// // task handlers

extern TaskHandle_t vTaskInitHandle ;
extern TaskHandle_t vTaskDriverLedsHandle ;
extern TaskHandle_t vTaskPassengerLedsHandle ;
extern TaskHandle_t vTaskSensorHandle ;
extern TaskHandle_t vTaskInputDriverHandle ;
extern TaskHandle_t vTaskInputPassengerHandle ;
extern TaskHandle_t vTaskDisplayStatus;
extern TaskHandle_t vTaskReportErrorHandle ;


// Global Variables
extern HeatLevel driver_heat_level  ;      // driver input heat level
extern HeatLevel passenger_heat_level ;   // passenger input heat level
extern uint32 driver_sens_reading ;              // driver's sensor  reading
extern uint32 passenger_sens_reading ;           // passenger's sensor reading
extern HeatLevel driver_heater_status  ;    // driver heater status ;
extern HeatLevel passenger_heater_status ;  // passenger heater status ;


/* Define Semaphore handles */

extern SemaphoreHandle_t xDriver_Input_Mutex;
extern SemaphoreHandle_t xPassenger_Input_Mutex;
extern SemaphoreHandle_t xDriver_Sens_Mutex;
extern SemaphoreHandle_t xPassenger_Sens_Mutex;
extern SemaphoreHandle_t xDriver_Heater_Mutex;
extern SemaphoreHandle_t xPassenger_Heater_Mutex;
extern SemaphoreHandle_t UART0_Mutex;







/*
 * Description : task that controls the leds (seat heater controller for the driver seat)
 */

void vDriverledsTask(void *pvParameters);



/*
 * Description : task that controls the passenger leds (seat heater controller for passenger seat)
 */

void vPassengerledsTask(void * pvParameters);

/*
 * Description : Task that Gets the Sensor Data  to save them
 *
 */

void vGetSensorsReading (void *pvParameters);
/*
 * Description : task that handles the input of the passengers.
 */
void vPassengerInputTask(void * pvParameters);


/*
 * Description : Event Triggered task to update user input level
 */
void vDriverInputTask(void * pvParameters);

/*
 * Description : task that prints the status of the seats
 */
void vDisplayStatusTask(void * pvParameters);

/*
 * Description : Task that implements diagnostics on the ram .
 */
void vDiagnosticsTaks(void *pvParameters);



#endif /* STASKS_H_ */
