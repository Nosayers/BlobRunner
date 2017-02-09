#include <pic32mx.h>
#include <stdint.h>
#include "blobrunner.h"


int main(void) { 

    OSCCONCLR = 0x100000;
    OSCCONSET = 0x080000;
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;

    /*Initialize display*/
    display_hardware_init();
    display_controller_init();

   // *display_buffer = *icon;

    display_update();

    return 0;
}
