# Miniproject Abstract (Extended)

by Max Körlinge

## Title

Blob Runner, a "Runner" game on the ChipKIT Uno32 board.

## Objective and Requirements

The purpose of the project was to create a game, "Blob Runner", in which a blob travels constantly horisontally (x-axis). However, there are obstacles in the way, which you have to manouver away from. Instead of the Blob moving, the surrounding landscape moves, and new obstacles arise constantly. These are generated randomly. The object is to run for as long as possible without hitting an obstacle. As you play, the game gets gradually more difficult since the speed increases with time. At the inevitable end, you receive a score based on how far you got.

The requirements were as follows:

* The game must progress automatically, that is, the landscape is generated and moves by itself, not by user input.
* Playable with I/O buttons on ChipKIT board.
* If blob hits an obstacle, game ends.
* Randomly generated landscape.
* It should get harder and harder the longer you get.
* Game shows the score of the current run at the end.

## Solution

* Using and scrolling the display: Four arrays of 128 bytes each are fed into the display page buffers constantly by the main loop. Whatever needs to be seen on the screen is written to these arrays. A function scrolls every item in these arrays one step to the left and fills the new column with 0s, at game clock tick.
* The Blob and its behaviour:
    * *button_interrupt*: The hardware is initialised to call an interrupt handler when BTN4-2 is pressed. Moving the Blob was made an interrupt to make moving it as responsive as possible.
    * *move_blob*: The interrupt handler checks for input from button to see which direction (up/down) the Blob should move. If its at an edge of the screen, it does not move.Move then tries to remove the blob and re-write to the display in the proper spot.
    * *remove_blob*: Removes Blob from the array where it currently is, by setting these values to 0.
    * *write_blob*: Writes the Blob to the array for the page where its current position is (a global var). This function also checks if there is anything other than a 0 at the spot in the array where it is trying to write the Blob. If there is, this is interpreted as the Blob hitting an obstacle, and the game ends (gameover).
* Randomly generated landscape: A function *generate_obstacles* is called at game clock tick that randomly decides wheither to send or not to send a new obstacle (by doing *send_block*, a function which keeps track of sending the obstacles into the display buffers), and if so, in which random lane. Seed for *srand* is set when the user first presses a button.
* Harder the longer you get: The main loop polls a timer, which when having counted to a certain amount (*game_speed*), calls game_clock_tick which means the game ticks forward at a certain pace. *game_speed* is decreased after clock ticks a certain number of times, which makes *game_clock_tick* happen more often, thus making the game go faster until the gamespeed variable is 0, which is the maximum speed.
* Show game score: The score is calculated using a global variable. There was, however, some difficulty displaying the start screen and game over screen. A font was pulled from the internet (see *font.c* for details), and two functions *put_huntchar* and *center_string* are used to print a char, and to print a string on the display, respectively. However, on the GAME OVER screen, there is currently a bug that is unsolved. When ending the game by running into an obstacle with the Blob from **below** or **above**, the display seems to offset its entire window weirdly (where three small dots showcase where it thinks the end of the screen is), and the GAME OVER screen is corrupted. Many attempts have been made to solve this, unsuccessfully, but at least the score is always legible.
* Many things related to game balance can easily be changed by changing the defined constants at the top of the *gamelogic.c* file, such as how often the game sends obstacles, how wide the obstacles are, the starting game speed, and more. 

## Verification
Most tests were to do with displaying elements, and for this, it was simply a matter of writing different values to the graphics buffer, turning the device on, and seeing how they worked. 

There was also a testcourse (see *levels.c*) made that sent block in a specific pattern, where the main game functionalities were tested before making obstacles randomly generated.

## Contributions
The project was made by Max Körlinge. 

Resources font.c file by Jared Sanson. stubs.c, vectors.S by Axel Isaksson and F Lundevall (see COPYING).  

## Reflections
Being the first time working with an embedded system, for me the most time-consuming part was figuring out how the display worked. I didn't want to use any code from the labs, because I didn't really understand what they did, so I had to spend a lot of time working through the manual and testing code before managing to make the display do what I wanted it to do. Writing the C functions for the actual game logic once I figured out the hardware went surprisingly smoothly. 

It has been interesting to work with a system where, unlike in other programming I've done, it is very difficult to find any information online about how to do things, except the actual manual. The process has been rewarding when it at last works the way you want it to, but also frustrating at times especially with the game-over-display-bug present in the game, which I haven't been able to solve. 

In the end, I'm happy to present a working game which can actually be quite fun to play.
