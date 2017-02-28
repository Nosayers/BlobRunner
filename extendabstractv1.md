# Miniproject Abstract (Extended)

by Max Körlinge

## Title

Blob Runner, a "Runner" game on the ChipKIT Uno32 board.

## Objective and Requirements

The purpose of the project was to create a game, "Blob Runner", in which a blob travels constantly horisontally (x-axis). However, there are obstacles in the way, which you have to manouver away from. Instead of the Blob moving, the surrounding landscape moves, and new obstacles arise constantly. These are generated randomly. The object is to run for as long as possible without hitting an obstacle. As you play, the game gets gradually more difficult since the speed increases with time. At the inevitable end, you receive a score based on how far you got.

To move the Blob, you use BTN3 to move up, and BTN2 to move down. BTN4 can be used to increase the speed manually, should one wish to do so.

The requirements were as follows:

* The game must progress automatically, that is, the landscape is generated and moves by itself, not by user input.
* Playable with I/O buttons on ChipKIT board.
* If blob hits an obstacle, game ends.
* Randomly generated landscape.
* It should get harder and harder the longer you get.
* Game shows the score of the current run at the end.

## Solution

One big hurdle to overcome was figuring out how the display worked, and how to structure the program to update the display and the playing field automatically. Since it turned out the display GDDRAM was divided into four pages and data was written to it by specifying a page and then the data, it was natural to have a similar layout for the game logic where the Blob and the obstacles can be present in one of four lanes. To do this, four arrays of 128 bytes each were created, and they are fed into the display page buffers constantly by the main loop. Whatever needs to be seen on the screen is written to these arrays. 

After figuring out how the pixels worked when you wrote something to the buffer (i.e, if you write 255 to column 127 on a page, the end-most segment (column) is filled), the next step was figuring out how to constantly scroll the graphics on the display and the playing field. Since the main loop constantly updates the display from the four previously mentioned arrays, this is achieved with a function that takes everything in these four arrays, scrolls every value one step to the left (erasing the left-most value), and fills in a 0 at the end of the array. It was found that doing this in the main loop made everything go way too quickly, so TIMER2 in the development board was initialised to have some control over how quickly the game progresses. The main loop instead polls the clock constantly to check if the timer flag is up, and if it is, it checks wheither the game should tick forward or not (depending on a counter variable). This way also, game speed can effectively be changed by changing how much the counter needs to go to before game ticking forward.

Now that it was possible to write, for example, a column to the display, and then have it travel across the screen at a steady pace, the Blob was created. It has several functions that help it behave properly:

* *button_interrupt*: The hardware is initialised to call an interrupt handler when BTN4-2 is pressed. Moving the Blob was made an interrupt to make moving it as responsive as possible.
* *move_blob*: The interrupt handler checks for input from button to see which direction (up/down) the Blob should move. If its at an edge of the screen, it does not move.Move then tries to remove the blob and re-write to the display in the proper spot.
* *remove_blob*: Removes Blob from the array where it currently is, by setting these values to 0.
* *write_blob*: Writes the Blob to the array for the page where its current position is (a global var). This function also checks if there is anything other than a 0 at the spot in the array where it is trying to write the Blob. If there is, this is interpreted as the Blob hitting an obstacle, and the game ends (gameover).

Note that removing the blob and rewriting it is constantly done in the main loop, otherwise Blob would only move when we press button, instead of remaining in one place on the screen, as it should.

To introduce obstacles into the playing field, there are a few functions assisting eachother. At the core, however, *send_block* both handles sending new obstacles, and keeps sending them if they are not fully sent yet. To explain further, when an obstacles first is sent, only the first column of it is sent to the graphics buffer. The object however was to have decent-sized rectangles as obstacles, not just thin vertical lines. send_block, together with related global variables, either starts the sending of a block by initiating the related counters, or checks and sees that a block is being sent and keeps sending filled columns to the graphics display until it has sent the full obstacle. send_block is called everytime the game ticks forward, to see if when ticking, it is in the process of sending any blocks.

Now, to do this randomly, also in when ticking the game forward, a function *generate_obstacles* is called that randomly decides wheither to send or not to send a new obstacle (by doing *send_block*), and if so, in which random lane. There was some difficulty generating a seed for *srand* to initialise the random numbers, but this is in the end solved when the user first presses a button, the interrupt handler reads the current time from the Timer and uses that as a seed. The function to generate obstacles then has an equal chance to send a block for each lane. The total chance to send a block is 11/304, which after some testing was decided to work fine. This can however be changed easily by changing *NEW_BLOCK_RATIO* (there are other constants defined that can easily be changed to determin game difficulty and behaviour). 

With the way the clock worked, being able to change speed with time was fairly straightforward. When the clock ticks, a global variable is increased, and when it hits certain thresholds, *game_speed* is decreased, which makes *game_clock_tick* happen more often, thus making the game go faster, until our gamespeed is 0, which is the maximum speed (the game clock ticks everytime the main loop loops).

The score is calculated using a global variable. There was, however, some difficulty displaying the start screen and game over screen. A font was pulled from the internet (see font.c for details), and two functions *put_huntchar* and *center_string* are used to print a char somewhere on the display, and to print a string on the display, respectively. However, on the GAME OVER screen, there is currently a bug that is unsolved. When ending the game by running into an obstacle with the Blob from **below** or **above**, the display seems to offset its entire window weirdly (where three small dots showcase where it thinks the end of the screen is), and the GAME OVER screen is corrupted. Many attempts have been made to solve this, unsuccessfully, but at least the score is always legible, even if it is not quite where it is supposed to be.  

There is also a rare bug that it has not been possible to replicate or find the reason for, where the Blob hits an invisible obstacle and the game ends prematurely. This is quite rare however.


## Verification
Most tests were to do with displaying elements, and for this, it was simply a matter of writing different values to the graphics buffer, turning the device on, and seeing how they worked. 

There was a testcourse (see *levels.c*) made that sent block in a specific pattern, where the main game functionalities were tested before making obstacles randomly generated.

Testing that the Blob hitting an obstacle worked was made by calling an infinite while loop and seeing that the game stopped when this occured.

The final product was tested by playing the game and hitting the obstacles from every possible way. A few external persons were also invited to try the game.

## Contributions
The project was made by Max Körlinge. 

Resources font.c file by Jared Sanson. stubs.c, vectors.S by Axel Isaksson and F Lundevall (see COPYING).  
