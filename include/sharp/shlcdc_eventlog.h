/* include/sharp/shlcdc_eventlog.h  (Display Driver)
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

#ifndef SHLCDC_EVENTLOG_H
#define SHLCDC_EVENTLOG_H

/* ------------------------------------------------------------------------- */
/* MACROS                                                                    */
/* ------------------------------------------------------------------------- */

#define SHLCDC_EVENTLOG_FB          0x00000000
#define SHLCDC_EVENTLOG_SHDISP      0x30000000
#define SHLCDC_EVENTLOG_USEREVENT   0x60000000
#define SHLCDC_EVENTLOG_ANY         0x90000000

#define SHLCDC_FUNCTION_LCD         0x00100000
#define SHLCDC_FUNCTION_BKL         0x00200000
#define SHLCDC_FUNCTION_3LED        0x00300000
#define SHLCDC_FUNCTION_ILMI        0x00400000
#define SHLCDC_FUNCTION_MDP         0x00500000
#define SHLCDC_FUNCTION_SENSOR      0x00600000
#define SHLCDC_FUNCTION_OTHER       0x0FF00000

#define DEVICE_NAME_EVENTLOG        "shlcdc_eventlog"
#define DEVICE_PATH_EVENTLOG        "/dev/shlcdc_eventlog"

#define SHLCDC_FILE_MSM_FB_KERL     0x00000000
#define SHLCDC_FILE_MDP_KERL        0x00001000
#define SHLCDC_FILE_FBMEM_KERL      0x00002000

#define SHLCDC_FILE_DISP_KERL       0x00000000
#define SHLCDC_FILE_DISP_SYSTEM     0x00001000
#define SHLCDC_FILE_DISP_PWM        0x00002000
#define SHLCDC_FILE_DISP_BL69Y6     0x00020000
#define SHLCDC_FILE_DISP_TAKT       0x00040000
#define SHLCDC_FILE_DISP_NICOLE     0x00041000
#define SHLCDC_FILE_DISP_PHARAOH    0x00042000
#define SHLCDC_FILE_DISP_KITE       0x00043000
#define SHLCDC_FILE_DISP_BD2802GU   0x00070000

#define SHLCDC_FILE_USER_SQE        0x00000000

enum enEventId_FB {
	EEVENTID_FB_IOCTL = 0,
	EEVENTID_FB_EARLY_SUSPEND,
	EEVENTID_FB_EARLY_RESUME,
	EEVENTID_FB_EARLY_SUSPEND_RET,
	EEVENTID_FB_EARLY_RESUME_RET,
	EEVENTID_FB_DSI_CLK_ENABLE,
	EEVENTID_FB_DSI_CLK_DISABLE,
	EEVENTID_FB_MDP_CLK_ENABLE,
	EEVENTID_FB_MDP_CLK_DISABLE,
	EEVENTID_FB_RECOVERY,
	EEVENTID_FB_RECOVERY_ERR,
	EEVENTID_FB_RECOVERY_OVER,
	EEVENTID_FB_SUSPEND_ERR,
	EEVENTID_FB_CABC_INIT,
	EEVENTID_FB_CABC_INIT_RET,
	EEVENTID_FB_CABC_INDOOR_ON,
	EEVENTID_FB_CABC_INDOOR_ON_RET,
	EEVENTID_FB_CABC_OUTDOOR_ON,
	EEVENTID_FB_CABC_OUTDOOR_ON_RET,
	EEVENTID_FB_CABC_OFF,
	EEVENTID_FB_CABC_OFF_RET,
	EEVENTID_FB_CABC_OUTDOOR_MODE,
	EEVENTID_FB_CABC_OUTDOOR_MODE_RET,
	EEVENTID_FB_CABC_SET_LPF,
	EEVENTID_FB_CABC_SET_LPF_RET,
	EEVENTID_FB_IOCTL_RET = 0x0e00,
	EEVENTID_FB_IOCTL_OUT = 0x0f00,
};

/* ------------------------------------------------------------------------- */
/* TYPES                                                                     */
/* ------------------------------------------------------------------------- */

struct stEventlog_st {
    unsigned long    kind;
    unsigned long    eventID;
    unsigned long    param;
};

enum enEventlog_Kind {
    EEVENTLOG_USER_EVENT = 0,
    EEVENTLOG_USER_BUSY  = 1,
    EEVENTLOG_USER_ERR   = 2,
};

/* ------------------------------------------------------------------------- */
/* IOCTL                                                                     */
/* ------------------------------------------------------------------------- */

#define SHLCDC_EVENTLOG_MAGIC 'l'
#define SHLCDC_EVENTLOG_USER    _IOW(SHLCDC_EVENTLOG_MAGIC, 0, struct stEventlog_st *)


#ifdef __KERNEL__

/* ------------------------------------------------------------------------- */
/* PROTOTYPES                                                                */
/* ------------------------------------------------------------------------- */

void shlcdc_eventlog_rec(unsigned long eventID, unsigned long param);
void shlcdc_busylog_rec(unsigned long eventID, unsigned long param);
void shlcdc_errlog_rec(unsigned long eventID, unsigned long param);

#define SHLCDC_EVENTLOG_REC(x,y,z,i,p)	shlcdc_eventlog_rec(SHLCDC_EVENTLOG_##x+SHLCDC_FUNCTION_##y+SHLCDC_FILE_##z+(i),(unsigned long)p)
#define SHLCDC_BUSYLOG_REC(x,y,z,i,p)	shlcdc_busylog_rec(SHLCDC_EVENTLOG_##x+SHLCDC_FUNCTION_##y+SHLCDC_FILE_##z+(i),(unsigned long)p)
#define SHLCDC_ERRLOG_REC(x,y,z,i,p)	shlcdc_errlog_rec(SHLCDC_EVENTLOG_##x+SHLCDC_FUNCTION_##y+SHLCDC_FILE_##z+(i),(unsigned long)p)

#ifndef CONFIG_SHLCDC_BOARD
void shlcdc_eventlog_rec(unsigned long eventID, unsigned long param)
{
    return;
}
void shlcdc_busylog_rec(unsigned long eventID, unsigned long param)
{
    return;
}
void shlcdc_errlog_rec(unsigned long eventID, unsigned long param)
{
    return;
}
#endif /* CONFIG_SHLCDC_BOARD */
#endif /* __KERNEL__ */

#endif /* SHLCDC_LCDC_EVENTLOG_H */
