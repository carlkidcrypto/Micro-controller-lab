// LEDA is defined in CerebotMX7cK.h
/* The following define statement should be declared in Project2.h
#define COUNTS_PER_MS 1000 // Initial guess
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
        sw_msDelay(ms);
    }
    return 1;
}
void sw_msDelay(unsigned int mS)
{
    int i;
    while (mS--) // SW Stop breakpoint
    {
        for (i = 0; i < COUNTS_PER_MS; i++) // 1 ms delay loop
        {
            // do nothing
        }
        LATBINV = LEDA; // Toggle LEDA each ms for instrumentation
    }
} // SW Stop breakpoint