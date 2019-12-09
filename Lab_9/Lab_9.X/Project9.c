#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project9.h"
#include "LCD.h"
#include "pwm.h"
#define INIT_DUTY_CYCLE 40
#define INIT_DUTY_CYCLE_FREQ 1000

/* ----- BEGIN: Variables used in loop ----- */
    int BTN_Status = 0;
    int LED_Code = 0;
/* ----- END: Variables used in loop ----- */
int main(void)
{
    initialize_system();
    //Run Read_Buttons(void)
    BTN_Status = read_buttons();
    LED_Code = decode_buttons(BTN_Status);
    control_leds(LED_Code);
    
    while(1)
    {
        
    }
    
}

int read_buttons(void)
{
    int BTN_Status;
    BTN_Status = PORTG&(BIT_6 | BIT_7);
    return BTN_Status;
}

int decode_buttons(int buttons)
{
 
    if(buttons == 0) // No buttons
    {
        // %40
        LCD_cls();
        LCD_puts("PWM: 40");
        pwm_set(40);
        return(LED1); // LED_1
    }
    else if(buttons == BTN1) //BTN 1
    {
        // %65 
        LCD_cls();
        LCD_puts("PWM: 65");
        pwm_set(65);
        return(LED2); //LED_2
    }
    else if(buttons == BTN2) // BTN 2
    {
        // %80
        LCD_cls();
        LCD_puts("PWM: 80");
        pwm_set(80);
        return(LED3); // LED_3
    }
    else if(buttons == (BTN1 | BTN2)) //BTN 1 & BTN 2 NOTE: Don't use "&"
    {
        // %95
        LCD_cls();
        LCD_puts("PWM: 95");
        pwm_set(95);
        return(LED4); // LED_4
    }
    else
        return(-1); // error none of the four choices
    
}

void control_leds(int leds)
{
    mPORTGClearBits(BRD_LEDS);
    mPORTGSetBits(leds);
}

void initialize_system() {
   Cerebot_mx7cK_setup(); // Initialize processor board
   // Set buttons as inputs
   PORTSetPinsDigitalIn(IOPORT_G, BIT_6 | BIT_7);
   PORTSetPinsDigitalOut(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15); 
   PORTClearBits(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15); 
   PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS); /* Set PmodSTEP LEDs outputs */
   LATBCLR = SM_LEDS;
   pwm_init(INIT_DUTY_CYCLE,INIT_DUTY_CYCLE_FREQ);
   init_CN_Interrupts();
   // Enable multi vectored interrupts
   INTEnableSystemMultiVectoredInt(); //done only once
   Initialize_LCD();
   
}

void Initialize_LCD() {
    Initialize_PMP();
    // Page 11
    hw_msDelay(30);
    writeLCD(0, 0x38); // RS and then data
    hw_msDelay(50);
    writeLCD(0, 0x0f);
    hw_msDelay(50);
    writeLCD(0, 0x01);
    hw_msDelay(5);
}

void Initialize_PMP() {
    int cfg1 = PMP_ON | PMP_READ_WRITE_EN | PMP_READ_POL_HI | PMP_WRITE_POL_HI;
    int cfg2 = PMP_DATA_BUS_8 | PMP_MODE_MASTER1 |
            PMP_WAIT_BEG_1 | PMP_WAIT_MID_2 | PMP_WAIT_END_1;
    int cfg3 = PMP_PEN_0; // only PMA0 enabled
    int cfg4 = PMP_INT_OFF; // no interrupts used
    mPMPOpen(cfg1, cfg2, cfg3, cfg4);
    // see page 15-16 of Project6.pdf

}


void init_CN_Interrupts()
{
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
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL1) CNIntHandler(void)
{
    /* User ISR code inserted here */
    LATBINV = LEDB;
    //delay 20 ms debounce period
    hw_msDelay(20);
    LATBINV = LEDB;
    //Run Read_Buttons(void)
    BTN_Status = read_buttons();
    //Pass read_buttons() value to decode_buttons()
    LED_Code = decode_buttons(BTN_Status);
    control_leds(LED_Code);
    
    /* Required to clear the interrupt flag in the ISR */
    mCNClearIntFlag(); // Macro function
}

void hw_msDelay(unsigned int mS)
{
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();         // Read core timer count - SW Start breakpoint
    tWait = (CORE_MS_TICK_RATE * mS); // Set time to wait
    while ((ReadCoreTimer() - tStart) < tWait);  // Wait for the time to pass
    LATBINV = LEDA; // Toggle LED at end of delay period
}
