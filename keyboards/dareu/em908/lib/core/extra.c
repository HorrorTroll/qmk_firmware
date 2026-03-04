// Copyright 2024 Dimitris Mantzouranis <d3xter93@gmail.com>
// Copyright 2026 fightforlife
// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <hal.h>

void early_hardware_init_post(void) {
	SN_SYS0->EXRSTCTRL_b.RESETDIS = 1; // Disable RESET
	SN_SYS0->SWDCTRL_b.SWDDIS = 1; // Disable SWD
}
