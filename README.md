## BLOBRUNNER

created by Max Körlinge

A game for microcontroller ChipKIT Uno32 development board with ChipKIT Basic I/O shield.

Blobrunner is, as the name suggests, a game where you are playing a blob on the screen, and you are supposed to run for as long as possible without running into one of the randomly generated obstacles that appear on the screen. The game becomes more difficult as speed increases with time.

### Requirements

* The device (ChipKIT Uno32 development board with ChipKIT Basic I/O shield), to run the entire project.
* MCB32 toolchain, for building the code onto the device: https://github.com/is1200-example-projects/mcb32tools/releases/

### Instructions

* Btn 3 - Move blob UP
* Btn 2 - Move blob DOWN
* Btn 4 - Manually increase speed

### Background

The game was created for a school project in the course Computer hardware engineering, at the Royal Institute of Technology (KTH) in Stockholm.
The program is written in C (small part in Assembler) for the MIPS processor present in the kit.

### Code reuse

Files stubs.c and vectors.S are reused from a course lab, see file COPYING. The Makefile was provided by the school as well.

### Known bugs

 * The microcontrollers push buttons sometimes double click unintentionally, losing us the game.
 * When hitting an obstacle from below or above (= not from the front), the display messes up the graphics on the GAME OVER screen. Tried many ways to solve this, like writing different amounts of 0s to buffer before the G-O screen, and turning off/on the screen. Nothing solved it yet.
 * Rare bug: Blob hits "gameover" even without being near an obstacle
