/* The following define statements are declared in CerebotMX7cK.h
#define GetSystemClock() (80000000ul) // Hz
#define GetInstructionClock() (GetSystemClock()/2)
#define CORE_MS_TICK_RATE (GetInstructionClock()/1000)
#define LEDA BIT_2 // IOPORT B
*/

#include <plib.h>
#include "CerebotMX7cK.h"
#include "config_bits.h"

int main()
{
    Cerebot_mx7cK_setup(); // initialize the board
    unsigned int mS = 5000;
    while (1)
    {
        hw_msDelay(mS);
    }
    return 1;
}

void hw_msDelay(unsigned int mS)
{
    unsigned int tWait, tStart;
    tStart = ReadCoreTimer();         // Read core timer count - SW Start breakpoint
    tWait = (CORE_MS_TICK_RATE * mS); // Set time to wait
    while ((ReadCoreTimer() - tStart) < tWait);  // Wait for the time to pass
    LATBINV = LEDA; // Toggle LED at end of delay period
} // SW Stop breakpoint