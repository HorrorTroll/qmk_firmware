// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <mcuconf.h>

#undef SN32_PWM_USE_CT16B1
#define SN32_PWM_USE_CT16B1 TRUE

#undef SN32_PWM_NO_RESET
#define SN32_PWM_NO_RESET TRUE
