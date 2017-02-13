#include <pic32mx.h>
#include <stdint.h>
#include "blobrunner.h"

/* KNOWN ISSUES:
 * The microcontrollers push buttons sometimes double click unintentionally,
 * screwing us.
 */

void *stdin, *stdout, *stderr;  //needed to workaround a mcb32 env bug with using stdlib

int main(void) { 
    //are these necessery? it works without so far
    /*
    OSCCONCLR = 0x100000;
    OSCCONSET = 0x080000;
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    */

    /* Setup clock */
    clock_init();

    /*Initialize display*/
    display_hardware_init();
    display_controller_init();
    display_clear();

    /*Setup Buttons I/O and the interrupts
     * connected to them*/
    button_init();
   
    //main loop
    while (1) {
        remove_blob(); //otherwise our blob sends a trail after it
        clock_check(); //check if game should tick forward or not
        write_blob();  //write blob to screen
        display_playing_field(); //update field
    }

    //enable_scrolling();
    return 0;
}
