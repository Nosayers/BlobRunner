/* Written 2017 by Max Körlinge (c) */

#include <stdint.h>
#include <pic32mx.h>        //mcb32 environment
#include "blobrunner.h"   //headers for this project

void *stdin, *stdout, *stderr;

/* display.c
 * 
 * Functions that deal with initializing the hardware and software to control the OLED display on the microcontroller.
 * Also the most basic functions that make writing to the graphics buffer possible.
 * The hardware and software initialization and the helping functions to them 
 * were written based on the pseudo code outlined in the chipkit basic io shield
 * reference manual found at
 * reference.digilentinc.com/_media/chipkit_shield_basic_io_shield:chipkit_basic_io_shield_rm.pdf
 * and with some help from looking at the code created by Axel Isaksson and F Lundevall for lab 3 in the course Computer hardware engineering at the Royal Institute of Technology
 * in Stockholm. 
 */

/* HARDWARE INITIALIZATION */
/* call this before initializing display controller */
void display_hardware_init (void) {

    /* Sätt bitarna för sätta på SPI2, vilken styr input till displayen */
    SPI2CON = 0;            //clear bits
    SPI2BRG = 4;            //BRG is Baud Rate Reigster. Baud rate = how often a signal changes per second. 
                            //Not sure what to set, in pseudocode its 15, in lab3s code from school its set to 4. 
                            //Going with 4. Might want to change to change framerate later.
    SPI2STATCLR = 0x40;     //clear SPI2STATbits.SPIROV, as per pseudoinstructions. No overflow has occured.
    SPI2CONSET = 0x40;      //set bit 6 to 1, for "Clock polarity select bit" 1. Not sure why, but its done in pseudocode on basicioshield reference manual
    SPI2CONSET = 0x20;		//set bit 5 to 1, means master mode
    SPI2CONSET = 0x8000;   	//set bit 15 to 1, which turns SPI2 ON

    // Vad gör detta? Kopierat från Isaksson/Lundevalls kod i frustration när inget fungerade.
    ODCF = 0x0;
    ODCG = 0x0;

    /* Sätt rätt register till output (display vill bara bli skriven till, dvs output) MED TRIS- */
    // sätt RF4,5,6 (dvs command/data,VBat,VDD) till output (vi ska skriva till dem) - dvs 0
    TRISFCLR = 0x70;        //set bits 4-5-6 to 0 to 0
    TRISGCLR = 0x200;       //sätt RG9, dvs RESETknappen för displayn, till output (0)
}

/* DISPLAY CONTROLLER INITIATION SEQUENCE */
void display_controller_init (void) {
    
    PORTFCLR = 0x10;        // vi ska skicka kmmandon så sätt kommando/data till kommandomode.. dvs RF4 till 0
    PORTFCLR = 0x40;        // clear bit 6
                            // apply power to vdd - RF6
                            // OBS power on = bit is set to 0 (!)
    delay(1000);
                            // send display off command
    spi2putbyte(0xAE);      //send AE which is command to turn off to display

    /* ssd1306.pdf manual: after vdd stable, set RES pin LOW for at least 3 us and, and then set it to HIGH (logic high). */
    PORTGCLR = 0x200;       //rd4 low
    delay(10);
    PORTGSET = 0x200;       //rd4 high
    delay(10);
    /* "send set charge pump and set pre-charge period commands" - i/o shield ref man */
    spi2putbyte(0x8D);
    spi2putbyte(0x14);
    spi2putbyte(0xD9);
    spi2putbyte(0xF1);

    /* apply power to VBAT
     * Manual: VBAT enable: Uno32 pin 40 - PIC32pin 32, Signal PMA8/U2TX/SCL2/CN18/RF5
     * dvs använd port F bit 5
     * OBS bit equal to 0 = power is on(!)
     */
    PORTFCLR = 0x20;        //clear bit 5
    delay(1000);            //wait 1000ms

    //"send commands to invert display. this puts display origin in upper left corner" - i/o shield ref man
    spi2putbyte(0xA1);      //remap columns
    spi2putbyte(0xC8);      //remap rows

    //"send commands select sequential COM configuration. This makes display memory non-interleaved" - i/o shield ref man (why??)
    spi2putbyte(0xDA);
    spi2putbyte(0x20);

    // send display on command
    // ssd1306.pdf manual: send command AFh for display ON.
    spi2putbyte(0xAF);
    /* END INITIATION SEQUENCE */
}

/* DISPLAY MEMORY
 * display memory is organized as four pages of 128 bytes each. Each memory page
 * corresponds to an 8-pixel-high column on the display. The LSB in display byte is the top most pixel, MSB is the bottom most pixel. (on one page).
 * The first byte in page is the left most pixels on display, and last byte the right most
 * so
 * ---------------
 *  is a page, where leftmost pixel - is the first byte, and the top most pixel in the first byte is the LSB
 */

/* Playing field is divided into the 4 pages. Player (Blob) is supposed to
 * move between the four lanes, and obstacles are supposed to appear in any
 * of the four lanes. So the playing field is effectively divided into 4 lanes
 * and so the graphics data is separated into the four different pages, to
 * simplify writing obstacles or the Blob to them, without spilling over into
 * other lanes. This function updates all pages/lanes.
 */
void display_playing_field(void) {
	ready_page_for_input(0);
        display_putbuffer(128, field_page0);
	ready_page_for_input(1);
        display_putbuffer(128, field_page1);
	ready_page_for_input(2);
        display_putbuffer(128, field_page2);
	ready_page_for_input(3);
        display_putbuffer(128, field_page3);
}

/* Helper function for display_playing_field (and others?)
 */
void ready_page_for_input(int page) {
	PORTFCLR = 0x10;
	spi2putbyte(0x22);  //setpage
	spi2putbyte(page);
	spi2putbyte(0x00);  //set lowest nib
	spi2putbyte(0x10);  //set highest nib
	PORTFSET = 0x10;    //data mode
}

/* Fill column
 * fills column (127 = far right, 0 = far left) on a page
 */
void fill_col(int pagenumber, int column) {
    uint8_t* page = field_pages[pagenumber];
    page[column] = 255;
}

/* Puts a string in the center of a page
 */
void center_string(char* str, int page) {
    char* pnt = str;
    int col;
    int strlen = 0;
    
    //count str length
    while (*pnt != '\0') {
        strlen++;
        pnt++;
    }

    //print str to screen (note; needs a screen update to be shown aswell)
    col = (128 - strlen*6) / 2;
    pnt = str;
    while (*pnt != '\0') {
        put_huntchar(*pnt, page, col);
        col += 10;
        pnt++;
    }
}

/* Puts a character with font HUNTER somewhere
 */
void put_huntchar(char ch, int page, int col) {

    uint8_t* lane = field_pages[page];
    int huntnum = ((int) ch) - 32; //finds the right character in the font array
    int i;
    int k = 0;
    for (i = col; i < (col + 8); i++) {
        lane[i] = hunter_font[huntnum][k++];
    }
}

/* This is done and shown at start of game
 * Shows instructions for Btns to use, and then scrolls it away
 */
void display_startscreen() {

    char* up = "BTN3 UP";
    char* down = "BTN2 DOWN";
    center_string(up, 0);
    center_string(down,3);
    display_playing_field();
    delay(700);
    
    int i;
    for (i = 0; i < 130; i++) {
        scroll_playingfield();
        display_playing_field();
        delay(10);
    }
    display_clear();
}

/* This happens to screen on gameover
 * Shows GAME OVER and the achieved score
 * TODO: This screen is bugged by being offset in a weird way, when
 * player hits an obstacle from below or above.
 */
