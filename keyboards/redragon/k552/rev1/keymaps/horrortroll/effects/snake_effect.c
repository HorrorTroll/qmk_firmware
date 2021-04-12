#define SNAKE_FPS 12

#include "effects/snake/snake.h"

uint32_t timer = 0;
uint32_t last_updated = 0;

RGB bitmap[102];

static void clear_bitmap(void) {
  for (uint8_t i = 0; i < 102; i++) {
    bitmap[i].r = 0;
    bitmap[i].g = 0;
    bitmap[i].b = 0;
  }
}

static void render(uint8_t led_min, uint8_t led_max) {
  for (uint8_t i = led_min; i < led_max; i++) {
      if (i < 102) {
          RGB rgb = bitmap[i];
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
      } else {
          rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
      }
  }
}

static bool SNAKE(effect_params_t* params) {
  if (timer > g_rgb_timer) {
    return false;
  }

  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  render(led_min, led_max);

  if (led_max == DRIVER_LED_TOTAL) {
    clear_bitmap();
    uint32_t delta_time = timer_elapsed32(last_updated);
    snake_update(delta_time, bitmap);
    last_updated = timer_read32();

    timer = g_rgb_timer + (1000 / SNAKE_FPS);
  }

  return led_max < DRIVER_LED_TOTAL;
}