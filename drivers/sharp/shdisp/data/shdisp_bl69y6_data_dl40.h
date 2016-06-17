/* drivers/sharp/shdisp/data/shdisp_bl69y6_data_dl40.h  (Display Driver)
 *
 * Copyright (C) 2011-2012 SHARP CORPORATION
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

/* ------------------------------------------------------------------------- */
/* SHARP DISPLAY DRIVER FOR KERNEL MODE                                      */
/* ------------------------------------------------------------------------- */

#ifndef SHDISP_BL69Y6_DATA_DL40_H
#define SHDISP_BL69Y6_DATA_DL40_H

/* ------------------------------------------------------------------------- */
/* INCLUDE FILES                                                             */
/* ------------------------------------------------------------------------- */

#include "../shdisp_bl69y6.h"

/* ------------------------------------------------------------------------- */
/* MACROS                                                                    */
/* ------------------------------------------------------------------------- */

#define SHDISP_BKL_FIX_TBL_NUM          23
#define SHDISP_BKL_AUTO_PWM_TBL_NUM     65

#define SHDISP_BKL_AUTO_TBL_NUM         16
#define SHDISP_TRI_LED_COLOR_TBL_NUM    8
#define NUM_SHDISP_BKL_TBL_MODE         (SHDISP_BKL_TBL_MODE_CHARGE + 1)

#define SHDISP_INT_ENABLE_GFAC          0x002C0308
#define SHDISP_LUX_CHANGE_LEVEL1        0x0B
#define SHDISP_LUX_CHANGE_LEVEL2        0x01

#define CABC_LUX_LEVEL_LUT0_1           0x01
#define CABC_LUX_LEVEL_LUT1_2           0x02
#define CABC_LUX_LEVEL_LUT2_3           0x03
#define CABC_LUX_LEVEL_LUT3_4           0x04
#define CABC_LUX_LEVEL_LUT4_5           0x05

#define SHDISP_COL_VARI_KIND            3
#define SHDISP_HANDSET_COLOR_WHITE      0x01
#define SHDISP_HANDSET_COLOR_PINK       0x02
#define SHDISP_HANDSET_COLOR_BLACK      0x06

/* ------------------------------------------------------------------------- */
/* MACROS(Register Value)                                                    */
/* ------------------------------------------------------------------------- */

#define BDIC_REG_SYSTEM4_VAL            0x28
#define BDIC_REG_SLOPE_VAL              0xCB
#define BDIC_REG_DCDC1_VLIM_VAL         0x78
#define BDIC_REG_DCDC_SYS_VAL           0x20
#define BDIC_REG_DCDC2_VO_VAL           0xDC
#define BDIC_REG_M1LED_VAL              0xBC
#define BDIC_REG_M2LED_VAL              0xBC
#define BDIC_REG_SYSTEM2_BKL            0x03

#define BDIC_REG_ALS_ADJ0_L_DEFAULT_A     0x5C
#define BDIC_REG_ALS_ADJ0_H_DEFAULT_A     0x3F
#define BDIC_REG_ALS_ADJ1_L_DEFAULT_A     0xAA
#define BDIC_REG_ALS_ADJ1_H_DEFAULT_A     0x4C
#define BDIC_REG_ALS_SHIFT_DEFAULT_A      0x03
#define BDIC_REG_CLEAR_OFFSET_DEFAULT_A   0x00
#define BDIC_REG_IR_OFFSET_DEFAULT_A      0x00

#define BDIC_REG_ALS_ADJ0_L_DEFAULT_B     0xAC
#define BDIC_REG_ALS_ADJ0_H_DEFAULT_B     0x0C
#define BDIC_REG_ALS_ADJ1_L_DEFAULT_B     0x00
#define BDIC_REG_ALS_ADJ1_H_DEFAULT_B     0x00
#define BDIC_REG_ALS_SHIFT_DEFAULT_B      0x03
#define BDIC_REG_CLEAR_OFFSET_DEFAULT_B   0x00
#define BDIC_REG_IR_OFFSET_DEFAULT_B      0x00

/* ------------------------------------------------------------------------- */
/* VARIABLES                                                                 */
/* ------------------------------------------------------------------------- */

