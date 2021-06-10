RGB_MATRIX_EFFECT(CUSTOM_GRADIENT)
RGB_MATRIX_EFFECT(DIAGONAL)
RGB_MATRIX_EFFECT(RAINBOW_REACTIVE_SIMPLE)
RGB_MATRIX_EFFECT(KITT)
RGB_MATRIX_EFFECT(GLITTER)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "effects/custom_gradient.c"
#include "effects/diagonal.c"
#include "effects/rainbow_reactive_simple.c"
#include "effects/kitt.c"
#include "effects/glitter/glitter.h"

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS