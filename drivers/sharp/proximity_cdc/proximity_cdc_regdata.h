/* driver/sharp/proximity_CDC/proximity_cdc_regdata.h  (proximityCDCSensor Driver)
 *
 * Copyright (C) 2013 SHARP CORPORATION All rights reserved.
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
/* SHARP PROXIMITY SENSOR DRIVER FOR KERNEL MODE                             */
/* ------------------------------------------------------------------------- */

#ifndef PROXIMITY_CDC_REGDATA_H
#define PROXIMITY_CDC_REGDATA_H

/* ------------------------------------------------------------------------- */
/* INCLUDE FILES                                                             */
/* ------------------------------------------------------------------------- */

#include <sharp/proximity_cdc.h>

/* ------------------------------------------------------------------------- */
/* MACROS                                                                   */
/* ------------------------------------------------------------------------- */

/* AD7146 register address */
#define AD7146_REG_PWR_CONTROL                     0x0000
#define AD7146_REG_STAGE_CAL_EN                    0x0001
#define AD7146_REG_AMB_COMP_CTRL0                  0x0002
#define AD7146_REG_AMB_COMP_CTRL1                  0x0003
#define AD7146_REG_AMB_COMP_CTRL2                  0x0004
#define AD7146_REG_STAGE_LOW_INT_ENABLE            0x0005
#define AD7146_REG_STAGE_HIGH_INT_ENABLE           0x0006
#define AD7146_REG_STAGE_COMPLETE_INT_ENABLE       0x0007
#define AD7146_REG_STAGE_LOW_INT_STATUS            0x0008
#define AD7146_REG_STAGE_HIGH_INT_STATUS           0x0009
#define AD7146_REG_STAGE_COMPLETE_INT_STATUS       0x000A
#define AD7146_REG_CDC_RESULT_S0                   0x000B
#define AD7146_REG_CDC_RESULT_S1                   0x000C
#define AD7146_REG_CDC_RESULT_S2                   0x000D
#define AD7146_REG_CDC_RESULT_S3                   0x000E
#define AD7146_REG_CDC_RESULT_S4                   0x000F
#define AD7146_REG_CDC_RESULT_S5                   0x0010
#define AD7146_REG_CDC_RESULT_S6                   0x0011
#define AD7146_REG_CDC_RESULT_S7                   0x0012
#define AD7146_REG_CDC_RESULT_S8                   0x0013
#define AD7146_REG_CDC_RESULT_S9                   0x0014
#define AD7146_REG_CDC_RESULT_S10                  0x0015
#define AD7146_REG_CDC_RESULT_S11                  0x0016
#define AD7146_REG_DEVICE_ID                       0x0017
#define AD7146_REG_PROXIMITY_STATUS                0x0042
#define AD7146_REG_MODULATOR_CONTROL               0x0045
#define AD7146_REG_STAGE0_CONNECTION_6_0           0x0080
#define AD7146_REG_STAGE0_CONNECTION_12_7          0x0081
#define AD7146_REG_STAGE0_AFE_OFFSET               0x0082
#define AD7146_REG_STAGE0_SENSITIVITY              0x0083
#define AD7146_REG_STAGE0_OFFSET_LOW               0x0084
#define AD7146_REG_STAGE0_OFFSET_HIGH              0x0085
#define AD7146_REG_STAGE0_OFFSET_HIGH_CLAMP        0x0086
#define AD7146_REG_STAGE0_OFFSET_LOW_CLAMP         0x0087
#define AD7146_REG_STAGE1_CONNECTION_6_0           0x0088
#define AD7146_REG_STAGE1_CONNECTION_12_7          0x0089
#define AD7146_REG_STAGE1_AFE_OFFSET               0x008A
#define AD7146_REG_STAGE1_SENSITIVITY              0x008B
#define AD7146_REG_STAGE1_OFFSET_LOW               0x008C
#define AD7146_REG_STAGE1_OFFSET_HIGH              0x008D
#define AD7146_REG_STAGE1_OFFSET_HIGH_CLAMP        0x008E
#define AD7146_REG_STAGE1_OFFSET_LOW_CLAMP         0x008F
#define AD7146_REG_STAGE2_CONNECTION_6_0           0x0090
#define AD7146_REG_STAGE2_CONNECTION_12_7          0x0091
#define AD7146_REG_STAGE2_AFE_OFFSET               0x0092
#define AD7146_REG_STAGE2_SENSITIVITY              0x0093
#define AD7146_REG_STAGE2_OFFSET_LOW               0x0094
#define AD7146_REG_STAGE2_OFFSET_HIGH              0x0095
#define AD7146_REG_STAGE2_OFFSET_HIGH_CLAMP        0x0096
#define AD7146_REG_STAGE2_OFFSET_LOW_CLAMP         0x0097
#define AD7146_REG_STAGE3_CONNECTION_6_0           0x0098
#define AD7146_REG_STAGE3_CONNECTION_12_7          0x0099
#define AD7146_REG_STAGE3_AFE_OFFSET               0x009A
#define AD7146_REG_STAGE3_SENSITIVITY              0x009B
#define AD7146_REG_STAGE3_OFFSET_LOW               0x009C
#define AD7146_REG_STAGE3_OFFSET_HIGH              0x009D
#define AD7146_REG_STAGE3_OFFSET_HIGH_CLAMP        0x009E
#define AD7146_REG_STAGE3_OFFSET_LOW_CLAMP         0x009F
#define AD7146_REG_STAGE4_CONNECTION_6_0           0x00A0
#define AD7146_REG_STAGE4_CONNECTION_12_7          0x00A1
#define AD7146_REG_STAGE4_AFE_OFFSET               0x00A2
#define AD7146_REG_STAGE4_SENSITIVITY              0x00A3
#define AD7146_REG_STAGE4_OFFSET_LOW               0x00A4
#define AD7146_REG_STAGE4_OFFSET_HIGH              0x00A5
#define AD7146_REG_STAGE4_OFFSET_HIGH_CLAMP        0x00A6
#define AD7146_REG_STAGE4_OFFSET_LOW_CLAMP         0x00A7
#define AD7146_REG_STAGE5_CONNECTION_6_0           0x00A8
#define AD7146_REG_STAGE5_CONNECTION_12_7          0x00A9
#define AD7146_REG_STAGE5_AFE_OFFSET               0x00AA
#define AD7146_REG_STAGE5_SENSITIVITY              0x00AB
#define AD7146_REG_STAGE5_OFFSET_LOW               0x00AC
#define AD7146_REG_STAGE5_OFFSET_HIGH              0x00AD
#define AD7146_REG_STAGE5_OFFSET_HIGH_CLAMP        0x00AE
#define AD7146_REG_STAGE5_OFFSET_LOW_CLAMP         0x00AF
#define AD7146_REG_STAGE6_CONNECTION_6_0           0x00B0
#define AD7146_REG_STAGE6_CONNECTION_12_7          0x00B1
#define AD7146_REG_STAGE6_AFE_OFFSET               0x00B2
#define AD7146_REG_STAGE6_SENSITIVITY              0x00B3
#define AD7146_REG_STAGE6_OFFSET_LOW               0x00B4
#define AD7146_REG_STAGE6_OFFSET_HIGH              0x00B5
#define AD7146_REG_STAGE6_OFFSET_HIGH_CLAMP        0x00B6
#define AD7146_REG_STAGE6_OFFSET_LOW_CLAMP         0x00B7
#define AD7146_REG_STAGE7_CONNECTION_6_0           0x00B8
#define AD7146_REG_STAGE7_CONNECTION_12_7          0x00B9
#define AD7146_REG_STAGE7_AFE_OFFSET               0x00BA
#define AD7146_REG_STAGE7_SENSITIVITY              0x00BB
#define AD7146_REG_STAGE7_OFFSET_LOW               0x00BC
#define AD7146_REG_STAGE7_OFFSET_HIGH              0x00BD
#define AD7146_REG_STAGE7_OFFSET_HIGH_CLAMP        0x00BE
#define AD7146_REG_STAGE7_OFFSET_LOW_CLAMP         0x00BF
#define AD7146_REG_STAGE8_CONNECTION_6_0           0x00C0
#define AD7146_REG_STAGE8_CONNECTION_12_7          0x00C1
#define AD7146_REG_STAGE8_AFE_OFFSET               0x00C2
#define AD7146_REG_STAGE8_SENSITIVITY              0x00C3
#define AD7146_REG_STAGE8_OFFSET_LOW               0x00C4
#define AD7146_REG_STAGE8_OFFSET_HIGH              0x00C5
#define AD7146_REG_STAGE8_OFFSET_HIGH_CLAMP        0x00C6
#define AD7146_REG_STAGE8_OFFSET_LOW_CLAMP         0x00C7
#define AD7146_REG_STAGE9_CONNECTION_6_0           0x00C8
#define AD7146_REG_STAGE9_CONNECTION_12_7          0x00C9
#define AD7146_REG_STAGE9_AFE_OFFSET               0x00CA
#define AD7146_REG_STAGE9_SENSITIVITY              0x00CB
#define AD7146_REG_STAGE9_OFFSET_LOW               0x00CC
#define AD7146_REG_STAGE9_OFFSET_HIGH              0x00CD
#define AD7146_REG_STAGE9_OFFSET_HIGH_CLAMP        0x00CE
#define AD7146_REG_STAGE9_OFFSET_LOW_CLAMP         0x00CF
#define AD7146_REG_STAGE10_CONNECTION_6_0          0x00D0
#define AD7146_REG_STAGE10_CONNECTION_12_7         0x00D1
#define AD7146_REG_STAGE10_AFE_OFFSET              0x00D2
#define AD7146_REG_STAGE10_SENSITIVITY             0x00D3
#define AD7146_REG_STAGE10_OFFSET_LOW              0x00D4
#define AD7146_REG_STAGE10_OFFSET_HIGH             0x00D5
#define AD7146_REG_STAGE10_OFFSET_HIGH_CLAMP       0x00D6
#define AD7146_REG_STAGE10_OFFSET_LOW_CLAMP        0x00D7
#define AD7146_REG_STAGE11_CONNECTION_6_0          0x00D8
#define AD7146_REG_STAGE11_CONNECTION_12_7         0x00D9
#define AD7146_REG_STAGE11_AFE_OFFSET              0x00DA
#define AD7146_REG_STAGE11_SENSITIVITY             0x00DB
#define AD7146_REG_STAGE11_OFFSET_LOW              0x00DC
#define AD7146_REG_STAGE11_OFFSET_HIGH             0x00DD
#define AD7146_REG_STAGE11_OFFSET_HIGH_CLAMP       0x00DE
#define AD7146_REG_STAGE11_OFFSET_LOW_CLAMP        0x00DF
#define AD7146_REG_STAGE0_CONV_DATA                0x00E0
#define AD7146_REG_STAGE0_FF_WORD0                 0x00E1
#define AD7146_REG_STAGE0_FF_WORD1                 0x00E2
#define AD7146_REG_STAGE0_FF_WORD2                 0x00E3
#define AD7146_REG_STAGE0_FF_WORD3                 0x00E4
#define AD7146_REG_STAGE0_FF_WORD4                 0x00E5
#define AD7146_REG_STAGE0_FF_WORD5                 0x00E6
#define AD7146_REG_STAGE0_FF_WORD6                 0x00E7
#define AD7146_REG_STAGE0_FF_WORD7                 0x00E8
#define AD7146_REG_STAGE0_SF_WORD0                 0x00E9
#define AD7146_REG_STAGE0_SF_WORD1                 0x00EA
#define AD7146_REG_STAGE0_SF_WORD2                 0x00EB
#define AD7146_REG_STAGE0_SF_WORD3                 0x00EC
#define AD7146_REG_STAGE0_SF_WORD4                 0x00ED
#define AD7146_REG_STAGE0_SF_WORD5                 0x00EE
#define AD7146_REG_STAGE0_SF_WORD6                 0x00EF
#define AD7146_REG_STAGE0_SF_WORD7                 0x00F0
#define AD7146_REG_STAGE0_SF_AMBIENT               0x00F1
#define AD7146_REG_STAGE0_FF_AVG                   0x00F2
#define AD7146_REG_STAGE0_PEAK_DETECT_WORD0        0x00F3
#define AD7146_REG_STAGE0_PEAK_DETECT_WORD1        0x00F4
#define AD7146_REG_STAGE0_MAX_WORD0                0x00F5
#define AD7146_REG_STAGE0_MAX_WORD1                0x00F6
#define AD7146_REG_STAGE0_MAX_WORD2                0x00F7
#define AD7146_REG_STAGE0_MAX_WORD3                0x00F8
#define AD7146_REG_STAGE0_MAX_AVG                  0x00F9
#define AD7146_REG_STAGE0_HIGH_THRESHOLD           0x00FA
#define AD7146_REG_STAGE0_MAX_TEMP                 0x00FB
#define AD7146_REG_STAGE0_MIN_WORD0                0x00FC
#define AD7146_REG_STAGE0_MIN_WORD1                0x00FD
#define AD7146_REG_STAGE0_MIN_WORD2                0x00FE
#define AD7146_REG_STAGE0_MIN_WORD3                0x00FF
#define AD7146_REG_STAGE0_MIN_AVG                  0x0100
#define AD7146_REG_STAGE0_LOW_THRESHOLD            0x0101
#define AD7146_REG_STAGE0_MIN_TEMP                 0x0102
#define AD7146_REG_STAGE1_CONV_DATA                0x0104
#define AD7146_REG_STAGE1_FF_WORD0                 0x0105
#define AD7146_REG_STAGE1_FF_WORD1                 0x0106
#define AD7146_REG_STAGE1_FF_WORD2                 0x0107
#define AD7146_REG_STAGE1_FF_WORD3                 0x0108
#define AD7146_REG_STAGE1_FF_WORD4                 0x0109
#define AD7146_REG_STAGE1_FF_WORD5                 0x010A
#define AD7146_REG_STAGE1_FF_WORD6                 0x010B
#define AD7146_REG_STAGE1_FF_WORD7                 0x010C
#define AD7146_REG_STAGE1_SF_WORD0                 0x010D
#define AD7146_REG_STAGE1_SF_WORD1                 0x010E
#define AD7146_REG_STAGE1_SF_WORD2                 0x010F
#define AD7146_REG_STAGE1_SF_WORD3                 0x0110
#define AD7146_REG_STAGE1_SF_WORD4                 0x0111
#define AD7146_REG_STAGE1_SF_WORD5                 0x0112
#define AD7146_REG_STAGE1_SF_WORD6                 0x0113
#define AD7146_REG_STAGE1_SF_WORD7                 0x0114
#define AD7146_REG_STAGE1_SF_AMBIENT               0x0115
#define AD7146_REG_STAGE1_FF_AVG                   0x0116
#define AD7146_REG_STAGE1_PEAK_DETECT_WORD0        0x0117
#define AD7146_REG_STAGE1_PEAK_DETECT_WORD1        0x0118
#define AD7146_REG_STAGE1_MAX_WORD0                0x0119
#define AD7146_REG_STAGE1_MAX_WORD1                0x011A
#define AD7146_REG_STAGE1_MAX_WORD2                0x011B
#define AD7146_REG_STAGE1_MAX_WORD3                0x011C
#define AD7146_REG_STAGE1_MAX_AVG                  0x011D
#define AD7146_REG_STAGE1_HIGH_THRESHOLD           0x011E
#define AD7146_REG_STAGE1_MAX_TEMP                 0x011F
#define AD7146_REG_STAGE1_MIN_WORD0                0x0120
#define AD7146_REG_STAGE1_MIN_WORD1                0x0121
#define AD7146_REG_STAGE1_MIN_WORD2                0x0122
#define AD7146_REG_STAGE1_MIN_WORD3                0x0123
#define AD7146_REG_STAGE1_MIN_AVG                  0x0124
#define AD7146_REG_STAGE1_LOW_THRESHOLD            0x0125
#define AD7146_REG_STAGE1_MIN_TEMP                 0x0126
#define AD7146_REG_STAGE2_CONV_DATA                0x0128
#define AD7146_REG_STAGE2_FF_WORD0                 0x0129
#define AD7146_REG_STAGE2_FF_WORD1                 0x012A
#define AD7146_REG_STAGE2_FF_WORD2                 0x012B
#define AD7146_REG_STAGE2_FF_WORD3                 0x012C
#define AD7146_REG_STAGE2_FF_WORD4                 0x012D
#define AD7146_REG_STAGE2_FF_WORD5                 0x012E
#define AD7146_REG_STAGE2_FF_WORD6                 0x012F
#define AD7146_REG_STAGE2_FF_WORD7                 0x0130
#define AD7146_REG_STAGE2_SF_WORD0                 0x0131
#define AD7146_REG_STAGE2_SF_WORD1                 0x0132
#define AD7146_REG_STAGE2_SF_WORD2                 0x0133
#define AD7146_REG_STAGE2_SF_WORD3                 0x0134
#define AD7146_REG_STAGE2_SF_WORD4                 0x0135
#define AD7146_REG_STAGE2_SF_WORD5                 0x0136
#define AD7146_REG_STAGE2_SF_WORD6                 0x0137
#define AD7146_REG_STAGE2_SF_WORD7                 0x0138
#define AD7146_REG_STAGE2_SF_AMBIENT               0x0139
#define AD7146_REG_STAGE2_FF_AVG                   0x013A
#define AD7146_REG_STAGE2_PEAK_DETECT_WORD0        0x013B
#define AD7146_REG_STAGE2_PEAK_DETECT_WORD1        0x013C
#define AD7146_REG_STAGE2_MAX_WORD0                0x013D
#define AD7146_REG_STAGE2_MAX_WORD1                0x013E
#define AD7146_REG_STAGE2_MAX_WORD2                0x013F
#define AD7146_REG_STAGE2_MAX_WORD3                0x0140
#define AD7146_REG_STAGE2_MAX_AVG                  0x0141
#define AD7146_REG_STAGE2_HIGH_THRESHOLD           0x0142
#define AD7146_REG_STAGE2_MAX_TEMP                 0x0143
#define AD7146_REG_STAGE2_MIN_WORD0                0x0144
#define AD7146_REG_STAGE2_MIN_WORD1                0x0145
#define AD7146_REG_STAGE2_MIN_WORD2                0x0146
#define AD7146_REG_STAGE2_MIN_WORD3                0x0147
#define AD7146_REG_STAGE2_MIN_AVG                  0x0148
#define AD7146_REG_STAGE2_LOW_THRESHOLD            0x0149
#define AD7146_REG_STAGE2_MIN_TEMP                 0x014A
#define AD7146_REG_STAGE3_CONV_DATA                0x014C
#define AD7146_REG_STAGE3_FF_WORD0                 0x014D
#define AD7146_REG_STAGE3_FF_WORD1                 0x014E
#define AD7146_REG_STAGE3_FF_WORD2                 0x014F
#define AD7146_REG_STAGE3_FF_WORD3                 0x0150
#define AD7146_REG_STAGE3_FF_WORD4                 0x0151
#define AD7146_REG_STAGE3_FF_WORD5                 0x0152
#define AD7146_REG_STAGE3_FF_WORD6                 0x0153
#define AD7146_REG_STAGE3_FF_WORD7                 0x0154
#define AD7146_REG_STAGE3_SF_WORD0                 0x0155
#define AD7146_REG_STAGE3_SF_WORD1                 0x0156
#define AD7146_REG_STAGE3_SF_WORD2                 0x0157
#define AD7146_REG_STAGE3_SF_WORD3                 0x0158
#define AD7146_REG_STAGE3_SF_WORD4                 0x0159
#define AD7146_REG_STAGE3_SF_WORD5                 0x015A
#define AD7146_REG_STAGE3_SF_WORD6                 0x015B
#define AD7146_REG_STAGE3_SF_WORD7                 0x015C
#define AD7146_REG_STAGE3_SF_AMBIENT               0x015D
#define AD7146_REG_STAGE3_FF_AVG                   0x015E
#define AD7146_REG_STAGE3_PEAK_DETECT_WORD0        0x015F
#define AD7146_REG_STAGE3_PEAK_DETECT_WORD1        0x0160
#define AD7146_REG_STAGE3_MAX_WORD0                0x0161
#define AD7146_REG_STAGE3_MAX_WORD1                0x0162
#define AD7146_REG_STAGE3_MAX_WORD2                0x0163
#define AD7146_REG_STAGE3_MAX_WORD3                0x0164
#define AD7146_REG_STAGE3_MAX_AVG                  0x0165
#define AD7146_REG_STAGE3_HIGH_THRESHOLD           0x0166
#define AD7146_REG_STAGE3_MAX_TEMP                 0x0167
#define AD7146_REG_STAGE3_MIN_WORD0                0x0168
#define AD7146_REG_STAGE3_MIN_WORD1                0x0169
#define AD7146_REG_STAGE3_MIN_WORD2                0x016A
#define AD7146_REG_STAGE3_MIN_WORD3                0x016B
#define AD7146_REG_STAGE3_MIN_AVG                  0x016C
#define AD7146_REG_STAGE3_LOW_THRESHOLD            0x016D
#define AD7146_REG_STAGE3_MIN_TEMP                 0x016F
#define AD7146_REG_STAGE4_CONV_DATA                0x0170
#define AD7146_REG_STAGE4_FF_WORD0                 0x0171
#define AD7146_REG_STAGE4_FF_WORD1                 0x0172
#define AD7146_REG_STAGE4_FF_WORD2                 0x0173
#define AD7146_REG_STAGE4_FF_WORD3                 0x0174
#define AD7146_REG_STAGE4_FF_WORD4                 0x0175
#define AD7146_REG_STAGE4_FF_WORD5                 0x0176
#define AD7146_REG_STAGE4_FF_WORD6                 0x0177
#define AD7146_REG_STAGE4_FF_WORD7                 0x0178
#define AD7146_REG_STAGE4_SF_WORD0                 0x0179
#define AD7146_REG_STAGE4_SF_WORD1                 0x017A
#define AD7146_REG_STAGE4_SF_WORD2                 0x017B
#define AD7146_REG_STAGE4_SF_WORD3                 0x017C
#define AD7146_REG_STAGE4_SF_WORD4                 0x017D
#define AD7146_REG_STAGE4_SF_WORD5                 0x017E
#define AD7146_REG_STAGE4_SF_WORD6                 0x017F
#define AD7146_REG_STAGE4_SF_WORD7                 0x0180
#define AD7146_REG_STAGE4_SF_AMBIENT               0x0181
#define AD7146_REG_STAGE4_FF_AVG                   0x0182
#define AD7146_REG_STAGE4_PEAK_DETECT_WORD0        0x0183
#define AD7146_REG_STAGE4_PEAK_DETECT_WORD1        0x0184
#define AD7146_REG_STAGE4_MAX_WORD0                0x0185
#define AD7146_REG_STAGE4_MAX_WORD1                0x0186
#define AD7146_REG_STAGE4_MAX_WORD2                0x0187
#define AD7146_REG_STAGE4_MAX_WORD3                0x0188
#define AD7146_REG_STAGE4_MAX_AVG                  0x0189
#define AD7146_REG_STAGE4_HIGH_THRESHOLD           0x018A
#define AD7146_REG_STAGE4_MAX_TEMP                 0x018B
#define AD7146_REG_STAGE4_MIN_WORD0                0x018C
#define AD7146_REG_STAGE4_MIN_WORD1                0x018D
#define AD7146_REG_STAGE4_MIN_WORD2                0x018E
#define AD7146_REG_STAGE4_MIN_WORD3                0x018F
#define AD7146_REG_STAGE4_MIN_AVG                  0x0190
#define AD7146_REG_STAGE4_LOW_THRESHOLD            0x0191
#define AD7146_REG_STAGE4_MIN_TEMP                 0x0192
#define AD7146_REG_STAGE5_CONV_DATA                0x0194
#define AD7146_REG_STAGE5_FF_WORD0                 0x0195
#define AD7146_REG_STAGE5_FF_WORD1                 0x0196
#define AD7146_REG_STAGE5_FF_WORD2                 0x0197
#define AD7146_REG_STAGE5_FF_WORD3                 0x0198
#define AD7146_REG_STAGE5_FF_WORD4                 0x0199
#define AD7146_REG_STAGE5_FF_WORD5                 0x019A
#define AD7146_REG_STAGE5_FF_WORD6                 0x019B
#define AD7146_REG_STAGE5_FF_WORD7                 0x019C
#define AD7146_REG_STAGE5_SF_WORD0                 0x019D
#define AD7146_REG_STAGE5_SF_WORD1                 0x019E
#define AD7146_REG_STAGE5_SF_WORD2                 0x019F
#define AD7146_REG_STAGE5_SF_WORD3                 0x01A0
#define AD7146_REG_STAGE5_SF_WORD4                 0x01A1
#define AD7146_REG_STAGE5_SF_WORD5                 0x01A2
#define AD7146_REG_STAGE5_SF_WORD6                 0x01A3
#define AD7146_REG_STAGE5_SF_WORD7                 0x01A4
#define AD7146_REG_STAGE5_SF_AMBIENT               0x01A5
#define AD7146_REG_STAGE5_FF_AVG                   0x01A6
#define AD7146_REG_STAGE5_PEAK_DETECT_WORD0        0x01A7
#define AD7146_REG_STAGE5_PEAK_DETECT_WORD1        0x01A8
#define AD7146_REG_STAGE5_MAX_WORD0                0x01A9
#define AD7146_REG_STAGE5_MAX_WORD1                0x01AA
#define AD7146_REG_STAGE5_MAX_WORD2                0x01AB
#define AD7146_REG_STAGE5_MAX_WORD3                0x01AC
#define AD7146_REG_STAGE5_MAX_AVG                  0x01AD
#define AD7146_REG_STAGE5_HIGH_THRESHOLD           0x01AE
#define AD7146_REG_STAGE5_MAX_TEMP                 0x01AF
#define AD7146_REG_STAGE5_MIN_WORD0                0x01B0
#define AD7146_REG_STAGE5_MIN_WORD1                0x01B1
#define AD7146_REG_STAGE5_MIN_WORD2                0x01B2
#define AD7146_REG_STAGE5_MIN_WORD3                0x01B3
#define AD7146_REG_STAGE5_MIN_AVG                  0x01B4
#define AD7146_REG_STAGE5_LOW_THRESHOLD            0x01B5
#define AD7146_REG_STAGE5_MIN_TEMP                 0x01B6
#define AD7146_REG_STAGE6_CONV_DATA                0x01B8
#define AD7146_REG_STAGE6_FF_WORD0                 0x01B9
#define AD7146_REG_STAGE6_FF_WORD1                 0x01BA
#define AD7146_REG_STAGE6_FF_WORD2                 0x01BB
#define AD7146_REG_STAGE6_FF_WORD3                 0x01BC
#define AD7146_REG_STAGE6_FF_WORD4                 0x01BD
#define AD7146_REG_STAGE6_FF_WORD5                 0x01BE
#define AD7146_REG_STAGE6_FF_WORD6                 0x01BF
#define AD7146_REG_STAGE6_FF_WORD7                 0x01C0
#define AD7146_REG_STAGE6_SF_WORD0                 0x01C1
#define AD7146_REG_STAGE6_SF_WORD1                 0x01C2
#define AD7146_REG_STAGE6_SF_WORD2                 0x01C3
#define AD7146_REG_STAGE6_SF_WORD3                 0x01C4
#define AD7146_REG_STAGE6_SF_WORD4                 0x01C5
#define AD7146_REG_STAGE6_SF_WORD5                 0x01C6
#define AD7146_REG_STAGE6_SF_WORD6                 0x01C7
#define AD7146_REG_STAGE6_SF_WORD7                 0x01C8
#define AD7146_REG_STAGE6_SF_AMBIENT               0x01C9
#define AD7146_REG_STAGE6_FF_AVG                   0x01CA
#define AD7146_REG_STAGE6_PEAK_DETECT_WORD0        0x01CB
#define AD7146_REG_STAGE6_PEAK_DETECT_WORD1        0x01CC
#define AD7146_REG_STAGE6_MAX_WORD0                0x01CD
#define AD7146_REG_STAGE6_MAX_WORD1                0x01CE
#define AD7146_REG_STAGE6_MAX_WORD2                0x01DF
#define AD7146_REG_STAGE6_MAX_WORD3                0x01D0
#define AD7146_REG_STAGE6_MAX_AVG                  0x01D1
#define AD7146_REG_STAGE6_HIGH_THRESHOLD           0x01D2
#define AD7146_REG_STAGE6_MAX_TEMP                 0x01D3
#define AD7146_REG_STAGE6_MIN_WORD0                0x01D4
#define AD7146_REG_STAGE6_MIN_WORD1                0x01D5
#define AD7146_REG_STAGE6_MIN_WORD2                0x01D6
#define AD7146_REG_STAGE6_MIN_WORD3                0x01D7
#define AD7146_REG_STAGE6_MIN_AVG                  0x01D8
#define AD7146_REG_STAGE6_LOW_THRESHOLD            0x01D9
#define AD7146_REG_STAGE6_MIN_TEMP                 0x01DA
#define AD7146_REG_STAGE7_CONV_DATA                0x01DC
#define AD7146_REG_STAGE7_FF_WORD0                 0x01DD
#define AD7146_REG_STAGE7_FF_WORD1                 0x01DE
#define AD7146_REG_STAGE7_FF_WORD2                 0x01DF
#define AD7146_REG_STAGE7_FF_WORD3                 0x01E0
#define AD7146_REG_STAGE7_FF_WORD4                 0x01E1
#define AD7146_REG_STAGE7_FF_WORD5                 0x01E2
#define AD7146_REG_STAGE7_FF_WORD6                 0x01E3
#define AD7146_REG_STAGE7_FF_WORD7                 0x01E4
#define AD7146_REG_STAGE7_SF_WORD0                 0x01E5
#define AD7146_REG_STAGE7_SF_WORD1                 0x01E6
#define AD7146_REG_STAGE7_SF_WORD2                 0x01E7
#define AD7146_REG_STAGE7_SF_WORD3                 0x01E8
#define AD7146_REG_STAGE7_SF_WORD4                 0x01E9
#define AD7146_REG_STAGE7_SF_WORD5                 0x01EA
#define AD7146_REG_STAGE7_SF_WORD6                 0x01EB
#define AD7146_REG_STAGE7_SF_WORD7                 0x01EC
#define AD7146_REG_STAGE7_SF_AMBIENT               0x01ED
#define AD7146_REG_STAGE7_FF_AVG                   0x01EE
#define AD7146_REG_STAGE7_PEAK_DETECT_WORD0        0x01EF
#define AD7146_REG_STAGE7_PEAK_DETECT_WORD1        0x01F0
#define AD7146_REG_STAGE7_MAX_WORD0                0x01F1
#define AD7146_REG_STAGE7_MAX_WORD1                0x01F2
#define AD7146_REG_STAGE7_MAX_WORD2                0x01F3
#define AD7146_REG_STAGE7_MAX_WORD3                0x01F4
#define AD7146_REG_STAGE7_MAX_AVG                  0x01F5
#define AD7146_REG_STAGE7_HIGH_THRESHOLD           0x01F6
#define AD7146_REG_STAGE7_MAX_TEMP                 0x01F7
#define AD7146_REG_STAGE7_MIN_WORD0                0x01F8
#define AD7146_REG_STAGE7_MIN_WORD1                0x01F9
#define AD7146_REG_STAGE7_MIN_WORD2                0x01FA
#define AD7146_REG_STAGE7_MIN_WORD3                0x01FB
#define AD7146_REG_STAGE7_MIN_AVG                  0x01FC
#define AD7146_REG_STAGE7_LOW_THRESHOLD            0x01FD
#define AD7146_REG_STAGE7_MIN_TEMP                 0x01FE
#define AD7146_REG_STAGE8_CONV_DATA                0x0200
#define AD7146_REG_STAGE8_FF_WORD0                 0x0201
#define AD7146_REG_STAGE8_FF_WORD1                 0x0202
#define AD7146_REG_STAGE8_FF_WORD2                 0x0203
#define AD7146_REG_STAGE8_FF_WORD3                 0x0204
#define AD7146_REG_STAGE8_FF_WORD4                 0x0205
#define AD7146_REG_STAGE8_FF_WORD5                 0x0206
#define AD7146_REG_STAGE8_FF_WORD6                 0x0207
#define AD7146_REG_STAGE8_FF_WORD7                 0x0208
#define AD7146_REG_STAGE8_SF_WORD0                 0x0209
#define AD7146_REG_STAGE8_SF_WORD1                 0x020A
#define AD7146_REG_STAGE8_SF_WORD2                 0x020B
#define AD7146_REG_STAGE8_SF_WORD3                 0x020C
#define AD7146_REG_STAGE8_SF_WORD4                 0x020D
#define AD7146_REG_STAGE8_SF_WORD5                 0x020E
#define AD7146_REG_STAGE8_SF_WORD6                 0x020F
#define AD7146_REG_STAGE8_SF_WORD7                 0x0210
#define AD7146_REG_STAGE8_SF_AMBIENT               0x0211
#define AD7146_REG_STAGE8_FF_AVG                   0x0212
#define AD7146_REG_STAGE8_PEAK_DETECT_WORD0        0x0213
#define AD7146_REG_STAGE8_PEAK_DETECT_WORD1        0x0214
#define AD7146_REG_STAGE8_MAX_WORD0                0x0215
#define AD7146_REG_STAGE8_MAX_WORD1                0x0216
#define AD7146_REG_STAGE8_MAX_WORD2                0x0217
#define AD7146_REG_STAGE8_MAX_WORD3                0x0218
#define AD7146_REG_STAGE8_MAX_AVG                  0x0219
#define AD7146_REG_STAGE8_HIGH_THRESHOLD           0x021A
#define AD7146_REG_STAGE8_MAX_TEMP                 0x021B
#define AD7146_REG_STAGE8_MIN_WORD0                0x021C
#define AD7146_REG_STAGE8_MIN_WORD1                0x021D
#define AD7146_REG_STAGE8_MIN_WORD2                0x021E
#define AD7146_REG_STAGE8_MIN_WORD3                0x021F
#define AD7146_REG_STAGE8_MIN_AVG                  0x0220
#define AD7146_REG_STAGE8_LOW_THRESHOLD            0x0221
#define AD7146_REG_STAGE8_MIN_TEMP                 0x0222
#define AD7146_REG_STAGE9_CONV_DATA                0x0224
#define AD7146_REG_STAGE9_FF_WORD0                 0x0225
#define AD7146_REG_STAGE9_FF_WORD1                 0x0226
#define AD7146_REG_STAGE9_FF_WORD2                 0x0227
#define AD7146_REG_STAGE9_FF_WORD3                 0x0228
#define AD7146_REG_STAGE9_FF_WORD4                 0x0229
#define AD7146_REG_STAGE9_FF_WORD5                 0x022A
#define AD7146_REG_STAGE9_FF_WORD6                 0x022B
#define AD7146_REG_STAGE9_FF_WORD7                 0x022C
#define AD7146_REG_STAGE9_SF_WORD0                 0x022D
#define AD7146_REG_STAGE9_SF_WORD1                 0x022E
#define AD7146_REG_STAGE9_SF_WORD2                 0x022F
#define AD7146_REG_STAGE9_SF_WORD3                 0x0230
#define AD7146_REG_STAGE9_SF_WORD4                 0x0231
#define AD7146_REG_STAGE9_SF_WORD5                 0x0232
#define AD7146_REG_STAGE9_SF_WORD6                 0x0233
#define AD7146_REG_STAGE9_SF_WORD7                 0x0234
#define AD7146_REG_STAGE9_SF_AMBIENT               0x0235
#define AD7146_REG_STAGE9_FF_AVG                   0x0236
#define AD7146_REG_STAGE9_PEAK_DETECT_WORD0        0x0237
#define AD7146_REG_STAGE9_PEAK_DETECT_WORD1        0x0238
#define AD7146_REG_STAGE9_MAX_WORD0                0x0239
#define AD7146_REG_STAGE9_MAX_WORD1                0x023A
#define AD7146_REG_STAGE9_MAX_WORD2                0x023B
#define AD7146_REG_STAGE9_MAX_WORD3                0x023C
#define AD7146_REG_STAGE9_MAX_AVG                  0x023D
#define AD7146_REG_STAGE9_HIGH_THRESHOLD           0x023E
#define AD7146_REG_STAGE9_MAX_TEMP                 0x023F
#define AD7146_REG_STAGE9_MIN_WORD0                0x0240
#define AD7146_REG_STAGE9_MIN_WORD1                0x0241
#define AD7146_REG_STAGE9_MIN_WORD2                0x0242
#define AD7146_REG_STAGE9_MIN_WORD3                0x0243
#define AD7146_REG_STAGE9_MIN_AVG                  0x0244
#define AD7146_REG_STAGE9_LOW_THRESHOLD            0x0245
#define AD7146_REG_STAGE9_MIN_TEMP                 0x0246
#define AD7146_REG_STAGE10_CONV_DATA               0x0248
#define AD7146_REG_STAGE10_FF_WORD0                0x0249
#define AD7146_REG_STAGE10_FF_WORD1                0x024A
#define AD7146_REG_STAGE10_FF_WORD2                0x024B
#define AD7146_REG_STAGE10_FF_WORD3                0x024C
#define AD7146_REG_STAGE10_FF_WORD4                0x024D
#define AD7146_REG_STAGE10_FF_WORD5                0x024E
#define AD7146_REG_STAGE10_FF_WORD6                0x024F
#define AD7146_REG_STAGE10_FF_WORD7                0x0250
#define AD7146_REG_STAGE10_SF_WORD0                0x0251
#define AD7146_REG_STAGE10_SF_WORD1                0x0252
#define AD7146_REG_STAGE10_SF_WORD2                0x0253
#define AD7146_REG_STAGE10_SF_WORD3                0x0254
#define AD7146_REG_STAGE10_SF_WORD4                0x0255
#define AD7146_REG_STAGE10_SF_WORD5                0x0256
#define AD7146_REG_STAGE10_SF_WORD6                0x0257
#define AD7146_REG_STAGE10_SF_WORD7                0x0258
#define AD7146_REG_STAGE10_SF_AMBIENT              0x0259
#define AD7146_REG_STAGE10_FF_AVG                  0x025A
#define AD7146_REG_STAGE10_PEAK_DETECT_WORD0       0x025B
#define AD7146_REG_STAGE10_PEAK_DETECT_WORD1       0x025C
#define AD7146_REG_STAGE10_MAX_WORD0               0x025D
#define AD7146_REG_STAGE10_MAX_WORD1               0x025E
#define AD7146_REG_STAGE10_MAX_WORD2               0x025F
#define AD7146_REG_STAGE10_MAX_WORD3               0x0260
#define AD7146_REG_STAGE10_MAX_AVG                 0x0261
#define AD7146_REG_STAGE10_HIGH_THRESHOLD          0x0262
#define AD7146_REG_STAGE10_MAX_TEMP                0x0263
#define AD7146_REG_STAGE10_MIN_WORD0               0x0264
#define AD7146_REG_STAGE10_MIN_WORD1               0x0265
#define AD7146_REG_STAGE10_MIN_WORD2               0x0266
#define AD7146_REG_STAGE10_MIN_WORD3               0x0267
#define AD7146_REG_STAGE10_MIN_AVG                 0x0268
#define AD7146_REG_STAGE10_LOW_THRESHOLD           0x0269
#define AD7146_REG_STAGE10_MIN_TEMP                0x026A
#define AD7146_REG_STAGE11_CONV_DATA               0x026C
#define AD7146_REG_STAGE11_FF_WORD0                0x026D
#define AD7146_REG_STAGE11_FF_WORD1                0x026E
#define AD7146_REG_STAGE11_FF_WORD2                0x026F
#define AD7146_REG_STAGE11_FF_WORD3                0x0270
#define AD7146_REG_STAGE11_FF_WORD4                0x0271
#define AD7146_REG_STAGE11_FF_WORD5                0x0272
#define AD7146_REG_STAGE11_FF_WORD6                0x0273
#define AD7146_REG_STAGE11_FF_WORD7                0x0274
#define AD7146_REG_STAGE11_SF_WORD0                0x0275
#define AD7146_REG_STAGE11_SF_WORD1                0x0276
#define AD7146_REG_STAGE11_SF_WORD2                0x0277
#define AD7146_REG_STAGE11_SF_WORD3                0x0278
#define AD7146_REG_STAGE11_SF_WORD4                0x0279
#define AD7146_REG_STAGE11_SF_WORD5                0x027A
#define AD7146_REG_STAGE11_SF_WORD6                0x027B
#define AD7146_REG_STAGE11_SF_WORD7                0x027C
#define AD7146_REG_STAGE11_SF_AMBIENT              0x027D
#define AD7146_REG_STAGE11_FF_AVG                  0x027E
#define AD7146_REG_STAGE11_PEAK_DETECT_WORD0       0x027F
#define AD7146_REG_STAGE11_PEAK_DETECT_WORD1       0x0280
#define AD7146_REG_STAGE11_MAX_WORD0               0x0281
#define AD7146_REG_STAGE11_MAX_WORD1               0x0282
#define AD7146_REG_STAGE11_MAX_WORD2               0x0283
#define AD7146_REG_STAGE11_MAX_WORD3               0x0284
#define AD7146_REG_STAGE11_MAX_AVG                 0x0285
#define AD7146_REG_STAGE11_HIGH_THRESHOLD          0x0286
#define AD7146_REG_STAGE11_MAX_TEMP                0x0287
#define AD7146_REG_STAGE11_MIN_WORD0               0x0288
#define AD7146_REG_STAGE11_MIN_WORD1               0x0289
#define AD7146_REG_STAGE11_MIN_WORD2               0x028A
#define AD7146_REG_STAGE11_MIN_WORD3               0x028B
#define AD7146_REG_STAGE11_MIN_AVG                 0x028C
#define AD7146_REG_STAGE11_LOW_THRESHOLD           0x028D
#define AD7146_REG_STAGE11_MIN_TEMP                0x028E

