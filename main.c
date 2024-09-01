

/* KERNEL Includes */
#include "FreeRTOS.h"
#include <STasks.h>

#include "task.h"
#include "buttons.h"
#include "event_groups.h"
#include "semphr.h"

/* MCAL Includes */

#include "gpio.h"
#include "GPTM.h"
#include "uart0.h"
#include "adc.h"

/********************************************** Systems Tasks Includes *********************************************************/
#include "dataTypes.h"






// Global Variables
HeatLevel driver_heat_level = OFF ;      // driver input heat level
HeatLevel passenger_heat_level = OFF ;   // passenger input heat level
uint32 driver_sens_reading ;              // driver's sensor  reading
uint32 passenger_sens_reading ;           // passenger's sensor reading
HeatLevel driver_heater_status  = OFF;    // driver heater status ;
HeatLevel passenger_heater_status = OFF;  // passenger heater status ;




/* Define Task Handles */

//TaskHandle_t vTaskRUMTIMEMEASURMENTS ;
TaskHandle_t vTaskInitHandle ;
TaskHandle_t vTaskDriverLedsHandle ;
TaskHandle_t vTaskPassengerLedsHandle ;
TaskHandle_t vTaskSensorHandle ;
TaskHandle_t vTaskInputDriverHandle ;
TaskHandle_t vTaskInputPassengerHandle ;
TaskHandle_t vTaskDisplayStatus;
TaskHandle_t vTaskReportErrorHandle ;



/* Define Event group handles  */
EventGroupHandle_t  eventGroup ;
/*
void vRunTimeMeasurementsTask(void *pvParameters);
uint32 ullTasksOutTime[9];
uint32 ullTasksInTime[9];
uint32 ullTasksExecutionTime[9];

*/

/* Define Semaphore handles */


SemaphoreHandle_t xDriver_Input_Mutex;
SemaphoreHandle_t xPassenger_Input_Mutex;
SemaphoreHandle_t xDriver_Sens_Mutex;
SemaphoreHandle_t xPassenger_Sens_Mutex;
SemaphoreHandle_t xDriver_Heater_Mutex;
SemaphoreHandle_t xPassenger_Heater_Mutex;
//SemaphoreHandle_t UART0_Mutex;

int main(void)

