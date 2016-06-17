/* driver/sharp/proximity_CDC/proximity_cdc.c  (proximityCDCSensor Driver)
 *
 * Copyright (C) 2014 SHARP CORPORATION All rights reserved.
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
/* SHARP PROXIMITY CDC SENSOR DRIVER FOR KERNEL MODE                         */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* INCLUDE FILES                                                             */
/* ------------------------------------------------------------------------- */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/gpio.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/device.h>
#include <sharp/proximity_cdc.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/wakelock.h>
#include <sharp/sh_smem.h>
#include <linux/proc_fs.h>
#include <linux/module.h>
#include <linux/pm_qos.h>
#include <mach/cpuidle.h>
#include <proximity_cdc_regdata.h>
#include <sharp/sh_boot_manager.h>
#include <sharp/shterm_k.h>

/* ------------------------------------------------------------------------- */
/* MACROS                                                                   */
/* ------------------------------------------------------------------------- */
/* #define PROX_CDC_ENABLE_SENS_WRITE */

#define HW_ES0      0x00
#define HW_ES05     0x04
#define HW_ES1      0x02
#define HW_PP1      0x06
#define HW_PP2      0x01
#define HW_PMP      0x07

#define PROX_CDC_INT_NUM    54
#define PROX_CDC_DEV_NO_CHECK   0
#define PROX_CDC_DEV_NO_EXIST   1
#define PROX_CDC_DEV_EXIST      2

#define PROX_CDC_IRQ_DISABLED   0
#define PROX_CDC_IRQ_ENABLED    1

#define PROX_CDC_DEFAULT_STAGE_NUM  0

#define PROX_CDC_SENSITIVITY_ADJUST_NEAR (0x06)
#define PROX_CDC_SENSITIVITY_ADJUST_FAR  (0x0F)

#define PROX_CDC_PM_QOS_LATENCY_VALUE 1

#define PROX_CDC_ROOP_CNT 10

/* adb debug_log */
static int    prox_cdc_func_log     = 0; /* log : Init = OFF */
static int    prox_cdc_func_fin_log = 0; /* log : Init = OFF */
static int    prox_cdc_debug_log    = 0; /* log : Init = OFF */
static int    prox_cdc_sensor_log   = 0; /* log : Init = OFF */
static int    prox_cdc_error_log    = 1; /* log : Init = ON  */
#if defined (CONFIG_ANDROID_ENGINEERING)
static int    prox_cdc_dump_log     = 0; /* log : Init = OFF */
#endif  /* CONFIG_ANDROID_ENGINEERING */

#if defined (CONFIG_ANDROID_ENGINEERING)
    module_param(prox_cdc_func_log, int, 0600);
    module_param(prox_cdc_func_fin_log, int, 0600);
    module_param(prox_cdc_debug_log, int, 0600);
    module_param(prox_cdc_sensor_log, int, 0600);
    module_param(prox_cdc_error_log, int, 0600);
    module_param(prox_cdc_dump_log, int, 0600);
#endif  /* CONFIG_ANDROID_ENGINEERING */

#define FUNC_LOG() \
    if(prox_cdc_func_log == 1){ \
       printk(KERN_DEBUG "[PROX_CDC][FUNC] %s is called\n", __func__); \
    }

#define FUNC_FIN_LOG() \
    if(prox_cdc_func_fin_log == 1){ \
       printk(KERN_DEBUG "[PROX_CDC][FUNC] %s is finished\n", __func__); \
    }