static const unsigned short shdisp_main_bkl_tbl[SHDISP_BKL_FIX_TBL_NUM][NUM_SHDISP_BKL_TBL_MODE] = {
    { 0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0010,    0x0010,    0x0010,    0x0010 },
    { 0x0010,    0x0010,    0x0010,    0x0010 },
    { 0x0010,    0x0010,    0x0010,    0x0010 },
    { 0x001A,    0x0020,    0x001A,    0x001A },
    { 0x002A,    0x0025,    0x0020,    0x002A },
    { 0x003A,    0x002A,    0x0025,    0x003A },
    { 0x004A,    0x002F,    0x002A,    0x004A },
    { 0x005F,    0x003A,    0x0035,    0x005F },
    { 0x006F,    0x0044,    0x003F,    0x006F },
    { 0x0089,    0x004F,    0x004F,    0x0089 },
    { 0x00A3,    0x005F,    0x005F,    0x00A3 },
    { 0x00C8,    0x0074,    0x0074,    0x00C8 },
    { 0x00ED,    0x0089,    0x0074,    0x00ED },
    { 0x0121,    0x00A3,    0x0074,    0x0121 },
    { 0x015B,    0x00C3,    0x0074,    0x015B },
    { 0x019B,    0x00E8,    0x0074,    0x019B },
    { 0x01E4,    0x0112,    0x0074,    0x01E4 },
    { 0x0238,    0x0141,    0x0074,    0x0238 },
    { 0x0292,    0x0176,    0x0074,    0x0292 },
    { 0x02FB,    0x01B0,    0x0074,    0x02FB },
    { 0x036A,    0x01F4,    0x0074,    0x036A },
    { 0x03E8,    0x023E,    0x0074,    0x03E8 },
};

static const unsigned short shdisp_main_dtv_bkl_tbl[SHDISP_BKL_FIX_TBL_NUM][NUM_SHDISP_BKL_TBL_MODE] = {
    { 0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0010,    0x0010,    0x0010,    0x0010 },
    { 0x0010,    0x0010,    0x0010,    0x0010 },
    { 0x0010,    0x0010,    0x0010,    0x0010 },
    { 0x001A,    0x0020,    0x001A,    0x001A },
    { 0x002A,    0x0025,    0x0020,    0x002A },
    { 0x003A,    0x002A,    0x0025,    0x003A },
    { 0x004A,    0x002F,    0x002A,    0x004A },
    { 0x005F,    0x003A,    0x0035,    0x005F },
    { 0x006F,    0x0044,    0x003F,    0x006F },
    { 0x0089,    0x004F,    0x004F,    0x0089 },
    { 0x00A3,    0x005F,    0x005F,    0x00A3 },
    { 0x00C8,    0x0074,    0x0074,    0x00C8 },
    { 0x00ED,    0x0089,    0x0074,    0x00ED },
    { 0x0121,    0x00A3,    0x0074,    0x0121 },
    { 0x015B,    0x00C3,    0x0074,    0x015B },
    { 0x019B,    0x00E8,    0x0074,    0x019B },
    { 0x01E4,    0x0112,    0x0074,    0x01E4 },
    { 0x0238,    0x0141,    0x0074,    0x0238 },
    { 0x0292,    0x0176,    0x0074,    0x0292 },
    { 0x02FB,    0x01B0,    0x0074,    0x02FB },
    { 0x036A,    0x01F4,    0x0074,    0x036A },
    { 0x03E8,    0x023E,    0x0074,    0x03E8 },
};