void display_gameover(void) {
    int ipage;
    int icolumn;
    char* go_str = "GAME OVER";
    char* score_str = "score:";
    //count number of digits in score
    int score = score_counter;
    int nrdigs = 0;
    while(score) {
        score /= 10;
        nrdigs++;
    }
    char* scr_str[nrdigs+1]; //dont forget \0
    sprintf(scr_str, "%d", score_counter);

    
    //scroll and clear playing field
    int i;
    for (i = 0; i < 128; i++) {
        field_page0[127] = 0;
        field_page1[127] = 0;
        field_page2[127] = 0;
        field_page3[127] = 0;
        scroll_playingfield();
        display_playing_field();
        delay(5);
    }

    display_clear();
    display_playing_field();
    delay(300);

    /* Had some display bugs when hitting obstacle from below or above
     * at high speeds, where the GAME OVER message was weirdly offset.
     * This tries to solve it by sleeping and reactivating display
     * Final note: None of it seems to make much difference
     */

    /* delay(10); */
    /* int k; */
    /* for (k = 0; k < 4; k++) { */
    /*     PORTFCLR = 0x10; */
    /*     spi2putbyte(0x22); */
    /*     spi2putbyte(k); */
    /*     PORTFSET = 0x10; */
    /*     for (i = 0; i < 900; i++) { */
    /*         spi2putbyte(0x0); */
    /*     } */
    /* } */

    /* for (i = 0; i < 300; i++) { */
    /*     scroll_playingfield(); */
    /*     display_playing_field(); */
    /* } */
    /* delay(10); */

    center_string(go_str, 0);
    center_string(score_str, 2);
    center_string(scr_str,3);
    display_playing_field();
    return;
}

/* Write data to display panel, "put buffer" */
void display_putbuffer(int number_bytes, uint8_t* buffer_to_send) {
    int i;
    for (i = 0; i < number_bytes; i++) {
        spi2putbyte(*buffer_to_send);       //send first byte in buffer, this function also waits to receive next byte
        buffer_to_send++;                   //increment pointer
    }
}

/* Send data to the display */
uint8_t spi2putbyte (uint8_t bytetowrite) {
    uint8_t bytetoread;
    /*Wait for transmitter to be ready*/
    while (!(SPI2STAT & 0x08));
    /*Write byte to buffer*/
    SPI2BUF = bytetowrite;

    /*Wait for receive byte*/
    while (!(SPI2STAT & 1));
    /*Read byte and return it - unnecessery for display which is readonly??*/
    bytetoread = SPI2BUF;
    return bytetoread;
}

/* This fills buffer with 0, effectively clearing screen
 */
void display_clear(void) {
    int ipage;
    int icolumn;

    for (ipage = 0; ipage < 4; ipage++) {
        PORTFCLR = 0x10;
        spi2putbyte(0x22);      //setpage
        spi2putbyte(ipage);
        spi2putbyte(0x0);       //set lowest nib
        spi2putbyte(0x10);      //set highest nib
        PORTFSET = 0x10;        //data mode
    	
        display_putbuffer(128, clear); //send one page of 0s to buffer
    }
} 

/* /1* NOT CURRENTLY USED. CHOSE OTHER WAY OF SCROLLING FIELD *1/ */
/* /1* Enable horizontal scrolling */
/*  * (send 2e to deactivate. must be deactivated before activating) */
/*  * might not want to use it for this project (use other way for game flow) */
/*  * followed example in ssd1306 controller manual */
/*  *1/ */
/* void enable_scrolling (void) { */
/*     PORTFCLR = 0x10; */
/*     spi2putbyte(0x29); */
/*     spi2putbyte(0x00); */
/*     spi2putbyte(0x00); */
/*     spi2putbyte(0x00); */
/*     spi2putbyte(0x07); */
/*     spi2putbyte(0x00); */
/*     spi2putbyte(0x2F); */
/* } */

/* // NOT CURRENTLY USED */
/* /1* Fill a rectangle on the specified page spec column */
/*  * max column = 15 (right edge of screen) */ 
/*  *1/ */
/* void fill_pix(int pagenumber, int column) { */
/*     uint8_t* page = field_pages[pagenumber]; */
/*     int cnt; */
/*     for (cnt = column*8; cnt < (column*8 + 8); cnt++) */
/*         page[cnt] = 255; */
/* } */
