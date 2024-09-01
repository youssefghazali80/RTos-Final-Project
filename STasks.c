/*
 * tasks.c
 *
 *  Created on: Aug 24, 2024
 *      Author: youssef mohamed ghazali
 */

#include "FreeRTOS.h"
#include "STasks.h"
#include "task.h"
#include "sensors.h"
#include "adc.h"
#include "gpio.h"
#include "event_groups.h"
#include "tm4c123gh6pm_registers.h"
#include "uart0.h"
#include "semphr.h"
#include "GPTM.h"




// External Global variables
extern EventGroupHandle_t  eventGroup;

















// Task that handles the LEDs (seat heater controller for the driver seat)
void vDriverledsTask(void *pvParameters){

    sint8 difference ;
    while (1){


        // Take the Mutex Attached to drive input heat level and to the sensor reading.
        xSemaphoreTake(xDriver_Sens_Mutex , portMAX_DELAY);
        xSemaphoreTake(xDriver_Input_Mutex , portMAX_DELAY);
        xSemaphoreTake(xDriver_Heater_Mutex , portMAX_DELAY);

        // Start handling the LEDs according to the difference between the wanted value and the current temperature.
        difference = driver_sens_reading - driver_heat_level;  // measure the difference between the current temperature and
        if (driver_sens_reading > SENSOR_MAX_VALID  || driver_sens_reading < SENSOR_MIN_VALID ){
            GPIO_DriverBlueLedOff();
            GPIO_DriverGreenLedOff();
            GPIO_DriverRedLedOn();
            driver_heater_status = OFF;
        }
        else if (difference > 0 ){
            // IF the current temperature is higher than the desired temperature turn the heater off
            GPIO_DriverBlueLedOff();
            GPIO_DriverGreenLedOff();
            GPIO_DriverRedLedOff();
            driver_heater_status = OFF;
        }
        else if (difference >= -5 && difference <= -2){
            // IF the current temperature is less than the the current temperature by 2 to 5 then heater is in medium intensity .
            GPIO_DriverBlueLedOff();
            GPIO_DriverRedLedOff();
            GPIO_DriverGreenLedOn();
            driver_heater_status = LOW;


        }
        else if (difference >= -10 ){
            // IF the current temperature is higher than -10 .
            GPIO_DriverRedLedOff();
            GPIO_DriverGreenLedOff();
            GPIO_DriverBlueLedOn();
            driver_heater_status = MEDIUM ;
        }
        else if (difference < -10){
            // IF the current temperature is more than -10 .
            GPIO_DriverRedLedOn();
            GPIO_DriverGreenLedOn();
            GPIO_DriverBlueLedOff();
            driver_heater_status = LOW;

        }

        // Give the Mutex Attached to drive input heat level and to the sensor reading.
        xSemaphoreGive(xDriver_Heater_Mutex);
        xSemaphoreGive(xDriver_Sens_Mutex);
        xSemaphoreGive(xDriver_Input_Mutex);
    }
}
// Description: Task that handles the passenger LEDs (seat heater controller for passenger seat)


