// Copyright (c) 2024 Hansem Ro <hansemro@outlook.com>
// Copyright 2025 HorrorTroll <https://github.com/HorrorTroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifdef RGB_MATRIX_ENABLE

#include "my937x.h"
#include "rgb_matrix.h"
#include "host.h"

/*
 * Channel Setup:
 *          ┌────────┐
 *     SDI->│        │->SDO1_2
 *  R_COL0<-│        │->Unused
 *  G_COL0<-│        │->B_COL4
 *  B_COL0<-│        │->G_COL4
 *  R_COL1<-│ MY937X │->R_COL4
 *  G_COL1<-│   A    │->B_COL3
 *  B_COL1<-│        │->G_COL3
 *  R_COL2<-│        │->R_COL3
 *  G_COL2<-│        │->B_COL2
 *          └────────┘
 *          ┌────────┐
 *  SDO1_2->│        │->SDO2_3
 *  R_COL5<-│        │->unused
 *  G_COL5<-│        │->B_COL9
 *  B_COL5<-│        │->G_COL9
 *  R_COL6<-│ MY937X │->R_COL9
 *  G_COL6<-│   B    │->B_COL8
 *  B_COL6<-│        │->G_COL8
 *  R_COL7<-│        │->R_COL8
 *  G_COL7<-│        │->B_COL7
 *          └────────┘
 *          ┌────────┐
 *  SDO2_3->│        │->SDO
 * R_COL10<-│        │->unused
 * G_COL10<-│        │->B_COL14
 * B_COL10<-│        │->G_COL14
 * R_COL11<-│ MY937X │->R_COL14
 * G_COL11<-│   C    │->B_COL13
 * B_COL11<-│        │->G_COL13
 * R_COL12<-│        │->R_COL13
 * G_COL12<-│        │->B_COL12
 *          └────────┘
 */
// clang-format off
const my937x_channel_t g_my937x_channels[MY937X_NUM_DRIVER][MY937X_NUM_CHANNELS] = {
    /* MY937X Driver A */
    {
    /*    Color channel (Red, Green, Blue, or Unused)
     *    |
     *    |                 MY937X managed col index
     *    |                 | */
        { MY937X_RED_CH,    0 }, /* MY937X output channel 0 */
        { MY937X_GREEN_CH,  0 }, /* MY937X output channel 1 */
        { MY937X_BLUE_CH,   0 }, /* ... */
        { MY937X_RED_CH,    1 },
        { MY937X_GREEN_CH,  1 },
        { MY937X_BLUE_CH,   1 },
        { MY937X_RED_CH,    2 },
        { MY937X_GREEN_CH,  2 },
        { MY937X_BLUE_CH,   2 },
        { MY937X_RED_CH,    3 },
        { MY937X_GREEN_CH,  3 },
        { MY937X_BLUE_CH,   3 },
        { MY937X_RED_CH,    4 },
        { MY937X_GREEN_CH,  4 },
        { MY937X_BLUE_CH,   4 },
        { MY937X_UNUSED_CH, 0 }, /* MY937X output channel 15 */
    },
    /* MY937X Driver B */
    {
        { MY937X_RED_CH,    5 },
        { MY937X_GREEN_CH,  5 },
        { MY937X_BLUE_CH,   5 },
        { MY937X_RED_CH,    6 },
        { MY937X_GREEN_CH,  6 },
        { MY937X_BLUE_CH,   6 },
        { MY937X_RED_CH,    7 },
        { MY937X_GREEN_CH,  7 },
        { MY937X_BLUE_CH,   7 },
        { MY937X_RED_CH,    8 },
        { MY937X_GREEN_CH,  8 },
        { MY937X_BLUE_CH,   8 },
        { MY937X_RED_CH,    9 },
        { MY937X_GREEN_CH,  9 },
        { MY937X_BLUE_CH,   9 },
        { MY937X_UNUSED_CH, 0 },
    },
    /* MY937X Driver C */
    {
        { MY937X_RED_CH,    10 },
        { MY937X_GREEN_CH,  10 },
        { MY937X_BLUE_CH,   10 },
        { MY937X_RED_CH,    11 },
        { MY937X_GREEN_CH,  11 },
        { MY937X_BLUE_CH,   11 },
        { MY937X_RED_CH,    12 },
        { MY937X_GREEN_CH,  12 },
        { MY937X_BLUE_CH,   12 },
        { MY937X_RED_CH,    13 },
        { MY937X_GREEN_CH,  13 },
        { MY937X_BLUE_CH,   13 },
        { MY937X_RED_CH,    14 },
        { MY937X_GREEN_CH,  14 },
        { MY937X_BLUE_CH,   14 },
        { MY937X_UNUSED_CH, 0  },
    },
};
// clang-format on
// LED Matrix to LED Index
// Since ROW2COL, MY937X manages columns while MCU manages rows
// clang-format off
const uint8_t g_my937x_led_matrix_co[MY937X_NUM_LED_GPIO_PINS][MY937X_NUM_CHANNELS] = {
    /* Cols:            0       1       2       3       4       5       6       7       8       9      10      11      12      13      14  */
    /* Row 0: */ {      0,      1,      2,      3,      4,      5,      6,      7,      8,      9,     10,     11,     12,     13, NO_LED },
    /* Row 1: */ {     14,     15,     16,     17,     18,     19,     20,     21,     22,     23,     24,     25,     26,     27,     28 },
    /* Row 2: */ {     29,     30,     31,     32,     33,     34,     35,     36,     37,     38,     39,     40, NO_LED,     41,     42 },
    /* Row 3: */ {     43, NO_LED,     44,     45,     46,     47,     48,     49,     50,     51,     52,     53,     54,     55,     56 },
    /* Row 4: */ {     57,     58,     59, NO_LED, NO_LED, NO_LED,     60, NO_LED, NO_LED, NO_LED,     61,     62,     63,     64,     65 },
};
// clang-format on


#endif
