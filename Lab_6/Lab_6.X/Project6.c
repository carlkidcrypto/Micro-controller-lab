#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project6.h"
#include "LCD.h"

/* ----- BEGIN: Timer 1 ----- */
#define T1_PRESCALE 1
#define TOGGLES_PER_SEC 1000
#define T1_TICK (FPB/T1_PRESCALE/TOGGLES_PER_SEC)
/* ----- END: timer 1 ----- */

int main()
{
    initialize_system();
    char string1[] = "Does Dr J prefer PIC32 or FPGA??"; 
    char string2[] = "Answer: \116\145\151\164\150\145\162\041"; 
    
    while(1)
    {
        LCD_puts(string1);
        Timer1_delay(5000);
        LCD_cls();
        LCD_puts(string2);
        Timer1_delay(5000);
        LCD_cls();
    }
    return 1;
}



void initialize_system()
{
    Cerebot_mx7cK_setup(); // Initialize processor board
    // Initialize Timer 1 for 1 ms
    OpenTimer1(T1_ON | T1_PS_1_1, (T1_TICK-1));
    initialize_LCD(); // Initialize the LCD
}

void initialize_LCD()
{
    initialize_PMP();
    // Page 11
    Timer1_delay(30);
    writeLCD(0,0x38); // RS and then data
    Timer1_delay(50);
    writeLCD(0,0x0f);
    Timer1_delay(50);
    writeLCD(0,0x01);
    Timer1_delay(5);   
}

void initialize_PMP()
{
    int cfg1 = PMP_ON|PMP_READ_WRITE_EN|PMP_READ_POL_HI|PMP_WRITE_POL_HI;
    int cfg2 = PMP_DATA_BUS_8 | PMP_MODE_MASTER1 |
    PMP_WAIT_BEG_1 | PMP_WAIT_MID_2 | PMP_WAIT_END_1;
    int cfg3 = PMP_PEN_0; // only PMA0 enabled
    int cfg4 = PMP_INT_OFF; // no interrupts used
    mPMPOpen(cfg1, cfg2, cfg3, cfg4); 
    // see page 15-16 of Project6.pdf
    
}

/* START FUNCTION DESCRIPTION ********************************************
Timer1_delay
SYNTAX: void Timer1_delay(int delay);
KEYWORDS: Millisecond, hardware delay, Timer A, multirate timer
DESCRIPTION: Delays the number of milliseconds specified by the passed
argument, delay..
Parameter 1: int - number of ms to delay
RETURN: None
END DESCRIPTION **********************************************************/
void Timer1_delay(int delay)
{
    while(delay--)
    {
        while(!mT1GetIntFlag()); // Wait for interrupt flag to be set
        mT1ClearIntFlag(); // Clear the interrupt flag
        LATBINV = LEDA; // Toggle LEDA
    }
}
	
