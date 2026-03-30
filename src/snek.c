#include "gba.h"

#define MAX_SNEK_LEN 100

const color snek_color = MAKE_COLOR(31, 0, 0);
const color food_color = MAKE_COLOR(0, 0, 31);

inline int col_val(int loc) {return loc & 0xff;}
inline int row_val(int loc) {return (loc & 0xff00) >> 8;}

inline unsigned int prng(unsigned int *seed, unsigned int max) {
    *seed ^= *seed << 13;
    *seed ^= *seed >> 17;
    *seed ^= *seed << 5;
    return *seed % max;
}

void place_food(unsigned int *seed) {
    int placed = 0;

    while(!placed) {
        int test_row = prng(seed, SCREEN_HEIGHT);
        int test_col = prng(seed, SCREEN_WIDTH);

        if (VRAM[test_row][test_col] == 0) {
            placed = 1;
            VRAM[test_row][test_col] = food_color;
        }
    }
}

int main() {
    REG_DISPCNT = DCNT_BG2 | DCNT_MODE3;

    // Randomly chosen from a Broadway song
    unsigned int seed = 525600;

    unsigned int snek_len = 10;

    place_food(&seed);

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

        
        if(KEY_PRESSED(KEY_B)) {
            place_food(&seed);
        }

        row = WRAP(row+directions[direction][0], SCREEN_HEIGHT);
        col = WRAP(col+directions[direction][1], SCREEN_WIDTH);

        if(VRAM[row][col] == food_color && snek_len < MAX_SNEK_LEN) {
            snek_len++;
        } 
        
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