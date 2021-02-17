/*
 * rotary.c
 *
 *  Created on: Mar 11, 2018
 *      Author: meshal
 */



#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include "launchpad.h"
#include "rotary.h"
#include <driverlib/adc.h>


void ADCInit() {
    // Enable the ADC0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    //
    // Wait for the ADC0 module to be ready.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
    { }
    // Configure ADC sequencer #0
    ADCSequenceConfigure(ADC0_BASE, 0 /* sequencer #0 */,
                         ADC_TRIGGER_PROCESSOR, 0 /* priority */);
    // Configure a single step of the sequencer
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0 /* step */,
                             ADC_CTL_IE /* set interrupt flag */
                             | ADC_CTL_END /* mark the last step */
                             | ADC_CTL_CH6 /* ADC channel 2 */); //J9 on groove
    // Enable the ADC sequencer
    ADCSequenceEnable(ADC0_BASE, 0 /* sequencer 0*/);
}




uint16_t adcRead()
{
    uint32_t adcReading;
    // Trigger ADC sample sequence
    ADCProcessorTrigger(ADC0_BASE, 0 /* sequencer #0 */);
    // Wait until the sample sequence has completed
    while (!ADCIntStatus(ADC0_BASE, 0 /* sequencer #0 */,
                         false /* no interrupt mask */)) {

    }
    // Read ADC 0
    ADCSequenceDataGet(ADC0_BASE, 0 /* sequencer #0 */,
                       &adcReading);
    return (uint16_t) adcReading;
}
