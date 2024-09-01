/*
 * adc.c
 *
 *  Created on: Aug 21, 2024
 *      Author: youssef
 */


#include "adc.h"
#include "tm4c123gh6pm_registers.h"
#include "sensors.h"


extern uint8 driver_sens_reading ;                  // variable for temperature reading sensor 1
extern uint8 passenger_sens_reading ;               // variable for temperature reading sensor 2


void ADCinit(void){
    // enable clock for ADC module 0 and wait till the module to start
    SYSCTL_RCGCADC_REG   |= 0x1;
    while (!(SYSCTL_PRADC_REG & 0x01));
    ADC0_ADCACTSS  |= (1<<0) ;                         // Enable sequencer 0 to be active.
    // ADC0_ADCIM     |= (1<<0);                       // Activate sequencer zero interrupts.
    ADC0_ADCEMUX   &= 0xFFFFFFF0 ;                     // clear the first 4 bits to make the trigger source of ADC is the processor.
    ADC0_ADCSSMUX0 |= 0x00000021 ;                     // Make the Sample number 0 to PE2(AIN1) AND sample number 2 to PE1(AIN2).
    ADC0_ADCSSCTL0 |= (1<<5);                          //  make sample one the last sample.
    ADC0_ADCPC      =(ADC0_ADCPC & 0) | 0x01;          // Configure the sample rate to 125 ksps.
    //NVIC_EN0_REG   |= (1<<14) ;                      // Enable the interrupt for ADC module 0 sequence zero.
    //NVIC_PRI3_REG  &= ~(0b111<<13)                   // Clear the 13 :15 bits in the register
    //NVIC_PRI3_REG  |= (ADC_INTERRUPT_PRIORITY<<13);  // Set the Priority for the ADC interrupt to 4 higher than the Button Interrupt

}
void ADCTrigger(void){
    ADC0_ADCPSSI |= 0x01 ;                               // Trigger sampling of sequencer 0.

}

/*
void ADCFinishedHandler(void){
    ADC0_ADCRIS |= (1<<0);                                                        // Clear sequencer 0 Interrupt flag
    driver_sens_reading = (ADC0_ADCSSFIFO0   / 4095) * SENSOR_MAX_VALUE;          // Read the First Sample which from AIN1 (PE2) driver sensor
    passenger_sens_reading = (ADC0_ADCSSFIFO0   / 4095) * SENSOR_MAX_VALUE;       // Read the second sample which is from AIN2 (PE1) passenger sensor

}
 */
