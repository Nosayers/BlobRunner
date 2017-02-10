#include <stdint.h>
#include <pic32mx.h> //are these two needed in here?
#include "blobrunner.h"

/* gamelogic.c
 * includes functions that deal with making the game run as intended
 */


void scroll_playingfield() {

    //scrolls everything in the field one step to the left (except the player blob which stays in the middle)
    //



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