{
    // Create the event group.
    eventGroup = xEventGroupCreate();   // Create event groups.



    // Create the Mutexes Needed to ensure Mutual Exclusion .
    xDriver_Input_Mutex = xSemaphoreCreateMutex();      // Mutex for Driver input level to Share it between Tasks.
    xPassenger_Input_Mutex = xSemaphoreCreateMutex();   // Mutex for Driver sensor reading to Share it between Tasks.
    xDriver_Sens_Mutex = xSemaphoreCreateMutex();       // Mutex for Passenger input level to Share it between Tasks.
    xPassenger_Sens_Mutex = xSemaphoreCreateMutex();    // Mutex for Passenger sensor reading to Share it between Tasks.
    xDriver_Heater_Mutex = xSemaphoreCreateMutex();    // Mutex for Passenger sensor reading to Share it between Tasks.
    xPassenger_Heater_Mutex = xSemaphoreCreateMutex();
    //UART0_Mutex =xSemaphoreCreateMutex();

    // Initialize all the buttons and LEDs in the Tiva c
    GPIO_ButtonsLedsPoteintiometerInit();


    // Enable Interrupts on both buttons PF0 AND PF4 and PF5
    GPIO_SW1EdgeTriggeredInterruptInit();
    GPIO_SW2EdgeTriggeredInterruptInit();
    GPIO_SW3EdgeTriggeredInterruptInit();




    // Initialize the UART channel
    UART0_Init();

    // Initialize ADC module 0 sequence 0
    ADCinit();

    // initialize the timer0
    GPTM_WTimer0Init();



    // Create the other tasks which control the heater
    //xTaskCreate(vRunTimeMeasurementsTask , "RUN_TIME_MEASURMNENT_TASK" ,50, NULL, 6, & vTaskRUMTIMEMEASURMENTS);
    xTaskCreate(vDriverInputTask , "DRIVER_INPUT_TASK" ,50, NULL, 5, & vTaskInputDriverHandle);
    xTaskCreate(vDriverledsTask , "DRIVER_LED_TASK" ,50, NULL, 2, & vTaskDriverLedsHandle);
    xTaskCreate(vGetSensorsReading , "SENSORS_TASK" , 50, NULL, 3, & vTaskSensorHandle);
    xTaskCreate(vPassengerInputTask , "PASSENGER_INPUT_TASK" ,50, NULL, 5, & vTaskInputPassengerHandle);
    xTaskCreate(vPassengerledsTask , "PASSENGER_LED_TASK" ,50, NULL, 2, & vTaskPassengerLedsHandle);
    xTaskCreate(vDisplayStatusTask , "DISPLAY_STATUS_TASK" ,50, NULL, 4, & vTaskDisplayStatus);
    xTaskCreate(vDiagnosticsTaks , "DIAGNOSTICS_TASK" ,150, NULL, 5, & vTaskReportErrorHandle);




/*
    vTaskSetApplicationTaskTag( vTaskInputDriverHandle, ( TaskHookFunction_t ) 2 );
    vTaskSetApplicationTaskTag( vTaskDriverLedsHandle, ( TaskHookFunction_t ) 3 );
    vTaskSetApplicationTaskTag( vTaskSensorHandle, ( TaskHookFunction_t ) 4 );
    vTaskSetApplicationTaskTag( vTaskInputPassengerHandle, ( TaskHookFunction_t ) 5 );
    vTaskSetApplicationTaskTag( vTaskPassengerLedsHandle, ( TaskHookFunction_t ) 6);
    vTaskSetApplicationTaskTag( vTaskDisplayStatus, ( TaskHookFunction_t ) 7 );
    vTaskSetApplicationTaskTag( vTaskReportErrorHandle, ( TaskHookFunction_t ) 8 );

*/



    vTaskStartScheduler();




}

/*
void vRunTimeMeasurementsTask(void *pvParamters){
    TickType_t xLastWakeTime ;                                       // Variable to Store the Wake Time of the task.
    xLastWakeTime = xTaskGetTickCount() ;

    vTaskDelayUntil (&xLastWakeTime , pdMS_TO_TICKS(10000));
    xSemaphoreTake(UART0_Mutex , portMAX_DELAY);

    UART0_SendString("driver Input Task execution time is ");
    UART0_SendInteger(ullTasksExecutionTime[2] / 10);
    UART0_SendString(" msec \r\n");

    UART0_SendString("DriverLEDTask execution time is ");
    UART0_SendInteger(ullTasksExecutionTime[3] / 10);
    UART0_SendString(" msec \r\n");

    UART0_SendString("SENSORS_TASK execution time is ");
    UART0_SendInteger(ullTasksExecutionTime[4] / 10);
    UART0_SendString(" msec \r\n");

    UART0_SendString("Passenger Input execution time is ");
    UART0_SendInteger(ullTasksExecutionTime[5] / 10);
    UART0_SendString(" msec \r\n");

    UART0_SendString("Passenger LEDS task execution time is ");
    UART0_SendInteger(ullTasksExecutionTime[6] / 10);
    UART0_SendString(" msec \r\n");

    UART0_SendString("Display Status Task execution time is ");
    UART0_SendInteger(ullTasksExecutionTime[7] / 10);
    UART0_SendString(" msec \r\n");

    UART0_SendString("Report  Task execution time is ");
    UART0_SendInteger(ullTasksExecutionTime[8] / 10);
    UART0_SendString(" msec \r\n");
    xSemaphoreGive(UART0_Mutex);


    vTaskDelete(NULL);
}
*/








