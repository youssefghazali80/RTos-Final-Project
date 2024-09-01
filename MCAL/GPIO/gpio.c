/**********************************************************************************************
 *
 * Module: GPIO
 *
 * File Name: GPIO.c
 *
 * Description: Source file for the TM4C123GH6PM DIO driver for TivaC Built-in Buttons and LEDs
 *
 * Author: Youssef mohamed ghazali
 *
 ***********************************************************************************************/
#include "gpio.h"
#include "tm4c123gh6pm_registers.h"
#include "buttons.h"
#include "Leds.h"
#include "sensors.h"
void GPIO_ButtonsLedsPoteintiometerInit(void)
{
    /*
     * PF0 --> SW1 (driver seat first button)
     * PF1 --> driver Red LED
     * PF2 --> driver Blue LED
     * PF3 --> driver Green LED
     * PF4 --> SW2 (driver seat second button)
     * PA5 --> SW3 (passenger  button)
     * PA2 --> passenger Red Led
     * PA3 --> passenger Blue Led
     * PA4 --> passenger Green Led
     * PE1 --> driver temperature sensor    (potentiometer for test cases)
     * PE2 --> passenger temperature sensor (potentiometer for test cases)
     *
     */

    /* Enable clock for PORTF AND PORTA and PORTE and wait for clock to start */

    SYSCTL_RCGCGPIO_REG |= 0x31;
    while(!(SYSCTL_PRGPIO_REG & 0x31));

    // port f Related configurations
    GPIO_PORTF_LOCK_REG   = 0x4C4F434B;                                                                         /* Unlock the GPIO_PORTF_CR_REG */
    GPIO_PORTF_CR_REG    |= (1<< DRIVER_BUTTON_1);                                                              /* Enable changes on driver first button (PF0) */
    GPIO_PORTF_AMSEL_REG &= 0xC0;                                                                               /* Disable Analog on PF0, PF1, PF2, PF3  ,PF4 and PF5 */
    GPIO_PORTF_PCTL_REG  &= 0xFFF00000;                                                                         /* Clear PMCx bits for PF0, PF1, PF2, PF3  ,PF4 and PF5 to use it as GPIO pins */
    GPIO_PORTF_DIR_REG   &= ~(1<<DRIVER_BUTTON_1) & ~(1<<DRIVER_BUTTON_2);                                     /* Configure PF0 & PF4  as input pins */
    GPIO_PORTF_DIR_REG   |= ((1<<DRIVER_RED_LED) | (1<<DRIVER_BLUE_LED) | (1<<DRIVER_GREEN_LED));               /* Configure PF1, PF2 & PF3 as output pins */
    GPIO_PORTF_AFSEL_REG &= 0xC0;                                                                               /* Disable alternative function on PF0, PF1, PF2, PF3 ,PF4 and PF5 */
    GPIO_PORTF_PUR_REG   |= ((1<<DRIVER_BUTTON_1)|(1<<DRIVER_BUTTON_2));                                        /* Enable pull-up on PF0 & PF4 */
    GPIO_PORTF_DEN_REG   |= 0x3F;                                                                               /* Enable Digital I/O on PF0, PF1, PF2, PF3 , PF4 . */
    GPIO_PORTF_DATA_REG  &= ~(1<<DRIVER_RED_LED) & ~(1<<DRIVER_BLUE_LED) & ~(1<<DRIVER_GREEN_LED);              /* Clear bits 1, 2 & 3 in Data register to turn off the LEDs */
    // port a related configurations
    GPIO_PORTA_AMSEL_REG &= 0xC3;                                                                              /* Disable Analog on PA2, PA3, PA4 , PA5 */
    GPIO_PORTA_PCTL_REG  &= 0xFF0000FF;                                                                         /* Clear PMCx bits for PA2 ,PA3, PA4  and PA5 to use it as GPIO pins */
    GPIO_PORTA_DIR_REG   |= (1 << PASSENGER_RED_LED) |(1 << PASSENGER_BLUE_LED) |(1 << PASSENGER_GREEN_LED);    /* configure PA2 ,PA3, and PA4 as output pins   */
    GPIO_PORTA_DIR_REG   &= ~(1 << PASSENGER_BUTTON_1);                                                         /* Configure PA5 as input pin */
    GPIO_PORTA_AFSEL_REG &= 0xC3;                                                                               /* disable alternative function for pins PA2 , PA3 , PA4 and PA5 */
    //GPIO_PORTA_PUR_REG   |= (1 << PASSENGER_BUTTON_1);                                                          /* Enable pull-up on PA5 */
    GPIO_PORTA_DEN_REG   |= 0x3C;                                                                               /* enable digital I/O on pin PA2, PA3  PA4 AND PA5 */
    GPIO_PORTA_DATA_REG  &= ~(1<<PASSENGER_RED_LED) &~(1<<PASSENGER_BLUE_LED) &~(1<<PASSENGER_GREEN_LED);     /* Clear bits 0 , 1 , 2 to turn off The LEDs initially */
    // Port E related configuration
    GPIO_PORTE_AFSEL_REG |= 0x6;
    GPIO_PORTE_AMSEL_REG |= 0x6;                                                                                 /* enable analog functions on pin PE1 and PE2 */
    GPIO_PORTE_DEN_REG   &= ~(1<<DRIVER_SENSOR_PIN) &~(1<<PASSENGER_SENSOR_PIN);                                 /* disable digital function on PE0 and PE1 pin */


}

