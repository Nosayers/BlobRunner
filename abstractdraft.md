# Miniprojekt Abstract (Draft)
Max Körlinge

## Title
Blob Runner

## Objective and Requirements
The purpose is to create a game, "Blob runner", in which a blob travels constantly horisontally (x-axis). However, there are obstacles in the way, which you have to manouver away from, using up and down keys - which are going to be the I/O buttons. Instead of the Blob moving, the surrounding landscape will move, and new obstacles will arise constantly. The object is to run for as long as possible without hitting an obstacle.
The requirements are as follows:
- The game must progress automatically, that is, the landscape moves by itself, not by user input.
- If blob hits an obstacle, game ends
- Playable with I/O buttons on ChipKIT board.

Optional features:
- Randomly generated landscape
- Highscore
- It should get harder and harder the longer you get

## Solution
Develop iteratively. Requires knowledge of how to write to display and update the display. Code will be written in C (unless assembly required?), in MCB32tools environment.

Iteration 1:
- Working graphics: Blob in the middle, and edges that can be seen to be "moving". Interrupts triggered by timing will progress the game at a staedy pace.
- Able to move Blob up and down using buttons

Iteration 2:
- One screen worth of obstacles (manually written).
- If blob hits obstacle, game ends.

Iteration 3:
- Manually write a semi-long course of obstacles, which can be presented as full game.
- Implement pause button

Iteration 4:
- Automatically generated infinite obstacle course

Iteration 5:
- Course gets more difficult with time
- Highscores are tracked and saved

## Verification
Somehow test so that the blob-hits-obstacle event always occurs correctly when you hit something and not when you do not. Perhaps make a test course and test every way to hit an obstacle?



