// Copyright 2022 Stefan Kerkmann (KarlK90)
// Copyright 2022 Ulrich Spörlein (@uqs)
// Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// Copyright 2019 Sunjun Kim
// Copyright 2020 Ploopy Corporation
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

#if !defined(PMW33XX_CPI)
#    define PMW33XX_CPI 1600U
#endif

#define PMW33XX_CPI_STEP 100
#define PMW33XX_CPI_MIN 100
#define PMW33XX_CPI_MAX 5000

// PMW3325 register addresses
// clang-format off
#define REG_Product_ID                 0x00
#define REG_Motion                     0x02
#define REG_Delta_X_L                  0x03
#define REG_Delta_X_H                  0x04
#define REG_Delta_Y_L                  0x05
#define REG_Delta_Y_H                  0x06
#define REG_SQUAL                      0x07
#define REG_Pixel_Sum                  0x08
#define REG_Maximum_Pixel              0x09
#define REG_Minimum_Pixel              0x0a
#define REG_Shutter_Lower              0x0b
#define REG_Shutter_Upper              0x0c
#define REG_Observation                0x15
#define REG_Motion_Burst               0x16
#define REG_Frame_Capture_Status       0x19
#define REG_Resolution                 0x1b
#define REG_Angle_Snap                 0x1e
#define REG_Axis_Control               0x20
#define REG_Run_Downshift              0x24
#define REG_Rest1_Period               0x25
#define REG_Rest1_Downshift            0x26
#define REG_Rest2_Preiod               0x27
#define REG_Rest2_Downshift            0x28
#define REG_Rest3_Period               0x29
#define REG_Frame_Capture              0x32
#define REG_Power_Up_Reset             0x3a
#define REG_Shutdown                   0x3b
#define REG_Inverse_Product_ID         0x3f
#define REG_Config                     0x40
// clang-format on
