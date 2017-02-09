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
    display_clear();
   
    display_playing_field();
    //enable_scrolling();
/*
    int i;
    for (i = 0; i < 8; i++)
	    field_page0[i] = 255;
        */
    
    fill_pix(0,15);
    fill_pix(3,14);

    display_playing_field();
    return 0;
}

