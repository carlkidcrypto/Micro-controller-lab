#ifndef __PROJECT_7_H__
    #define __PROJECT_7_H__
#endif

/* Function prototypes */
void initialize_system();
void read_buttons(void);
int decode_buttons();
void control_leds();
int stepper_state_machine();
void output_to_stepper_motor();
/* Software timer definition */
#define COUNTS_PER_MS 8894 /* Exact value is to be determined */
/* Function Prototypes */
void Timer1_delay(int delay);
void hw_msDelay(unsigned int mS);
void Timer1_Setup();
void CNI_Setup(); // change notice interrupt
void Stepper_Motor_LEDS_Setup();
void Initialize_PMP();
void Initialize_LCD();
void Initialize_UART();


/* End of Project7.h */