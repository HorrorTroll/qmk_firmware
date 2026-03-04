// Copyright 2021 gloryhzw
// Copyright 2026 fightforlife
// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

// Small rand() implementation
//
// The libc rand implementation takes to much flash space, so an smaller rand() function is used. 
#include "quantum.h"

static uint32_t seed = 0;

int rand(void) {
    // If seed is 0 (first run), grab the system uptime.
    // We OR it with 1 to ensure the seed is never 0.
    if (!seed) {
        seed = timer_read32() | 1;
    }

    // Classic LCG: seed = (a * seed + c)
    seed = (1103515245 * seed + 12345);
    
    // Mask to 0x7FFFFFFF to ensure the result is a positive 31-bit integer
    return (int)(seed & 0x7FFFFFFF);
}