/* ------------------------------------------------------------------------- */
/* ENUM                                                                      */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* VARIABLES                                                                 */
/* ------------------------------------------------------------------------- */
const static unsigned short prox_cdc_reg_stage_afe_offset[NUM_SH_PROXIMITY_CDC_STAGE] =
{
    AD7146_REG_STAGE0_AFE_OFFSET,
    AD7146_REG_STAGE1_AFE_OFFSET,
    AD7146_REG_STAGE2_AFE_OFFSET,
    AD7146_REG_STAGE3_AFE_OFFSET,
    AD7146_REG_STAGE4_AFE_OFFSET,
    AD7146_REG_STAGE5_AFE_OFFSET,
    AD7146_REG_STAGE6_AFE_OFFSET,
    AD7146_REG_STAGE7_AFE_OFFSET,
    AD7146_REG_STAGE8_AFE_OFFSET,
    AD7146_REG_STAGE9_AFE_OFFSET,
    AD7146_REG_STAGE10_AFE_OFFSET,
    AD7146_REG_STAGE11_AFE_OFFSET,
};

const static unsigned short prox_cdc_reg_cdc_result[NUM_SH_PROXIMITY_CDC_STAGE] =
{
    AD7146_REG_CDC_RESULT_S0,
    AD7146_REG_CDC_RESULT_S1,
    AD7146_REG_CDC_RESULT_S2,
    AD7146_REG_CDC_RESULT_S3,
    AD7146_REG_CDC_RESULT_S4,
    AD7146_REG_CDC_RESULT_S5,
    AD7146_REG_CDC_RESULT_S6,
    AD7146_REG_CDC_RESULT_S7,
    AD7146_REG_CDC_RESULT_S8,
    AD7146_REG_CDC_RESULT_S9,
    AD7146_REG_CDC_RESULT_S10,
    AD7146_REG_CDC_RESULT_S11,
};

