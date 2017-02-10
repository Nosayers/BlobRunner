#include <stdint.h> //needed?
#include <pic32mx.h> 
#include "blobrunner.h"

/* gamelogic.c
 * includes functions that deal with making the game run as intended
 */

/* GLOBALS */
int GAME_SPEED = 1;     // speed 10 means game ticks once every second
int TIMEOUTCOUNT = 0;   // for clock_check
int BLOCK_COUNT_LANE0 = -1; //for send_block, to keep track of block sending
int BLOCK_COUNT_LANE1 = -1;
int BLOCK_COUNT_LANE2 = -1;
int BLOCK_COUNT_LANE3 = -1;
int* BLOCKS_COUNTERS[4] = {
                            &BLOCK_COUNT_LANE0, &BLOCK_COUNT_LANE1,
                            &BLOCK_COUNT_LANE2, &BLOCK_COUNT_LANE3,
                            };
int BLOCKS_SENDING[4] = {0, 0, 0, 0};   //boolean for if this block is sending

/* NOTE
 * det är något fel med pekarna här inne
 * den skickar fill_col, men den verkar inte ha koll på hur många som kallats
 * och fyller inte en hel ruta.
 * problem kan ligga i gameclocktick också
 */
void send_block(int lane) {
    int* block_counter = BLOCKS_COUNTERS[lane];

    if (*block_counter == -1) {
        BLOCKS_SENDING[lane] = 1;
        *block_counter = 0;
    }

    fill_col(lane, 127);
    *block_counter++;
    
    if (*block_counter > 7) {
        *block_counter = -1;
        BLOCKS_SENDING[lane] = 0;
    }
}

/* This happens when game ticks
 */
void game_clock_tick() {
    scroll_playingfield();

    int k;
    for (k = 0; k > 4; k++) {
        if (BLOCKS_SENDING[k] == 1) {
            send_block(k);
        }
    }
}

/* Check if game should tick forward
 * called repeatedly in main loop
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

/* Write blob should write blob in proper place, while everything else scrolls
 * Could also check if blob hits something and its gameover
 */
void write_blob() {};

/* scrolls everything in the field one step to the left 
 * (except the player blob which stays in the middle)
 */
void scroll_playingfield() {
    page_scroll(0);
    page_scroll(1);
    page_scroll(2);
    page_scroll(3);
    //make this be based on a normal timer with timer flag = scroll
}

/* Helper function for scroll_playingfield()
 */
void page_scroll(int pagenr) {
    //scrolls everything in the field one step to the left (except the player blob which stays in the middle)
    uint8_t pagecp[128]; 
    uint8_t* realpage = field_pages[pagenr];

    int cnt;
    int i = 0;
    for (cnt = 1; cnt < 128; cnt++) {
        pagecp[i] = realpage[cnt];
        i++;
    }
    pagecp[i] = 0;

    //copy new page
    for (cnt = 0; cnt < 128; cnt++) {
        realpage[cnt] = pagecp[cnt];
    }
}


void generate_obstacles() {

    //generates obstacles in the 4 different lanes

    //preferably somehow randomly

    //preferably different kinds of obstacles (start with just filling squares)
}

void check_gameover() {
    //check if player blob has hit an obstacle

    //probably done by reading whats in the field_page at that spot before moving there, and if theres a conflic, you lose
}

void set_difficulty() {

    //difficulty could be the speed of the game, or how often the obstacles are generated - probably best to do speed
    
}

void move_player() {

    //input from btns to move player UP or DOWN
    //this should be done as an INTERRUPT, so you can move player
    //even in between game "ticks"

    //must write the player blob on the correct spot on the display
    
    //must check if doing so hits an obstacle, in that case, game over
}

void start_screen() {

    //display something on startup

}

void game_over() {

    //display something on game over

    //optional: save highscores, display highscores
}

//just do this in main instead?
void clock_init() {
    //set bit 15 to enable, bit 6-5-4 (prescaling) to 111(1:256)
    T2CON = 0x00008070;
    //set period to 31250, makes it timeout once every 0.1s (100ms)
    PR2 = 31250;
}

/* Speed set to higher number means slower speed
 * actual speed is (in real time) speed * 0.1(s)
 */
void set_speed(int speed) {
    GAME_SPEED = speed;
}
