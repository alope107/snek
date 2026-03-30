#include "gba.h"

#define SNEK_LEN 100
inline int col_val(int loc) {return loc & 0xff;}
inline int row_val(int loc) {return (loc & 0xff00) >> 8;}

int main() {
    REG_DISPCNT = DCNT_BG2 | DCNT_MODE3;

    color snek_color = make_color(31, 0, 0);

    int directions[4][2] = {
        {-1, 0},
        {0, 1},
        {1, 0},
        {0, -1},
    };

    int row=50, col=50;
    int direction = 0;

    int history[SNEK_LEN] = {};
    int hist_loc = 0;

    while(1) {
        vblank();
        if(KEY_PRESSED(KEY_UP)) {
            direction = 0;
        } else if (KEY_PRESSED(KEY_RIGHT)) {
            direction = 1;
        } else if (KEY_PRESSED(KEY_DOWN)) {
            direction = 2;
        } else if (KEY_PRESSED(KEY_LEFT)) {
            direction = 3;
        }

        row = WRAP(row+directions[direction][0], SCREEN_HEIGHT);
        col = WRAP(col+directions[direction][1], SCREEN_WIDTH);

        
        VRAM[row_val(history[hist_loc])][col_val(history[hist_loc])] = 0;
        VRAM[row][col] = snek_color;

        history[hist_loc] = (row << 8) | col;
        hist_loc++;

        if (hist_loc == SNEK_LEN) {
            hist_loc = 0;
        }
    };
    return 0;
}