static const unsigned short shdisp_main_bkl_pwm_low_tbl[NUM_SHDISP_BKL_TBL_MODE][SHDISP_BKL_AUTO_PWM_TBL_NUM] = {
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0021, 0x0032, 0x0040,
        0x004D, 0x0062, 0x0073, 0x0092, 0x00C8, 0x00E7, 0x00F8, 0x010B,
        0x012C, 0x0147, 0x0177, 0x0198, 0x01C6, 0x01E7, 0x0200, 0x0226,
        0x0251, 0x0281, 0x02A3, 0x02D7, 0x0300, 0x0322, 0x0341, 0x03E8
    },
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0019, 0x0023, 0x0028,
        0x002E, 0x0036, 0x003E, 0x004D, 0x0068, 0x0073, 0x007D, 0x0087,
        0x0096, 0x00A5, 0x00C6, 0x00DB, 0x00FA, 0x010D, 0x0121, 0x013C,
        0x015A, 0x017B, 0x0194, 0x01BB, 0x01D7, 0x01EF, 0x0202, 0x0241
    },
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0019, 0x0023, 0x0028,
        0x002E, 0x0036, 0x003E, 0x004D, 0x0068, 0x0073, 0x007D, 0x0087,
        0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087,
        0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087
    },
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x002A, 0x0043, 0x005E,
        0x0073, 0x009A, 0x00C0, 0x0104, 0x0194, 0x01E1, 0x0228, 0x0285,
        0x0328, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8,
        0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8
    }
};

static const unsigned short shdisp_main_bkl_pwm_high_tbl[NUM_SHDISP_BKL_TBL_MODE][SHDISP_BKL_AUTO_PWM_TBL_NUM] = {
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0021, 0x0032, 0x0040,
        0x004D, 0x0062, 0x0073, 0x0092, 0x00C8, 0x00E7, 0x00F8, 0x010B,
        0x012C, 0x0147, 0x0177, 0x0198, 0x01C6, 0x01E7, 0x0200, 0x0226,
        0x0251, 0x0281, 0x02A3, 0x02D7, 0x0300, 0x0322, 0x0341, 0x03E8
    },
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0019, 0x0023, 0x0028,
        0x002E, 0x0036, 0x003E, 0x004D, 0x0068, 0x0073, 0x007D, 0x0087,
        0x0096, 0x00A5, 0x00C6, 0x00DB, 0x00FA, 0x010D, 0x0121, 0x013C,
        0x015A, 0x017B, 0x0194, 0x01BB, 0x01D7, 0x01EF, 0x0202, 0x0241
    },
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x0019, 0x0023, 0x0028,
        0x002E, 0x0036, 0x003E, 0x004D, 0x0068, 0x0073, 0x007D, 0x0087,
        0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087,
        0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087, 0x0087
    },
    {
        0x1D00,
        0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0006, 0x0009, 0x000B,
        0x000E, 0x0014, 0x001A, 0x002A, 0x0061, 0x0089, 0x00B3, 0x00DF,
        0x013C, 0x0186, 0x024C, 0x02F6, 0x0444, 0x05D3, 0x0769, 0x0AA9,
        0x0FAD, 0x184C, 0x2128, 0x3364, 0x4621, 0x5941, 0x6CB1, 0x7791,
        0x0013, 0x0013, 0x0013, 0x0013, 0x0013, 0x002A, 0x0043, 0x005E,
        0x0073, 0x009A, 0x00C0, 0x0104, 0x0194, 0x01E1, 0x0228, 0x0285,
        0x0328, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8,
        0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8, 0x03E8
    }
};

static const unsigned char shdisp_main_bkl_opt_low_tbl[SHDISP_BKL_AUTO_TBL_NUM][1+NUM_SHDISP_BKL_TBL_MODE] = {
    { BDIC_REG_OPT0,    0x0A,   0x06,   0x06,   0x0A },
    { BDIC_REG_OPT1,    0x0C,   0x07,   0x07,   0x0C },
    { BDIC_REG_OPT2,    0x10,   0x09,   0x09,   0x10 },
    { BDIC_REG_OPT3,    0x15,   0x0C,   0x0C,   0x15 },
    { BDIC_REG_OPT4,    0x1A,   0x0F,   0x0F,   0x1C },
    { BDIC_REG_OPT5,    0x20,   0x13,   0x13,   0x23 },
    { BDIC_REG_OPT6,    0x26,   0x16,   0x16,   0x2C },
    { BDIC_REG_OPT7,    0x2C,   0x1A,   0x1A,   0x35 },
    { BDIC_REG_OPT8,    0x33,   0x1D,   0x1D,   0x40 },
    { BDIC_REG_OPT9,    0x3A,   0x22,   0x1D,   0x4C },
    { BDIC_REG_OPT10,   0x42,   0x26,   0x1D,   0x5A },
    { BDIC_REG_OPT11,   0x4A,   0x2B,   0x1D,   0x69 },
    { BDIC_REG_OPT12,   0x52,   0x2F,   0x1D,   0x71 },
    { BDIC_REG_OPT13,   0x5A,   0x34,   0x1D,   0x77 },
    { BDIC_REG_OPT14,   0x65,   0x3B,   0x1D,   0x7D },
    { BDIC_REG_OPT15,   0x83,   0x47,   0x1D,   0x83 }
};

