#include <stdint.h>
#include <pic32mx.h>        //mcb32 environment
#include "blobrunner.h"   //headers for this project


/* The hardware and software initialization and the helping functions to them 
 * were written based on the pseudo code outlined in the chipkit basic io shield
 * reference manual found at
 * reference.digilentinc.com/_media/chipkit_shield_basic_io_shield:chipkit_basic_io_shield_rm.pdf
 * and with some glances at the code (but none directly copied) created by Axel Isaksson and F Lundevall for
 * lab 3 in the course Computer hardware engineering at the Royal Institute of Technology
 * in Stockholm.
 */


//display is serial device accessed using SPI, write-only.
//Maximum clock frequence is 10MHz!
//OBS: Each register has a SET, CLR, INV register, which sets, clears or inverts certain bits. So, if you write 0x0001 to SET a register, it only sets bit 0 to 1. If you write 0x0001 to CLR a register, you clear this (set it to 0). INV inverts the current value in the bit. Source: Section 12 I/O Ports in pic32 ref manual.


/* HARDWARE INITIALIZATION */
void display_hardware_init (void) {
    /* call this before initializing display controller */

    /* Sätt bitarna för sätta på SPI2, vilken styr input till displayen */
    SPI2CON = 0; //clear bits
    SPI2BRG = 4;   //BRG is Baud Rate Reigster. Baud rate = how often a signal changes per second. Not sure what to set, in pseudocode its 15, in lab3s code from school its set to 4. Going with 4. Might want to change to change framerate later.
    SPI2STATCLR = 0x40;    //clear SPI2STATbits.SPIROV, as per pseudoinstructions. No overflow has occured.
    SPI2CONSET = 0x40;     //set bit 6 to 1, for "Clock polarity select bit" 1. Not sure why, but its done in pseudocode on basicioshield reference manual
    SPI2CONSET = 0x20; //set bit 5 to 1, means master mode
    SPI2CONSET = 0x8000;    //set bit 15 to 1, which turns SPI2 ON

    // Vad gör detta?
    ODCF = 0x0;
    ODCG = 0x0;
    /* Sätt rätt register till output (display vill bara bli skriven till, dvs output) MED TRIS- */
    // sätt RF4,5,6 (dvs command/data,VBat,VDD) till output (vi ska skriva till dem) - dvs 0
    TRISFCLR = 0x70;  //set bits 4-5-6 to 0 to 0
    //sätt RG9, dvs RESETknappen för displayn, till output (0)
    TRISGCLR = 0x200;
    /* END HARDWARE INITIALIZATION */
}

/* DISPLAY INITIATION SEQUENCE */
void display_controller_init (void) {
    // vi ska skicka kmmandon så sätt kommando/data till kommandomode.. dvs RF4 till 0
    PORTFCLR = 0x10;
    // apply power to vdd - RF6
    // OBS power on = bit is set to 0 (!)
    PORTFCLR = 0x40;      // clear bit 6
    delay(1000);

    // send display off command
    spi2putbyte(0xAE);         //send AE which is command to turn off to display

    // ssd1306.pdf manual: after vdd stable, set RES pin LOW for at least 3 us and, and then set it to HIGH (logic high).
    PORTGCLR = 0x200;    //rd4 low
    delay(10);
    PORTGSET = 0x200;    //rd4 high
    delay(10);
    //"send set charge pump and set pre-charge period commands" - i/o shield ref man
    spi2putbyte(0x8D);
    spi2putbyte(0x14);
    spi2putbyte(0xD9);
    spi2putbyte(0xF1);

    // apply power to VBAT
    // Manual: VBAT enable: Uno32 pin 40 - PIC32pin 32, Signal PMA8/U2TX/SCL2/CN18/RF5
    // dvs använd port F bit 5
    // OBS bit equal to 0 = power is on(!)
    PORTFCLR = 0x20;      //clear bit 5
    delay(1000);    //wait 1000ms

    //"send commands to invert display. this puts display origin in upper left corner" - i/o shield ref man
    spi2putbyte(0xA1);     //remap columns
    spi2putbyte(0xC8);     //remap rows

    //"send commands select sequential COM configuration. This makes display memory non-interleaved" - i/o shield ref man (why??)
    spi2putbyte(0xDA);
    spi2putbyte(0x20);

    // send display on command
    // ssd1306.pdf manual: send command AFh for display ON.
    spi2putbyte(0xAF);
    /* END INITIATION SEQUENCE */
}

/* DISPLAY POWER OFF SEQUENCE */ 
void display_poweroff (void) {

    // send display off
    spi2putbyte(0xAE);
    delay(100);

    // power off VBAT
    PORTFSET = 0x0020;
    delay(1000);

    // pwoer off VDD
    PORTFSET = 0x0040; 
    delay(1000);
}

/* DISPLAY MEMORY UPDATE */
/* display memory is organized as four pages of 128 bytes each. Each memory page
 * corresponds to an 8-pixel-high column on the display. The LSB in display byte is the top most pixel, MSB is the bottom most pixel. (on one page).
 * The first byte in page is the left most pixels on display, and last byte the right most
 * so
 * ---------------
 *  is a page, where leftmost pixel - is the first byte, and the top most pixel in the first byte is the LSB
 */
void display_update(void) {
    int ipage;
    int icolumn;
    uint8_t* pb;

    pb = /*INSERT "THIS FUNCTION ASSUMES THAT THE DISPLAY BUFFER TO BE COPIED IS THE GLOBAL VAR HERE"*/ 

    for(ipage = 0; ipage < 4; ipage++) {        //<4 because 4 is the max number of pages
        //TODO find in basic io shield ref manual, in pseudo code for graphics display
    }
}

/* WRITE DATA TO DISPLAY PANEL, "PUT BUFFER" */
void display_putbuffer(int number_bytes, uint8_t* buffer_to_send) {
    int i;
    /*Write/read the data*/
    for (i = 0; i < number_bytes; i++) {
        spi2putbyte(*buffer_to-send);       //send first byte in buffer, this function also waits to receive next byte
        buffer_to_send++;                   //increment pointer
    }
}


/* SEND DATA TO THE DISPLAY */
uint8_t spi2putbyte (uint8_t bytetowrite) {
    uint8_t bytetoread;
    /*Wait for transmitter to be ready*/
    while (!(SPI2STAT & 0x08));
    /*Write byte to buffer*/
    SPI2BUF = bytetowrite;

    /*Wait for receive byte*/
    while (!(SPI2STAT & 1));
    /*Read byte and return it - unnecessery for display??*/
    bytetoread = SPI2BUF;
    return bytetoread;
}

// Kolla https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf 9 Command Table för "Scrolling command table", hur man får displayen att scrolla sidleds??+
