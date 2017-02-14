#include <stdint.h>
#include <stdlib.h> //needed for rand
#include <pic32mx.h> 
#include "blobrunner.h"

/* gamelogic.c
 * includes functions that deal with making the game run as intended
 */

/* GLOBALS */
int GAME_SPEED = 10;     // speed 10 means game ticks once every 0.1s. Speed 2 will be challenging, 1 will be very hard
int TIMEOUTCOUNT = 0;   // for clock_check
int BLOCK_COUNT_LANE0 = -1; //for send_block, to keep track of block sending
int BLOCK_COUNT_LANE1 = -1;
int BLOCK_COUNT_LANE2 = -1;
int BLOCK_COUNT_LANE3 = -1;
int* BLOCKS_COUNTERS[4] = {
                            &BLOCK_COUNT_LANE0, &BLOCK_COUNT_LANE1,
                            &BLOCK_COUNT_LANE2, &BLOCK_COUNT_LANE3,
                            };
int BLOB_LANE = 0; //current player position (which lane is it in)
int RAND_INITIALIZED = 0; //is rand seeded?

/* TODO : fix so it doesnt random blocks too close, and also
 * make a counter so that it atleast generates a block sometimes
 */
int PROXIMITY_COUNTER = 0;
int NEW_BLOCK_MINIMUM = 0;
void generate_obstacles() {

    if (RAND_INITIALIZED && (NEW_BLOCK_MINIMUM > 14)) {
        send_block((rand() % 4));
    }
    else if (RAND_INITIALIZED && !(PROXIMITY_COUNTER)) { //om prox cnt = 0 och rand init = 1..
        volatile int randnr = rand() % 301; 

        //not pretty, but we use fallthrough to get the ranges 0-3,4-7, and so on..
        switch (randnr) {
            case 0:
            case 1:
            case 2:
                PROXIMITY_COUNTER = 1;
                send_block(0);
                break;
            case 3:
            case 4:
            case 5:
                PROXIMITY_COUNTER = 1;
                send_block(1);
                break;
            case 6:
            case 7:
            case 8:
                PROXIMITY_COUNTER = 1;
                send_block(2);
                break;
            case 9:
            case 10:
            case 11:
                PROXIMITY_COUNTER = 1;
                send_block(3);
                break;
            default:
                break;
        }
    }
    //preferably different kinds of obstacles (start with just filling squares)
}


/* Interrupt routine, when a button interrupt happens this is called */
/* Egen notis: Denna deklareras i assembler i vectors.S. Där finns själva
   interrupthandlern, som ser till att spara alla register, och den kallar
   på denna funktion i sin tur, sedan återställer den alla register */
void button_interrupt(void) {

    if (RAND_INITIALIZED == 0) {
        volatile unsigned int r = TMR2; //use current value when first button pressed as seed for random 
        srand(r);
        RAND_INITIALIZED = 1;
    }

    //read buttons and move player up or down
    volatile int p = ((PORTD >> 5) & 0x7);
    switch (p) {
        case 1:
            move_player(1);
            break;
        case 2:
            move_player(-1);
            break;
        case 4: //increase speed with button 4. temporary dev thing
            set_speed(-1);
        default:
            break;
    }
    IFSCLR(1) = 0x1; //clear flag
    return;
}

void game_over() {

    //display something on game over

    while(1); //placeholder
    //optional: save highscores, display highscores
}

/* Move player moves the blob up or down in lanes. It is called from
 * button_interrupt in buttons_handler.c when a button is pressed.
 * Button 2: move player up
 * Button 3: move player down
 */
void move_player(int dir) {
    remove_blob(); //otherwise our old blob position stays in graphic
    BLOB_LANE += dir; //if dir +1 moves player down, if dir -1 moves player up

    if ((BLOB_LANE < 0) || (BLOB_LANE > 3)) { //if were going out of bounds, go back
        BLOB_LANE -= dir;
    }
    write_blob();
}

/* Write blob should write blob in proper place, while everything else scrolls
 * Checks when writing blob if theres already something other than 0 there,
 * if so, its game over.
 */
