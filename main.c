////////////////////////////////////////////////////////////////////////////////////
// ECE 2534:        Lab 2
// Name:            John Mert
// File name:       main.c
// Description:     Uses Sonar to calculate distance
// Written by:      CDP (and modified by ALA, WTB, HZ, JST)
// Last modified:   01/20/2017

// Includes

#include <stdio.h>
#include <plib.h>
#include "PmodOLED.h"
#include "OledChar.h"
#include "OledGrph.h"
#include "delay.h"
#include "Digilent_config.h"
#include "myUART.h"

// Defines
#define PBCLK_F 10000000
#define BAUD_RATE 9600
#define _PLIB_DISABLE_LEGACY  // The use of legacy code is not allowed in this course.
#define BUTTON1 (1 << 6)
#define BUTTON1BIT 6
#define BUTTON2 (1 << 7)
#define BUTTON2BIT 7
#define LEDS    (0xF << 12)

// Global variables (CAPITALIZED)

// Prototypes
void initGPIO();
void setLEDs(int curr, int lights);
void initTimer();

int main()
{
    
    int lights = 1;//Variables for values
    int max = 0;
    int min = 255;
    int curr = 0;
    
    int button1cur = 0;//button1 checkers
    int button1prev = 0;     
    int button1pressed = 0;
    
    int button2cur = 0;//button2 checkers
    int button2prev = 0;     
    int button2pressed = 0;
 
    char maxs[6]="Max: ";//strings for the terminal
    char mins[6]="Min: ";
    char curs[11]="Distance: ";
    char new[5]="\n\r";
    char smh[11]=" ";
    char asc[5];
    char maxvalue[4];
    char minvalue[4];
    char currvalue[4];
    
    // Initialize everything
    initGPIO();
    initTimer();
    initUART(UART1, PBCLK_F, BAUD_RATE);
    initUART(UART2, PBCLK_F, BAUD_RATE);
   
    // Main processing loop
    while(1)
    {
        // Tasks that must be done continuously go here.
        //receiving sonar data
        int a = 0;
        while(a<5)
        {
           asc[a]=UARTReceiveByte(UART2);
           a++;
        }
        
        int y = asc[1] - '0';//decoding sonar data
        int o = asc[2] - '0';
        int b = asc[3] - '0';
        curr = y*100 + o*10 + b;
        
        // button1cur is 1 when pushed and 0 when not pushed
        if ((PORTG & BUTTON1) >> BUTTON1BIT)
            button1cur = 1;             
        else                 
            button1cur = 0;                         
        // button1cur = ((PORTG & BUTTON1) >> BUTTON1BIT);
        button1pressed = ((!button1cur) && (button1prev));
        button1prev = button1cur;
        if(button1pressed){                         
            max = 0;                  
        }
        
        // button2cur is 1 when pushed and 0 when not pushed
        if ((PORTG & BUTTON2) >> BUTTON2BIT)
            button2cur = 1;             
        else                 
            button2cur = 0;                         
        // button2cur = ((PORTG & BUTTON2) >> BUTTON2BIT);
        button2pressed = ((!button2cur) && (button2prev));
        button2prev = button2cur;
        if(button2pressed){                         
            min = 255;                  
        }
        
        
        if(curr > max)//Adjusting max and min
        {
            max = curr;
        }
        if(curr < min)
        {
            min = curr;
        }
        
        setLEDs(curr,lights);// Setting up LEDS
        
        /////////Terminal Message////////////
        if(INTGetFlag(INT_T2))   //500 ms indicator    
        {
            INTClearFlag(INT_T2);    // Clear the INTERRUPT flag so we don't respond until it sets again
            
            sprintf(maxvalue, "%d", max);//printing max min and current to the terminal
            sprintf(minvalue, "%d", min);
            sprintf(currvalue, "%d", curr);
            UARTSendString(UART1,curs);
            UARTSendString(UART1,currvalue);
            UARTSendString(UART1,smh);
            UARTSendString(UART1,maxs);
            UARTSendString(UART1,maxvalue);
            UARTSendString(UART1,smh);
            UARTSendString(UART1,mins);
            UARTSendString(UART1,minvalue); 
            UARTSendString(UART1,new);
        }
    }
    return 0;
}

void initGPIO()
{
    // Initialize GPIO
    TRISGSET = BUTTON1 | BUTTON2;   // Inputs
    TRISGCLR = LEDS;                // Outputs
}



void setLEDs(int curr, int lights)
{
    if(curr >63)// >63
        {
            lights = 1;
            PORTGCLR = LEDS;
            PORTGSET = (lights << 12);
            PORTGSET = (lights << 13);
            PORTGSET = (lights << 14);
            PORTGSET = (lights << 15);
        }
        if(curr<64 && curr>31)// 64>x>31
        {
            lights = 1;
            PORTGCLR = LEDS;
            PORTGSET = (lights << 12);
            PORTGSET = (lights << 13);
            PORTGSET = (lights << 14);
        }
        if(curr<32 && curr>15)//32>x>15
        {
            lights = 1;
            PORTGCLR = LEDS;
            PORTGSET = (lights << 12);
            PORTGSET = (lights << 13);
        }
        if(curr<16 && curr>7)// 16>x>7
        {
            lights = 1;
            PORTGCLR = LEDS;
            PORTGSET = (lights << 12);
        }
        if(8>curr)// 8>x
        {
            lights = 1;
            PORTGCLR = LEDS;
        }
}
void initTimer()
{
    // Set up Timer2 to roll over every 0.1 second
    // Rollover frequency = 10MHz/(64 * 15625) = 10 Hz
    //T2_ ON TURN ON THE TIMER
    //T2_IDLE_CON CONTINUE COUNTING WHEN PROC IS IDLE
    //T2_SOURCE_INT MEANS THE CLOCK SOURCE COMES FROM INSIDE THE CHIP
    //T2_PS_1_64 CLOCK DIVIDER SCALE
    //T2_GATE_OFF ITS NOT GATED
    //LAST NUMBER IS THE PERIOD REGISTER VALUE PRx
    OpenTimer2(T2_ON | T2_IDLE_CON | T2_SOURCE_INT | T2_PS_1_256 | T2_GATE_OFF, 19529);

    // Clear Timer2 (INTERRUPT) rollover flag
    INTClearFlag(INT_T2);
}
