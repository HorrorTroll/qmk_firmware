RGB_MATRIX_EFFECT(SNAKE)
RGB_MATRIX_EFFECT(KITT)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#define SNAKE_FPS 12

#include "snake.h"

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

// variable for startup animation
bool BASE_EFFECT_NOT_STARTED_YET = true;
uint8_t base_effect_startup_counter = 255;

uint8_t led_count = 13;
uint8_t led_first = 36;
static uint8_t time_to_led(uint8_t time, uint8_t led_behind) {
    uint16_t led_time = led_count * time;
    uint16_t step = ((2 * led_count + (led_time / 128)) - led_behind) % (2 * led_count);
    uint8_t led;
    if (step < led_count) {
        led = step;
    } else {
        led = led_count - 1 - (step - led_count);
    }
    return led;
}

static HSV KITT_math(HSV hsv, uint8_t i, uint8_t time) {

    // reset base effect startup
    if (i == 0) {
        BASE_EFFECT_NOT_STARTED_YET = true;
    }

    hsv.h = 0;
    hsv.s = 255;

    if (i >= led_first && i < led_first + led_count) {
        uint8_t j = i - led_first;
        if (j == time_to_led(time, 0)) {
            hsv.v = hsv.v;
        } else if (j == time_to_led(time, 1)) {
            hsv.v = hsv.v/2;
        } else if (j == time_to_led(time, 2)) {
            hsv.v = hsv.v/4;
        } else if (j == time_to_led(time, 3)) {
            hsv.v = hsv.v/8;
        } else {
            hsv.v = 0;
        }
    } else {
        hsv.v = 0;
    }

    return hsv;
}

bool KITT(effect_params_t* params) {
    return effect_runner_i(params, &KITT_math);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS