#include "effects/glitter/glitter.h"

static HSV GLITTER_math(HSV hsv, uint8_t i, uint8_t time, uint16_t time2) {
    hsv.h = g_led_config.point[i].x - time;
    hsv.v = time2 + rand() % DRIVER_LED_TOTAL;
    return hsv;
}

bool GLITTER(effect_params_t* params) { return effect_glitter(params, &GLITTER_math); }
