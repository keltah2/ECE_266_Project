; pwmled.asm: Assembly PWM-LED functions
; Lab 8, ECE 266, spring 2018
; Created by Zhao Zhang

; Include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","inc/hw_ints.h","driverlib/sysctl.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","launchpad.h","pwmbuzzer.h"
                .text

; Field and any other declarations
WTIMER0   		.field  SYSCTL_PERIPH_WTIMER0
TIMER0BASE     .field  WTIMER0_BASE
PORTC           .field  SYSCTL_PERIPH_GPIOC
PIN_ROUTE_4     .field  GPIO_PC4_WT0CCP0
PORTC_BASE      .field  GPIO_PORTC_BASE
TIMERA		    .field	TIMER_A

                .asmfunc
pwmBuzzInit      PUSH    {lr}

				;//Enable the GPIO port peripheral
                LDR     r0, PORTC
                BL      SysCtlPeripheralEnable

				; Enable Wide Timer 0
                LDR     r0, WTIMER0
                BL      SysCtlPeripheralEnable

				; Connect pins to those timers
                LDR     r0, PORTC_BASE
                MOV     r1, #(GPIO_PIN_4)
                BL      GPIOPinTypeTimer

                LDR     r0, PIN_ROUTE_4
                BL      GPIOPinConfigure ; double check

 				; Select PWM for Wide Timer 0 sub-Timer A
                LDR     r0, TIMER0BASE
                MOV     r1, #TIMER_CFG_SPLIT_PAIR
                MOV     r2, #TIMER_CFG_A_PWM
                ORR     r1, r2
                BL      TimerConfigure

				; The default setting is very bright, so change it to low density
                LDR     r0, TIMER0BASE
                LDR     r1, TIMERA
                MOV     r2, #100
                BL      TimerLoadSet

                LDR     r0, TIMER0BASE
                LDR     r1, TIMERA
                MOV     r2, #99
                BL      TimerMatchSet

				; Enable the Wide Timer 0's TimerA
                LDR     r0, TIMER0BASE
                LDR     r1, TIMERA
                BL      TimerEnable

                POP     {pc}
                .endasmfunc

pwmBuzzSetIntensity .asmfunc
				PUSH    {lr}


                MOV	r2, r0
                MOV r3, r1
                LDR r0, TIMER0BASE
                LDR r1, TIMERA
                BL  TimerLoadSet

                SUB r2, r3
                LDR r0, TIMER0BASE
                LDR r1, TIMERA
                BL  TimerMatchSet

                POP     {pc}
                .endasmfunc

