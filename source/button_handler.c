#include <stdint.h>
#include <pic32mx.h>
#include "blobrunner.h"

/* Interrupt Service Routine */
/* Egen notis: Denna deklareras i assembler i vectors.S. Där finns själva
   interrupthandlern, som ser till att spara alla register, och den kallar
   på denna funktion i sin tur, sedan återställer den alla register */
void button_interrupt(void)
{
    //read buttons and move player up or down
    volatile int p = ((PORTD >> 5) & 0x7);

    switch (p) {
        case 1:
            move_player(1);
            break;
        case 2:
            move_player(-1);
            break;
        default:
            break;
    }

    IFSCLR(1) = 0x1; //clear flag

    return;
}

void buttons_init(void) {

    /* Change notice (CN) pins provide PIC32 abivlity to generate interrupts to processor in response
     * to the change of state on selected input pins (corresopnding TRISxbits must be 1).
     * Enabled pin values are compared with values sampled during the last read operation of the designated PORTx reg.
     * If pin value is different from last value read, mismatch condition is generated. The enabled pins all generate
     * the same interrupt signal (ORed together). Enabled pins are "sampled" on every internal system clock cycle, SYSCLK.
     * - Section 12 i/o. 12.3.3
     *   1. Disable CPU itnerrupts
     *   2. Set desired CN I/O pin as an input by setting TRISx corresponding to 1
*/    TRISDSET = 0xf0;  /*
     *   3. Enable CN module by setting the ON bit (CNCONx<15>) = 1
     */ CNCONSET = 0x8000; /*
     *   4. Enable individual CN input pins, enable optional pull-ups or pull-downs, dvs CN14, Cn15, CN16 (se chipkit data sheet för 34,35,36pin)
     */ CNENSET = 0x1C000; /*
     *   5. Read the corresponding PORTx registers to clear the CN interrupt
     *   6. Configure the CN interrupt priority bits, CNIP<2:0> (IPC6<20:18>) and subpriority bits CNIS<1:0> (IPC6<17:16>
     */ IPC(6) |= 0x1F0000; /*
     *   7. Clear the CN interrupt flag bit by setting CNIF bit (IFS1<0> = 0
     */ IFSCLR(1) = 0x1; /*
     *   8. Configure CN pin interrupt for a specific edge detect using EDGEDETECT bit in CNCONx reg, and set up. We dont EDGEDETECT? So skip, or set 0. CNCONbit11
     *   edge control using the CNENx/CNNEx bits (?)
     *   9. Enable CN interrupt enable bit, by setting CNIE bit (IEC1<0>) = 1
     */ IECSET(1) = 0x1; /*
     *   10. Enable CPU interrupts (noteself: use assembly stuff from lab3)
     */
    enable_interrupts();
}