static const unsigned char shdisp_main_dtv_bkl_opt_low_tbl[SHDISP_BKL_AUTO_TBL_NUM][1+NUM_SHDISP_BKL_TBL_MODE] = {
    { BDIC_REG_OPT0,    0x0A,   0x06,   0x06,   0x0A },
    { BDIC_REG_OPT1,    0x0C,   0x07,   0x07,   0x0C },
    { BDIC_REG_OPT2,    0x10,   0x09,   0x09,   0x10 },
    { BDIC_REG_OPT3,    0x15,   0x0C,   0x0C,   0x15 },
    { BDIC_REG_OPT4,    0x1A,   0x0F,   0x0F,   0x1C },
    { BDIC_REG_OPT5,    0x20,   0x13,   0x13,   0x23 },
    { BDIC_REG_OPT6,    0x26,   0x16,   0x16,   0x2C },
    { BDIC_REG_OPT7,    0x2C,   0x1A,   0x1A,   0x35 },
    { BDIC_REG_OPT8,    0x33,   0x1D,   0x1D,   0x40 },
    { BDIC_REG_OPT9,    0x3A,   0x22,   0x1D,   0x4C },
    { BDIC_REG_OPT10,   0x42,   0x26,   0x1D,   0x5A },
    { BDIC_REG_OPT11,   0x4A,   0x2B,   0x1D,   0x69 },
    { BDIC_REG_OPT12,   0x52,   0x2F,   0x1D,   0x71 },
    { BDIC_REG_OPT13,   0x5A,   0x34,   0x1D,   0x77 },
    { BDIC_REG_OPT14,   0x65,   0x3B,   0x1D,   0x7D },
    { BDIC_REG_OPT15,   0x83,   0x47,   0x1D,   0x83 }
};

static const unsigned char shdisp_main_bkl_opt_high_tbl[SHDISP_BKL_AUTO_TBL_NUM][1+NUM_SHDISP_BKL_TBL_MODE] = {
    { BDIC_REG_OPT0,    0x52,   0x2F,   0x1D,   0x71 },
    { BDIC_REG_OPT1,    0x58,   0x33,   0x1D,   0x76 },
    { BDIC_REG_OPT2,    0x5E,   0x36,   0x1D,   0x7A },
    { BDIC_REG_OPT3,    0x63,   0x39,   0x1D,   0x7C },
    { BDIC_REG_OPT4,    0x68,   0x3C,   0x1D,   0x7F },
    { BDIC_REG_OPT5,    0x6D,   0x3F,   0x1D,   0x80 },
    { BDIC_REG_OPT6,    0x73,   0x42,   0x1D,   0x82 },
    { BDIC_REG_OPT7,    0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT8,    0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT9,    0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT10,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT11,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT12,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT13,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT14,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT15,   0x83,   0x47,   0x1D,   0x83 }
};

static const unsigned char shdisp_main_dtv_bkl_opt_high_tbl[SHDISP_BKL_AUTO_TBL_NUM][1+NUM_SHDISP_BKL_TBL_MODE] = {
    { BDIC_REG_OPT0,    0x52,   0x2F,   0x1D,   0x71 },
    { BDIC_REG_OPT1,    0x58,   0x33,   0x1D,   0x76 },
    { BDIC_REG_OPT2,    0x5E,   0x36,   0x1D,   0x7A },
    { BDIC_REG_OPT3,    0x63,   0x39,   0x1D,   0x7C },
    { BDIC_REG_OPT4,    0x68,   0x3C,   0x1D,   0x7F },
    { BDIC_REG_OPT5,    0x6D,   0x3F,   0x1D,   0x80 },
    { BDIC_REG_OPT6,    0x73,   0x42,   0x1D,   0x82 },
    { BDIC_REG_OPT7,    0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT8,    0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT9,    0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT10,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT11,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT12,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT13,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT14,   0x83,   0x47,   0x1D,   0x83 },
    { BDIC_REG_OPT15,   0x83,   0x47,   0x1D,   0x83 }
};



