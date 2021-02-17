; ADC function for rotary
; Lab 6, ECE 266, spring 2018
; Created by Zhao Zhang

; To include names declared in C
 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/adc.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "rotary.h"

					.text
					.global ADCInit
					.global adcRead

KNOB_PERIPH		.field SYSCTL_PERIPH_ADC0
KNOB_PORT		.field ADC0_BASE
adcConfig		.field ADC_TRIGGER_PROCESSOR
adcInterrupt	.field ADC_CTL_IE
adcEnd			.field ADC_CTL_END
adcChannel		.field ADC_CTL_CH2

; void KNOBInit: Initialization

ADCInit				PUSH {LR}

					LDR r0, KNOB_PERIPH
					BL SysCtlPeripheralEnable

					LDR r0, KNOB_PORT
					MOV r1, #0
					LDR r2, adcConfig
					MOV r3, #0
					BL	ADCSequenceConfigure

					LDR r0, KNOB_PORT
					MOV r1, #0
					MOV r2, #0
					LDR r3, adcInterrupt
					LDR r4, adcEnd
					LDR r5, adcChannel
					ORR r3, r3, r4
					ORR r3, r3, r5
					BL ADCSequenceStepConfigure

					LDR r0, KNOB_PORT
					MOV r1, #0
					BL ADCSequenceEnable

					POP {PC}

adcRead				PUSH {LR}

					LDR r0, KNOB_PORT
					MOV r1, #0
					BL ADCProcessorTrigger

while_loop			LDR r0, KNOB_PORT
					MOV r1, #0
					MOV r2, #0
					BL ADCIntStatus
					CMP r0, #0
					BEQ while_loop

					SUB sp, #4

					LDR r0, KNOB_PORT
					MOV r1, #0
					MOV r2, sp
					BL ADCSequenceDataGet

					POP {r0, pc}

