#include <pic32mx.h>
#include <stdint.h>
#include "blobrunner.h"

int main(void) { 

    //are these necessery? it works without so far
    /*
    OSCCONCLR = 0x100000;
    OSCCONSET = 0x080000;
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    */

    clock_init();

    /*Initialize display*/
    display_hardware_init();
    display_controller_init();
    display_clear();
   
    //display_playing_field();

    //main loop
    while (1) {
            
        clock_check();
        display_playing_field();

    }

    //enable_scrolling();
    return 0;
}

