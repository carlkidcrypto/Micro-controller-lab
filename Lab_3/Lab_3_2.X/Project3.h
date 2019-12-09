#ifndef __PROJECT_3_H__
    #define __PROJECT_3_H__
#endif

/* Function prototypes */
void initialize_system();
int read_buttons(void);
int decode_buttons(int buttons,int* stepper_direction,int* stepper_mode,int* stepper_delay);
void control_leds(int leds);
int stepper_state_machine(int stepper_direction,int stepper_mode);
void output_to_stepper_motor(int stepper_code);
/* Software timer definition */
#define COUNTS_PER_MS 8894 /* Exact value is to be determined */
/* Function Prototypes */
void sw_msDelay(unsigned int mS); /* Software only delay */
//void hw_msDelay(unsigned int mS); /* Hardware-assisted delay */





/* End of Project3.h */