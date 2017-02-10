# Miniprojekt Abstract (Draft)
by Max Körlinge

## Title
Blob Runner

## Objective and Requirements
Aiming to make an Advanced project.

The purpose is to create a game, "Blob runner", in which a blob travels constantly horisontally (x-axis). However, there are obstacles in the way, which you have to manouver away from. Instead of the Blob moving, the surrounding landscape will move, and new obstacles will arise constantly. The object is to run for as long as possible without hitting an obstacle.

The requirements are as follows:

* The game must progress automatically, that is, the landscape is generated and moves by itself, not by user input.
* If blob hits an obstacle, game ends
* Playable with I/O buttons on ChipKIT board.

Optional features:

* Randomly generated landscape
* It should get harder and harder the longer you get
* Highscore

## Solution
Develop iteratively. Requires knowledge of how to write to display and update the display, and handle I/O from buttons. Code will be written in C (unless assembly required?), in MCB32tools environment.

Iteration 1:

* Working graphics: Blob in the middle, and items that can be seen to be "moving" in the same direction. Polling systems timer progresses the game. 

Iteration 2:

* Able to move blob using buttons. Input from buttons is handled as interrups to move blob instantly (not wait for timer)

Iteration 3:

* If blob hits obstacle, game ends.
* Manually write a semi-long course of obstacles, which can be presented as full game.

Iteration 4:

* Implement pause button
* Automatically generated infinite obstacle course

Iteration 5:

* Course gets more difficult with time
* Highscores are tracked and saved
* Start and gameover screens.

## Verification
Somehow test so that the blob-hits-obstacle event always occurs correctly when you hit something and not when you do not. Perhaps make a test course and test every way to hit an obstacle?



