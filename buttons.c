/*
 * buttons.c
 *
 *  Created on: Aug 21, 2024
 *      Author: youssef mohamed ghazali
 */

#include "FreeRTOS.h"
#include "buttons.h"
#include "task.h"
#include "buttons.h"
#include "event_groups.h"
#include "semphr.h"
#include "tm4c123gh6pm_registers.h"




extern EventGroupHandle_t eventGroup;

void DriverButtonPressedHandler (void){
    // Create two variables one to be used to see if a context switch will be asked after the ISR.
    BaseType_t xHigherPriorityTaskWoken, xResult;
    // Variable to see what event we will set .
    uint32 bits_to_set;
    xHigherPriorityTaskWoken = pdFALSE;


    if (GPIO_PORTF_RIS_REG & (1 << DRIVER_BUTTON_1)){
        //  Clear the Interrupt Flag.
        GPIO_PORTF_ICR_REG |= (1 << DRIVER_BUTTON_1);
        //   Assign event flag  1 to be set.
        bits_to_set = DRIVER_BUTTON1_MASK ;
    }
    else if ( GPIO_PORTF_RIS_REG & (1 <<DRIVER_BUTTON_2)) {
        //  Clear the Interrupt Flag.
        GPIO_PORTF_ICR_REG |= (1 << DRIVER_BUTTON_2);
        //  Assign event flag 2 to be set.
        bits_to_set = DRIVER_BUTTON2_MASK ;
    }
    //  Set the Event Corresponding to the button.
    xResult = xEventGroupSetBitsFromISR(eventGroup, bits_to_set, &xHigherPriorityTaskWoken);

    if( xResult != pdFAIL )
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
          switch should be requested. The macro used is port specific and will
          be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
          the documentation page for the port being used. */

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );


}
}
void PassengerButtonPressedHandler(void){
    // Create two variables one to be used to see if a context switch will be asked after the ISR.
    BaseType_t xHigherPriorityTaskWoken, xResult;
    // Variable to see what event we will set .
    uint32 bits_to_set;
    xHigherPriorityTaskWoken = pdFALSE;
    //  Clear the Interrupt Flag.
    GPIO_PORTA_ICR_REG |= (1<<PASSENGER_BUTTON_1);

    //  Assign event flag 2 to be set.
    bits_to_set = PASSENGER_BUTTON_MASK;



    //  Set the Event Corresponding to the button.
    xResult = xEventGroupSetBitsFromISR(eventGroup, bits_to_set, &xHigherPriorityTaskWoken);

    if( xResult != pdFAIL )
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
          switch should be requested. The macro used is port specific and will
          be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
          the documentation page for the port being used. */

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

    }
}




