/*
 * main.c for ECE 266 Project
 *
 *  Created on: Aug 23, 2016
 *  Last updated on: Sep 19, 2017
 *      Author: zzhang
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "rotary.h"
#include "pwmbuzzer.h"

//Global variables
int count = 0; // makes sure song doesn't start playing right away when pressing SW2 first time
static int i = 0; //cycles through song's array
uint16_t position; //ADC reading
int currentSong; //saves song choice of user

// Display mode of the 7-segment display
// display starts in option mode and paused song state
enum {Percent, Option}  sysState = Option ;
static enum {Run, Pause} songState = Pause;

// A3 = 1 C4 = 2 C#4 = 3, D4 = 4, D#4 = 5, E4 = 6,
// F4 = 7, F#4 = 8, G4 = 9, G#4 = 10, A4 = 11, A#4 = 12, B4 = 13
// G3 = 14, B3 = 15, F3 = 16,  F3# = 17, no note = 18

//Buzzer intensity table
pwm_buzz intensityLvlTable[] = {
  {227227,10000}, {190840 , 10000}, {180505, 10000}, {170068, 10000}, {160772, 10000},
  {151515, 10000}, {143266, 10000}, {135135, 10000}, {127551, 10000}, {120481, 10000},
  {113636, 10000}, {107296, 10000}, {101214, 10000}, {255102, 10000}, {202429, 10000},
  {285714, 10000}, {270270,10000}, {100, 1}
};

// Songs:
// 5) Ariana Grande - One Last Time
// 4) Pirates of the Caribbean Theme
// 3) Europe - Final Countdown
// 2) Smash Mouth - All Star
// 1) Mortal Kombat Theme
// 0) AC/DC - Thunderstruck

int notes5 [] = {2, 2, 2, 2, 4, 4, 4, 6, 6, 4, 4, 2, 7, 6, 6, 1, 1, 1, 14, 1, 1, 1, 14, 14, 2, 2, 6, 4, 4, 4};
int notes4 [] = {4, 4, 4, 6, 7, 7, 7, 9, 6, 6, 6, 4,2, 4, 4, 4, 2, 4, 4, 4, 6, 7, 7, 7, 9, 6, 6, 4,2, 4, 4, 4, 2, 4, 4, 4, 7, 9,9,9, 11, 12, 12, 11, 9, 11, 11, 11, 11, 7, 7, 9, 9, 11, 11, 11, 11, 6,6, 4,2, 4, 4, 4, 2};
int notes3 [] = {3, 15, 3, 3,  17, 17, 17, 17, 4, 3, 4, 3, 15, 15, 15, 15, 4, 3, 4, 4, 17, 17, 17, 17, 15, 1, 15, 1, 10, 15, 11, 11, 11, 11};
int notes2 [] = {14,14, 4, 15, 15, 1,1, 14, 14, 2, 2, 15, 15, 1, 1, 14, 14, 14, 4, 15, 15, 1, 1, 9, 9, 6,6,4,4,4, 4, 4};
int notes1 [] = {1,1,2,1,4,2,6,4,2,2,6,2,9,2,6,2,14,14,15,14,2,14,4,2,16,16,2,16,2,16,2,15};
int notes0 [] = {5, 13, 8, 13, 5, 13, 8, 13, 5, 13, 8, 13, 5, 13, 8, 13, 6, 13, 9, 13, 6, 13, 9, 13, 6, 13, 9, 13, 6, 13, 9, 13};



uint8_t a = 0;
uint8_t b = 0;
uint8_t c = 0;
uint8_t d = 0;

// The initial state of the 7-segment display: "0000"
seg7Display_t seg7Display = {
    0, 0, 0, 0
};
void volumeControl(uint32_t time){

    int cycle = i;
    int arraySize;
    //static int cycle = 0;

    schdCallback(volumeControl, time + 250);

    uint32_t volume = position * 100;

    if (songState == Run){
        if(volume < 1){
              pwmBuzzSetIntensity(intensityLvlTable[17].pwmPeriod,intensityLvlTable[17].pwmDutyCycle);
            }
        else{
            //intensityLvlTable[notes1[cycle]-1].pwmDutyCycle = volume;
            if (currentSong == 0){
               intensityLvlTable[notes0[cycle]-1].pwmDutyCycle = volume;
               //intensityLvlTable[notes0[cycle]-1].pwmDutyCycle = volume;
                arraySize = sizeof(notes0) / sizeof(notes0[0]) - 1;
                    pwmBuzzSetIntensity(intensityLvlTable[notes0[cycle]-1].pwmPeriod,intensityLvlTable[notes0[cycle]-1].pwmDutyCycle);
            }
            else if (currentSong == 1){
                intensityLvlTable[notes1[cycle]-1].pwmDutyCycle = volume;
                arraySize = sizeof(notes1) / sizeof(notes1[0]) - 1;
                pwmBuzzSetIntensity(intensityLvlTable[notes1[cycle]-1].pwmPeriod,intensityLvlTable[notes1[cycle]-1].pwmDutyCycle);
            }
            else if (currentSong == 2){
                            intensityLvlTable[notes2[cycle]-1].pwmDutyCycle = volume;
                            arraySize = sizeof(notes2) / sizeof(notes2[0]) - 1;
                            pwmBuzzSetIntensity(intensityLvlTable[notes2[cycle]-1].pwmPeriod,intensityLvlTable[notes2[cycle]-1].pwmDutyCycle);
                        }
            else if (currentSong == 3){
                            intensityLvlTable[notes3[cycle]-1].pwmDutyCycle = volume;
                            arraySize = sizeof(notes3) / sizeof(notes3[0]) - 1;
                            pwmBuzzSetIntensity(intensityLvlTable[notes3[cycle]-1].pwmPeriod,intensityLvlTable[notes3[cycle]-1].pwmDutyCycle);
                        }
            else if (currentSong == 4){
                            intensityLvlTable[notes4[cycle]-1].pwmDutyCycle = volume;
                            arraySize = sizeof(notes4) / sizeof(notes4[0]) - 1;
                            pwmBuzzSetIntensity(intensityLvlTable[notes4[cycle]-1].pwmPeriod,intensityLvlTable[notes4[cycle]-1].pwmDutyCycle);
                        }
            else if (currentSong >= 5){
                            intensityLvlTable[notes5[cycle]-1].pwmDutyCycle = volume;
                            arraySize = sizeof(notes5) / sizeof(notes5[0]) - 1;
                            pwmBuzzSetIntensity(intensityLvlTable[notes5[cycle]-1].pwmPeriod,intensityLvlTable[notes5[cycle]-1].pwmDutyCycle);
                        }
            if(cycle == arraySize)
                    cycle = 0;
                else
                    cycle++;
       }
    }
}

// Callback function for updating display
void pwmBuzzerPlay(uint32_t time)
{
    //static int i = 0;

    int arraySize;

    // Pre-schedule the next callback with 70 ms delay
        schdCallback(pwmBuzzerPlay, time + 250);

    // Return immediately if system is paused
    if (songState == Pause || position == 0)
    {
        pwmBuzzSetIntensity(intensityLvlTable[17].pwmPeriod,intensityLvlTable[17].pwmDutyCycle ); // mutes the volume from the buzzer if paused)
        return;
    }
    // Cycles through the notes of the song
    //
    if (currentSong == 0){
        /*if (notes0[i]-1 == 17){
                           intensityLvlTable[notes0[i]-1].pwmDutyCycle = 1;
                           intensityLvlTable[notes0[i]-1].pwmPeriod = 100;
                       }*/
        arraySize = sizeof(notes0) / sizeof(notes0[0]) - 1;
        //if (notes0[i] != 17)
        pwmBuzzSetIntensity(intensityLvlTable[notes0[i]-1].pwmPeriod,intensityLvlTable[notes0[i]-1].pwmDutyCycle);
        //else
          //  pwmBuzzSetIntensity(intensityLvlTable[16].pwmPeriod,intensityLvlTable[16].pwmDutyCycle);
    }
    else if (currentSong == 1){
        arraySize = sizeof(notes1) / sizeof(notes1[0]) - 1;
        pwmBuzzSetIntensity(intensityLvlTable[notes1[i]-1].pwmPeriod,intensityLvlTable[notes1[i]-1].pwmDutyCycle);
    }
    else if (currentSong == 2){
            arraySize = sizeof(notes2) / sizeof(notes2[0]) - 1;
            pwmBuzzSetIntensity(intensityLvlTable[notes2[i]-1].pwmPeriod,intensityLvlTable[notes2[i]-1].pwmDutyCycle);
    }
    else if (currentSong == 3){
            arraySize = sizeof(notes3) / sizeof(notes3[0]) - 1;
            pwmBuzzSetIntensity(intensityLvlTable[notes3[i]-1].pwmPeriod,intensityLvlTable[notes3[i]-1].pwmDutyCycle);
    }
    else if (currentSong == 4){
            arraySize = sizeof(notes4) / sizeof(notes4[0]) - 1;
            pwmBuzzSetIntensity(intensityLvlTable[notes4[i]-1].pwmPeriod,intensityLvlTable[notes4[i]-1].pwmDutyCycle);
    }
    else if (currentSong >= 5){
            arraySize = sizeof(notes5) / sizeof(notes5[0]) - 1;
            pwmBuzzSetIntensity(intensityLvlTable[notes5[i]-1].pwmPeriod,intensityLvlTable[notes5[i]-1].pwmDutyCycle);
    }
    if(i == arraySize){
        i = 0;
    }
    else
        i++;
}


