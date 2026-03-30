#include "gba.h"

#define MAX_SNEK_LEN 100

const color snek_color = MAKE_COLOR(31, 0, 0);

typedef unsigned int location;

inline location make_loc(int row, int col) { return (row << 8) | col; }
inline unsigned int col_val(location loc) {return loc & 0xff;}
inline unsigned int row_val(location loc) {return (loc & 0xff00) >> 8;}


typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction;

const int directions[4][2] = {
    [UP]    = {-1, 0}, 
    [RIGHT] = {0, 1}, 
    [DOWN]  = {1, 0}, 
    [LEFT]  = {0, -1}, 
};


int main() {
    REG_DISPCNT = DCNT_BG2 | DCNT_MODE3;

    unsigned int snek_len = 10;

    int row=50, col=50;
    direction dir = UP;

    location history[MAX_SNEK_LEN] = {};
    int hist_head = 0;
    int hist_tail = 0;

    while(1) {
        vblank();
        if(KEY_HELD(KEY_UP) && dir != DOWN) {
            dir = UP;
        } else if (KEY_HELD(KEY_RIGHT) && dir != LEFT) {
            dir = RIGHT;
        } else if (KEY_HELD(KEY_DOWN) && dir != UP) {
            dir = DOWN;
        } else if (KEY_HELD(KEY_LEFT) && dir != RIGHT) {
            dir = LEFT;
        }

        row = WRAP(row+directions[dir][0], SCREEN_HEIGHT);
        col = WRAP(col+directions[dir][1], SCREEN_WIDTH);
        
        VRAM[row][col] = snek_color;

        history[hist_head] = make_loc(row, col);
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