#define DEBUG_LOG(format, ...) \
    if(prox_cdc_debug_log == 1){ \
       printk(KERN_DEBUG "[PROX_CDC][DEBUG][%s]" format "\n", __func__, ## __VA_ARGS__); \
    }

#define DEBUG_SENSOR_LOG(format, ...) \
    if(prox_cdc_sensor_log == 1){ \
       printk(KERN_DEBUG "[PROX_CDC][SENSOR][%s]" format "\n", __func__, ## __VA_ARGS__); \
    }

#define DEBUG_ERROR_LOG(format, ...) \
    if(prox_cdc_error_log == 1){ \
       printk(KERN_ERR "[PROX_CDC][ERR][%s]" format "\n", __func__, ## __VA_ARGS__); \
    }

#if defined (CONFIG_ANDROID_ENGINEERING)
#define DEBUG_DUMP_LOG(format, ...) \
    if(prox_cdc_dump_log == 1){ \
       printk(KERN_DEBUG "[PROX_CDC][DUMP][%s]" format "\n", __func__, ## __VA_ARGS__); \
    }
#endif  /* CONFIG_ANDROID_ENGINEERING */

/* ------------------------------------------------------------------------- */
/* ENUM                                                                      */
/* ------------------------------------------------------------------------- */

enum {
    PROX_CDC_BOOT_DIAG = 0,
    PROX_CDC_BOOT_NORMAL,
    NUM_PROX_CDC_BOOT
};

enum {
    PROX_CDC_HW_DB = 0,
    PROX_CDC_HW_ES0,
    PROX_CDC_HW_ES1,
    PROX_CDC_HW_PP1,
    PROX_CDC_HW_PP2,
    PROX_CDC_HW_PMP,
    NUM_PROX_CDC_HW
};

enum {
    PROX_CDC_DB = 0,
    PROX_CDC_HANDSET,
    NUM_PROX_CDC_PRODUCT
};

/* ------------------------------------------------------------------------- */
/* PROTOTYPES                                                                */
/* ------------------------------------------------------------------------- */

static unsigned short PROX_CDC_get_stage_num(void);
static int  PROX_CDC_open(struct inode *inode, struct file *filp);
static int  PROX_CDC_release(struct inode *inode, struct file *filp);
static int  PROX_CDC_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos);
static long PROX_CDC_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int  PROX_CDC_Remove(void);
static int  PROX_CDC_Probe(void);
#ifdef CONFIG_PM
static int PROX_CDC_suspend(struct device *dev);
static int PROX_CDC_resume(struct device *dev);
#endif /* CONFIG_PM */
static irqreturn_t PROX_CDC_INT_isr(int irq, void *dev_id);
static void PROX_CDC_workqueue_handler(struct work_struct* work);
static int  PROX_CDC_IOCTL_Enable(void);
static int  PROX_CDC_IOCTL_Disable(void);
static int  PROX_CDC_IOCTL_ambient_read(unsigned short stage, unsigned short *data);
static int  PROX_CDC_IOCTL_ambient_reset(void);
static int  PROX_CDC_IOCTL_pos_threshold_read(unsigned short stage, unsigned short *data);
static int  PROX_CDC_IOCTL_neg_threshold_read(unsigned short stage, unsigned short *data);
static int  PROX_CDC_IOCTL_pos_threshold_sens_write(unsigned short stage, unsigned short data);
static int  PROX_CDC_IOCTL_neg_threshold_sens_write(unsigned short stage, unsigned short data);
static int  PROX_CDC_IOCTL_result_read(unsigned short stage, unsigned short *data);
static int  PROX_CDC_IOCTL_read_data(unsigned short *data);
static int  PROX_CDC_IOCTL_stage_read_data(unsigned short stage, unsigned short *data);
static int  PROX_CDC_IOCTL_offset_adj_write(unsigned short stage, unsigned short *data);
static int  PROX_CDC_IOCTL_get_status(struct proximity_cdc_status *status);
static int  PROX_CDC_IOCTL_powermode(unsigned char mode);
static int  PROX_CDC_REG_Recalibration(void);
static int  PROX_CDC_REG_Enable(void);
static int  PROX_CDC_REG_Disable(void);
static int  PROX_CDC_REG_Initialize(void);
static void  PROX_CDC_get_smem_data(void);
static int  PROX_CDC_offset_adj_write_pos(unsigned short stage);
static int  PROX_CDC_offset_adj_write_pos_swap(unsigned short stage);
static int  PROX_CDC_offset_adj_write_neg(unsigned short stage);
static int  PROX_CDC_offset_adj_write_neg_swap(unsigned short stage);
static int  PROX_CDC_i2c_init(struct i2c_client *client, const struct i2c_device_id * devid);
static int  PROX_CDC_i2c_exit(struct i2c_client *client);
static int  PROX_CDC_i2c_write(unsigned short addr, unsigned short data);
static int  PROX_CDC_i2c_read(unsigned short addr, unsigned short *data);
static int  PROX_CDC_i2c_seq_write(unsigned short addr, unsigned short *data, int len);
#if defined (CONFIG_ANDROID_ENGINEERING)
static int  PROX_CDC_i2c_seq_read(unsigned short addr, unsigned short *data, int len);
#endif /* CONFIG_ANDROID_ENGINEERING */
static int  PROX_CDC_i2c_set_bit_write(unsigned short addr, unsigned short data);
static int  PROX_CDC_i2c_clr_bit_write(unsigned short addr, unsigned short data);
#if defined (PROX_CDC_ENABLE_SENS_WRITE)
static int  PROX_CDC_i2c_msk_bit_reg(unsigned short addr, unsigned short data, unsigned short msk);
#endif /* PROX_CDC_ENABLE_SENS_WRITE */
#if defined (CONFIG_ANDROID_ENGINEERING)
static int  PROX_CDC_proc_write(struct file *file, const char *buffer, unsigned long count, void *data);
#endif /* CONFIG_ANDROID_ENGINEERING */
static int  PROX_CDC_check_exist(void);
static int  __init PROX_CDC_Init(void);
static void __exit PROX_CDC_Exit(void);


/* ------------------------------------------------------------------------- */
/* VARIABLES                                                                 */
/* ------------------------------------------------------------------------- */

typedef struct drv_data_tag     drv_data;
typedef struct work_struct      WorkStruct;
typedef struct input_dev        InputDev;
typedef struct device           Device;
typedef struct prox_cdc_data_tag
{
    struct i2c_client *this_client;
} prox_cdc_i2c_data_t;

struct drv_data_tag
{
    int         irq_gpio;
    InputDev    *input_dev;
    WorkStruct  IrqWork;
    int         irq;
};

struct proximity_cdc_procfs {
    int id;
    int par[4];
};

struct workqueue_struct *prox_cdc_wq;
static struct work_struct prox_cdc_wk;

static drv_data     *poProximityCDCRec;
static atomic_t     open_flag   = ATOMIC_INIT(0);
static atomic_t     sensor_data = ATOMIC_INIT(1); /* Init = Far */
static atomic_t     enable_mode = ATOMIC_INIT(0); /* 0=Disable,1=Enable */

static int prox_cdc_irq_status = PROX_CDC_IRQ_DISABLED;

static unsigned char stage_data[NUM_SH_PROXIMITY_CDC_STAGE];
static unsigned char bk_stage_data[NUM_SH_PROXIMITY_CDC_STAGE];

static struct wake_lock prox_cdc_timeout_wake_lock;
static struct wake_lock prox_cdc_wake_lock;

static spinlock_t prox_cdc_spinlock;
static struct mutex prox_cdc_lock;

static unsigned int prox_cdc_stage_read_flg = 0;
static unsigned short prox_cdc_stage_num = PROX_CDC_DEFAULT_STAGE_NUM;
static unsigned short prox_cdc_hw_revision = HW_PMP;
#if defined (CONFIG_ANDROID_ENGINEERING)
    module_param(prox_cdc_stage_num, short, 0600);
    module_param(prox_cdc_stage_read_flg, int, 0600);
#endif  /* CONFIG_ANDROID_ENGINEERING */

static int prox_cdc_smem_flg = 1;
#if defined (CONFIG_ANDROID_ENGINEERING)
    module_param(prox_cdc_smem_flg, int, 0600);
#endif  /* CONFIG_ANDROID_ENGINEERING */

static unsigned char prox_cdc_dev_exist = PROX_CDC_DEV_NO_CHECK;

static struct pm_qos_request prox_cdc_qos_cpu_dma_latency;

static struct file_operations PROX_ctl_fops = {
    .owner          = THIS_MODULE,
    .open           = PROX_CDC_open,
    .release        = PROX_CDC_release,
    .unlocked_ioctl = PROX_CDC_ioctl,
    .read           = PROX_CDC_read,
};

static struct miscdevice PROX_cdc_device = {
    .minor   = MISC_DYNAMIC_MINOR,
    .name    = "proximity_cdc_dev",
    .fops    = &PROX_ctl_fops,
};

static const struct i2c_device_id prox_cdc_id[] = {
    { "prox_cdc_i2c", 0 },
    { }
};

#ifdef CONFIG_OF
static const struct of_device_id prox_cdc_i2c_table[] = {
	{ .compatible = "sharp,cdc_proximity",},
	{}
};
#else
#define prox_cdc_i2c_table NULL;
#endif /* CONFIG_OF */

#ifdef CONFIG_PM
static atomic_t prox_cdc_suspend_status = ATOMIC_INIT(0); /* 0=resume,1=suspend */

static const struct dev_pm_ops prox_cdc_pmops = {
	.suspend       = PROX_CDC_suspend,
	.resume        = PROX_CDC_resume,
};
#define PROX_CDC_PMOPS (&prox_cdc_pmops)
#else
#define PROX_CDC_PMOPS NULL
#endif /* CONFIG_PM */

static struct i2c_driver prox_cdc_driver = {
    .driver = {
        .owner  = THIS_MODULE,
        .name   = "prox_cdc_i2c",
        .pm     = PROX_CDC_PMOPS,
#ifdef CONFIG_OF
        .of_match_table = prox_cdc_i2c_table,
#endif /* CONFIG_OF */
    },
    .class      = I2C_CLASS_HWMON,
    .probe      = PROX_CDC_i2c_init,
    .id_table   = prox_cdc_id,
    .remove     = PROX_CDC_i2c_exit,
};

static prox_cdc_i2c_data_t *prox_cdc_i2c_p = NULL;

#define PROX_CDC_STAGE0_CONFIGURATION  0x37BF
static unsigned short prox_cdc_bank2_stage_param[NUM_SH_PROXIMITY_CDC_STAGE][8] = 
{
    { 0x35BF,    0x1FFF,    0x0500,    0x2F2F,    0x00B4,    0x00B4,    0x0026,    0x0026 },
    { 0x3FFF,    0x1DEF,    0x0002,    0x2F2F,    0x0028,    0x0028,    0x0026,    0x0026 },
    { 0x2FFF,    0x1FF7,    0x0000,    0x2F2F,    0x0020,    0x0020,    0x0026,    0x0026 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 },
    { 0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000,    0x0000 }
};

static const unsigned short prox_cdc_enable[NUM_SH_PROXIMITY_CDC_STAGE] =
{ 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF };

static const unsigned short prox_cdc_stage[NUM_PROX_CDC_BOOT][NUM_PROX_CDC_HW] =
{
    {2, 0, 0, 0, 0, 0}, /* Diag */
    {2, 0, 0, 0, 0, 0}  /* Normal */
};

#ifdef CONFIG_ANDROID_ENGINEERING
static struct hrtimer prox_cdc_threshold_timer;
static struct work_struct prox_cdc_th_work_data;

static unsigned long prox_cdc_th_interval_ms = 0;
static unsigned long th_msec = 0;
static unsigned long th_msec_tmp = 0;
static unsigned char hrtimer_flg = 0;
#endif /* CONFIG_ANDROID_ENGINEERING */

/* ------------------------------------------------------------------------- */
/* FUNCTIONS                                                                 */
/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */
/* PROX_CDC_qos_start                                                        */
/* ------------------------------------------------------------------------- */
void PROX_CDC_qos_start(void)
{
    pm_qos_update_request(&prox_cdc_qos_cpu_dma_latency, PROX_CDC_PM_QOS_LATENCY_VALUE);
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_qos_end                                                          */
/* ------------------------------------------------------------------------- */
void PROX_CDC_qos_end(void)
{
    pm_qos_update_request(&prox_cdc_qos_cpu_dma_latency, PM_QOS_DEFAULT_VALUE);
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_enable_irq                                                       */
/* ------------------------------------------------------------------------- */
static void PROX_CDC_enable_irq(void)
{
    unsigned long flags;
    
    spin_lock_irqsave(&prox_cdc_spinlock, flags);
    
    if (prox_cdc_irq_status == PROX_CDC_IRQ_DISABLED) {
        enable_irq_wake(gpio_to_irq(PROX_CDC_INT_NUM));
        enable_irq(gpio_to_irq(PROX_CDC_INT_NUM));
        prox_cdc_irq_status = PROX_CDC_IRQ_ENABLED;
    }
    
    spin_unlock_irqrestore(&prox_cdc_spinlock, flags);
    
    return;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_disable_irq                                                      */
/* ------------------------------------------------------------------------- */
static void PROX_CDC_disable_irq(void)
{
    unsigned long flags;
    
    spin_lock_irqsave(&prox_cdc_spinlock, flags);
    
    if (prox_cdc_irq_status == PROX_CDC_IRQ_ENABLED) {
        disable_irq_nosync(gpio_to_irq(PROX_CDC_INT_NUM));
        disable_irq_wake(gpio_to_irq(PROX_CDC_INT_NUM));
        prox_cdc_irq_status = PROX_CDC_IRQ_DISABLED;
    }
    
    spin_unlock_irqrestore(&prox_cdc_spinlock, flags);
    
    return;
}

#ifdef CONFIG_ANDROID_ENGINEERING
/* ------------------------------------------------------------------------- */
/* PROX_CDC_th_dump_func                                                       */
/* ------------------------------------------------------------------------- */
static void PROX_CDC_th_dump_func(struct work_struct *work)
{
    int i;
    unsigned short stage;
    unsigned long sec, nsec;
    unsigned short cdc_readdata[NUM_SH_PROXIMITY_CDC_STAGE];
    unsigned short h_th_readdata[NUM_SH_PROXIMITY_CDC_STAGE];
    unsigned short l_th_readdata[NUM_SH_PROXIMITY_CDC_STAGE];
    
    mutex_lock(&prox_cdc_lock);
    
    FUNC_LOG();
    
    stage = PROX_CDC_get_stage_num();
    
    if( atomic_read(&enable_mode) ) {
        if (prox_cdc_th_interval_ms) {
            if (th_msec >= 1000) {
                sec  = th_msec / 1000;
                nsec = (th_msec % 1000) * 1000 * 1000;
            } else {
                sec  = 0;
                nsec = th_msec * 1000 * 1000;
            }
            hrtimer_start(&prox_cdc_threshold_timer, ktime_set(sec, nsec), HRTIMER_MODE_REL);
        }
        memset(cdc_readdata, 0, sizeof(cdc_readdata));
        memset(h_th_readdata, 0, sizeof(h_th_readdata));
        memset(l_th_readdata, 0, sizeof(l_th_readdata));
        
        for( i = 0; i <= stage; i++ ) {
            PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[i], &cdc_readdata[i]);
            PROX_CDC_i2c_read(prox_cdc_reg_stage_high_threshold[i], &h_th_readdata[i]);
            PROX_CDC_i2c_read(prox_cdc_reg_stage_low_threshold[i], &l_th_readdata[i]);
            DEBUG_DUMP_LOG("stage[%d] cdc: %d, h_th: %d, l_th: %d", i, cdc_readdata[i], h_th_readdata[i], l_th_readdata[i]);
        }
    } else {
        th_msec = th_msec_tmp;
        hrtimer_flg = 0;
        wake_unlock(&prox_cdc_wake_lock);
    }
    
    FUNC_FIN_LOG();
    mutex_unlock(&prox_cdc_lock);
    return;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_th_timer_callback                                                */
/* ------------------------------------------------------------------------- */
static enum hrtimer_restart PROX_CDC_th_timer_callback(struct hrtimer *timer)
{
    queue_work(prox_cdc_wq, &prox_cdc_th_work_data);
    return HRTIMER_NORESTART;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_dump_start                                                       */
/* ------------------------------------------------------------------------- */
static int PROX_CDC_dump_start(int val)
{
    unsigned long sec, nsec;
    
    mutex_lock(&prox_cdc_lock);
    
    prox_cdc_th_interval_ms = val;
    
    if (prox_cdc_th_interval_ms) {
        if (prox_cdc_th_interval_ms < 10) {
            DEBUG_LOG("interval time is Err, Set default 50ms\n");
            prox_cdc_th_interval_ms = 10;
        }
        
        if(th_msec_tmp == 0){
            th_msec = prox_cdc_th_interval_ms;
            th_msec_tmp = prox_cdc_th_interval_ms;
        } else {
            th_msec_tmp = prox_cdc_th_interval_ms;
        }
        
        if( atomic_read(&enable_mode) ) {
            if (th_msec >= 1000) {
                sec  = th_msec / 1000;
                nsec = (th_msec % 1000) * 1000 * 1000;
            } else {
                sec  = 0;
                nsec = th_msec * 1000 * 1000;
            }
            if (hrtimer_flg == 0) {
                wake_lock(&prox_cdc_wake_lock);
                hrtimer_start(&prox_cdc_threshold_timer, ktime_set(sec, nsec), HRTIMER_MODE_REL);
                hrtimer_flg = 1;
            }
        } else {
            th_msec = th_msec_tmp;
        }
    } else {
        th_msec = prox_cdc_th_interval_ms;
        th_msec_tmp = prox_cdc_th_interval_ms;
        hrtimer_flg = 0;
        wake_unlock(&prox_cdc_wake_lock);
    }
    
    mutex_unlock(&prox_cdc_lock);
    
    return 0;
}
#endif /* CONFIG_ANDROID_ENGINEERING */

/* ------------------------------------------------------------------------- */
/* PROX_CDC_get_stage_num                                                    */
/* ------------------------------------------------------------------------- */
static unsigned short PROX_CDC_get_stage_num(void)
{
    unsigned short mode;
    int boot;
    unsigned short sh_hw_handset;
    sharp_smem_common_type *p_sh_smem_common_type = NULL;

    if( prox_cdc_stage_read_flg == 0 ) {

        mode = sh_boot_get_bootmode();
        if( mode == 0 ) {
            /* Error */
            prox_cdc_stage_num = PROX_CDC_DEFAULT_STAGE_NUM;
            DEBUG_ERROR_LOG("[PROX_CDC_get_stage_num Error : sh_boot_get_bootmode ]");
            FUNC_FIN_LOG();
            return prox_cdc_stage_num;
        }
        
        if (prox_cdc_hw_revision == 0xFF ) {
            /* Error */
            prox_cdc_stage_num = PROX_CDC_DEFAULT_STAGE_NUM;
            DEBUG_ERROR_LOG("[PROX_CDC_get_stage_num Error : sh_boot_get_hw_revision ]");
            FUNC_FIN_LOG();
            return prox_cdc_stage_num;
        }

        p_sh_smem_common_type = sh_smem_get_common_address();
        sh_hw_handset = p_sh_smem_common_type->sh_hw_handset;

        if( (mode == SH_BOOT_D) || (mode == SH_BOOT_F_F) ) {
            boot = PROX_CDC_BOOT_DIAG;
        } else {
            boot = PROX_CDC_BOOT_NORMAL;
        }

        if( sh_hw_handset == PROX_CDC_DB ) {
            prox_cdc_stage_num = prox_cdc_stage[boot][PROX_CDC_HW_DB];
        }else {
            switch( prox_cdc_hw_revision ) {
            case HW_ES0:
            case HW_ES05:
                prox_cdc_stage_num = prox_cdc_stage[boot][PROX_CDC_HW_ES0];
                break;
            case HW_ES1:
                prox_cdc_stage_num = prox_cdc_stage[boot][PROX_CDC_HW_ES1];
                break;
            case HW_PP1:
                prox_cdc_stage_num = prox_cdc_stage[boot][PROX_CDC_HW_PP1];
                break;
            case HW_PP2:
                prox_cdc_stage_num = prox_cdc_stage[boot][PROX_CDC_HW_PP2];
                break;
            case HW_PMP:
                prox_cdc_stage_num = prox_cdc_stage[boot][PROX_CDC_HW_PMP];
                break;
            default:
                DEBUG_ERROR_LOG("[PROX_CDC_get_stage_num Error : hw_revision case]");
                prox_cdc_stage_num = PROX_CDC_DEFAULT_STAGE_NUM;
                break;
            }
        }
        prox_cdc_stage_read_flg = 1;
    }

    return prox_cdc_stage_num;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_calc_threshold                                                   */
/* ------------------------------------------------------------------------- */
static void PROX_CDC_calc_threshold(unsigned short stage, unsigned char stage_data)
{
    int ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    unsigned short sf_ambient = 0;
    unsigned short l_offset = prox_cdc_bank2_stage_param[stage][4];
    unsigned short h_offset = prox_cdc_bank2_stage_param[stage][5];
    unsigned short h_threshold = 0;
    unsigned short l_threshold = 0;
    
    FUNC_LOG();
    
    ret = PROX_CDC_i2c_read(prox_cdc_reg_stage_sf_ambient[stage], &sf_ambient);
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_ERROR_LOG("<OTHER> i2c read control error.");
        return;
    }
    
    DEBUG_LOG("S%d_AMBIENT  = %04X", stage, sf_ambient);
    
    if (!stage_data) {
        h_threshold = sf_ambient + (h_offset / 4) + ((h_offset - (h_offset / 4)) / 16) * PROX_CDC_SENSITIVITY_ADJUST_NEAR;
        l_threshold = sf_ambient - (l_offset / 4) - ((l_offset - (l_offset / 4)) / 16) * PROX_CDC_SENSITIVITY_ADJUST_NEAR;
    } else {
        h_threshold = sf_ambient + (h_offset / 4) + ((h_offset - (h_offset / 4)) / 16) * PROX_CDC_SENSITIVITY_ADJUST_FAR;
        l_threshold = sf_ambient - (l_offset / 4) - ((l_offset - (l_offset / 4)) / 16) * PROX_CDC_SENSITIVITY_ADJUST_FAR;
    }
    
    ret += PROX_CDC_i2c_write(prox_cdc_reg_stage_high_threshold[stage], h_threshold);
    ret += PROX_CDC_i2c_write(prox_cdc_reg_stage_low_threshold[stage], l_threshold);
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_ERROR_LOG("<OTHER> i2c write control error.");
        return;
    }
    
    DEBUG_LOG("S%d_H_TH  = %04X", stage, h_threshold);
    DEBUG_LOG("S%d_L_TH  = %04X", stage, l_threshold);
    
    FUNC_FIN_LOG();
    
    return;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_open                                                             */
/* ------------------------------------------------------------------------- */

static int PROX_CDC_open(struct inode *inode, struct file *filp)
{
    mutex_lock(&prox_cdc_lock);
    
    FUNC_LOG();
    
    if (PROX_CDC_check_exist() != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_LOG("PROX_CDC_check_exist error");
        mutex_unlock(&prox_cdc_lock);
        return -1;
    }

    if (inode == NULL) {
        DEBUG_ERROR_LOG("[PROX_CDC_open NULL_Error : inode]");
        mutex_unlock(&prox_cdc_lock);
        return -1;
    }
    
    if (filp == NULL) {
        DEBUG_ERROR_LOG("[PROX_CDC_open NULL_Error : filp]");
        mutex_unlock(&prox_cdc_lock);
        return -1;
    }
    
    if (atomic_cmpxchg(&open_flag, 0, 1) == 0) {
        /* Init = Far */
        atomic_set(&sensor_data, 1);
        memset(stage_data, 1, sizeof(stage_data));
        memset(bk_stage_data, 1, sizeof(bk_stage_data));
    }else{
        DEBUG_ERROR_LOG("[PROX_CDC_open flag_Error : open_flag ]");
    }
    
    FUNC_FIN_LOG();
    
    mutex_unlock(&prox_cdc_lock);
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_release                                                          */
/* ------------------------------------------------------------------------- */

static int PROX_CDC_release(struct inode *inode, struct file *filp)
{
    mutex_lock(&prox_cdc_lock);
    
    FUNC_LOG();
    
    if (PROX_CDC_check_exist() != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_LOG("PROX_CDC_check_exist error");
        mutex_unlock(&prox_cdc_lock);
        return -1;
    }

    if (inode == NULL) {
        DEBUG_ERROR_LOG("[PROX_CDC_release NULL_Error : inode]");
        mutex_unlock(&prox_cdc_lock);
        return -1;
    }
    
    if (filp == NULL) {
        DEBUG_ERROR_LOG("[PROX_CDC_release NULL_Error : filp]");
        mutex_unlock(&prox_cdc_lock);
        return -1;
    }
    
    PROX_CDC_IOCTL_Disable();
    
    atomic_set(&open_flag, 0);
    
    wake_unlock(&prox_cdc_timeout_wake_lock);
    
    FUNC_FIN_LOG();
    
    mutex_unlock(&prox_cdc_lock);
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_read                                                             */
/* ------------------------------------------------------------------------- */

static int PROX_CDC_read(struct file *filp, char __user *buf,
            size_t count, loff_t *ppos)
{
    FUNC_LOG();
    
    if (PROX_CDC_check_exist() != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_LOG("PROX_CDC_check_exist error");
        return -1;
    }

    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_ioctl                                                            */
/* ------------------------------------------------------------------------- */

static long PROX_CDC_ioctl(struct file *filp,
                         unsigned int cmd, unsigned long arg)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    void __user *argp = (void __user *)arg;
    proximity_cdc_ioctl io_data;
    struct proximity_cdc_status status;
    
    mutex_lock(&prox_cdc_lock);
    
    if (PROX_CDC_check_exist() != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_LOG("PROX_CDC_check_exist error");
        mutex_unlock(&prox_cdc_lock);
        return SH_PROXIMITY_CDC_RESULT_SUCCESS;
    }

    if (filp == NULL) {
        DEBUG_ERROR_LOG("[PROX_CDC_ioctl NULL_Error : filp]");
        mutex_unlock(&prox_cdc_lock);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    switch (cmd) {
    case CDC_IOCTL_ENABLE:
        ret = PROX_CDC_IOCTL_Enable();
        break;
    
    case CDC_IOCTL_DISABLE:
        ret = PROX_CDC_IOCTL_Disable();
        break;
    
    case CDC_IOCTL_READ_DATA:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_read_data(&io_data.data);
        if(copy_to_user(argp, &io_data, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;
    
    case CDC_IOCTL_AMBIENT_READ:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_ambient_read(io_data.stage, &io_data.data);
        if(copy_to_user(argp, &io_data, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;
    
    case CDC_IOCTL_AMBIENT_RESET:
        ret = PROX_CDC_IOCTL_ambient_reset();
        break;
    
    case CDC_IOCTL_POS_THRESHOLD_READ:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_pos_threshold_read(io_data.stage, &io_data.data);
        if(copy_to_user(argp, &io_data, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;
    
    case CDC_IOCTL_NEG_THRESHOLD_READ:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_neg_threshold_read(io_data.stage, &io_data.data);
        if(copy_to_user(argp, &io_data, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;
    
    case CDC_IOCTL_POS_THRESHOLD_SENS_WRITE:
#if defined (PROX_CDC_ENABLE_SENS_WRITE)
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
#endif /* PROX_CDC_ENABLE_SENS_WRITE */
        ret = PROX_CDC_IOCTL_pos_threshold_sens_write(io_data.stage, io_data.data);
        break;
    
    case CDC_IOCTL_NEG_THRESHOLD_SENS_WRITE:
#if defined (PROX_CDC_ENABLE_SENS_WRITE)
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
#endif /* PROX_CDC_ENABLE_SENS_WRITE */
        ret = PROX_CDC_IOCTL_neg_threshold_sens_write(io_data.stage, io_data.data);
        break;
    
    case CDC_IOCTL_RESULT_READ:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_result_read(io_data.stage, &io_data.data);
        if(copy_to_user(argp, &io_data, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;
    
    case CDC_IOCTL_STAGE_READ_DATA:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_stage_read_data(io_data.stage, &io_data.data);
        if(copy_to_user(argp, &io_data, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;

    case CDC_IOCTL_OFFSET_ADJ_WRITE:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_offset_adj_write(io_data.stage, &io_data.data);
        if(copy_to_user(argp, &io_data, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;

    case CDC_IOCTL_GET_STATUS:
        if(copy_from_user(&status, argp, sizeof(struct proximity_cdc_status))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_get_status(&status);
        if(copy_to_user(argp, &status, sizeof(struct proximity_cdc_status))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        break;

    case CDC_IOCTL_POWERMODE:
        if(copy_from_user(&io_data, argp, sizeof(io_data))){
            mutex_unlock(&prox_cdc_lock);
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }
        ret = PROX_CDC_IOCTL_powermode(io_data.mode);
        break;

    default:
        mutex_unlock(&prox_cdc_lock);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    mutex_unlock(&prox_cdc_lock);
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_Remove                                                           */
/* ------------------------------------------------------------------------- */

static int PROX_CDC_Remove(void)
{
    FUNC_LOG();
    
    input_free_device(poProximityCDCRec->input_dev);
    kfree(poProximityCDCRec);
    
    FUNC_FIN_LOG();
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}


/* ------------------------------------------------------------------------- */
/* PROX_CDC_Probe                                                            */
/* ------------------------------------------------------------------------- */

static int PROX_CDC_Probe(void)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    
    FUNC_LOG();
    
    /* Allocate memory for driver data */
    poProximityCDCRec = kzalloc(sizeof(drv_data), GFP_KERNEL);
    
    if (!poProximityCDCRec) {
        DEBUG_ERROR_LOG("memory allocation failed.");
        ret = -ENOMEM;
        goto memory_error;
    }
    
    ret = request_irq(gpio_to_irq(PROX_CDC_INT_NUM), PROX_CDC_INT_isr,
                      IRQF_TRIGGER_LOW, "prox_CDC", NULL);
    
    if (ret) {
        DEBUG_ERROR_LOG("request_irq error.");
        goto request_irq_error;
    }else{
        disable_irq(gpio_to_irq(PROX_CDC_INT_NUM));
        prox_cdc_irq_status = PROX_CDC_IRQ_DISABLED;
    }

    ret = gpio_request(PROX_CDC_INT_NUM, "PROXIMITY_CDC_INT");
    if (ret) {
        DEBUG_ERROR_LOG("gpio_request failed.");
        goto gpio_request_error;
    }

    gpio_direction_input(PROX_CDC_INT_NUM);
    
    PROX_CDC_check_exist();
    ret = PROX_CDC_REG_Initialize();
    
    if (ret) {
        DEBUG_ERROR_LOG("initialize failed.");
        goto initialize_error;
    }
    
    ret = misc_register(&PROX_cdc_device);
    
    if (ret) {
        DEBUG_ERROR_LOG("misc_register failed.");
        goto misc_register_error;
    }
    
    /* Declare input device */
    poProximityCDCRec->input_dev = input_allocate_device();
    
    if (!poProximityCDCRec->input_dev) {
        ret = -ENOMEM;
        DEBUG_ERROR_LOG("Failed to allocate input device.");
        goto input_dev_error;
    }
    
    /* Setup input device */
    set_bit(EV_ABS, poProximityCDCRec->input_dev->evbit);
    
    /* proximity value near=0, far=1 */
    input_set_abs_params(poProximityCDCRec->input_dev, ABS_DISTANCE, 0, 1, 0, 0);
    
    /* Set name */
    poProximityCDCRec->input_dev->name = "proximity_cdc";
    
    /* Register */
    ret = input_register_device(poProximityCDCRec->input_dev);
    
    if (ret) {
        DEBUG_ERROR_LOG("Unable to register input device.");
        goto input_register_error;
    }
    
    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;

input_register_error:
    input_free_device(poProximityCDCRec->input_dev);
input_dev_error:
    misc_deregister(&PROX_cdc_device);
misc_register_error:
initialize_error:
    gpio_free(PROX_CDC_INT_NUM);
gpio_request_error:
    free_irq(gpio_to_irq(PROX_CDC_INT_NUM), 0);
request_irq_error:
    kfree(poProximityCDCRec);
memory_error:
    return ret;
}

#ifdef CONFIG_PM
/* ------------------------------------------------------------------------- */
/* PROX_CDC_suspend                                                          */
/* ------------------------------------------------------------------------- */
static int PROX_CDC_suspend(struct device *dev)
{
    mutex_lock(&prox_cdc_lock);
    FUNC_LOG();
    
    atomic_set(&prox_cdc_suspend_status, 1);
    
    FUNC_FIN_LOG();
    mutex_unlock(&prox_cdc_lock);
    
    return 0;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_resume                                                           */
/* ------------------------------------------------------------------------- */
static int PROX_CDC_resume(struct device *dev)
{
    mutex_lock(&prox_cdc_lock);
    FUNC_LOG();
    
    atomic_set(&prox_cdc_suspend_status, 0);
    
    if(atomic_read(&enable_mode) == 1) {
        PROX_CDC_enable_irq();
    }
    
    FUNC_FIN_LOG();
    mutex_unlock(&prox_cdc_lock);
    
    return 0;
}
#endif /* CONFIG_PM */

/* --------------------------------------------------------------------------*/
/* PROX_CDC_workqueue_handler                                                */
/* --------------------------------------------------------------------------*/
static void PROX_CDC_workqueue_handler(struct work_struct* work)
{
    unsigned short readdata = 0;
    unsigned short cdc_readdata[NUM_SH_PROXIMITY_CDC_STAGE];
    unsigned short h_th_readdata[NUM_SH_PROXIMITY_CDC_STAGE];
    unsigned short l_th_readdata[NUM_SH_PROXIMITY_CDC_STAGE];
    unsigned short stage;
    int data;
    int input_flg = 0;
    int i;
    
    mutex_lock(&prox_cdc_lock);
    
    FUNC_LOG();
    
    if(atomic_read(&enable_mode) == 0) {
        DEBUG_LOG("enable_mode = 0");
        goto func_fin;
    }
    
#ifdef CONFIG_PM
    if(atomic_read(&prox_cdc_suspend_status) == 1) {
        DEBUG_LOG("i2c driver already suspend");
        PROX_CDC_enable_irq();
        goto func_suspend_fin;
    }
#endif /* CONFIG_PM */
    
    stage = PROX_CDC_get_stage_num();
    
    memset(cdc_readdata, 0, sizeof(cdc_readdata));
    memset(h_th_readdata, 0, sizeof(h_th_readdata));
    memset(l_th_readdata, 0, sizeof(l_th_readdata));
    
    for( i = 0; i <= stage; i++ ) {
        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[i], &cdc_readdata[i]);
        DEBUG_LOG("CDC_RESULT_S%d  = %04X", i, cdc_readdata[i]);
        
        PROX_CDC_i2c_read(prox_cdc_reg_stage_high_threshold[i], &h_th_readdata[i]);
        DEBUG_LOG("S%d_H_TH  = %04X", i, h_th_readdata[i]);
        
        PROX_CDC_i2c_read(prox_cdc_reg_stage_low_threshold[i], &l_th_readdata[i]);
        DEBUG_LOG("S%d_L_TH  = %04X", i, l_th_readdata[i]);
    }
    
    /* Proximity Detect */
    PROX_CDC_i2c_read(AD7146_REG_STAGE_HIGH_INT_STATUS , &readdata);
    DEBUG_LOG("HIGH_INT_STATUS = %04X",readdata);
    
    for( i = 0; i <= stage; i++ ) {
        if( readdata & (0x0001 << i)) {
            stage_data[i] = 0;
        } else {
            stage_data[i] = 1;
        }
        if (bk_stage_data[i] != stage_data[i]) {
            PROX_CDC_calc_threshold(i, stage_data[i]);
        }
    }
    
    memcpy(bk_stage_data, stage_data, sizeof(stage_data));
    
    if (!stage_data[SH_PROXIMITY_CDC_STAGE0]) {
        data = atomic_cmpxchg(&sensor_data, 1, 0);
        if(data == 1) {
            input_flg = 1;
        }
    } else {
        data = atomic_cmpxchg(&sensor_data, 0, 1);
        if(data == 0) {
            input_flg = 1;
        }
    }
    
    if( input_flg == 1 ) {
        wake_lock_timeout(&prox_cdc_timeout_wake_lock , (1 * HZ));
        
        input_report_abs(poProximityCDCRec->input_dev, ABS_DISTANCE, atomic_read(&sensor_data));
        input_sync(poProximityCDCRec->input_dev);
        
        DEBUG_SENSOR_LOG("SENSOR_DATA = %x\n",atomic_read(&sensor_data));
    }
    
    /* Error Detect */
    PROX_CDC_i2c_read(AD7146_REG_STAGE_LOW_INT_STATUS , &readdata);
    DEBUG_LOG("LOW_INT_STATUS  = %04X",readdata);
    
    if((readdata & 0xFFFF) != 0){
        PROX_CDC_REG_Recalibration();
        DEBUG_LOG("Recalibration Occur.");
    }

    PROX_CDC_enable_irq();
    
func_fin:
    wake_unlock(&prox_cdc_wake_lock);
func_suspend_fin:
    FUNC_FIN_LOG();
    mutex_unlock(&prox_cdc_lock);
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_INT_isr                                                          */
/* ------------------------------------------------------------------------- */
static irqreturn_t PROX_CDC_INT_isr(int irq, void *dev_id)
{
    FUNC_LOG();
    wake_lock(&prox_cdc_wake_lock);
    
    PROX_CDC_disable_irq();
    
#ifdef CONFIG_PM
    if(atomic_read(&prox_cdc_suspend_status) == 0) {
        queue_work(prox_cdc_wq, &prox_cdc_wk);
    }
#else
    queue_work(prox_cdc_wq, &prox_cdc_wk);
#endif /* CONFIG_PM */
    
    FUNC_FIN_LOG();
    return IRQ_HANDLED;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_Enable                                                     */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_Enable(void)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    int retry;
#ifdef CONFIG_ANDROID_ENGINEERING
	unsigned long sec, nsec;
#endif /* CONFIG_ANDROID_ENGINEERING */
    
    FUNC_LOG();
    
    if(atomic_read(&enable_mode) == 0) {
        for(retry = 0; retry <= 3; retry++) {
            ret = PROX_CDC_REG_Enable();
            if(ret == SH_PROXIMITY_CDC_RESULT_SUCCESS){
                PROX_CDC_REG_Recalibration();
                PROX_CDC_enable_irq();
                atomic_set(&enable_mode, 1);
#ifdef CONFIG_SHTERM
                shterm_k_set_info( SHTERM_INFO_PROXIMITY_CDC, 1 );
#endif /* CONFIG_SHTERM */
                break;
            }
            DEBUG_ERROR_LOG("retry count = %d" , retry);
        }
        
#ifdef CONFIG_ANDROID_ENGINEERING
        if (prox_cdc_th_interval_ms) {
            if (th_msec >= 1000) {
                sec  = th_msec / 1000;
                nsec = (th_msec % 1000) * 1000 * 1000;
            } else {
                sec  = 0;
                nsec = th_msec * 1000 * 1000;
            }
            
            if (hrtimer_flg == 0) {
                wake_lock(&prox_cdc_wake_lock);
                hrtimer_start(&prox_cdc_threshold_timer, ktime_set(sec, nsec), HRTIMER_MODE_REL);
                hrtimer_flg = 1;
            }
        }
#endif /* CONFIG_ANDROID_ENGINEERING */
    } else {
        ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    }
    
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_Disable                                                    */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_Disable(void)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    int retry;

    FUNC_LOG();
    
    if(atomic_read(&enable_mode) == 1) {
        PROX_CDC_disable_irq();
        for(retry = 0; retry <= 3; retry++) {
            ret = PROX_CDC_REG_Disable();
            if(ret == SH_PROXIMITY_CDC_RESULT_SUCCESS){
                memset(bk_stage_data, 1, sizeof(bk_stage_data));
#ifdef CONFIG_SHTERM
                shterm_k_set_info( SHTERM_INFO_PROXIMITY_CDC, 0 );
#endif /* CONFIG_SHTERM */
                atomic_set(&enable_mode, 0);
                wake_unlock(&prox_cdc_wake_lock);
                break;
            }
            DEBUG_ERROR_LOG("retry count = %d" , retry);
        }
    } else {
        ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    }
    
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_ambient_read                                               */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_ambient_read(unsigned short stage, unsigned short *data)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    unsigned short readdata = 0;
    FUNC_LOG();
    
    if(stage > PROX_CDC_get_stage_num()){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    if(data == NULL){
        DEBUG_ERROR_LOG("NULL_POINTER data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    ret = PROX_CDC_i2c_read(prox_cdc_reg_stage_sf_ambient[stage], &readdata);

    *data = readdata;
    
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_ambient_reset                                              */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_ambient_reset(void)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    FUNC_LOG();
    
    ret = PROX_CDC_REG_Recalibration();
    
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_pos_threshold_read                                         */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_pos_threshold_read(unsigned short stage, unsigned short *data)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    unsigned short readdata = 0;
    FUNC_LOG();
    
    if(stage > PROX_CDC_get_stage_num()){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    if(data == NULL){
        DEBUG_ERROR_LOG("NULL_POINTER data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    ret = PROX_CDC_i2c_read(prox_cdc_reg_stage_high_threshold[stage] , &readdata);
    DEBUG_LOG("S%d_POS_TH  = %04X", stage, readdata);
    
    *data = readdata;
    
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_neg_threshold_read                                         */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_neg_threshold_read(unsigned short stage, unsigned short *data)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    unsigned short readdata = 0;
    FUNC_LOG();
    
    if(stage > PROX_CDC_get_stage_num()){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    if(data == NULL){
        DEBUG_ERROR_LOG("NULL_POINTER data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    ret = PROX_CDC_i2c_read(prox_cdc_reg_stage_low_threshold[stage] , &readdata);
    DEBUG_LOG("S%d_NEG_TH  = %04X", stage, readdata);
    
    *data = readdata;
    
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_pos_threshold_sens_write                                   */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_pos_threshold_sens_write(unsigned short stage, unsigned short data)
{
#if defined (PROX_CDC_ENABLE_SENS_WRITE)
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    FUNC_LOG();
    
    if(stage > PROX_CDC_get_stage_num()){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    if(data >= NUM_SH_PROXIMITY_CDC_SENS){
        DEBUG_ERROR_LOG("not support data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    ret = PROX_CDC_i2c_msk_bit_reg(prox_cdc_reg_stage_sensitivity[stage], (data << 8), 0x0F00);
    
    FUNC_FIN_LOG();
    
    return ret;
#else
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
#endif /* PROX_CDC_ENABLE_SENS_WRITE */
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_neg_threshold_sens_write                                   */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_neg_threshold_sens_write(unsigned short stage, unsigned short data)
{
#if defined (PROX_CDC_ENABLE_SENS_WRITE)
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    FUNC_LOG();
    
    if(stage > PROX_CDC_get_stage_num()){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    if(data >= NUM_SH_PROXIMITY_CDC_SENS){
        DEBUG_ERROR_LOG("not support data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    ret = PROX_CDC_i2c_msk_bit_reg(prox_cdc_reg_stage_sensitivity[stage], data, 0x000F);
    
    FUNC_FIN_LOG();
    
    return ret;
#else
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
#endif /* PROX_CDC_ENABLE_SENS_WRITE */
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_result_read                                                */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_result_read(unsigned short stage, unsigned short *data)
{
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
    unsigned short readdata = 0;

    FUNC_LOG();
    
    if(stage > PROX_CDC_get_stage_num()){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    if(data == NULL){
        DEBUG_ERROR_LOG("NULL_POINTER data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    ret = PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage] , &readdata);
    DEBUG_LOG("CDC_RESULT_S%d  = %04X", stage, readdata);
    
    *data = readdata;
    
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_read_data                                                  */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_read_data(unsigned short *data)
{
    FUNC_LOG();
    
    if(data == NULL){
        DEBUG_ERROR_LOG("NULL_POINTER data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    *data = (unsigned short)atomic_read(&sensor_data);
    
    DEBUG_SENSOR_LOG("sensor_data = %04X",*data);
    
    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_stage_read_data                                            */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_stage_read_data(unsigned short stage, unsigned short *data)
{
    FUNC_LOG();

    if(stage > PROX_CDC_get_stage_num()){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    if(data == NULL){
        DEBUG_ERROR_LOG("NULL_POINTER data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    *data = stage_data[stage];
    DEBUG_SENSOR_LOG("stage%d_data = %04X", stage, *data);
    
    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_offset_adj_write                                           */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_offset_adj_write(unsigned short stage, unsigned short *data)
{
    int ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    unsigned short readdata = 0;
    unsigned short stage_num;
    
    FUNC_LOG();
    
    stage_num = PROX_CDC_get_stage_num();
    
    if(stage > stage_num){
        DEBUG_ERROR_LOG("not support stage");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }

    if(data == NULL){
        DEBUG_ERROR_LOG("NULL_POINTER data");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }

    ret += PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , (0xC00C | (stage_num << 4)));
    
    ret += PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage] , 0x0000);

    msleep(4 * (stage_num + 1));
    
    ret += PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &readdata);

    DEBUG_LOG("CDC_RESULT_S%d  = %04X", stage, readdata);

    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_ERROR_LOG("<OTHER> i2c control error.");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }

    if( readdata > 0x82DC ) {
        /* Positive */
        ret = PROX_CDC_offset_adj_write_pos(stage);

    } else if( readdata < 0x7724  ) {
        /* Negative */
        ret = PROX_CDC_offset_adj_write_neg(stage);
    }

    PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], data);

    ret += PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , (0xC00E | (stage_num << 4)));

    DEBUG_LOG("STAGE%d_AFE_OFFSET  = %04X", stage, *data);

    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_get_status                                                 */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_get_status(struct proximity_cdc_status *status)
{
    if( atomic_read(&enable_mode) == 1 ) {
        status->enable_status = SH_PROXIMITY_CDC_STATUS_ENABLE;
    } else {
        status->enable_status = SH_PROXIMITY_CDC_STATUS_DISABLE;
    }

    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_IOCTL_powermode                                                  */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_IOCTL_powermode(unsigned char mode)
{
    unsigned short stage_num;
    int ret = SH_PROXIMITY_CDC_RESULT_FAILURE;

    FUNC_LOG();
    
    stage_num = PROX_CDC_get_stage_num();

    if(mode >= NUM_SH_PROXIMITY_CDC_POWERMODE){
        DEBUG_ERROR_LOG("not support mode");
        return SH_PROXIMITY_CDC_RESULT_FAILURE_USER;
    }
    
    switch(mode){
        case 0:
            ret = PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , 0xC00F | (stage_num << 4));
            DEBUG_LOG("PowerMode is FullShutDownMode");
            break;
            
        case 1:
            ret = PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , 0xC00C | (stage_num << 4));
            DEBUG_LOG("PowerMode is FullPowerMode");
            break;
            
        case 2:
            ret = PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , 0xC00E | (stage_num << 4));
            DEBUG_LOG("PowerMode is LowPowerMode");
            break;

        default:
            break;
    }
        
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_REG_Enable                                                       */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_REG_Enable(void)
{
    int ret =  SH_PROXIMITY_CDC_RESULT_SUCCESS;
    int i;
    unsigned short stage;
    unsigned short readdata = 0;

    FUNC_LOG();
    
    stage = PROX_CDC_get_stage_num();

    if( prox_cdc_smem_flg == 0 ) {
        PROX_CDC_get_smem_data();
    }

    for(i = 0; i <= stage; i++){
        ret += PROX_CDC_i2c_seq_write(prox_cdc_reg_stage_connection6[i], &prox_cdc_bank2_stage_param[i][0] , 8);
    }
        
    ret += PROX_CDC_i2c_write(AD7146_REG_AMB_COMP_CTRL0             , 0x012B);
    ret += PROX_CDC_i2c_write(AD7146_REG_AMB_COMP_CTRL1             , 0xA064);
    ret += PROX_CDC_i2c_write(AD7146_REG_AMB_COMP_CTRL2             , 0x0882);
    ret += PROX_CDC_i2c_write(AD7146_REG_STAGE_LOW_INT_ENABLE       , prox_cdc_enable[stage]);
    ret += PROX_CDC_i2c_write(AD7146_REG_STAGE_HIGH_INT_ENABLE      , prox_cdc_enable[stage]);
    ret += PROX_CDC_i2c_write(AD7146_REG_STAGE_COMPLETE_INT_ENABLE  , 0x0000);
    
    msleep(1);
    
    if (( prox_cdc_hw_revision != HW_PP2) && ( prox_cdc_hw_revision != HW_PMP )) {
        ret += PROX_CDC_i2c_set_bit_write(AD7146_REG_STAGE_CAL_EN   , prox_cdc_enable[stage]);
    } else {
        ret += PROX_CDC_i2c_set_bit_write(AD7146_REG_STAGE_CAL_EN   , (0x5000 | prox_cdc_enable[stage]));
    }
    
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_ERROR_LOG("<OTHER> i2c control error.");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }

    ret = PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , 0xC00C | (stage << 4));

    msleep(4 * (stage + 1));

    ret = PROX_CDC_i2c_read(AD7146_REG_STAGE_HIGH_INT_STATUS , &readdata);
    DEBUG_LOG("HIGH_INT_STATUS = %04X",readdata);
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_ERROR_LOG("<OTHER> i2c control error.");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    ret = PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , 0xC00E | (stage << 4));

    for( i = 0; i <= stage; i++ ) {
        if( readdata & (0x0001 << i)) {
            stage_data[i] = 0;
        } else {
            stage_data[i] = 1;
        }
    }
    
    if (!stage_data[SH_PROXIMITY_CDC_STAGE0]) {
        atomic_set(&sensor_data, 0);
    } else {
        atomic_set(&sensor_data, 1);
    }
    
    DEBUG_SENSOR_LOG("SENSOR_DATA = %x\n",atomic_read(&sensor_data));
    
    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_REG_Disable                                                      */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_REG_Disable(void)
{
    int ret =  SH_PROXIMITY_CDC_RESULT_SUCCESS;
    unsigned short stage;
    
    FUNC_LOG();
    
    stage = PROX_CDC_get_stage_num();
    
    ret += PROX_CDC_i2c_clr_bit_write(AD7146_REG_STAGE_CAL_EN , 0xFFFF);
    
    msleep(1);
    
    ret += PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL          , 0xC01F);
    
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_ERROR_LOG("<OTHER> i2c control error.");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_REG_Initialize                                                   */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_REG_Initialize(void)
{
    int ret =  SH_PROXIMITY_CDC_RESULT_SUCCESS;
    
    FUNC_LOG();
    
    if (PROX_CDC_check_exist() != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_LOG("PROX_CDC_check_exist error");
        return SH_PROXIMITY_CDC_RESULT_SUCCESS;
    }
    
    ret += PROX_CDC_i2c_write(AD7146_REG_PWR_CONTROL , 0xC01F);
    
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        DEBUG_ERROR_LOG("<OTHER> i2c control error.");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_REG_Recalibration                                                */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_REG_Recalibration(void)
{
    int ret =  SH_PROXIMITY_CDC_RESULT_SUCCESS;
    int i = 1;
    int roop = 1;
    unsigned short stage;
    unsigned short h_readdata = 0;
    unsigned short l_readdata = 0;
    unsigned char roop_cnt = 0;
    
    FUNC_LOG();
    
    stage = PROX_CDC_get_stage_num();

    while(roop){
        roop = 0;

        if( roop_cnt == PROX_CDC_ROOP_CNT ){
            DEBUG_ERROR_LOG("<OTHER> Recalibration Retry Over.");
            break;
        }
        roop_cnt++;

        ret += PROX_CDC_i2c_set_bit_write(AD7146_REG_AMB_COMP_CTRL0 , 0x4000);
        
        if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
            DEBUG_ERROR_LOG("<OTHER> i2c control error.");
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }

        msleep(4 * (stage + 1));

        ret += PROX_CDC_i2c_read(AD7146_REG_STAGE_HIGH_INT_STATUS , &h_readdata);
        DEBUG_LOG("HIGH_INT_STATUS = %04X",h_readdata);

        ret += PROX_CDC_i2c_read(AD7146_REG_STAGE_LOW_INT_STATUS , &l_readdata);
        DEBUG_LOG("LOW_INT_STATUS  = %04X",l_readdata);

        if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
            DEBUG_ERROR_LOG("<OTHER> INT_STATUS i2c control error.");
            return SH_PROXIMITY_CDC_RESULT_FAILURE;
        }

        for( i = 0; i <= stage; i++ ) {
            if((l_readdata) & (0x0001 << i)) {
                roop = 1;
                break;
            }
        }
    }

    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_get_smem_data                                                    */
/* ------------------------------------------------------------------------- */

static void  PROX_CDC_get_smem_data(void)
{
    int stage_num;
    unsigned short cprx_offset_hl[NUM_SH_PROXIMITY_CDC_STAGE][2];
    unsigned short cprx_cal_offset[NUM_SH_PROXIMITY_CDC_STAGE];
    sharp_smem_common_type *p_sh_smem_common_type = NULL;
    unsigned long value;

    FUNC_LOG();

    memset((void*)cprx_cal_offset, 0x0000, sizeof(cprx_cal_offset));
    memset((void*)cprx_offset_hl, 0x0000, sizeof(cprx_offset_hl));
    p_sh_smem_common_type = sh_smem_get_common_address();

    memcpy(cprx_cal_offset, p_sh_smem_common_type->shdiag_cprx_cal_offset, sizeof(cprx_cal_offset));
    memcpy(cprx_offset_hl, p_sh_smem_common_type->shdiag_cprx_offset_hl, sizeof(cprx_offset_hl));

    if (( prox_cdc_hw_revision != HW_PP2) && ( prox_cdc_hw_revision != HW_PMP )) {
        prox_cdc_bank2_stage_param[0][0] = PROX_CDC_STAGE0_CONFIGURATION;
    }

    for(stage_num = 0; stage_num < NUM_SH_PROXIMITY_CDC_STAGE; stage_num++){
        if ((stage_num == SH_PROXIMITY_CDC_STAGE0) || (stage_num == SH_PROXIMITY_CDC_STAGE1)) {
            if (cprx_cal_offset[stage_num]) {
                prox_cdc_bank2_stage_param[stage_num][2] = cprx_cal_offset[stage_num]; 
            }
        } else {
            prox_cdc_bank2_stage_param[stage_num][2] = cprx_cal_offset[stage_num];
        }
        DEBUG_LOG("smem prox_cdc_bank2_stage_param[%d][2] = %04X", stage_num, prox_cdc_bank2_stage_param[stage_num][2]);
    }

    for(stage_num = 0; stage_num < NUM_SH_PROXIMITY_CDC_STAGE; stage_num++){
        if ((stage_num == SH_PROXIMITY_CDC_STAGE0) || (stage_num == SH_PROXIMITY_CDC_STAGE1)) {
            if (cprx_offset_hl[stage_num][0]) {
                prox_cdc_bank2_stage_param[stage_num][4] = cprx_offset_hl[stage_num][0];
            }
        } else {
            prox_cdc_bank2_stage_param[stage_num][4] = cprx_offset_hl[stage_num][0];
        }
        DEBUG_LOG("smem prox_cdc_bank2_stage_param[%d][4] = %04X", stage_num, prox_cdc_bank2_stage_param[stage_num][4]);
    }
    
    for(stage_num = 0; stage_num < NUM_SH_PROXIMITY_CDC_STAGE; stage_num++){
        if ((stage_num == SH_PROXIMITY_CDC_STAGE0) || (stage_num == SH_PROXIMITY_CDC_STAGE1)) {
            if (cprx_offset_hl[stage_num][1]) {
                prox_cdc_bank2_stage_param[stage_num][5] = cprx_offset_hl[stage_num][1];
            }
        } else {
            prox_cdc_bank2_stage_param[stage_num][5] = cprx_offset_hl[stage_num][1];
        }
        DEBUG_LOG("smem prox_cdc_bank2_stage_param[%d][5] = %04X", stage_num, prox_cdc_bank2_stage_param[stage_num][5]);
    }
    
    for(stage_num = 0; stage_num < NUM_SH_PROXIMITY_CDC_STAGE; stage_num++){
        value = prox_cdc_bank2_stage_param[stage_num][4];
        if(value >= 0xD555){
            prox_cdc_bank2_stage_param[stage_num][6] = 0xFFFF;
        }else{
            prox_cdc_bank2_stage_param[stage_num][6] = (unsigned short)((value * (0x000C)) / (0x000A));
            DEBUG_LOG("smem prox_cdc_bank2_stage_param[%d][6] = %04X", stage_num, prox_cdc_bank2_stage_param[stage_num][6]);
        }
    }

    for(stage_num = 0; stage_num < NUM_SH_PROXIMITY_CDC_STAGE; stage_num++){
        value = prox_cdc_bank2_stage_param[stage_num][5];
        if(value >= 0xD555){
            prox_cdc_bank2_stage_param[stage_num][7] = 0xFFFF;
        }else{
            prox_cdc_bank2_stage_param[stage_num][7] = (unsigned short)((value * (0x000C)) / (0x000A));
        }
    }
    FUNC_FIN_LOG();
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_offset_adj_write_pos                                             */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_offset_adj_write_pos(unsigned short stage)
{
    int ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    int roop = 1;
    unsigned short afe_offset = 0;
    unsigned short cdc_before = 0;
    unsigned short cdc_after = 0;
    unsigned short stage_num;

    FUNC_LOG();
    
    stage_num = PROX_CDC_get_stage_num();

    while(roop) {
        roop = 0;

        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_before);
        
        PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
        afe_offset += 0x0100;
        PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        DEBUG_LOG("STAGE%d_AFE_OFFSET  = %04X  after", stage, afe_offset);
        
        msleep(4 * (stage_num + 1));

        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_after);
        DEBUG_LOG("CDC_RESULT_S%d  = %04X", stage, cdc_after);
        
        if( cdc_after > 0x8000 ) {
            if( ((afe_offset & 0x3F00) == 0x2F00) && (cdc_after == 0xFFFF) ) {
                PROX_CDC_i2c_set_bit_write(prox_cdc_reg_stage_afe_offset[stage], 0x0080);
                /* swap */
                ret = PROX_CDC_offset_adj_write_pos_swap(stage);
            } else {
                if( (afe_offset & 0x3F00) == 0x3F00 ) {
                    PROX_CDC_i2c_set_bit_write(prox_cdc_reg_stage_afe_offset[stage], 0x0080);
                    /* swap */
                    ret = PROX_CDC_offset_adj_write_pos_swap(stage);
                } else {
                    /* retry */
                    roop = 1;
                }
            }           
        } else if( (0x8000 - cdc_after) > (cdc_before - 0x8000) ) {
            afe_offset = 0;
            PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
            afe_offset -= 0x0100;
            PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        }
        DEBUG_LOG("roop end");
    }

    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_offset_adj_write_pos_swap                                        */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_offset_adj_write_pos_swap(unsigned short stage)
{
    int ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    int roop = 1;
    unsigned short afe_offset = 0;
    unsigned short cdc_before = 0;
    unsigned short cdc_after = 0;
    unsigned short stage_num;

    FUNC_LOG();
    
    stage_num = PROX_CDC_get_stage_num();
    
    while(roop) {
        roop = 0;

        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_before);
        
        PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
        afe_offset += 0x0001;
        PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        DEBUG_LOG("STAGE%d_AFE_OFFSET  = %04X  after", stage, afe_offset);
        
        msleep(4 * (stage_num + 1));

        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_after);
        DEBUG_LOG("CDC_RESULT_S%d  = %04X", stage, cdc_after);

        if( cdc_after > 0x8000 ) {
            if( (afe_offset & 0x003F) == 0x003F ) {
                ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
            } else {
                /* retry */
                roop = 1;
            }
        } else if( (0x8000 - cdc_after) > (cdc_before - 0x8000) ) {
            afe_offset = 0;
            PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
            afe_offset -= 0x0001;
            PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        }
    }
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_offset_adj_write_neg                                             */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_offset_adj_write_neg(unsigned short stage)
{
    int ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    int roop = 1;
    unsigned short afe_offset = 0;
    unsigned short cdc_before = 0;
    unsigned short cdc_after = 0;
    unsigned short stage_num;

    FUNC_LOG();
    
    stage_num = PROX_CDC_get_stage_num();
    
    while(roop) {
        roop = 0;

        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_before);
        
        PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
        afe_offset += 0x0001;
        PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        DEBUG_LOG("STAGE%d_AFE_OFFSET  = %04X  after", stage, afe_offset);
        
        msleep(4 * (stage_num + 1));
        
        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_after);
        DEBUG_LOG("CDC_RESULT_S%d  = %04X", stage, cdc_after);
        
        if( cdc_after < 0x8000 ) {
            if( ((afe_offset & 0x003F) == 0x002F) && (cdc_after == 0x0000) ) {
                PROX_CDC_i2c_set_bit_write(prox_cdc_reg_stage_afe_offset[stage], 0x8000);
                /* swap */
                ret = PROX_CDC_offset_adj_write_neg_swap(stage);
            } else {
                if( (afe_offset & 0x003F) == 0x003F ) {
                    PROX_CDC_i2c_set_bit_write(prox_cdc_reg_stage_afe_offset[stage], 0x8000);
                    /* swap */
                    ret = PROX_CDC_offset_adj_write_neg_swap(stage);
                } else {
                    /* retry */
                    roop = 1;
                }
            }       
        } else if( (cdc_after - 0x8000) > (0x8000 - cdc_before) ) {
            afe_offset = 0;
            PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
            afe_offset -= 0x0001;
            PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        }
    }
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_offset_adj_write_neg_swap                                        */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_offset_adj_write_neg_swap(unsigned short stage)
{
    int ret = SH_PROXIMITY_CDC_RESULT_SUCCESS;
    int roop = 1;
    unsigned short afe_offset = 0;
    unsigned short cdc_before = 0;
    unsigned short cdc_after = 0;
    unsigned short stage_num;

    FUNC_LOG();
    
    stage_num = PROX_CDC_get_stage_num();
    
    while(roop) {
        roop = 0;

        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_before);
        
        PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
        afe_offset += 0x0100;
        PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        DEBUG_LOG("POS_AFE_OFFSET  = %04X  after", afe_offset);
        
        msleep(4 * (stage_num + 1));

        PROX_CDC_i2c_read(prox_cdc_reg_cdc_result[stage], &cdc_after);
        DEBUG_LOG("CDC_RESULT_S0  = %04X", cdc_after);

        if( cdc_after < 0x8000 ) {
            if( (afe_offset & 0x3F00) == 0x3F00 ) {
                ret = SH_PROXIMITY_CDC_RESULT_FAILURE;
            } else {
                /* retry */
                roop = 1;
            }
        } else if( (cdc_after - 0x8000) > (0x8000 - cdc_before) ) {
            afe_offset = 0;
            PROX_CDC_i2c_read(prox_cdc_reg_stage_afe_offset[stage], &afe_offset);
            afe_offset -= 0x0100;
            PROX_CDC_i2c_write(prox_cdc_reg_stage_afe_offset[stage], afe_offset);
        }
    }
    FUNC_FIN_LOG();
    
    return ret;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_init                                                         */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_i2c_init(struct i2c_client *client, const struct i2c_device_id * devid)
{
    prox_cdc_i2c_data_t* i2c_p;
    
    FUNC_LOG();

    if(prox_cdc_i2c_p != NULL){
        return -EPERM;
    }
    
    i2c_p = (prox_cdc_i2c_data_t*)kzalloc(sizeof(prox_cdc_i2c_data_t),GFP_KERNEL);
    if (i2c_p == NULL) {
        return -ENOMEM;
    }
    
    prox_cdc_i2c_p = i2c_p;
    
    i2c_set_clientdata(client,i2c_p);
    i2c_p->this_client = client;
    
    PROX_CDC_Probe();
    
    FUNC_FIN_LOG();
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_exit                                                         */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_i2c_exit(struct i2c_client *client)
{
    prox_cdc_i2c_data_t* i2c_p;
    
    i2c_p = i2c_get_clientdata(client);
    
    kfree(i2c_p);
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_write                                                        */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_i2c_write(unsigned short addr, unsigned short data)
{
    struct i2c_msg msg;
    unsigned char write_buf[4];
    int i2c_ret;
    int result = 1;
    int retry;
    
    if (addr > 0x28F) {
        DEBUG_ERROR_LOG("<OTHER> invalid addr.\n");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    msg.addr     = prox_cdc_i2c_p->this_client->addr;
    msg.flags    = 0;
    msg.len      = 4;
    msg.buf      = write_buf;
    write_buf[0] = (addr >> 8) & 0xff;
    write_buf[1] = addr & 0xff;
    write_buf[2] = (data >> 8) & 0xff;
    write_buf[3] = data & 0xff;
    
    PROX_CDC_qos_start();
    
    for (retry = 0; retry <= 10; retry++) {
        i2c_ret = i2c_transfer(prox_cdc_i2c_p->this_client->adapter, &msg, 1);
        if (i2c_ret > 0) {
            result = 0;
            break;
        }
    }
    
    PROX_CDC_qos_end();
    
    if (result == 1) {
        DEBUG_ERROR_LOG("<OTHER> i2c_transfer time out(i2c_ret = %d).\n", i2c_ret);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_read                                                         */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_i2c_read(unsigned short addr, unsigned short *data)
{
    struct i2c_msg msg;
    unsigned char write_buf[2];
    unsigned char read_buf[2];
    int i2c_ret;
    int result = 1;
    int retry;
    
    if (data == NULL) {
        DEBUG_ERROR_LOG("<NULL_POINTER> data.\n");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    if (addr > 0x28F) {
        DEBUG_ERROR_LOG("<OTHER> invalid addr.\n");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    PROX_CDC_qos_start();
    
    for (retry = 0; retry <= 10; retry++) {
        msg.addr     = prox_cdc_i2c_p->this_client->addr;
        msg.flags    = 0;
        msg.len      = 2;
        msg.buf      = write_buf;
        write_buf[0] = (addr >> 8) & 0xff;
        write_buf[1] = addr & 0xff;
        
        i2c_ret = i2c_transfer(prox_cdc_i2c_p->this_client->adapter, &msg, 1);
        
        if (i2c_ret > 0) {
            msg.addr  = prox_cdc_i2c_p->this_client->addr;
            msg.flags = I2C_M_RD;
            msg.len   = 2;
            msg.buf   = read_buf;
            
            i2c_ret = i2c_transfer(prox_cdc_i2c_p->this_client->adapter, &msg, 1);
            if (i2c_ret > 0) {
                *data = (read_buf[0] << 8) | read_buf[1];
                result = 0;
                break;
            }
        }
    }
    
    PROX_CDC_qos_end();
    
    if (result == 1) {
        DEBUG_ERROR_LOG("<OTHER> i2c_transfer time out(i2c_ret = %d).\n", i2c_ret);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_seq_write                                                    */
/* ------------------------------------------------------------------------- */

static int  PROX_CDC_i2c_seq_write(unsigned short addr, unsigned short *data, int len)
{
    struct i2c_msg msg;
    unsigned char write_buf[32];
    int i;
    int i2c_ret;
    int result = 1;
    int retry;
    int temp_len;
    
    if (addr > 0x28F) {
        DEBUG_ERROR_LOG("<INVALID_VALUE> len(%d).\n", len);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    msg.addr     = prox_cdc_i2c_p->this_client->addr;
    msg.flags    = 0;
    msg.len      = (len*2)+2;
    msg.buf      = write_buf;
    write_buf[0] = (addr >> 8) & 0xff;
    write_buf[1] = addr & 0xff;
    temp_len = (len*2)+2;
    for (i = 2; i < temp_len; i=i+2){
        write_buf[i]   = (*data >> 8) & 0xff;
        write_buf[i+1] = *data & 0xff;
        data++;
    }
    
    PROX_CDC_qos_start();
    
    for (retry = 0; retry <= 10; retry++) {
        i2c_ret = i2c_transfer(prox_cdc_i2c_p->this_client->adapter, &msg, 1);
        if ( i2c_ret > 0 ) {
            result = 0;
            break;
        }
    }
    
    PROX_CDC_qos_end();
    
    if (result == 1) {
        DEBUG_ERROR_LOG("<OTHER> i2c_transfer time out(i2c_ret = %d).\n", i2c_ret);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_seq_read                                                     */
/* ------------------------------------------------------------------------- */
#if defined (CONFIG_ANDROID_ENGINEERING)
static int  PROX_CDC_i2c_seq_read(unsigned short addr, unsigned short *data, int len)
{
    struct i2c_msg msg[2];
    int i;
    int i2c_ret;
    int result = 1;
    int retry;
    int temp_len;
    unsigned char write_buf[2];
    unsigned char read_buf[32];
    
    if (data == NULL) {
        DEBUG_ERROR_LOG("<NULL_POINTER> data.\n");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    if (addr > 0x28F) {
        DEBUG_ERROR_LOG("<OTHER> invalid addr.\n");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    msg[0].addr     = prox_cdc_i2c_p->this_client->addr;
    msg[0].flags    = (prox_cdc_i2c_p->this_client->flags & I2C_M_TEN);
    msg[0].len      = 2;
    msg[0].buf      = write_buf;
    write_buf[0] = (addr >> 8) & 0xff;
    write_buf[1] = addr & 0xff;
    
    msg[1].addr     = prox_cdc_i2c_p->this_client->addr;
    msg[1].flags    = (prox_cdc_i2c_p->this_client->flags & I2C_M_TEN) | I2C_M_RD;
    msg[1].len      = len*2;
    msg[1].buf      = read_buf;
    
    PROX_CDC_qos_start();
    
    for (retry = 0; retry <= 10; retry++) {
        i2c_ret = i2c_transfer(prox_cdc_i2c_p->this_client->adapter, msg, 2);
        temp_len = len * 2;
        if (i2c_ret > 0) {
            for (i = 0; i < temp_len; i=i+2){
               *data = (read_buf[i] << 8) | read_buf[i+1];
               data++;
            }
            result = 0;
            break;
       }     
    }
    
    PROX_CDC_qos_end();
    
    if (result == 1) {
        DEBUG_ERROR_LOG("<OTHER> i2c_transfer time out(i2c_ret = %d).\n", i2c_ret);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }

    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}
#endif /* CONFIG_ANDROID_ENGINEERING */

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_set_bit_write                                                */
/* ------------------------------------------------------------------------- */
static int  PROX_CDC_i2c_set_bit_write(unsigned short addr, unsigned short data)
{
    int  ret;
    unsigned short  set_bit = 0;
    
    ret = PROX_CDC_i2c_read(addr, &set_bit);
    
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        return ret;
    }
    
    set_bit |= data;
    
    return PROX_CDC_i2c_write(addr, set_bit);
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_clr_bit_write                                                */
/* ------------------------------------------------------------------------- */
static int  PROX_CDC_i2c_clr_bit_write(unsigned short addr, unsigned short data)
{
    int  ret;
    unsigned short  clr_bit = 0;
    
    ret = PROX_CDC_i2c_read(addr, &clr_bit);
    
    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        return ret;
    }
    
    clr_bit &= (unsigned short)~data;
    
    return PROX_CDC_i2c_write(addr, clr_bit);
}

#if defined (PROX_CDC_ENABLE_SENS_WRITE)
/* ------------------------------------------------------------------------- */
/* PROX_CDC_i2c_msk_bit_reg                                                  */
/* ------------------------------------------------------------------------- */
static int  PROX_CDC_i2c_msk_bit_reg(unsigned short addr, unsigned short data, unsigned short msk)
{
    int  ret;
    unsigned short  src_bit = 0;
    unsigned short  dst_bit = 0;

    ret = PROX_CDC_i2c_read(addr, &src_bit);

    if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
        return ret;
    }

    dst_bit = ((unsigned short)~msk & src_bit) | (msk & data);

    return PROX_CDC_i2c_write(addr, dst_bit);
}
#endif /* PROX_CDC_ENABLE_SENS_WRITE */

/* ------------------------------------------------------------------------- */
/* PROX_CDC_proc_write                                                       */
/* ------------------------------------------------------------------------- */
#if defined (CONFIG_ANDROID_ENGINEERING)
static int PROX_CDC_proc_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
#define PROXIMITY_CDC_LEN_ID    (2)
#define PROXIMITY_CDC_LEN_PARAM (4)
#define PROXIMITY_CDC_PARAM_MAX (4)
    
    unsigned long len = count;
    struct proximity_cdc_procfs proximity_cdc_pfs;
    char buf[PROXIMITY_CDC_LEN_PARAM + 1];
    char kbuf[PROXIMITY_CDC_LEN_ID + PROXIMITY_CDC_PARAM_MAX * PROXIMITY_CDC_LEN_PARAM];
    int i;
    int j;
    unsigned short addres = 0x0000;
    unsigned short readbuffer[16] = {0};
    unsigned short writebuffer[2] = {0};
    
    FUNC_LOG();
    
    if(count == 0) {
        DEBUG_ERROR_LOG("input Err");
        return 0;
    }
    
    len--;
    if (len < PROXIMITY_CDC_LEN_ID) return count;
    
    if (len > (PROXIMITY_CDC_LEN_ID + PROXIMITY_CDC_PARAM_MAX * PROXIMITY_CDC_LEN_PARAM)){
       len = PROXIMITY_CDC_LEN_ID + PROXIMITY_CDC_PARAM_MAX * PROXIMITY_CDC_LEN_PARAM;
    }
    
    if (copy_from_user(kbuf, buffer, len)) return -EFAULT;
    
    memcpy(buf, kbuf, PROXIMITY_CDC_LEN_ID);
    buf[PROXIMITY_CDC_LEN_ID] = '\0';
    proximity_cdc_pfs.id = simple_strtol(buf, NULL, 10);
    proximity_cdc_pfs.par[0] = 0;
    proximity_cdc_pfs.par[1] = 0;
    proximity_cdc_pfs.par[2] = 0;
    proximity_cdc_pfs.par[3] = 0;
    
    for(i = 0; (i + 1) * PROXIMITY_CDC_LEN_PARAM <= (len - PROXIMITY_CDC_LEN_ID); i++){
        memcpy(buf, &(kbuf[PROXIMITY_CDC_LEN_ID + i * PROXIMITY_CDC_LEN_PARAM]), PROXIMITY_CDC_LEN_PARAM);
        buf[PROXIMITY_CDC_LEN_PARAM] = '\0';
        proximity_cdc_pfs.par[i] = simple_strtol(buf, NULL, 16);
    }
    
    DEBUG_LOG("proximity_cdc_pfs.id = %02X",proximity_cdc_pfs.id);
    DEBUG_LOG("proximity_cdc_pfs.par[0] = %04X",proximity_cdc_pfs.par[0]);
    DEBUG_LOG("proximity_cdc_pfs.par[1] = %04X",proximity_cdc_pfs.par[1]);
    DEBUG_LOG("proximity_cdc_pfs.par[2] = %04X",proximity_cdc_pfs.par[2]);
    DEBUG_LOG("proximity_cdc_pfs.par[3] = %04X",proximity_cdc_pfs.par[3]);
    
    if(proximity_cdc_pfs.id == 0){
        DEBUG_LOG(">>>>>>>PROXIMITY REGISTER WRITE-----");
        PROX_CDC_i2c_write((unsigned short)proximity_cdc_pfs.par[0], (unsigned short)proximity_cdc_pfs.par[1]);
        DEBUG_LOG(">>>>>>>PROXIMITY REGISTER WRITE-----\n");
    }
    
    if(proximity_cdc_pfs.id == 1){
        DEBUG_LOG(">>>>>>>PROXIMITY REGISTER READ----");
        PROX_CDC_i2c_read((unsigned short)proximity_cdc_pfs.par[0], readbuffer);
        DEBUG_LOG("PROX_I2cRead_Data = %04X",readbuffer[0]);
        DEBUG_LOG("<<<<<<<PROXIMITY REGISTER READ-----\n");
    }
    
    if(proximity_cdc_pfs.id == 2){
        writebuffer[0] = proximity_cdc_pfs.par[1];
        writebuffer[1] = proximity_cdc_pfs.par[2];
        DEBUG_LOG(">>>>>>>PROXIMITY REGISTER SQE WRITE-----");
        PROX_CDC_i2c_seq_write((unsigned short)proximity_cdc_pfs.par[0], writebuffer, 2);
        DEBUG_LOG("PROX_I2cWriteData = %04X",writebuffer[0]);
        DEBUG_LOG("PROX_I2cWriteData = %04X",writebuffer[1]);
        DEBUG_LOG("<<<<<<<PROXIMITY REGISTER SQE WRITE-----\n");
    }
    
    if(proximity_cdc_pfs.id == 3){
        DEBUG_LOG(">>>>>>>PROXIMITY REGISTER SQE READ-----");
        for (i = 0; i < 82; i++){
            PROX_CDC_i2c_seq_read(addres, readbuffer,8);
            printk("Adr = %04X:",addres);
            for (j = 0; j < 8; j++){
              printk(" %04X",readbuffer[j]);
            }
        printk("\n");
        addres = addres + 0x0008;
        }
        DEBUG_LOG("<<<<<<<PROXIMITY REGISTER SQE READ-----\n");
    }
    
    if(proximity_cdc_pfs.id == 4){
        PROX_CDC_IOCTL_Enable();
    }
    if(proximity_cdc_pfs.id == 5){
        PROX_CDC_IOCTL_Disable();
    }
    if(proximity_cdc_pfs.id == 6){
        PROX_CDC_REG_Initialize();
    }
    if(proximity_cdc_pfs.id == 7){
        PROX_CDC_REG_Recalibration();
    }
    if(proximity_cdc_pfs.id == 8){
        PROX_CDC_IOCTL_ambient_read((unsigned short)proximity_cdc_pfs.par[0], readbuffer);
        DEBUG_LOG("ambient = %04X",readbuffer[0]);
    }
    if(proximity_cdc_pfs.id == 9){
        PROX_CDC_IOCTL_ambient_reset();
    }
    if(proximity_cdc_pfs.id == 10){
        PROX_CDC_IOCTL_pos_threshold_read((unsigned short)proximity_cdc_pfs.par[0], readbuffer);
        DEBUG_LOG("pos_threshold = %04X",readbuffer[0]);
    }
    if(proximity_cdc_pfs.id == 11){
        PROX_CDC_IOCTL_neg_threshold_read((unsigned short)proximity_cdc_pfs.par[0], readbuffer);
        DEBUG_LOG("neg_threshold = %04X",readbuffer[0]);
    }
    if(proximity_cdc_pfs.id == 12){
        PROX_CDC_IOCTL_pos_threshold_sens_write((unsigned short)proximity_cdc_pfs.par[0], (unsigned short)proximity_cdc_pfs.par[1]);
    }
    if(proximity_cdc_pfs.id == 13){
        PROX_CDC_IOCTL_neg_threshold_sens_write((unsigned short)proximity_cdc_pfs.par[0], (unsigned short)proximity_cdc_pfs.par[1]);
    }
    if(proximity_cdc_pfs.id == 14){
        PROX_CDC_IOCTL_stage_read_data((unsigned short)proximity_cdc_pfs.par[0], readbuffer);
        DEBUG_LOG("stage_read_data = %04X",readbuffer[0]);
    }
    
    if(proximity_cdc_pfs.id == 15){
        PROX_CDC_dump_start(proximity_cdc_pfs.par[0]);
    }
    
    FUNC_FIN_LOG();
    
    return count;
}
#endif /* CONFIG_ANDROID_ENGINEERING */

/* ------------------------------------------------------------------------- */
/* PROX_CDC_check_exist                                                      */
/* ------------------------------------------------------------------------- */
static int  PROX_CDC_check_exist(void)
{
    int ret;
    unsigned short read_data = 0;
    
    if (prox_cdc_dev_exist == PROX_CDC_DEV_NO_CHECK) {
        ret = PROX_CDC_i2c_read(AD7146_REG_DEVICE_ID, &read_data);
        if (ret != SH_PROXIMITY_CDC_RESULT_SUCCESS) {
            prox_cdc_dev_exist = PROX_CDC_DEV_NO_EXIST;
        } else {
            prox_cdc_dev_exist = PROX_CDC_DEV_EXIST;
        }
    }
    
    if (prox_cdc_dev_exist == PROX_CDC_DEV_NO_EXIST) {
        DEBUG_LOG("<WARNING> Proximity_CDC device is not connected.");
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }
    
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_Init                                                             */
/* ------------------------------------------------------------------------- */

static int __init PROX_CDC_Init(void)
{
    int ret;
    int mode = 0;
#if defined (CONFIG_ANDROID_ENGINEERING)
    struct proc_dir_entry *entry;
#endif /* CONFIG_ANDROID_ENGINEERING */
    
    FUNC_LOG();
    
    wake_lock_init(&prox_cdc_timeout_wake_lock, WAKE_LOCK_SUSPEND, "prox_cdc_timeout_wake_lock");
    wake_lock_init(&prox_cdc_wake_lock, WAKE_LOCK_SUSPEND, "prox_cdc_wake_lock");
    
    pm_qos_add_request(&prox_cdc_qos_cpu_dma_latency, PM_QOS_CPU_DMA_LATENCY, PM_QOS_DEFAULT_VALUE);

    prox_cdc_hw_revision = sh_boot_get_hw_revision();
    PROX_CDC_get_smem_data();
    
    mode = sh_boot_get_bootmode();
    if( mode == 0 ) {
        /* Error */
        DEBUG_ERROR_LOG("Error : sh_boot_get_bootmode");
        prox_cdc_smem_flg = PROX_CDC_BOOT_DIAG;
    }else if( (mode == SH_BOOT_D) || (mode == SH_BOOT_F_F) ) {
        prox_cdc_smem_flg = PROX_CDC_BOOT_DIAG;
    } else {
        prox_cdc_smem_flg = PROX_CDC_BOOT_NORMAL;
    }
    
    ret = i2c_add_driver(&prox_cdc_driver);
    if (ret < 0) {
        DEBUG_ERROR_LOG("<RESULT_FAILURE> i2c_add_driver(&prox_cdc_driver). [ret = %d]", ret);
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }

    mutex_init(&prox_cdc_lock);
    spin_lock_init(&prox_cdc_spinlock);

#if defined (CONFIG_ANDROID_ENGINEERING)
    entry = create_proc_entry("driver/PROXIMITY_CDC", 0666, NULL); 
    
    if (entry == NULL) {
        return SH_PROXIMITY_CDC_RESULT_FAILURE;
    }

    entry->write_proc = PROX_CDC_proc_write;
#endif /* CONFIG_ANDROID_ENGINEERING */
    
    prox_cdc_wq = create_singlethread_workqueue("prox_cdc_wq");
    INIT_WORK(&prox_cdc_wk, PROX_CDC_workqueue_handler);
    
#ifdef CONFIG_ANDROID_ENGINEERING
    INIT_WORK(&prox_cdc_th_work_data, PROX_CDC_th_dump_func);
    
    hrtimer_init(&prox_cdc_threshold_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    prox_cdc_threshold_timer.function = PROX_CDC_th_timer_callback;
#endif /* CONFIG_ANDROID_ENGINEERING */
    
    FUNC_FIN_LOG();
    return SH_PROXIMITY_CDC_RESULT_SUCCESS;
}

/* ------------------------------------------------------------------------- */
/* PROX_CDC_Exit                                                             */
/* ------------------------------------------------------------------------- */

static void __exit PROX_CDC_Exit(void)
{
    FUNC_LOG();
    
    if(prox_cdc_wq) {
        flush_workqueue(prox_cdc_wq);
        destroy_workqueue(prox_cdc_wq);
        prox_cdc_wq = NULL;
    }
    
    pm_qos_remove_request(&prox_cdc_qos_cpu_dma_latency);
    
    wake_unlock(&prox_cdc_wake_lock);
    wake_lock_destroy(&prox_cdc_wake_lock);
    wake_unlock(&prox_cdc_timeout_wake_lock);
    wake_lock_destroy(&prox_cdc_timeout_wake_lock);
    
    i2c_del_driver(&prox_cdc_driver);
    free_irq(gpio_to_irq(PROX_CDC_INT_NUM), 0);
    
    gpio_free(PROX_CDC_INT_NUM);
    
    FUNC_FIN_LOG();
    PROX_CDC_Remove();
}

module_init(PROX_CDC_Init);
module_exit(PROX_CDC_Exit);

MODULE_DESCRIPTION("proximity sensor driver");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("SHARP CORPORATION");
MODULE_VERSION("1.0");

/* ------------------------------------------------------------------------- */
/* END OF FILE                                                               */
/* ------------------------------------------------------------------------- */