void vPassengerledsTask(void * pvParameters){
    sint8 difference ;



    while (1){


        // Take the Mutex Attached to drive input heat level and to the sensor reading.
        xSemaphoreTake(xPassenger_Sens_Mutex , portMAX_DELAY);
        xSemaphoreTake(xPassenger_Input_Mutex , portMAX_DELAY);
        xSemaphoreTake(xPassenger_Heater_Mutex , portMAX_DELAY);



        // Measure the difference between the current temperature
        difference = passenger_sens_reading - passenger_heat_level;


        if (passenger_sens_reading > SENSOR_MAX_VALID  || passenger_sens_reading < SENSOR_MIN_VALID ){
            GPIO_PassengerBlueLedOff();
            GPIO_PassengerGreenLedOff();
            GPIO_PassengerRedLedOn();
            passenger_heater_status = OFF;
        }

        else if (difference > 0 ){
            // If the current temperature is higher than the desired temperature turn the heater off
            GPIO_PassengerBlueLedOff();
            GPIO_PassengerGreenLedOff();
            GPIO_PassengerRedLedOff();
            passenger_heater_status = OFF;
        }

        else if (difference >= -5 && difference <= -2){
            // If the current temperature is less than the the current temperature by 2 to 5 then heater is in low intensity
            GPIO_PassengerBlueLedOff();
            GPIO_PassengerRedLedOff();
            GPIO_PassengerGreenLedOn();
            passenger_heater_status = LOW;


        }
        else if (difference >= -10 ){
            // If the current temperature is lower than the desired temperature by 5 :10 then the heater in medium intensity
            GPIO_PassengerRedLedOff();
            GPIO_PassengerGreenLedOff();
            GPIO_PassengerBlueLedOn();
            passenger_heater_status =MEDIUM;

        }
        else if (difference < -10){
            // If the current temperature is lower than the desired temperature by more than 10 then the heater will be in high intesity
            GPIO_PassengerRedLedOff();
            GPIO_PassengerGreenLedOn();
            GPIO_PassengerBlueLedOn();
            passenger_heater_status = HIGH;

        }
        // Give the Mutex Attached to drive input heat level and to the sensor reading.
        xSemaphoreGive(xPassenger_Heater_Mutex);
        xSemaphoreGive(xPassenger_Input_Mutex);
        xSemaphoreGive(xPassenger_Sens_Mutex);


    }




}


/*
 * Description : Periodic task that triggers the ADC to get the input
 */
void vGetSensorsReading (void *pvParameters){


    TickType_t xLastWakeTime ;                                       // Variable to Store the Wake Time of the task.
    xLastWakeTime = xTaskGetTickCount() ;                            // Get the last time the task get awaken.
    while (1){
        ADCTrigger();                                                // Trigger the ADC interrupt to get sensor Readings
        while ((ADC0_ADCRIS &  0X01));                              // Wait for the ADC conversion to finish it will take 8 us so we can wait by polling
        // Take the mutexes Regarding the two sensors reading
        xSemaphoreTake(xDriver_Sens_Mutex , portMAX_DELAY);
        xSemaphoreTake(xPassenger_Sens_Mutex ,portMAX_DELAY);

        // Assign the new reading.


        driver_sens_reading = (ADC0_ADCSSFIFO0 *45) / 4095;                      // Get  the driver seat temperature sensor data .
        passenger_sens_reading = (ADC0_ADCSSFIFO0 *45) / 4095 ;                   // Get the passenger seat temperature sensor data .


        // Give the mutexes Regarding the two sensor reading .
        xSemaphoreGive(xPassenger_Sens_Mutex);
        xSemaphoreGive(xDriver_Sens_Mutex);


        ADC0_ADCISC |=  (1<<0);                                       // Clear the completion Flag.
        vTaskDelayUntil (&xLastWakeTime , pdMS_TO_TICKS(1000));      // Make this Task Periodic.



    }



}
/*
 * Description : Event Triggered task to update user input level
 */
void vDriverInputTask(void * pvParameters){
    while (1){
        // Wait for the Interrupt Attached The driver button to be triggered which by its Turn will Set the event Flags
        xEventGroupWaitBits(eventGroup , DRIVER_BUTTON1_MASK | DRIVER_BUTTON2_MASK ,pdTRUE, pdFALSE, portMAX_DELAY);

        // Take the semaphore attached to the heat driver input level.
        xSemaphoreTake(xDriver_Input_Mutex , portMAX_DELAY);

        // Start Acting on the input driver.
        switch (driver_heat_level){
        case OFF :
            driver_heat_level = LOW;
            break;
        case LOW :
            driver_heat_level = MEDIUM;
            break;
        case MEDIUM :
            driver_heat_level = HIGH;
            break;
        case HIGH :
            driver_heat_level = OFF ;
            break;
        default:
            break;
        }

        // Give the Semaphore Attached to the heat driver Input .
        xSemaphoreGive(xDriver_Input_Mutex);
        // Set The bit That enable the task of the diagnostics
        xEventGroupSetBits(eventGroup, DIAGNOSTIC_INPUT_MASK);



    }

}

