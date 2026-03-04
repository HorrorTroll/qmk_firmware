// Copyright 2021 gloryhzw
// Copyright 2026 fightforlife
// Copyright 2026 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

// Size optimizations to fit VIA support
//
// These optimizations are pretty intrusive because they remove the idle thread.
// Without idle thread some sleep related functions no longer function.
// These broken functions are overwritten with patched variants.
//
// These optimization are copied from:
// https://github.com/gloryhzw/qmk_firmware/blob/sn32/keyboards/gmmk/full
//
// This file adds patch sleep related functions that work without idle threads.

#include "quantum.h"

/*
 * must override sleep functions without idle thread
 */
#define CYCLES_PER_LOOP 9
#define LOOP_TIMES (48000000 / (CH_CFG_ST_FREQUENCY) / (CYCLES_PER_LOOP))

void chThdSleep(sysinterval_t time)  {
    uint32_t loops = time * LOOP_TIMES;
    for (uint32_t i = 0; i < loops; i++) __NOP();
}

/* suspend thread used in usb_main.c */
msg_t chThdSuspendTimeoutS(thread_reference_t* trp, sysinterval_t timeout) {
    osalSysUnlock();
    osalSysLock();
    return MSG_OK;
}
