#pragma once

#include <stdint.h>

#include "color.h"

#define DIRECTION_RIGHT 0
#define DIRECTION_UP 1
#define DIRECTION_LEFT 2
#define DIRECTION_DOWN 3

#define SNAKE_LENGTH_MAX 48

#define SNAKE_MS_PER_MOVE_MIN 80

// Number of columns on the grid.
#define SNAKE_GRID_WIDTH 17

#define SNAKE_GRID_HEIGHT 6

// uint8_t represents index of cell
// in 4x12 grid.
// idx := row * COLS + col.
typedef struct _snake_status_t {
  char direction;
  char last_moved_direction;
  uint8_t snake[SNAKE_LENGTH_MAX];
  uint8_t snake_head_idx;
  uint8_t snake_length;
  uint8_t food;
  // to create a "pulsing" animation
  uint8_t food_timer;
  uint8_t score;
  // when to 'move' the snake
  uint32_t snake_anim_counter;
  // how many ms to add to each counter
  uint16_t snake_ms_per_move;
} snake_status_t;

extern snake_status_t snake_status;

void snake_init(void);
void snake_update(uint32_t delta_time, RGB bitmap[102]);