const static unsigned short prox_cdc_reg_stage_connection6[NUM_SH_PROXIMITY_CDC_STAGE] =
{
    AD7146_REG_STAGE0_CONNECTION_6_0,
    AD7146_REG_STAGE1_CONNECTION_6_0,
    AD7146_REG_STAGE2_CONNECTION_6_0,
    AD7146_REG_STAGE3_CONNECTION_6_0,
    AD7146_REG_STAGE4_CONNECTION_6_0,
    AD7146_REG_STAGE5_CONNECTION_6_0,
    AD7146_REG_STAGE6_CONNECTION_6_0,
    AD7146_REG_STAGE7_CONNECTION_6_0,
    AD7146_REG_STAGE8_CONNECTION_6_0,
    AD7146_REG_STAGE9_CONNECTION_6_0,
    AD7146_REG_STAGE10_CONNECTION_6_0,
    AD7146_REG_STAGE11_CONNECTION_6_0,
};

const static unsigned short prox_cdc_reg_stage_sf_ambient[NUM_SH_PROXIMITY_CDC_STAGE] =
{
    AD7146_REG_STAGE0_SF_AMBIENT,
    AD7146_REG_STAGE1_SF_AMBIENT,
    AD7146_REG_STAGE2_SF_AMBIENT,
    AD7146_REG_STAGE3_SF_AMBIENT,
    AD7146_REG_STAGE4_SF_AMBIENT,
    AD7146_REG_STAGE5_SF_AMBIENT,
    AD7146_REG_STAGE6_SF_AMBIENT,
    AD7146_REG_STAGE7_SF_AMBIENT,
    AD7146_REG_STAGE8_SF_AMBIENT,
    AD7146_REG_STAGE9_SF_AMBIENT,
    AD7146_REG_STAGE10_SF_AMBIENT,
    AD7146_REG_STAGE11_SF_AMBIENT,
};