static const struct shdisp_bdic_led_color_index shdisp_triple_led_color_index_tbl[SHDISP_TRI_LED_COLOR_TBL_NUM] = {
    {0, 0, 0,  0}
   ,{1, 0, 0,  1}
   ,{0, 1, 0,  2}
   ,{1, 1, 0,  3}
   ,{0, 0, 1,  4}
   ,{1, 0, 1,  5}
   ,{0, 1, 1,  6}
   ,{1, 1, 1,  7}
};

static const unsigned char shdisp_clrvari_index[SHDISP_COL_VARI_KIND] = {
    SHDISP_HANDSET_COLOR_BLACK,
    SHDISP_HANDSET_COLOR_PINK,
    SHDISP_HANDSET_COLOR_WHITE
};

static const unsigned char shdisp_triple_led_tbl[SHDISP_COL_VARI_KIND][SHDISP_TRI_LED_COLOR_TBL_NUM][3] = {
  {
    { 0x00, 0x00, 0x00 },
    { 0x0C, 0x00, 0x00 },
    { 0x00, 0x0D, 0x00 },
    { 0x07, 0x0E, 0x00 },
    { 0x00, 0x00, 0x17 },
    { 0x0A, 0x00, 0x09 },
    { 0x00, 0x12, 0x09 },
    { 0x06, 0x0C, 0x0F }
  },
  {
    { 0x00, 0x00, 0x00 },
    { 0x0C, 0x00, 0x00 },
    { 0x00, 0x0D, 0x00 },
    { 0x07, 0x0E, 0x00 },
    { 0x00, 0x00, 0x17 },
    { 0x0A, 0x00, 0x09 },
    { 0x00, 0x12, 0x09 },
    { 0x06, 0x0C, 0x0F }
  },
  {
    { 0x00, 0x00, 0x00 },
    { 0x0C, 0x00, 0x00 },
    { 0x00, 0x0D, 0x00 },
    { 0x07, 0x0E, 0x00 },
    { 0x00, 0x00, 0x17 },
    { 0x0A, 0x00, 0x09 },
    { 0x00, 0x12, 0x09 },
    { 0x06, 0x0C, 0x0F }
  }
};

static const unsigned char shdisp_triple_led_anime_tbl[SHDISP_COL_VARI_KIND][2][SHDISP_TRI_LED_COLOR_TBL_NUM][3] = {
  {
    {
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 }
    },
    {
        { 0x00, 0x00, 0x00 },
        { 0x0C, 0x00, 0x00 },
        { 0x00, 0x0D, 0x00 },
        { 0x07, 0x0E, 0x00 },
        { 0x00, 0x00, 0x17 },
        { 0x0A, 0x00, 0x09 },
        { 0x00, 0x12, 0x09 },
        { 0x06, 0x0C, 0x0F }
    }
  },
  {
    {
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 }
    },
    {
        { 0x00, 0x00, 0x00 },
        { 0x0C, 0x00, 0x00 },
        { 0x00, 0x0D, 0x00 },
        { 0x07, 0x0E, 0x00 },
        { 0x00, 0x00, 0x17 },
        { 0x0A, 0x00, 0x09 },
        { 0x00, 0x12, 0x09 },
        { 0x06, 0x0C, 0x0F }
    }
  },
  {
    {
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 },
        { 0x00, 0x00, 0x00 }
    },
    {
        { 0x00, 0x00, 0x00 },
        { 0x0C, 0x00, 0x00 },
        { 0x00, 0x0D, 0x00 },
        { 0x07, 0x0E, 0x00 },
        { 0x00, 0x00, 0x17 },
        { 0x0A, 0x00, 0x09 },
        { 0x00, 0x12, 0x09 },
        { 0x06, 0x0C, 0x0F }
    }
  }
};

