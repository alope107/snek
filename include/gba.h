#ifndef GBA_H
#define GBA_H

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

// Video RAM indexed as a 2d array of shorts (individual pixels in mode 3)
#define VRAM ((volatile unsigned short (*)[SCREEN_WIDTH])0x06000000)

// Video RAM indexed as unsigned ints. Useful for being able to overwrite 2 pixels at once
#define VRAM_INT ((volatile unsigned int (*)[SCREEN_WIDTH/2])0x06000000)

// Vertical scanline location
#define REG_VCOUNT (*(volatile unsigned short*)0x04000006)

// Holds display control values
#define REG_DISPCNT *((volatile unsigned short*)0x04000000)

// Bitmap mode with direct colors and no backbuffer
// The only mode needed for this game
#define DCNT_MODE3      0x0003 

// layers
#define DCNT_BG0        0x0100
#define DCNT_BG1        0x0200
#define DCNT_BG2        0x0400
#define DCNT_BG3        0x0800
#define DCNT_OBJ        0x1000

// Currently held keys (see key bit locations below)
// Counterintutively, for the GBA hardware held keys have their bit set to 0, unheld are set to 1
#define REG_KEYINPUT *((volatile unsigned short*)0x04000130)

// Key bit locations in REG_KEYINPUT
#define KEY_A (1)
#define KEY_B (1 << 1)
#define KEY_SELECT (1 << 2)
#define KEY_START (1 << 3)
#define KEY_RIGHT (1 << 4)
#define KEY_LEFT (1 << 5)
#define KEY_UP (1 << 6)
#define KEY_DOWN (1 << 7)
#define KEY_L (1 << 8)
#define KEY_R (1 << 9)

// 0 if key is not held, nonzero if key is held
// More intuitive than how it is actually represented in the hardware
#define KEY_HELD(key) (~(REG_KEYINPUT) & key)

// Wraps value around mod back to zero or vice versa
// Assumes abs(val) < 2*mod
#define WRAP(value, mod) (value > mod ? value - mod : (value < 0 ? value + mod : value))


// GBA uses 15 bit BGR colors
typedef unsigned short color;
// Creates a GBA compatible color using RGB values in the range 0-31
#define MAKE_COLOR(r, g, b) ( r | (g << 5) | (b << 10))

// Waits until the scanline reaches vblank where it's safe to draw without tearing
inline void vblank() {
    while(REG_VCOUNT >= SCREEN_HEIGHT);
    while(REG_VCOUNT < SCREEN_HEIGHT);
}

#endif