/************* Project 1 ***************
 * File name:	Project1.c
 *
 * Author:	Richard W. Wall
 * Date:	August 10, 2013    - Tested with MPLab X
 * Notes: 	This project converts the binary encoded buttons to
 *              set one of the four Cerebot MX7cK LEDs.
****************************************/

#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project1.h"

int main()
{
    initialize_system();
    
    while(1)
    {
        
        //Run Read_Buttons(void)
        int BTN_Status;
        BTN_Status = read_buttons();
        
        //Pass read_buttons() value to decode_buttons()
        int LED_Code;
        LED_Code = decode_buttons(BTN_Status);
        
        //Pass LED_Code into control_leds()
        control_leds(LED_Code);
        
        
        
        
    }
    return 1;
}

/* initialize_system Function Description *************************************
 * SYNTAX:          void initialize_system();
 * PARAMETER1:      No Parameters
 * KEYWORDS:        initialize
 * DESCRIPTION:     Sets Registers to default (0) to prevent non zero values 
 *                  from popping up.
 * RETURN VALUE:    None
 * END DESCRIPTION ************************************************************/
void initialize_system()
{
    Cerebot_mx7cK_setup(); // Initialize processor board
}

/* read_buttons Function Description *****************************************
 * SYNTAX:          int read_buttons(void);
 * KEYWORDS:        button, read, inputs
 * DESCRIPTION:     Reads the status of the input buttons.  Button status is
 *                  reported for button bit positions only. All other bits in the
 *                  returned value are set to zero as shown below:
 *
 *      Port G Bit position [15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0]
 *      Port G Bit value    [ 0| 0| 0| 0| 0| 0| 0| 0|B2|B1| 0| 0| 0| 0| 0| 0]
 *
 *      B1 will be 1 if BTN1 is pressed otherwise B1 will be zero
 *      B2 will be 1 if BTN2 is pressed otherwise B2 will be zero
 *
 * PARAMETERS:      None
 * RETURN VALUE:    int variable containing button status
 * NOTES:           BTN3 on the Cerebot MX7cK is used in this function.  Adding
 *                  BTN3 detection is an assignment left to the reader.
 *                  Refer to Cerebot MX7cK reference manual.
 * END DESCRIPTION ************************************************************/
int read_buttons(void)
{
    int BTN_Status;
    BTN_Status = PORTG&(BIT_6 | BIT_7);
    return BTN_Status;
}

/* decode_buttons Function Description ***************************************
 * SYNTAX:          int decode_buttons(int);
 * KEYWORDS:        button, decode, LEDs
 * DESCRIPTION:     Maps the button status to light one of four LEDS
 *
 *         Port G Bit position [15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0]
 *         Port G Bit value    [L4|L3|L2|L1| X| X| X| X| X| X| X| X| X| X| X| X]
 *
 *         LED1 = ON for L1 = 1, LED1 = OFF for L1 = 0
 *         LED2 = ON for L2 = 1, LED2 = OFF for L2 = 0
 *         LED3 = ON for L3 = 1, LED3 = OFF for L3 = 0
 *         LED4 = ON for L4 = 1, LED4 = OFF for L4 = 0
 *         X = Don't care
 * 
 * PARAMETER1:      button status
 * RETURN VALUE:    LED 1 through 4 ON/OFF control
 * NOTES:           Refer to Cerebot MX7cK data sheet for button and LED bit
 *                  positions
 * END DESCRIPTION ************************************************************/
int decode_buttons(int buttons)
{
    if(buttons == 0) // No buttons
        return(LED1); // LED_1
    else if(buttons == BTN1) //BTN 1
        return(LED2); //LED_2
    else if(buttons == BTN2) // BTN 2
        return(LED3); // LED_3
    else if(buttons == (BTN1 | BTN2)) //BTN 1 & BTN 2 NOTE: Don't use "&"
        return(LED4); // LED_4
    else
        return(-1); // error none of the four choices
    
}

/* control_leds Function Description *****************************************
 * SYNTAX:          void control_leds(int leds);
 * KEYWORDS:        led, output, lights
 * DESCRIPTION:     lights the LEDs specified the bit positions in the input
 *                  argument.
 * PARAMETER1:      integer LED1 through LED4 ON/OFF control
 * RETURN VALUE:    None.
 * NOTES:	Only the bit positions related to LED1 through LED4 are modified
 *              by this function. The BRD_LEDS mask constant initially clears
 *              all bit LATG positions associated with LED1 through LED4.
 *              The BRD_LEDs mask is also used to limit the Port G bits that
 *              can be modified to thoes in the BRD_LEDS mask field.
 *              The read-modify-write instruction is not atomic.
 *              Refer to Cerebot MX7cK data sheet for LED bit positions
 * END DESCRIPTION **********************************************************/
void control_leds(int leds)
{
    mPORTGClearBits(BRD_LEDS);
    mPORTGSetBits(leds);
}		
	
/* End of Project1.c */
