#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project5.h"

#define T1_PRESCALE 1
#define TOGGLES_PER_SEC 1000
#define T1_TICK (FPB/T1_PRESCALE/TOGGLES_PER_SEC)
/* ----- BEGIN: Variables used in loop ----- */
    int stepper_motor_code = 0;
    int BTN_Status = 0;
    int LED_Code = 0;
    int stepper_direction = 0;
    int stepper_mode = 0;
    int stepper_delay = 0;
    int step_delay_counter = 0;
/* ----- END: Variables used in loop ----- */
    
int main()
{
    initialize_system();
    
    
    /* ----- BEGIN: Run at least once to grab initial values ----- */
    //Run Read_Buttons(void)
    BTN_Status = read_buttons();
    //Pass read_buttons() value to decode_buttons()
    LED_Code = decode_buttons(BTN_Status,&stepper_direction,&stepper_mode,&stepper_delay);
    //Pass LED_Code into control_leds()
    control_leds(LED_Code);
    /* ----- END: Run at least once to grab initial values ----- */
    
    step_delay_counter = stepper_delay;
    
    while(1)
    {
        
        
 
        
 
    }
    return 1;
}

void initialize_system()
{
    Cerebot_mx7cK_setup(); // Initialize processor board
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS); /* Set PmodSTEP LEDs outputs */
    LATBCLR = SM_LEDS;
    
    /* ----- BEGIN Timer 1 interrupts ----- */
    // Initialize Timer 1 for 1 ms
    //configure Timer 1 with internal clock, 1:1 prescale, PR1 for 1 ms period
    OpenTimer1(T1_ON | T1_PS_1_1, (T1_TICK-1));
    // set up the timer interrupt with a priority of 2, sub priority 0
    mT1SetIntPriority(2); // Group priority range: 1 to 7
    mT1SetIntSubPriority(0); // Subgroup priority range: 0 to 3
    mT1IntEnable(1); // Enable T1 interrupts
    // Global interrupts must enabled to complete the initialization.
    /* ----- END: Timer 1 Interrupts ----- */
    
    /* ----- BEGIN: CN Interrupts ----- */
    unsigned int dummy; // used to hold PORT read value
    // BTN1 and BTN2 pins set for input by Cerebot header file
    // PORTSetPinsDigitalIn(IOPORT_G, BIT_6 | BIT7); //
    // Enable CN for BTN1 and BTN2
    mCNOpen(CN_ON,(CN8_ENABLE | CN9_ENABLE), 0);
    // Set CN interrupts priority level 1 sub priority level 0
    mCNSetIntPriority(1); // Group priority (1 to 7)
    mCNSetIntSubPriority(0); // Subgroup priority (0 to 3)
    // read port to clear difference
    dummy = PORTReadBits(IOPORT_G, BTN1 | BTN2);
    mCNClearIntFlag(); // Clear CN interrupt flag
    mCNIntEnable(1); // Enable CN interrupts
    // Global interrupts must enabled to complete the initialization.
    /* ----- END: CN Interrupts ----- */
    
    // Enable multi vectored interrupts
    INTEnableSystemMultiVectoredInt(); //done only once

}

void __ISR(_TIMER_1_VECTOR, IPL2) Timer1Handler(void)
{
    //call the timer function for 1 ms
    Timer1_delay(1);
    /* User generated code to service the interrupt is inserted here */
    LATBINV = LEDA; // Toggle LEDA
    step_delay_counter = step_delay_counter - 1;
    if(step_delay_counter == 0)
        {
           //determine the new output code for the stepper motor
           stepper_motor_code = stepper_state_machine(stepper_direction,stepper_mode);
           //push motor code to motor
           output_to_stepper_motor(stepper_motor_code);
           LATBINV = LEDB;
           step_delay_counter = stepper_delay; // reset the counter
        }
   
    mT1ClearIntFlag(); // Macro function to clear the interrupt flag
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL1) CNIntHandler(void)
{
    /* User ISR code inserted here */
    LATBINV = LEDC;
    //delay 20 ms debounce period
    hw_msDelay(20);
    LATBINV = LEDC;
    //Run Read_Buttons(void)
    BTN_Status = read_buttons();
    //Pass read_buttons() value to decode_buttons()
    LED_Code = decode_buttons(BTN_Status,&stepper_direction,&stepper_mode,&stepper_delay);
    //Pass LED_Code into control_leds()
    control_leds(LED_Code);
    /* Required to clear the interrupt flag in the ISR */
    mCNClearIntFlag(); // Macro function
}


