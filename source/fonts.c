#include <stdint.h>
#include "blobrunner.h"

/* THIS FILE WAS DOWNLOADED FROM
 * jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
 *
 * created by Jared Sanson
 */

void *stdin, *stdout, *stderr;

const unsigned char hunter_font[96][8] = {
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //  
	{0x5f,0x5f,0x00,0x00,0x00,0x00,0x00,0x00}, // !
	{0x07,0x07,0x00,0x07,0x07,0x00,0x00,0x00}, // "
	{0x14,0x7f,0x7f,0x14,0x7f,0x7f,0x14,0x00}, // #
	{0x6f,0x6f,0x6b,0xeb,0x6b,0x7b,0x7b,0x00}, // $
	{0x63,0x73,0x38,0x1c,0x0e,0x67,0x63,0x00}, // %
	{0x7f,0x7f,0x6b,0x63,0x68,0x78,0x78,0x00}, // &
	{0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00}, // '
	{0xff,0xff,0x80,0x00,0x00,0x00,0x00,0x00}, // (
	{0x80,0xff,0xff,0x00,0x00,0x00,0x00,0x00}, // )
	{0x36,0x08,0x7f,0x7f,0x7f,0x08,0x36,0x00}, // *
	{0x08,0x08,0x3e,0x08,0x08,0x00,0x00,0x00}, // +
	{0xe0,0xe0,0x00,0x00,0x00,0x00,0x00,0x00}, // ,
	{0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00}, // -
	{0x60,0x60,0x00,0x00,0x00,0x00,0x00,0x00}, // .
	{0x70,0x1c,0x07,0x00,0x00,0x00,0x00,0x00}, // /
	{0x7f,0x7f,0x63,0x63,0x63,0x7f,0x7f,0x00}, // 0
	{0x60,0x66,0x66,0x7f,0x7f,0x60,0x60,0x00}, // 1
	{0x7b,0x7b,0x6b,0x6b,0x6b,0x6f,0x6f,0x00}, // 2
	{0x63,0x63,0x6b,0x6b,0x6b,0x7f,0x7f,0x00}, // 3
	{0x1f,0x1f,0x18,0x18,0x18,0x7e,0x7e,0x00}, // 4
	{0x6f,0x6f,0x6b,0x6b,0x6b,0x7b,0x7b,0x00}, // 5
	{0x7f,0x7f,0x6c,0x6c,0x6c,0x7c,0x7c,0x00}, // 6
	{0x43,0x63,0x73,0x3b,0x1f,0x0f,0x07,0x00}, // 7
	{0x7f,0x7f,0x6b,0x6b,0x6b,0x7f,0x7f,0x00}, // 8
	{0x1f,0x1f,0x1b,0x1b,0x1b,0x7f,0x7f,0x00}, // 9
	{0x6c,0x6c,0x00,0x00,0x00,0x00,0x00,0x00}, // :
	{0xec,0xec,0x00,0x00,0x00,0x00,0x00,0x00}, // ;
	{0x08,0x14,0x22,0x22,0x00,0x00,0x00,0x00}, // <
	{0x14,0x14,0x14,0x14,0x14,0x00,0x00,0x00}, // =
	{0x22,0x22,0x14,0x08,0x00,0x00,0x00,0x00}, // >
	{0x03,0x03,0x5b,0x5b,0x0b,0x0f,0x0f,0x00}, // ?
	{0x7f,0x7f,0x63,0x6f,0x6b,0x6f,0x6f,0x00}, // @
	{0x7f,0x7f,0x1b,0x1b,0x1b,0x7f,0x7f,0x00}, // A
	{0x7f,0x7f,0x6b,0x6b,0x6f,0x7e,0x7c,0x00}, // B
	{0x7f,0x7f,0x63,0x63,0x63,0x63,0x63,0x00}, // C
	{0x7f,0x7f,0x63,0x63,0x67,0x7e,0x7c,0x00}, // D
	{0x7f,0x7f,0x6b,0x6b,0x6b,0x6b,0x6b,0x00}, // E
	{0x7f,0x7f,0x0b,0x0b,0x0b,0x03,0x03,0x00}, // F
	{0x7f,0x7f,0x63,0x63,0x6b,0x7b,0x7b,0x00}, // G
	{0x7f,0x7f,0x18,0x18,0x18,0x7f,0x7f,0x00}, // H
	{0x63,0x63,0x7f,0x7f,0x63,0x63,0x63,0x00}, // I
	{0x60,0x60,0x60,0x60,0x60,0x7f,0x3f,0x00}, // J
	{0x7f,0x7f,0x18,0x18,0x1e,0x7f,0x79,0x00}, // K
	{0x7f,0x7f,0x60,0x60,0x60,0x60,0x60,0x00}, // L
	{0x7f,0x7f,0x03,0x06,0x03,0x7f,0x7f,0x00}, // M
	{0x7f,0x7f,0x0e,0x1c,0x38,0x7f,0x7f,0x00}, // N
	{0x7f,0x7f,0x63,0x63,0x63,0x7f,0x7f,0x00}, // O
	{0x7f,0x7f,0x1b,0x1b,0x1b,0x1f,0x1f,0x00}, // P
	{0x7f,0x7f,0x63,0xf3,0xf3,0x7f,0x7f,0x00}, // Q
	{0x7f,0x7f,0x1b,0x3b,0x7b,0x7f,0x5f,0x00}, // R
	{0x6f,0x6f,0x6b,0x6b,0x6b,0x7b,0x7b,0x00}, // S
	{0x03,0x03,0x7f,0x7f,0x03,0x03,0x03,0x00}, // T
	{0x7f,0x7f,0x60,0x60,0x60,0x7f,0x7f,0x00}, // U
	{0x1f,0x3f,0x70,0x60,0x70,0x3f,0x1f,0x00}, // V
	{0x3f,0x7f,0x60,0x30,0x60,0x7f,0x3f,0x00}, // W
	{0x77,0x7f,0x1c,0x08,0x1c,0x7f,0x77,0x00}, // X
	{0x07,0x0f,0x7c,0x78,0x7c,0x0f,0x07,0x00}, // Y
	{0x73,0x7b,0x6b,0x6b,0x6b,0x6f,0x67,0x00}, // Z
	{0xff,0xff,0x80,0x00,0x00,0x00,0x00,0x00}, // [
	{0x07,0x1c,0x70,0x00,0x00,0x00,0x00,0x00}, // "\"
	{0x80,0xff,0xff,0x00,0x00,0x00,0x00,0x00}, // ]
	{0x00,0x7f,0x40,0x7f,0x00,0x00,0x00,0x00}, // ^
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00}, // _
	{0x00,0x7f,0x40,0x7f,0x00,0x00,0x00,0x00}, // `
	{0x7f,0x7f,0x1b,0x1b,0x1b,0x7f,0x7f,0x00}, // a
	{0x7f,0x7f,0x6b,0x6b,0x6f,0x7e,0x7c,0x00}, // b
	{0x7f,0x7f,0x63,0x63,0x63,0x63,0x63,0x00}, // c
	{0x7f,0x7f,0x63,0x63,0x67,0x7e,0x7c,0x00}, // d
	{0x7f,0x7f,0x6b,0x6b,0x6b,0x6b,0x6b,0x00}, // e
	{0x7f,0x7f,0x0b,0x0b,0x0b,0x03,0x03,0x00}, // f
	{0x7f,0x7f,0x63,0x63,0x6b,0x7b,0x7b,0x00}, // g
	{0x7f,0x7f,0x18,0x18,0x18,0x7f,0x7f,0x00}, // h
	{0x63,0x63,0x7f,0x7f,0x63,0x63,0x63,0x00}, // i
	{0x60,0x60,0x60,0x60,0x60,0x7f,0x3f,0x00}, // j
	{0x7f,0x7f,0x18,0x18,0x1e,0x7f,0x79,0x00}, // k
	{0x7f,0x7f,0x60,0x60,0x60,0x60,0x60,0x00}, // l
	{0x7f,0x7f,0x03,0x06,0x03,0x7f,0x7f,0x00}, // m
	{0x7f,0x7f,0x0e,0x1c,0x38,0x7f,0x7f,0x00}, // n
	{0x7f,0x7f,0x63,0x63,0x63,0x7f,0x7f,0x00}, // o
	{0x7f,0x7f,0x1b,0x1b,0x1b,0x1f,0x1f,0x00}, // p
	{0x7f,0x7f,0x63,0xf3,0xf3,0x7f,0x7f,0x00}, // q
	{0x7f,0x7f,0x1b,0x3b,0x7b,0x7f,0x5f,0x00}, // r
	{0x6f,0x6f,0x6b,0x6b,0x6b,0x7b,0x7b,0x00}, // s
	{0x03,0x03,0x7f,0x7f,0x03,0x03,0x03,0x00}, // t
	{0x7f,0x7f,0x60,0x60,0x60,0x7f,0x7f,0x00}, // u
	{0x1f,0x3f,0x70,0x60,0x70,0x3f,0x1f,0x00}, // v
	{0x3f,0x7f,0x60,0x30,0x60,0x7f,0x3f,0x00}, // w
	{0x77,0x7f,0x1c,0x08,0x1c,0x7f,0x77,0x00}, // x
	{0x07,0x0f,0x7c,0x78,0x7c,0x0f,0x07,0x00}, // y
	{0x73,0x7b,0x6b,0x6b,0x6b,0x6f,0x67,0x00}, // z
	{0x1c,0xff,0xff,0x80,0x00,0x00,0x00,0x00}, // {
	{0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // |
	{0x80,0xff,0xff,0x1c,0x00,0x00,0x00,0x00}, // }
	{0x00,0x7f,0x40,0x7f,0x00,0x00,0x00,0x00}, // ~
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};
