#include <stdint.h>
#include <stdlib.h> //needed for rand
#include <pic32mx.h> 
#include "blobrunner.h"

/* gamelogic.c
 * includes core game functions not related to hardware
 */

/* Game options. Change to make game more difficult, easier, etc */
#define MAX_SPACE_BETWEEN_BLOCKS 14
#define MIN_SPACE_BETWEEN_BLOCKS 4
#define OBSTACLE_BLOCK_WIDTH 7
#define STARTING_GAME_SPEED 5

/* GLOBALS */
int rand_initialized = 0; //is rand seeded?
int game_speed = STARTING_GAME_SPEED;     // speed 10 means game ticks once every 0.1s. Speed 2 will be challenging, 1 will be very hard
int timeoutcount = 0;   // for clock_check
int block_count_lane0 = -1; //for send_block, to keep track of block sending
int block_count_lane1 = -1;
int block_count_lane2 = -1;
int block_count_lane3 = -1;
int* block_counters[4] = {
                            &block_count_lane0, &block_count_lane1,
                            &block_count_lane2, &block_count_lane3,
                            };
int blob_lane = 0; //current player position (which lane is it in)
int proximity_counter = 0; //for generate_obstacles, make sure dont send obstacles too close for player to navigate in between. If > 0, prevents obstaclesending
int new_block_counter = 0; //for generates_obstacles, make sure we send a new block atleast every MAX_SPACE_BETWEEN_BLOCKS

/* Generates obstacles randomly
 * is called from game_clock_tick
 * uses random seed generated by time until first button press to send obstacles
 * randomly, but within certain restrictions
 * proximity: we try to avoid generating obstacles across all lanes at one, making
 * it impossible to get through for the player
 * new block counter: we try to generate atleast one obstacle every MAX_SPACE_BETWEEN_BLOCKS
 */
void generate_obstacles() {

    if (rand_initialized && (new_block_counter > MAX_SPACE_BETWEEN_BLOCKS)) {
        send_block((rand() % 4));
    }
    else if (rand_initialized && !(proximity_counter)) { //om prox cnt = 0 och rand init = 1..
        volatile int randnr = rand() % 301; 

        //not pretty, but we use fallthrough to get the ranges 0-3,4-7, and so on..
        switch (randnr) {
            case 0:
            case 1:
            case 2:
                proximity_counter = 1;
                send_block(0);
                break;
            case 3:
            case 4:
            case 5:
                proximity_counter = 1;
                send_block(1);
                break;
            case 6:
            case 7:
            case 8:
                proximity_counter = 1;
                send_block(2);
                break;
            case 9:
            case 10:
            case 11:
                proximity_counter = 1;
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

    if (rand_initialized == 0) {
        volatile unsigned int r = TMR2; //use current value when first button pressed as seed for random 
        srand(r);
        rand_initialized = 1;
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


    display_playing_field(); //otherwise it sometimes looks like we didnt actually lose

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
    blob_lane += dir; //if dir +1 moves player down, if dir -1 moves player up

    if ((blob_lane < 0) || (blob_lane > 3)) { //if were going out of bounds, go back
        blob_lane -= dir;
    }
    write_blob();
}

/* Write blob should write blob in proper place blob_lane, while everything else scrolls
 * Checks when writing blob if theres already something other than 0 there,
 * if so, its game over.
 */
void write_blob() {

    uint8_t* lane = field_pages[blob_lane];
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

    uint8_t* lane = field_pages[blob_lane];
    int i;
    for (i = 0; i < 6; i++) {
        lane[24+i] = 0; //set old blob pixel to 0
    }
}

/* Start and handle the sending of a block obstacle
 * Related globals: BLOCK_COUNT_LANE#
 * Keeps sending one column of the block each game tick until the block
 * is complete
 * If a block should be sent is also checked in game_clock_tick()
 */
void send_block(int lane) {

    int *block_counter = block_counters[lane];

    if (*block_counter == -1) {
        *block_counter = 0;
    }

    fill_col(lane, 127);
    *block_counter += 1;
    new_block_counter = 0;
    
    if (*block_counter > OBSTACLE_BLOCK_WIDTH) {
        *block_counter = -1;
    }
}

/* This happens when game ticks
 * - Scrolls playing field one column to the left
 * - Checks if blocks are being sent, if so, keep sending them (until theyre done)
 * - Handles globals related to sending blocks not too close and not too far from eachother
 * - Calls the random obstacle generator
 *   
 *   Note that player hitting obstacles is not checked here, but in write_blob
 */
void game_clock_tick() {

    scroll_playingfield();
    display_playing_field();
    
    /* For manual level designs (currently theres only a level1) */
    //current level being played
    //level_one();

    //check if blocks are being sent, if so, keep sending
    int k;
    for (k = 0; k < 4; k++) {
        int* block_counter = block_counters[k];
        if (*block_counter != -1) {
            send_block(k);
        }
    }

    //increase this each tick to guarantee a certain rate of blocks
    new_block_counter++;

    //avoiding blocks too close together
    if (proximity_counter > 0) {
        proximity_counter ++;
        if (proximity_counter > MIN_SPACE_BETWEEN_BLOCKS)
            proximity_counter = 0; //now we can send block again
    }
    
    generate_obstacles(); //random generation
}

/* Check if game should tick forward
 * called repeatedly in main loop
 * If timeoutcount hits game_speed, it ticks
 * Both are global vars
 */
void clock_check() {

    volatile int timeoutFlag;
    timeoutFlag = (IFS(0) & 0x00000100) >> 8; //fetch 8th bit in IFS0

    if (timeoutFlag == 1) {
        IFS(0) &= 0xfffffeff; //set bit 8 to 0 again
        timeoutcount += 1;

        //ticks game forward after game_speed clock pulses (which are 1/0.1s)
        if (timeoutcount >= game_speed) {
            timeoutcount = 0;
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

/* Speed set to higher number means slower speed
 * actual speed is (in real time) speed * 0.01(s)
 * If you pass a negative number, you instead set speed faster relative
 * to current speed.
 */
void set_speed(int speed) {
    if (speed < 0) {
        game_speed += speed;
    } else {
        game_speed = speed;
    }
}
