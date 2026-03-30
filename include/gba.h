#ifndef GBA_H
#define GBA_H

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define VRAM ((volatile unsigned short (*)[SCREEN_WIDTH])0x06000000)
#define VRAM_INT ((volatile unsigned int (*)[SCREEN_WIDTH/2])0x06000000)
#define VRAM_M4 ((volatile unsigned int (*)[SCREEN_WIDTH/4])0x06000000)
#define REG_VCOUNT (*(volatile unsigned short*)0x04000006)
#define REG_DISPCNT *((volatile unsigned short*)0x04000000)
#define REG_KEYINPUT *((volatile unsigned short*)0x04000130)



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

#define KEY_PRESSED(key) (~(REG_KEYINPUT) & key)

#define HORIZ (KEY_PRESSED(KEY_LEFT) ? -1 : (KEY_PRESSED(KEY_RIGHT) ? 1 : 0))
#define VERT (KEY_PRESSED(KEY_UP) ? -1 : (KEY_PRESSED(KEY_DOWN) ? 1 : 0))

// --- REG_DISPCNT defines ---
#define DCNT_MODE0     0x0000
#define DCNT_MODE1      0x0001
#define DCNT_MODE2      0x0002
#define DCNT_MODE3      0x0003
#define DCNT_MODE4      0x0004
#define DCNT_MODE5      0x0005
// layers
#define DCNT_BG0        0x0100
#define DCNT_BG1        0x0200
#define DCNT_BG2        0x0400
#define DCNT_BG3        0x0800
#define DCNT_OBJ        0x1000

#define NUM_DOTS 4
#define CYCLE_SIZE 30
#define TOP 80
#define LEFT 120

#define WRAP(value, mod) (value > mod ? value - mod : (value < 0 ? value + mod : value))

typedef unsigned short color;

#define PALETTE ((volatile color*) 0x05000000)

// TODO(make inline again?)
 color make_color(unsigned int r, unsigned int g, unsigned int b) { 
    return r | (g << 5) | (b << 10);
}

inline void vblank() {
    while(REG_VCOUNT >= 160);
    while(REG_VCOUNT < 160);
}

#endif