void GPIO_DriverRedLedOn(void)
{
    GPIO_PORTF_DATA_REG |= (1<<DRIVER_RED_LED);  /* Red LED ON */
}
void GPIO_PassengerRedLedOn(void)
{
    GPIO_PORTA_DATA_REG |= (1<<PASSENGER_RED_LED);  /* Red LED ON */
}

void GPIO_DriverBlueLedOn(void)
{
    GPIO_PORTF_DATA_REG |= (1<<DRIVER_BLUE_LED);  /* Blue LED ON */
}
void GPIO_PassengerBlueLedOn(void)
{
    GPIO_PORTA_DATA_REG |= (1<<PASSENGER_BLUE_LED);  /* Blue LED ON */
}

void GPIO_DriverGreenLedOn(void)
{
    GPIO_PORTF_DATA_REG |= (1<<DRIVER_GREEN_LED);  /* Green LED ON */
}
void GPIO_PassengerGreenLedOn(void)
{
    GPIO_PORTA_DATA_REG |= (1<<PASSENGER_GREEN_LED);  /* Green LED ON */
}

void GPIO_DriverRedLedOff(void)
{
    GPIO_PORTF_DATA_REG &= ~(1<< DRIVER_RED_LED);  /* Red LED OFF */
}
void GPIO_PassengerRedLedOff(void)
{
    GPIO_PORTA_DATA_REG &= ~(1<<PASSENGER_RED_LED);  /* Red LED OFF */
}

void GPIO_DriverBlueLedOff(void)
{
    GPIO_PORTF_DATA_REG &= ~(1<<DRIVER_BLUE_LED);  /* Blue LED OFF */
}
void GPIO_PassengerBlueLedOff(void)
{
    GPIO_PORTA_DATA_REG &= ~(1<<PASSENGER_BLUE_LED);  /* Blue LED OFF */
}

void GPIO_DriverGreenLedOff(void)
{
    GPIO_PORTF_DATA_REG &= ~(1<<DRIVER_GREEN_LED);  /* Green LED OFF */
}
void GPIO_PassengerGreenLedOff(void)
{
    GPIO_PORTA_DATA_REG &= ~(1<<PASSENGER_GREEN_LED);  /* Green LED OFF */
}



uint8 GPIO_SW1GetState(void)
{
    return ((GPIO_PORTF_DATA_REG >> DRIVER_BUTTON_1) & 0x01);
}

uint8 GPIO_SW2GetState(void)
{
    return ((GPIO_PORTF_DATA_REG >> DRIVER_BUTTON_2) & 0x01);
}
uint8 GPIO_SW3GetState(void)
{
    return ((GPIO_PORTA_DATA_REG >> PASSENGER_BUTTON_1) & 0x01);
}

void GPIO_SW1EdgeTriggeredInterruptInit(void)

{
    GPIO_PORTF_IS_REG    &= ~(1<<0);      /* PF0 detect edges */
    GPIO_PORTF_IBE_REG   &= ~(1<<0);      /* PF0 will detect a certain edge */
    GPIO_PORTF_IEV_REG   &= ~(1<<0);      /* PF0 will detect a falling edge */
    GPIO_PORTF_ICR_REG   |= (1<<0);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
    GPIO_PORTF_IM_REG    |= (1<<0);       /* Enable Interrupt on PF0 pin */
    /* Set GPIO PORTF priority as 5 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_PRI7_REG = (NVIC_PRI7_REG & GPIO_PORTF_PRIORITY_MASK) | (GPIO_PORTF_INTERRUPT_PRIORITY<<GPIO_PORTF_PRIORITY_BITS_POS);
    NVIC_EN0_REG         |= 0x40000000;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Register */
}



void GPIO_SW2EdgeTriggeredInterruptInit(void)
{
    GPIO_PORTF_IS_REG    &= ~(1<<4);      /* PF4 detect edges */
    GPIO_PORTF_IBE_REG   &= ~(1<<4);      /* PF4 will detect a certain edge */
    GPIO_PORTF_IEV_REG   &= ~(1<<4);      /* PF4 will detect a falling edge */
    GPIO_PORTF_ICR_REG   |= (1<<4);       /* Clear Trigger flag for PF4 (Interrupt Flag) */
    GPIO_PORTF_IM_REG    |= (1<<4);       /* Enable Interrupt on PF4 pin */
    /* Set GPIO PORTF priority as 5 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_PRI7_REG = (NVIC_PRI7_REG & GPIO_PORTF_PRIORITY_MASK) | (GPIO_PORTF_INTERRUPT_PRIORITY<<GPIO_PORTF_PRIORITY_BITS_POS);
    NVIC_EN0_REG         |= 0x40000000;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Register */
}

void GPIO_SW3EdgeTriggeredInterruptInit(void)
{
    GPIO_PORTA_IS_REG    &= ~(1<<5);      /* PA5 detect edges */
    GPIO_PORTA_IBE_REG   &= ~(1<<5);      /* PA5 will detect a certain edge */
    GPIO_PORTA_IEV_REG   &= ~(1<<5);      /* PA5 will detect a falling edge */
    GPIO_PORTA_ICR_REG   |= (1<<5);       /* Clear Trigger flag for PA5 (Interrupt Flag) */
    GPIO_PORTA_IM_REG    |= (1<<5);       /* Enable Interrupt on PA5 pin */
    NVIC_PRI0_REG        |= (NVIC_PRI0_REG & GPIO_PORTF_PRIORITY_MASK) | (GPIO_PORTF_INTERRUPT_PRIORITY<<5);
    NVIC_EN0_REG         |= 0x00000001;   /* Enable NVIC Interrupt for GPIO PORTA by set bit number 0 in EN0 Register */
}
