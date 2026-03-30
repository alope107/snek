#include "gba.h"

#define MAX_SNEK_LEN 1000

const color SNEK_COLOR = MAKE_COLOR(31, 0, 0);
const color FILL_COLOR = MAKE_COLOR(0, 0, 31);

// A location allows for packing 16 bit row and column values into a single word
typedef unsigned int location;
inline location make_loc(int row, int col) { return (row << 8) | col; }
inline unsigned int get_col(location loc) {return loc & 0xff;}
inline unsigned int get_row(location loc) {return (loc & 0xff00) >> 8;}

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction;

// row, column deltas for each direction
const int directions[4][2] = {
    [UP]    = {-1, 0}, 
    [RIGHT] = {0, 1}, 
    [DOWN]  = {1, 0}, 
    [LEFT]  = {0, -1}, 
};


// Fills the entire screen with the given color
void fill_screen(color fill_color) {
    // In this function we treat VRAM as a 2D array of ints, despite each pixel actually being a short
    // This allows us to fill the screen in half as many cycles

    // Repeat the color twice in an unsigned int so both pixels will be set to the same color
    unsigned int repeated_fill = fill_color | fill_color << 16;
    for(int r = 0; r < SCREEN_HEIGHT; r++) {
        // Only need to go to SCREEN_WIDTH/2 because we fill two pixels with each step
        for(int c = 0; c < SCREEN_WIDTH/2; c++) {
            VRAM_INT[r][c] = repeated_fill;
        }
    }
}

int main() {
    // Set the GBA to use background 2 and mode 3 (unbuffered direct color mode)
    REG_DISPCNT = DCNT_BG2 | DCNT_MODE3;

    fill_screen(FILL_COLOR);

    unsigned int snek_len = 10;

    int row=50, col=50;
    direction dir = UP;

    // history holds the coordinates of the previous locations in a circular buffer
    location history[MAX_SNEK_LEN] = {};
    
    // start and end of the circular buffer. Starts at 0 because the snake hasn't gone anywhere yet
    int hist_head = 0;
    int hist_tail = 0;

    while(1) {
        // wait for next vblank, enforces one step per frame (approx 60fps)
        vblank();

        if(KEY_HELD(KEY_START)) {
            fill_screen(FILL_COLOR);
        }

        if(KEY_HELD(KEY_UP) && dir != DOWN) {
            dir = UP;
        } else if (KEY_HELD(KEY_RIGHT) && dir != LEFT) {
            dir = RIGHT;
        } else if (KEY_HELD(KEY_DOWN) && dir != UP) {
            dir = DOWN;
        } else if (KEY_HELD(KEY_LEFT) && dir != RIGHT) {
            dir = LEFT;
        }

        // Grow the snek if A held
        if(KEY_HELD(KEY_A) && snek_len < MAX_SNEK_LEN) {
            snek_len++;
        }

        // Shrink the snek if B held
        if(KEY_HELD(KEY_B) && snek_len > 2) {
            snek_len--;
        } 
        else { // Only move forward if snek is not shrinking
            // snek wraps around screen horizontally and vertically
            row = WRAP(row+directions[dir][0], SCREEN_HEIGHT);
            col = WRAP(col+directions[dir][1], SCREEN_WIDTH);

            // draw to VRAM and update history buffer
            VRAM[row][col] = SNEK_COLOR;
            history[hist_head] = make_loc(row, col);
            hist_head++;
        }

        // if head and tail are on opposite sides of the buffer, wrap to normalize
        int wrapped_tail = hist_tail < hist_head ? hist_tail : hist_tail - MAX_SNEK_LEN;

        // Do not start earasing tail until the snek has reached its full length
        if(hist_head - wrapped_tail == snek_len) {
            VRAM[get_row(history[hist_tail])][get_col(history[hist_tail])] = 0;
            hist_tail++;
        }

        // wrap head/tail in buffer if needed
        if (hist_head == MAX_SNEK_LEN) hist_head = 0;
        if (hist_tail == MAX_SNEK_LEN) hist_tail = 0;
    };
    return 0;
}