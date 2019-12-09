/******************************** Project 2 ****************************
*
* File: Project2.c
* Author name: Richard Wall
* Rev. Date: May 22, 2013
*
* Project Description: The purpose of this project is to investigate
* the characteristics and limitations of two types of polling delay.
*
**********************************************************************/
#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project2.h"
int main()
{
    int mS = 1;    /* Set total delay time – change as needed */
    system_init(); /* Setup system Hardware. */
    while (1)
    {
        LATBINV = LEDB; /* Toggle LEDB each delay period */
        /* Run with only one of the two following statements uncommented */
        sw_msDelay(mS); /* Software only delay */
        // hw_msDelay(mS); /* Hardware-assisted delay */
    }
    return 0; /* Returning a value is expected but this statement
* should never execute */
}
/* system_init FUNCTION DESCRIPTION **************************************
* SYNTAX: void system_init (void);
* KEYWORDS: initialization system hardware
* DESCRIPTION: Sets up the configuration for Port B to control LEDA
* - LEDH.
* RETURN VALUE: none
* END DESCRIPTION *********************************************************/
void system_init(void)
{
    // Setup processor board
    Cerebot_mx7cK_setup();
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS); /* Set PmodSTEP LEDs outputs */
    LATBCLR = SM_LEDS;                        /* Turn off LEDA through LEDH */
}
/* sw_msDelay (mS) Function Description **********************************
* SYNTAX: void sw_ms_delay(unsigned int mS);
* DESCRIPTION: This is a millisecond delay function that will repeat
* a specified number of times. The constant "COUNTS_PER_MS"
* must be calibrated for the system frequency.
* KEYWORDS: delay, ms, milliseconds, software delay
* PARAMETER1: mS - the total number of milliseconds to delay
* RETURN VALUE: None:
* Notes: The basic loop counter "COUNTS_PER_MS " is dependent on
* the CPU frequency. LEDA will toggle at 500 Hz.
*END DESCRIPTION *********************************************************/
void sw_msDelay(unsigned int mS)
{
    /* Use code from Listing 3 */
}
/*hw_msDelay Function Description ******************************************
* SYNTAX: void hw_msDelay(unsigned int mS);
* DESCRIPTION: This is a millisecond delay function uses the core time
* to set the base millisecond delay period. Delay periods
* of zero are permitted. LEDA is toggled each millisecond.
* KEYWORDS: delay, ms, milliseconds, software delay, core timer
* PARAMETER1: mS - the total number of milliseconds to delay
* RETURN VALUE: None:
* END DESCRIPTION *********************************************************/
void hw_msDelay(unsigned int mS)
{
    /* Use code from Listing 4 */
}
// End of Project2.c