/*
 * Description : Event triggered task to update user input level
 */
void vPassengerInputTask(void * pvParameters){

    while (1){
        // Wait for the Interrupt Attached The driver button to be triggered which by its Turn will Set the event Flags
        xEventGroupWaitBits(eventGroup , PASSENGER_BUTTON_MASK ,pdTRUE, pdFALSE, portMAX_DELAY);

        // Take the Mutex attached to the  Passenger heater input level.
        xSemaphoreTake(xPassenger_Input_Mutex , portMAX_DELAY);

        // Start Acting on the passenger input heat level.
        switch (passenger_heat_level){

        case OFF :
            passenger_heat_level = LOW;
            break;
        case LOW :
            passenger_heat_level = MEDIUM;
            break;
        case MEDIUM :
            passenger_heat_level = HIGH;
            break;
        case HIGH :
            passenger_heat_level = OFF ;
            break;
        default:
            break;

        }

        // Give the Semaphore Attached to the heat driver Input.
        xSemaphoreGive(xPassenger_Input_Mutex) ;
        // Set The bit That enable the task of the diagnostics
        xEventGroupSetBits(eventGroup, DIAGNOSTIC_INPUT_MASK);



    }

}


void vDisplayStatusTask(void * pvParameters){

    TickType_t xLastWakeTime ;                                       // Variable to Store the Wake Time of the task.
    xLastWakeTime = xTaskGetTickCount() ;

    while (1){
        //xSemaphoreTake(UART0_Mutex , portMAX_DELAY);
        xSemaphoreTake(xDriver_Sens_Mutex , portMAX_DELAY);
        xSemaphoreTake(xDriver_Input_Mutex , portMAX_DELAY);
        xSemaphoreTake(xDriver_Heater_Mutex , portMAX_DELAY);

        UART0_SendString  ("\r \n driver seat status");
        UART0_SendString  ("\r \n The temperature of the seat is : ");
        UART0_SendInteger (driver_sens_reading);
        UART0_SendString  ("\r \n The required heat level is  : ");
        switch (driver_heat_level){
        case OFF :
            UART0_SendString  ("Off");
            break;
        case LOW :
            UART0_SendString  ("low");
            break;
        case MEDIUM :
            UART0_SendString  ("Medium");
            break;
        case HIGH :
            UART0_SendString  ("High");
            break;
        default:
            break;
        }
        UART0_SendString  ("\r \n The current heater intensity is : ");
        switch (driver_heater_status){
        case OFF :
            UART0_SendString  ("Off");
            break;
        case LOW :
            UART0_SendString  ("low");
            break;
        case MEDIUM :
            UART0_SendString  ("Medium");
            break;
        case HIGH :
            UART0_SendString  ("High");
            break;
        default:
            break;
        }
        // Give the Mutex Attached to drive input heat level and to the sensor reading.
        xSemaphoreGive(xDriver_Heater_Mutex);
        xSemaphoreGive(xDriver_Sens_Mutex);
        xSemaphoreGive(xDriver_Input_Mutex);


        xSemaphoreTake(xPassenger_Sens_Mutex , portMAX_DELAY);
        xSemaphoreTake(xPassenger_Input_Mutex , portMAX_DELAY);
        xSemaphoreTake(xPassenger_Heater_Mutex , portMAX_DELAY);

        UART0_SendString  ("\r \n passenger seat status ");
        UART0_SendString  ("\r \n The temperature of the seat is : ");
        UART0_SendInteger (passenger_sens_reading);
        UART0_SendString  ("\r \n The required heat level is : ");
        switch (passenger_heat_level){
        case OFF :
            UART0_SendString  ("Off");
            break;
        case LOW :
            UART0_SendString  ("low");
            break;
        case MEDIUM :
            UART0_SendString  ("Medium");
            break;
        case HIGH :
            UART0_SendString  ("High");
            break;
        default:
            break;
        }
        UART0_SendString  ("\r \n The current heater intensity is : ");
        switch (passenger_heater_status){
        case OFF :
            UART0_SendString  ("Off");
            break;
        case LOW :
            UART0_SendString  ("low");
            break;
        case MEDIUM :
            UART0_SendString  ("Medium");
            break;
        case HIGH :
            UART0_SendString  ("High");
            break;
        default:
            break;
        }
        xSemaphoreGive(xPassenger_Heater_Mutex);
        xSemaphoreGive(xPassenger_Input_Mutex);
        xSemaphoreGive(xPassenger_Sens_Mutex);
        //xSemaphoreGive(UART0_Mutex);

        vTaskDelayUntil (&xLastWakeTime , pdMS_TO_TICKS(5000));


    }





}
void vDiagnosticsTaks(void *pvParameters){
    EventBits_t uxBits;
    volatile HeatLevelDiagnostics level_diagnostics ;
    volatile FailureDiagnostics failure_diagnostics [50];
    uint32 count = 0;
    while (1){
        // wait for the bit that enables diagnostics to be stored in the ram
        uxBits = xEventGroupWaitBits(eventGroup , DIAGNOSTIC_INPUT_MASK | DIAGNOSTIC_PASSENGER_FAILURE_MASK |
                                     DIAGNOSTIC_DRIVER_FAILURE_MASK ,pdTRUE, pdFALSE, portMAX_DELAY);
        if (uxBits & DIAGNOSTIC_INPUT_MASK ){
            // Take the mutex attached  to the driver input and passenger input .
            xSemaphoreTake(xDriver_Input_Mutex , portMAX_DELAY);
            xSemaphoreTake(xPassenger_Input_Mutex , portMAX_DELAY);
            //  assign the latest input value in this variable
            level_diagnostics.driver_heat_level = driver_heat_level;
            level_diagnostics.Passenger_heat_level = passenger_heat_level;
            level_diagnostics.TimeStamp = GPTM_WTimer0Read();



            // Give the Mutex attached to the driver input and passenger input.
            xSemaphoreGive(xPassenger_Input_Mutex);
            xSemaphoreGive(xDriver_Input_Mutex);






        }
        else if (uxBits & DIAGNOSTIC_DRIVER_FAILURE_MASK){
            if (count < 50){
                xSemaphoreTake(xDriver_Sens_Mutex , portMAX_DELAY);
                if (driver_sens_reading >SENSOR_MAX_VALID){
                    failure_diagnostics [count].failure = SENSOR_DRIVER_EXCEED_VALID;
                    failure_diagnostics [count].TimeStamp = GPTM_WTimer0Read();
                    count ++ ;

                }
                else {
                    failure_diagnostics [count].failure = SENSOR_DRIVER_BELOW_VALID;
                    failure_diagnostics [count].TimeStamp = GPTM_WTimer0Read();
                    count ++ ;

                }
                xSemaphoreGive(xDriver_Sens_Mutex);


            }

        }
        else if (uxBits & DIAGNOSTIC_PASSENGER_FAILURE_MASK){
            if (count < 50){

                xSemaphoreTake(xPassenger_Sens_Mutex , portMAX_DELAY);
                if (passenger_sens_reading >SENSOR_MAX_VALID){
                    failure_diagnostics [count].failure = SENSOR_PASSENGER_EXCEED_VALID ;
                    failure_diagnostics [count].TimeStamp = GPTM_WTimer0Read();
                    count ++ ;

                }
                else {
                    failure_diagnostics [count].failure = SENSOR_PASSENGER_BELOW_VALID;
                    failure_diagnostics [count].TimeStamp = GPTM_WTimer0Read();
                    count ++ ;

                }
                // Give the Semaphore.
                xSemaphoreGive(xPassenger_Sens_Mutex);

            }

        }





    }




}
