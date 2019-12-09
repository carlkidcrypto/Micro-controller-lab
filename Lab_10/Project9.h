void initialize_system();
void init_CN_Interrupts();
void hw_msDelay(unsigned int mS);
int read_buttons(void);
int decode_buttons(int buttons);
void control_leds(int leds);
void Initialize_PMP();
void Initialize_LCD();