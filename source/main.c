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

    /* Setup clock */
    clock_init();

    /*Initialize display*/
    display_hardware_init();
    display_controller_init();
    display_clear();

    /*Setup Buttons I/O and the interrupts */
    buttons_init();
   
    //display_playing_field();
    //main loop
    while (1) {
        write_blob();
        clock_check();
        //write_blob()  should write blob in its place in its lane,
        //and check if its being written on an object and if so should fail
        display_playing_field();
    }

    //enable_scrolling();
    return 0;
}