static const unsigned char shdisp_main_bkl_chg_high_tbl[14][2] = {
    { 0x00, 0xA9 },
    { 0x00, 0xA5 },
    { 0x00, 0xB4 },
    { 0x00, 0xAE },
    { 0x00, 0xBE },
    { 0x00, 0xBB },
    { 0x00, 0xC6 },
    { 0x00, 0xC3 },
    { 0x00, 0xD0 },
    { 0x00, 0xCC },
    { 0x00, 0xD8 },
    { 0x00, 0xD6 },
    { 0x00, 0xE3 },
    { 0x00, 0xE0 }
};

static const struct shdisp_bdic_bkl_lux_str shdisp_bdic_bkl_lux_tbl[2][32] = {
    {
        { 0, 0x0000,      0 },
        { 0, 0x0001,      0 },
        { 0, 0x0002,     10 },
        { 0, 0x0003,     15 },
        { 0, 0x0004,     20 },
        { 0, 0x0007,     30 },
        { 0, 0x0009,     40 },
        { 0, 0x000C,     50 },
        { 0, 0x0010,     60 },
        { 0, 0x0016,     80 },
        { 0, 0x0021,    100 },
        { 0, 0x0045,    150 },
        { 0, 0x0074,    300 },
        { 0, 0x009D,    400 },
        { 0, 0x00C8,    500 },
        { 0, 0x010D,    600 },
        { 0, 0x0160,    800 },
        { 0, 0x01E8,   1000 },
        { 0, 0x02A0,   1500 },
        { 0, 0x039C,   2000 },
        { 0, 0x050B,   3000 },
        { 0, 0x069D,   4000 },
        { 0, 0x0908,   5000 },
        { 0, 0x0D2A,   7000 },
        { 0, 0x13FC,  10000 },
        { 0, 0x1CB9,  15000 },
        { 0, 0x2A45,  20000 },
        { 0, 0x3CC2,  30000 },
        { 0, 0x4FB0,  40000 },
        { 0, 0x62F8,  50000 },
        { 0, 0x7220,  60000 },
        { 0, 0xFFFF,  65535 }
    },
    {
        { 0, 0x0000,      0 },
        { 0, 0x0001,      0 },
        { 0, 0x0002,     10 },
        { 0, 0x0003,     15 },
        { 0, 0x0004,     20 },
        { 0, 0x0007,     30 },
        { 0, 0x0009,     40 },
        { 0, 0x000C,     50 },
        { 0, 0x0010,     60 },
        { 0, 0x0016,     80 },
        { 0, 0x0021,    100 },
        { 0, 0x0045,    150 },
        { 0, 0x0074,    300 },
        { 0, 0x009D,    400 },
        { 0, 0x00C8,    500 },
        { 0, 0x010D,    600 },
        { 0, 0x0160,    800 },
        { 0, 0x01E8,   1000 },
        { 0, 0x02A0,   1500 },
        { 0, 0x039C,   2000 },
        { 0, 0x050B,   3000 },
        { 0, 0x069D,   4000 },
        { 0, 0x0908,   5000 },
        { 0, 0x0D2A,   7000 },
        { 0, 0x13FC,  10000 },
        { 0, 0x1CB9,  15000 },
        { 0, 0x2A45,  20000 },
        { 0, 0x3CC2,  30000 },
        { 0, 0x4FB0,  40000 },
        { 0, 0x62F8,  50000 },
        { 0, 0x7220,  60000 },
        { 0, 0xFFFF,  65535 }
    }
};

/* ------------------------------------------------------------------------- */
/* TYPES                                                                     */
/* ------------------------------------------------------------------------- */



/* ------------------------------------------------------------------------- */
/* PROTOTYPES                                                                */
/* ------------------------------------------------------------------------- */



#endif /* SHDISP_BL69Y6_DATA_DL40_H */

/* ------------------------------------------------------------------------- */
/* END OF FILE                                                               */
/* ------------------------------------------------------------------------- */