const static unsigned short prox_cdc_reg_stage_high_threshold[NUM_SH_PROXIMITY_CDC_STAGE] =
{
    AD7146_REG_STAGE0_HIGH_THRESHOLD,
    AD7146_REG_STAGE1_HIGH_THRESHOLD,
    AD7146_REG_STAGE2_HIGH_THRESHOLD,
    AD7146_REG_STAGE3_HIGH_THRESHOLD,
    AD7146_REG_STAGE4_HIGH_THRESHOLD,
    AD7146_REG_STAGE5_HIGH_THRESHOLD,
    AD7146_REG_STAGE6_HIGH_THRESHOLD,
    AD7146_REG_STAGE7_HIGH_THRESHOLD,
    AD7146_REG_STAGE8_HIGH_THRESHOLD,
    AD7146_REG_STAGE9_HIGH_THRESHOLD,
    AD7146_REG_STAGE10_HIGH_THRESHOLD,
    AD7146_REG_STAGE11_HIGH_THRESHOLD,
};

const static unsigned short prox_cdc_reg_stage_low_threshold[NUM_SH_PROXIMITY_CDC_STAGE] =
{
    AD7146_REG_STAGE0_LOW_THRESHOLD,
    AD7146_REG_STAGE1_LOW_THRESHOLD,
    AD7146_REG_STAGE2_LOW_THRESHOLD,
    AD7146_REG_STAGE3_LOW_THRESHOLD,
    AD7146_REG_STAGE4_LOW_THRESHOLD,
    AD7146_REG_STAGE5_LOW_THRESHOLD,
    AD7146_REG_STAGE6_LOW_THRESHOLD,
    AD7146_REG_STAGE7_LOW_THRESHOLD,
    AD7146_REG_STAGE8_LOW_THRESHOLD,
    AD7146_REG_STAGE9_LOW_THRESHOLD,
    AD7146_REG_STAGE10_LOW_THRESHOLD,
    AD7146_REG_STAGE11_LOW_THRESHOLD,
};

const static unsigned short prox_cdc_reg_stage_sensitivity[NUM_SH_PROXIMITY_CDC_STAGE] =
{
    AD7146_REG_STAGE0_SENSITIVITY,
    AD7146_REG_STAGE1_SENSITIVITY,
    AD7146_REG_STAGE2_SENSITIVITY,
    AD7146_REG_STAGE3_SENSITIVITY,
    AD7146_REG_STAGE4_SENSITIVITY,
    AD7146_REG_STAGE5_SENSITIVITY,
    AD7146_REG_STAGE6_SENSITIVITY,
    AD7146_REG_STAGE7_SENSITIVITY,
    AD7146_REG_STAGE8_SENSITIVITY,
    AD7146_REG_STAGE9_SENSITIVITY,
    AD7146_REG_STAGE10_SENSITIVITY,
    AD7146_REG_STAGE11_SENSITIVITY,
};

#endif      /* PROXIMITY_CDC_REGDATA_H */
/* ------------------------------------------------------------------------- */
/* END OF FILE                                                               */
/* ------------------------------------------------------------------------- */