int read_buttons(void)
{
    int BTN_Status;
    BTN_Status = PORTG&(BIT_6 | BIT_7);
    return BTN_Status;
}

int decode_buttons(int buttons,int* stepper_direction,int* stepper_mode,int* stepper_delay)
{
    const int Half_Step = 2;
    const int Full_Step = 1;
    
    if(buttons == 0) // No buttons
    {
        *stepper_direction = CW; //clockwise
        *stepper_mode = HS; // half step @ 15 RPM
        *stepper_delay = ((60000)/(15*100*Half_Step)); // number of ms Eq 1
        return(LED1); // LED_1
    }
    else if(buttons == BTN1) //BTN 1
    {
        *stepper_direction = CW; //clockwise
        *stepper_mode = FS; // FuLL step @ 15 RPM
        *stepper_delay = ((60000)/(15*100*Full_Step)); // number of ms
        return(LED2); //LED_2
    }
    else if(buttons == BTN2) // BTN 2
    {
        *stepper_direction = CCW; //counter clockwise
        *stepper_mode = HS; // half step @ 10 RPM for 100 steps
        *stepper_delay = ((60000)/(10*100*Half_Step)); // number of ms
        return(LED3); // LED_3
    }
    else if(buttons == (BTN1 | BTN2)) //BTN 1 & BTN 2 NOTE: Don't use "&"
    {
        *stepper_direction = CCW; //counter clockwise
        *stepper_mode = FS; // Full step @ 25 RPM
        *stepper_delay = ((60000)/(25*100*Full_Step)); // number of ms
        return(LED4); // LED_4
    }
    else
        return(-1); // error none of the four choices
    
}

int stepper_state_machine(int stepper_direction,int stepper_mode)
{
    enum{S0_5 = 0,S1,S1_5,S2,S2_5,S3,S3_5,S4}; // Declaration of states
    const unsigned int sm_code[] = {0x0A,0x08,0x09,0x01,0x05,0x04,0x06,0x02}; // stepper motor control codes
    static unsigned int pstate;
    switch (pstate) // Next state logic
    {
        case S0_5:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S1;
                else // Full step
                    pstate =S1_5;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S4;
                else // Full step
                    pstate = S3_5;
            }
            break;
            
        case S1:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S1_5;
                else // Full step
                    pstate =S2;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S0_5;
                else // Full step
                    pstate = S4;
            }
            break;
            
        case S1_5:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S2;
                else // Full step
                    pstate =S2_5;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S1;
                else // Full step
                    pstate = S0_5;
            }
            break;
            
        case S2:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S2_5;
                else // Full step
                    pstate =S3;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S1_5;
                else // Full step
                    pstate = S1;
            }
            break;
            
        case S2_5:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S3;
                else // Full step
                    pstate =S3_5;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S2;
                else // Full step
                    pstate = S1_5;
            }
            break;
            
        case S3:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S3_5;
                else // Full step
                    pstate =S4;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S2_5;
                else // Full step
                    pstate = S2;
            }
            break;
            
        case S3_5:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S4;
                else // Full step
                    pstate =S0_5;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S3;
                else // Full step
                    pstate = S2_5;
            }
            break;
            
        case S4:
            if (stepper_direction == CW)
            {
                if (stepper_mode == HS) 
                    pstate = S0_5;
                else // Full step
                    pstate =S1;
            }
            else // Counterclockwise
            {
                if (stepper_mode == HS) 
                    pstate = S3_5;
                else // Full step
                    pstate = S3;
            }
            break;
    }
return sm_code[pstate];
}

void output_to_stepper_motor(int stepper_code)
{
    int temp,new_data;
    temp = LATB; // grab whats in PORTB
    new_data = stepper_code;
    new_data <<= 7; //shift left by 7
    temp = temp & ~(SM1 | SM2 | SM3 | SM4); //clears the bits SM1 through SM4
    temp = temp | new_data; // modifies temp to contain new output code
    LATB = temp; // write back out to LATB
    
}

void control_leds(int leds)
{
    mPORTGClearBits(BRD_LEDS);
    mPORTGSetBits(leds);
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
    }
}

void hw_msDelay(unsigned int mS)
{
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();         // Read core timer count - SW Start breakpoint
    tWait = (CORE_MS_TICK_RATE * mS); // Set time to wait
    while ((ReadCoreTimer() - tStart) < tWait);  // Wait for the time to pass
    LATBINV = LEDA; // Toggle LED at end of delay period
}
	
