#include "snake.h"

#include <stdlib.h>

#include "timer.h"
#include "lib/lib8tion/lib8tion.h"

snake_status_t snake_status = {
};

void snake_init() {
  snake_status.direction = DIRECTION_RIGHT;
  snake_status.last_moved_direction = DIRECTION_RIGHT;
  snake_status.snake[3] = 16;
  snake_status.snake[2] = 15;
  snake_status.snake[1] = 14;
  snake_status.snake[0] = 13;
  snake_status.snake_head_idx = 3;
  snake_status.snake_length = 4;
  snake_status.food = 20;
  snake_status.score = 0;
  snake_status.snake_anim_counter = 250;
  snake_status.snake_ms_per_move = 250;

  srand(timer_read32());
}

// wraps around automatically
uint8_t next_cell_wraparound(uint8_t cell, char direction) {
  uint8_t row = cell / SNAKE_GRID_WIDTH;
  uint8_t col = cell % SNAKE_GRID_WIDTH;
  uint8_t next_row = row;
  uint8_t next_col = col;
  switch (direction) {
  case DIRECTION_RIGHT:
    next_col = (col == SNAKE_GRID_WIDTH - 1) ? 0 : col + 1;
    break;
  case DIRECTION_UP:
    next_row = (row == 0) ? SNAKE_GRID_HEIGHT - 1 : row - 1;
    break;
  case DIRECTION_LEFT:
    next_col = (col == 0) ? SNAKE_GRID_WIDTH - 1 : col - 1;
    break;
  case DIRECTION_DOWN:
    next_row = (row == SNAKE_GRID_HEIGHT - 1) ? 0 : row + 1;
    break;
  }
  return (next_row * SNAKE_GRID_WIDTH) + next_col;
}

void game_over(void) {
  // just start the game again
  snake_init();
}


bool collides_with_snake(uint8_t cell) {
  uint8_t snake_head_idx = snake_status.snake_head_idx;
  for (uint8_t i = 0; i < snake_status.snake_length; i++) {
    uint8_t snake_idx = (snake_head_idx >= i) ? snake_head_idx - i : SNAKE_LENGTH_MAX - (i - snake_head_idx);
    uint8_t snake_cell = snake_status.snake[snake_idx];

    if (snake_cell == cell) {
      return true;
    }
  }

  return false;
}

bool collides_with_food(uint8_t cell) {
  return snake_status.food == cell;
}

// moves the food to a new location (avoids colliding with snake)
//
// noop if snake is its max length (or longer)
void change_food(void) {
  if (snake_status.snake_length >= SNAKE_LENGTH_MAX) {
    return;
  }
  uint8_t food = rand() % (SNAKE_GRID_HEIGHT * SNAKE_GRID_WIDTH);
  while (collides_with_snake(food)) {
    food--;
  }
  snake_status.food = food;
}

void move_snake(void) {
  uint8_t snake_head_cell = snake_status.snake[snake_status.snake_head_idx];
  uint8_t next_cell = next_cell_wraparound(snake_head_cell, snake_status.direction);
  if (collides_with_snake(next_cell)) {
    game_over();
  } else {
    if (collides_with_food(next_cell) && snake_status.snake_length < SNAKE_LENGTH_MAX) {
      snake_status.snake_length++;
      snake_status.score++;
      uint8_t delta_ms = 5 * snake_status.score;
      snake_status.snake_ms_per_move = (snake_status.snake_ms_per_move > SNAKE_MS_PER_MOVE_MIN + delta_ms) ?
        snake_status.snake_ms_per_move - delta_ms :
        SNAKE_MS_PER_MOVE_MIN;
      change_food();
    }
    uint8_t next_snake_head_idx = (snake_status.snake_head_idx == SNAKE_LENGTH_MAX - 1) ? 0 : snake_status.snake_head_idx + 1;
    snake_status.snake[next_snake_head_idx] = next_cell;
    snake_status.snake_head_idx = next_snake_head_idx;
    snake_status.last_moved_direction = snake_status.direction;
  }
}

// assuming plank_mit layout has per-key RGB layout
// and only one RGB beneath the space key
void update_bitmap_from_cell(RGB bitmap[102], uint8_t cell, RGB color) {
  if (cell < 102) {
    bitmap[cell].r = color.r;
    bitmap[cell].g = color.g;
    bitmap[cell].b = color.b;
  } else if (cell == 102 || cell == 103) {
    // additive average of the two
    bitmap[102].r += color.r / 2;
    bitmap[102].g += color.g / 2;
    bitmap[102].b += color.b / 2;
  } else if (cell > 103) {
    bitmap[cell - 1].r = color.r;
    bitmap[cell - 1].g = color.g;
    bitmap[cell - 1].b = color.b;
  }
}

void render_to_bitmap(RGB bitmap[102]) {
  // snake
  uint8_t snake_head_idx = snake_status.snake_head_idx;
  for (uint8_t i = 0; i < snake_status.snake_length; i++) {
    uint8_t snake_idx = (snake_head_idx >= i) ? snake_head_idx - i : SNAKE_LENGTH_MAX - (i - snake_head_idx);
    uint8_t snake_cell = snake_status.snake[snake_idx];
    uint8_t hue = 0xF0 / snake_status.snake_length * i;
    uint8_t val = 0xFF - (0x2F / snake_status.snake_length * i);
    HSV snake_color_hsv = { hue, 0xFF, val };
    RGB snake_color = hsv_to_rgb(snake_color_hsv);
    update_bitmap_from_cell(bitmap, snake_cell, snake_color);
  }

  // food
  uint8_t food_val = 0xB0 + (cos8(snake_status.food_timer / 4) / 4);
  HSV food_color_hsv = {0xFF, 0x00, food_val}; // white-ish
  RGB food_color = hsv_to_rgb(food_color_hsv);
  update_bitmap_from_cell(bitmap, snake_status.food, food_color);
}

void snake_update(uint32_t delta_time, RGB bitmap[102]) {
  if (snake_status.snake_anim_counter > delta_time) {
    snake_status.snake_anim_counter -= delta_time;
  } else {
    uint32_t remaining = delta_time - snake_status.snake_anim_counter;
    snake_status.snake_anim_counter = snake_status.snake_ms_per_move + remaining;
    move_snake();
  }

  // ignore/accept overflow
  snake_status.food_timer += delta_time;

  render_to_bitmap(bitmap);
}