void
displayUpdate(uint32_t time)                          // The scheduled time
{

    position = adcRead();
    position = (position * 100) / 4095;
    if (sysState == Percent) {
        if((position / 10) == 10)
        {
            //seg7Display.d4 = 0; //leftmost
            seg7Display.d3 = 1; // third from right
            seg7Display.d2 = 0; //second from right
            seg7Display.d1 = 0; //rightmost
        }
        else{
            c = position / 10;
            d = position % (c*10);
            seg7Display.d1 = d;
            seg7Display.d2 = c;
            seg7Display.d3 = 0;
            seg7Display.d4 = 0;
        }
}

    else if(sysState == Option) {
          if (position > 50){
              seg7Display.d1 = 5;
              seg7Display.d2 = 0;
              //seg7Display.d3 = 0;
          }
          else if (position < 50){
              d = position / 10;
              seg7Display.d1 = d;
              seg7Display.d2 = 0;
          }
        }

    seg7DigitUpdate(&seg7Display);

    // Call back after 200 milliseconds
    schdCallback(displayUpdate, time + 200);
}

// Callback function for checking push button.
void
checkPushButton(uint32_t time)
{
    int code = pbRead();
    uint32_t delay; //changed to = 10

    switch (code) {
    case 1: //SW1
        // set display mode to percent
        //sysState2 = Run;
        //sysState = Percent;
        sysState = Option;
        songState = Pause;
        i = 0; //makes sure to reset the song to starting point
        count = 0; //makes sure song doesn't start playing right away when pressing SW2 first time
        delay = 250;                // software debouncing
        break;

    case 2:             // SW2
        if (sysState == Option){
            currentSong = position / 10; //saves song # of user choice
            sysState = Percent;}
        if (sysState == Percent){
                if(songState == Pause && count > 0)
                    songState = Run;
                else
                    songState = Pause;
        }
            count = 1;
            delay = 250;
            break;


    default:
        delay = 10;
    }

    schdCallback(checkPushButton, time + delay);
}

int main(void)
 {
    lpInit();
    ADCInit();
    seg7Init();
    pwmBuzzInit();

    schdCallback(checkPushButton,1005);
    schdCallback(displayUpdate, 1010);
    schdCallback(volumeControl, 1015);
    schdCallback(pwmBuzzerPlay, 1020);

    while(true)
    {
        schdExecute();
    }

}