void write_blob() {

    uint8_t* lane = field_pages[BLOB_LANE];
    int i;
    for (i = 0; i < 6; i++) {
        if (lane[24+i] != 0) { //game over if hit obstacle
            game_over();
        }
        lane[24+i] = blob[i]; //fill the columns with the blob data
    }
}

/* Remove blob removes blob from where it is/was. Used when moving the
 * blob and also in main game loop to avoid trailing graphics from old blobs
 */
void remove_blob() {

    uint8_t* lane = field_pages[BLOB_LANE];
    int i;
    for (i = 0; i < 6; i++) {
        lane[24+i] = 0; //set old blob pixel to 0
    }
}

/* Start and handle the sending of a block obstacle
 * Related globals: BLOCK_COUNT_LANE#
 * Keeps sending one column of the block each game tick until the block
 * is complete
 * If any block should be sent is also checked in game_clock_tick()
 */
void send_block(int lane) {

    int *block_counter = BLOCKS_COUNTERS[lane];

    if (*block_counter == -1) {
        *block_counter = 0;
    }

    fill_col(lane, 127);
    *block_counter += 1;
    NEW_BLOCK_MINIMUM = 0;
    
    if (*block_counter > 7) {
        *block_counter = -1;
    }
}

/* This happens when game ticks
 */
void game_clock_tick() {

    scroll_playingfield();
    //current level being played
    //level_one();

    //check if blocks are being sent, if so, keep sending
    int k;
    for (k = 0; k < 4; k++) {
        int* block_counter = BLOCKS_COUNTERS[k];
        if (*block_counter != -1) {
            send_block(k);
        }
    }

    //increase this each tick to guarantee a certain rate of blocks
    NEW_BLOCK_MINIMUM++;

    //avoiding blocks too close together
    if (PROXIMITY_COUNTER > 0) {
        PROXIMITY_COUNTER ++;
        if (PROXIMITY_COUNTER > 4)
            PROXIMITY_COUNTER = 0;
    }
    
    generate_obstacles(); //random generation
}

/* Check if game should tick forward
 * called repeatedly in main loop
 * If TIMEOUTCOUNT hits GAME_SPEED, it ticks
 * Both are global vars
 */
void clock_check() {

    volatile int timeoutFlag;
    timeoutFlag = (IFS(0) & 0x00000100) >> 8; //fetch 8th bit in IFS0

    if (timeoutFlag == 1) {
        IFS(0) &= 0xfffffeff; //set bit 8 to 0 again
        TIMEOUTCOUNT += 1;

        //ticks game forward after GAME_SPEED clock pulses (which are 1/0.1s)
        if (TIMEOUTCOUNT >= GAME_SPEED) {
            TIMEOUTCOUNT = 0;
            game_clock_tick();
        }
    }
}

/* scrolls everything in the field one step to the left 
 * (except the player blob which stays in the middle)
 */
void scroll_playingfield() {
    page_scroll(0);
    page_scroll(1);
    page_scroll(2);
    page_scroll(3);
}

/* Helper function for scroll_playingfield()
 * scrolls everything in a page one column to the left
 */
void page_scroll(int pagenr) {
    uint8_t pagecopy[128]; 
    uint8_t* realpage = field_pages[pagenr];

    int cnt;
    int i = 0;
    for (cnt = 1; cnt < 128; cnt++) {
        pagecopy[i] = realpage[cnt];
        i++;
    }
    pagecopy[i] = 0;    //add new empty column at far right

    //copy new page
    for (cnt = 0; cnt < 128; cnt++) {
        realpage[cnt] = pagecopy[cnt];
    }
}


void start_screen() {

    //display something on startup

}

//just do this in main instead?
void clock_init() {
    //set bit 15 to enable, bit 6-5-4 (prescaling) to 111(1:256)
    T2CON = 0x00008070;
    //set period to 3125, makes it timeout once every 0.01s (100ms)
    PR2 = 3125;
}

/* Speed set to higher number means slower speed
 * actual speed is (in real time) speed * 0.01(s)
 * If you pass a negative number, you instead set speed faster relative
 * to current speed.
 */
void set_speed(int speed) {
    if (speed < 0) {
        GAME_SPEED += speed;
    } else {
        GAME_SPEED = speed;
    }
}
