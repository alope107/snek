#include "gba.h"

#define MAX_SNEK_LEN 100

inline int col_val(int loc) {return loc & 0xff;}
inline int row_val(int loc) {return (loc & 0xff00) >> 8;}

int main() {
    REG_DISPCNT = DCNT_BG2 | DCNT_MODE3;

    unsigned int snek_len = 10;

    color snek_color = make_color(31, 0, 0);

    int directions[4][2] = {
        {-1, 0},
        {0, 1},
        {1, 0},
        {0, -1},
    };

    int row=50, col=50;
    int direction = 0;

    int history[MAX_SNEK_LEN] = {};
    int hist_head = 0;
    int hist_tail = 0;

    while(1) {
        vblank();
        if(KEY_PRESSED(KEY_UP) && direction != 2) {
            direction = 0;
        } else if (KEY_PRESSED(KEY_RIGHT) && direction != 3) {
            direction = 1;
        } else if (KEY_PRESSED(KEY_DOWN) && direction != 0) {
            direction = 2;
        } else if (KEY_PRESSED(KEY_LEFT) && direction != 1) {
            direction = 3;
        }

        if(KEY_PRESSED(KEY_A) && snek_len < MAX_SNEK_LEN) {
            snek_len++;
        } 

        row = WRAP(row+directions[direction][0], SCREEN_HEIGHT);
        col = WRAP(col+directions[direction][1], SCREEN_WIDTH);
        
        VRAM[row][col] = snek_color;

        history[hist_head] = (row << 8) | col;
        hist_head++;

        int wrapped_tail = hist_tail < hist_head ? hist_tail : hist_tail - MAX_SNEK_LEN;

        if(hist_head - wrapped_tail == snek_len) {
            VRAM[row_val(history[hist_tail])][col_val(history[hist_tail])] = 0;
            hist_tail++;
        }

        if (hist_head == MAX_SNEK_LEN) hist_head = 0;
        if (hist_tail == MAX_SNEK_LEN) hist_tail = 0;
    };
    return 0;
}