/*
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2008-2011, Code Aurora Forum. All rights reserved.
 * Author: Brian Swetland <swetland@google.com>
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

#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/bootmem.h>
#include <linux/power_supply.h>


#include <mach/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/flash.h>
#include <asm/setup.h>
#ifdef CONFIG_CACHE_L2X0
#include <asm/hardware/cache-l2x0.h>
#endif

#include <asm/mach/mmc.h>
#include <mach/vreg.h>
#include <mach/mpp.h>
#include <mach/board.h>
#include <mach/pmic.h>
#include <mach/msm_iomap.h>
#include <mach/msm_rpcrouter.h>
#include <mach/msm_hsusb.h>
#include <mach/rpc_hsusb.h>
#include <mach/rpc_pmapp.h>
#include <mach/msm_serial_hs.h>
#include <mach/memory.h>
#include <mach/msm_battery.h>
#include <mach/rpc_server_handset.h>
#include <mach/msm_tsif.h>
#include <mach/socinfo.h>
#include <mach/pmic.h>	//Teddy

#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>	// FEATURE_M7LAB
#include <linux/android_pmem.h>
#include <mach/camera.h>

#include "devices.h"
#include "clock.h"
#include "msm-keypad-devices.h"
#include "pm.h"
#ifdef CONFIG_ARCH_MSM7X27
#include <linux/msm_kgsl.h>
#endif

#ifdef CONFIG_USB_ANDROID
#include <linux/usb/android_composite.h>
#endif

#include "smd_private.h"	//	Teddy 2011-08-29
//leesh_sensor
#ifdef CONFIG_M7LAB_VENUS_SENSOR
#include <linux/bma222.h>
#include <linux/akm8963.h>
#include <linux/i2c-gpio.h>
#endif
//leesh_sensor

#ifdef CONFIG_M7LAB_VENUS_VIBRATOR	//	Teddy
//extern void msm_init_pmic_vibrator(void); //by peng.xiong
#endif	//	CONFIG_M7LAB_VENUS_VIBRATOR

#ifdef CONFIG_ARCH_MSM7X25
#define MSM_PMEM_MDP_SIZE	0xb21000
#define MSM_PMEM_ADSP_SIZE	0x97b000
#define MSM_PMEM_AUDIO_SIZE	0x121000
#define MSM_FB_SIZE		0x200000
#define PMEM_KERNEL_EBI1_SIZE	0x64000
#endif

#ifdef CONFIG_ARCH_MSM7X27
#define MSM_PMEM_MDP_SIZE	0x1B76000
#define MSM_PMEM_ADSP_SIZE	0xB71000
#define MSM_PMEM_AUDIO_SIZE	0x5B000
#define MSM_FB_SIZE		0x177000
#define PMEM_KERNEL_EBI1_SIZE	0x1C000
#endif
unsigned int lcd_id = 0;
static struct resource smc91x_resources[] = {
	[0] = {
		.start	= 0x9C004300,
		.end	= 0x9C0043ff,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= MSM_GPIO_TO_INT(132),
		.end	= MSM_GPIO_TO_INT(132),
		.flags	= IORESOURCE_IRQ,
	},
};

#ifdef CONFIG_USB_FUNCTION
static struct usb_mass_storage_platform_data usb_mass_storage_pdata = {
	.nluns          = 0x02,
	.buf_size       = 16384,
	.vendor         = "GOOGLE",
	.product        = "Mass storage",
	.release        = 0xffff,
};

static struct platform_device mass_storage_device = {
	.name           = "usb_mass_storage",
	.id             = -1,
	.dev            = {
		.platform_data          = &usb_mass_storage_pdata,
	},
};
#endif
#ifdef CONFIG_USB_ANDROID
#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-07-08
static char *usb_functions_default_adb[] = {
	"diag",
	"adb",
	"nmea",
	"usb_mass_storage",
};

static char *usb_functions_default[] = {
	"diag",
	"nmea",
	"usb_mass_storage",
};

static char *usb_functions_ums_adb[] = {
	"adb",
	"usb_mass_storage",
};

static char *usb_functions_ums[] = {
	"usb_mass_storage",
};
//[Bug] 231953 Protocol2 Tao Chenli 2012-03-23 begin 
static char *usb_functions_rndis[] = {
	"rndis",
};

static char *usb_functions_rndis_adb[] = {
	"rndis",
	"adb",
};
//[Bug] 231953 Protocol2 Tao Chenli 2012-03-23 end 
#else
static char *usb_functions_default[] = {
	"diag",
	"modem",
	"nmea",
	"rmnet",
	"usb_mass_storage",
};

static char *usb_functions_default_adb[] = {
	"diag",
	"adb",
	"modem",
	"nmea",
	"rmnet",
	"usb_mass_storage",
};

static char *usb_functions_rndis[] = {
	"rndis",
};

static char *usb_functions_rndis_adb[] = {
	"rndis",
	"adb",
};
#endif	//	CONFIG_M7LAB_JUKE_USB

static char *usb_functions_all[] = {
#ifdef CONFIG_USB_ANDROID_RNDIS
	"rndis",
#endif
#ifdef CONFIG_USB_ANDROID_DIAG
	"diag",
#endif
	"adb",
#ifdef CONFIG_USB_F_SERIAL
	"modem",
	"nmea",
#endif
#ifdef CONFIG_USB_ANDROID_RMNET
	"rmnet",
#endif
	"usb_mass_storage",
#ifdef CONFIG_USB_ANDROID_ACM
	"acm",
#endif
};

static struct android_usb_product usb_products[] = {
#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-07-04
	{
		.product_id	= 0x00F6,
		.num_functions	= ARRAY_SIZE(usb_functions_default),
		.functions	= usb_functions_default,
	},
	{
		.product_id	= 0x00F5,
		.num_functions	= ARRAY_SIZE(usb_functions_default_adb),
		.functions	= usb_functions_default_adb,
	},
	{
		.product_id	= 0xD00D,
		.num_functions	= ARRAY_SIZE(usb_functions_ums),
		.functions	= usb_functions_ums,
	},
	{
		.product_id	= 0x00F7,
		.num_functions	= ARRAY_SIZE(usb_functions_ums_adb),
		.functions	= usb_functions_ums_adb,
	},
	{  //[Bug] 231953 Protocol2 Tao Chenli 2012-03-23 begin 
		.product_id	= 0x00FA,
		.num_functions	= ARRAY_SIZE(usb_functions_rndis),
		.functions	= usb_functions_rndis,
	},
	{
		.product_id	= 0x00FB,
		.num_functions	= ARRAY_SIZE(usb_functions_rndis_adb),
		.functions	= usb_functions_rndis_adb,
	},//[Bug] 231953 Protocol2 Tao Chenli 2012-03-23 end 
#else
	{
		.product_id	= 0x9026,
		.num_functions	= ARRAY_SIZE(usb_functions_default),
		.functions	= usb_functions_default,
	},
	{
		.product_id	= 0x9025,
		.num_functions	= ARRAY_SIZE(usb_functions_default_adb),
		.functions	= usb_functions_default_adb,
	},
	{
		.product_id	= 0xf00e,
		.num_functions	= ARRAY_SIZE(usb_functions_rndis),
		.functions	= usb_functions_rndis,
	},
	{
		.product_id	= 0x9024,
		.num_functions	= ARRAY_SIZE(usb_functions_rndis_adb),
		.functions	= usb_functions_rndis_adb,
	},
#endif	//	CONFIG_M7LAB_JUKE_USB
};

#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-10-07
static struct android_usb_product usb_products_npi[] = {
	{
		.product_id	= 0x00F6,
		.num_functions	= ARRAY_SIZE(usb_functions_default),
		.functions	= usb_functions_default,
	},
	{
		.product_id	= 0x00F5,
		.num_functions	= ARRAY_SIZE(usb_functions_default_adb),
		.functions	= usb_functions_default_adb,
	},
	{
		.product_id	= 0xD00D,
		.num_functions	= ARRAY_SIZE(usb_functions_ums),
		.functions	= usb_functions_ums,
	},
	{
		.product_id	= 0x00F7,
		.num_functions	= ARRAY_SIZE(usb_functions_ums_adb),
		.functions	= usb_functions_ums_adb,
	},
};
#endif	//	CONFIG_M7LAB_JUKE_USB

static struct usb_mass_storage_platform_data mass_storage_pdata = {
	.nluns		= 1,
#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-07-14
	.vendor		= "Alcatel onetouch",
	.product        = "USB Mass storage",
#else
	.vendor		= "Qualcomm Incorporated",
	.product        = "Mass storage",
#endif	//	CONFIG_M7LAB_JUKE_USB
	.release	= 0x0100,
	.can_stall	= 1,
};

static struct platform_device usb_mass_storage_device = {
	.name	= "usb_mass_storage",
	.id	= -1,
	.dev	= {
		.platform_data = &mass_storage_pdata,
	},
};

static struct usb_ether_platform_data rndis_pdata = {
	/* ethaddr is filled by board_serialno_setup */
#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-07-11
	.vendorID	= 0x1BBB,
	.vendorDescr	= "",
#else
	.vendorID	= 0x05C6,
	.vendorDescr	= "Qualcomm Incorporated",
#endif	//	CONFIG_M7LAB_JUKE_USB
};

static struct platform_device rndis_device = {
	.name	= "rndis",
	.id	= -1,
	.dev	= {
		.platform_data = &rndis_pdata,
	},
};

static struct android_usb_platform_data android_usb_pdata = {
#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-07-04
	.vendor_id	= 0x1BBB,
	.product_id	= 0x00F6,	//	Teddy 2011-10-04 change 0xD00D=>0x00F6
	.version	= 0x0100,
	.product_name		= "Alcatel onetouch",
	.manufacturer_name	= "Alcatel onetouch",
#else
	.vendor_id	= 0x05C6,
	.product_id	= 0x9026,
	.version	= 0x0100,
	.product_name		= "Qualcomm HSUSB Device",
	.manufacturer_name	= "Qualcomm Incorporated",
#endif	//	CONFIG_M7LAB_JUKE_USB
	.num_products = ARRAY_SIZE(usb_products),
	.products = usb_products,
	.num_functions = ARRAY_SIZE(usb_functions_all),
	.functions = usb_functions_all,
	.serial_number = "1234567890ABCDEF",
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id		= -1,
	.dev		= {
		.platform_data = &android_usb_pdata,
	},
};

static int __init board_serialno_setup(char *serialno)
{
	int i;
	char *src = serialno;

	/* create a fake MAC address from our serial number.
	 * first byte is 0x02 to signify locally administered.
	 */
	rndis_pdata.ethaddr[0] = 0x02;
	for (i = 0; *src; i++) {
		/* XOR the USB serial across the remaining bytes */
		rndis_pdata.ethaddr[i % (ETH_ALEN - 1) + 1] ^= *src++;
	}

	android_usb_pdata.serial_number = serialno;
	return 1;
}
__setup("androidboot.serialno=", board_serialno_setup);
#endif

static struct platform_device smc91x_device = {
	.name		= "smc91x",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(smc91x_resources),
	.resource	= smc91x_resources,
};

#ifdef CONFIG_USB_FUNCTION
static struct usb_function_map usb_functions_map[] = {
	{"diag", 0},
	{"adb", 1},
	{"modem", 2},
	{"nmea", 3},
	{"mass_storage", 4},
	{"ethernet", 5},
	{"rmnet", 6},
};

/* dynamic composition */
static struct usb_composition usb_func_composition[] = {
	{
		.product_id         = 0x9012,
		.functions	    = 0x5, /* 0101 */
	},

	{
		.product_id         = 0x9013,
		.functions	    = 0x15, /* 10101 */
	},

	{
		.product_id         = 0x9014,
		.functions	    = 0x30, /* 110000 */
	},

	{
		.product_id         = 0x9016,
		.functions	    = 0xD, /* 01101 */
	},

	{
		.product_id         = 0x9017,
		.functions	    = 0x1D, /* 11101 */
	},

	{
		.product_id         = 0xF000,
		.functions	    = 0x10, /* 10000 */
	},

	{
		.product_id         = 0xF009,
		.functions	    = 0x20, /* 100000 */
	},

	{
		.product_id         = 0x9018,
		.functions	    = 0x1F, /* 011111 */
	},
#ifdef CONFIG_USB_FUNCTION_RMNET
	{
		.product_id         = 0x9021,
		/* DIAG + RMNET */
		.functions	    = 0x41,
	},
	{
		.product_id         = 0x9022,
		/* DIAG + ADB + RMNET */
		.functions	    = 0x43,
	},
#endif

};

static struct msm_hsusb_platform_data msm_hsusb_pdata = {
	.version	= 0x0100,
	.phy_info	= (USB_PHY_INTEGRATED | USB_PHY_MODEL_65NM),
	.vendor_id          = 0x5c6,
	.product_name       = "Qualcomm HSUSB Device",
	.serial_number      = "1234567890ABCDEF",
	.manufacturer_name  = "Qualcomm Incorporated",
	.compositions	= usb_func_composition,
	.num_compositions = ARRAY_SIZE(usb_func_composition),
	.function_map   = usb_functions_map,
	.num_functions	= ARRAY_SIZE(usb_functions_map),
	.config_gpio    = NULL,
};
#endif

#ifdef CONFIG_USB_EHCI_MSM
static void msm_hsusb_vbus_power(unsigned phy_info, int on)
{
	if (on)
		msm_hsusb_vbus_powerup();
	else
		msm_hsusb_vbus_shutdown();
}

static struct msm_usb_host_platform_data msm_usb_host_pdata = {
	.phy_info       = (USB_PHY_INTEGRATED | USB_PHY_MODEL_65NM),
};

static void __init msm7x2x_init_host(void)
{
	if (machine_is_msm7x25_ffa() || machine_is_msm7x27_ffa())
		return;

	msm_add_host(0, &msm_usb_host_pdata);
}
#endif

#ifdef CONFIG_USB_MSM_OTG_72K
static int hsusb_rpc_connect(int connect)
{
	if (connect)
		return msm_hsusb_rpc_connect();
	else
		return msm_hsusb_rpc_close();
}
#endif

#ifdef CONFIG_USB_MSM_OTG_72K
struct vreg *vreg_3p3;
static int msm_hsusb_ldo_init(int init)
{
	if (init) {
		/*
		 * PHY 3.3V analog domain(VDDA33) is powered up by
		 * an always enabled power supply (LP5900TL-3.3).
		 * USB VREG default source is VBUS line. Turning
		 * on USB VREG has a side effect on the USB suspend
		 * current. Hence USB VREG is explicitly turned
		 * off here.
		 */
		vreg_3p3 = vreg_get(NULL, "usb");
		if (IS_ERR(vreg_3p3))
			return PTR_ERR(vreg_3p3);
		vreg_enable(vreg_3p3);
		vreg_disable(vreg_3p3);
		vreg_put(vreg_3p3);
	}

	return 0;
}

static int msm_hsusb_pmic_notif_init(void (*callback)(int online), int init)
{
	int ret;

	if (init) {
		ret = msm_pm_app_rpc_init(callback);
	} else {
		msm_pm_app_rpc_deinit(callback);
		ret = 0;
	}
	return ret;
}

static int msm_otg_rpc_phy_reset(void __iomem *regs)
{
	return msm_hsusb_phy_reset();
}

static struct msm_otg_platform_data msm_otg_pdata = {
	.rpc_connect	= hsusb_rpc_connect,
	.pmic_vbus_notif_init         = msm_hsusb_pmic_notif_init,
	.chg_vbus_draw		 = hsusb_chg_vbus_draw,
	.chg_connected		 = hsusb_chg_connected,
	.chg_init		 = hsusb_chg_init,
#ifdef CONFIG_USB_EHCI_MSM
	.vbus_power = msm_hsusb_vbus_power,
#endif
	.ldo_init		= msm_hsusb_ldo_init,
	.pclk_required_during_lpm = 1,
	.pclk_src_name		= "ebi1_usb_clk",
};

#ifdef CONFIG_USB_GADGET
static struct msm_hsusb_gadget_platform_data msm_gadget_pdata;
#endif
#endif

#define SND(desc, num) { .name = #desc, .id = num }
static struct snd_endpoint snd_endpoints_list[] = {
	SND(HANDSET, 0),
	SND(HEADSET_LOOPBACK, 1),//gtsystem_S_jongwoo.park_2011-06-22_headset loppback*/
	SND(MONO_HEADSET, 2),
	SND(HEADSET, 3),
	SND(HANDSET_LOOPBACK, 5),//gtsystem_S_jongwoo.park_2011-06-22_handset loppback*/
	SND(SPEAKER, 6),
	SND(TTY_HEADSET, 8),
	// gtsystem_S_jongwoo.park_2011-06-14 set dual path during playing ringtone
	SND(HEADSET_AND_SPEAKER, 7),
	SND(TTY_VCO, 9),
	SND(TTY_HCO, 10),
	SND(BT, 12),
	SND(IN_S_SADC_OUT_HANDSET, 16),
	SND(IN_S_SADC_OUT_SPEAKER_PHONE, 25),
	SND(CURRENT, 27),
	SND(HEADSET_MEDIA, 31), //	GTSystem yjkim_audio_path 20110822
	SND(SPEAKER_MEDIA, 26), //	GTSystem yjkim_audio_path 20110822
	SND(STEREO_HEADSET_FM, 34),//zhangzhenjun for FM
	SND(SPEAKER_PHONE_FM, 35),
};
#undef SND

static struct msm_snd_endpoints msm_device_snd_endpoints = {
	.endpoints = snd_endpoints_list,
	.num = sizeof(snd_endpoints_list) / sizeof(struct snd_endpoint)
};

static struct platform_device msm_device_snd = {
	.name = "msm_snd",
	.id = -1,
	.dev    = {
		.platform_data = &msm_device_snd_endpoints
	},
};

#define DEC0_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#ifdef CONFIG_ARCH_MSM7X25
#define DEC1_FORMAT ((1<<MSM_ADSP_CODEC_WAV)|(1<<MSM_ADSP_CODEC_ADPCM)| \
	(1<<MSM_ADSP_CODEC_YADPCM)|(1<<MSM_ADSP_CODEC_QCELP)| \
	(1<<MSM_ADSP_CODEC_MP3))
#define DEC2_FORMAT ((1<<MSM_ADSP_CODEC_WAV)|(1<<MSM_ADSP_CODEC_ADPCM)| \
	(1<<MSM_ADSP_CODEC_YADPCM)|(1<<MSM_ADSP_CODEC_QCELP)| \
	(1<<MSM_ADSP_CODEC_MP3))
#define DEC3_FORMAT 0
#define DEC4_FORMAT 0
#else
#define DEC1_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC2_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC3_FORMAT ((1<<MSM_ADSP_CODEC_MP3)| \
	(1<<MSM_ADSP_CODEC_AAC)|(1<<MSM_ADSP_CODEC_WMA)| \
	(1<<MSM_ADSP_CODEC_WMAPRO)|(1<<MSM_ADSP_CODEC_AMRWB)| \
	(1<<MSM_ADSP_CODEC_AMRNB)|(1<<MSM_ADSP_CODEC_WAV)| \
	(1<<MSM_ADSP_CODEC_ADPCM)|(1<<MSM_ADSP_CODEC_YADPCM)| \
	(1<<MSM_ADSP_CODEC_EVRC)|(1<<MSM_ADSP_CODEC_QCELP))
#define DEC4_FORMAT (1<<MSM_ADSP_CODEC_MIDI)
#endif

static unsigned int dec_concurrency_table[] = {
	/* Audio LP */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DMA)), 0,
	0, 0, 0,

	/* Concurrency 1 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	 /* Concurrency 2 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 3 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 4 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 5 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_TUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),

	/* Concurrency 6 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	0, 0, 0, 0,

	/* Concurrency 7 */
	(DEC0_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC1_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC2_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC3_FORMAT|(1<<MSM_ADSP_MODE_NONTUNNEL)|(1<<MSM_ADSP_OP_DM)),
	(DEC4_FORMAT),
};

#define DEC_INFO(name, queueid, decid, nr_codec) { .module_name = name, \
	.module_queueid = queueid, .module_decid = decid, \
	.nr_codec_support = nr_codec}

static struct msm_adspdec_info dec_info_list[] = {
	DEC_INFO("AUDPLAY0TASK", 13, 0, 11), /* AudPlay0BitStreamCtrlQueue */
#ifdef CONFIG_ARCH_MSM7X25
	DEC_INFO("AUDPLAY1TASK", 14, 1, 5),  /* AudPlay1BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY2TASK", 15, 2, 5),  /* AudPlay2BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY3TASK", 16, 3, 0),  /* AudPlay3BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY4TASK", 17, 4, 0),  /* AudPlay4BitStreamCtrlQueue */
#else
	DEC_INFO("AUDPLAY1TASK", 14, 1, 11),  /* AudPlay1BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY2TASK", 15, 2, 11),  /* AudPlay2BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY3TASK", 16, 3, 11),  /* AudPlay3BitStreamCtrlQueue */
	DEC_INFO("AUDPLAY4TASK", 17, 4, 1),  /* AudPlay4BitStreamCtrlQueue */
#endif
};

static struct msm_adspdec_database msm_device_adspdec_database = {
	.num_dec = ARRAY_SIZE(dec_info_list),
	.num_concurrency_support = (ARRAY_SIZE(dec_concurrency_table) / \
					ARRAY_SIZE(dec_info_list)),
	.dec_concurrency_table = dec_concurrency_table,
	.dec_info_list = dec_info_list,
};

static struct platform_device msm_device_adspdec = {
	.name = "msm_adspdec",
	.id = -1,
	.dev    = {
		.platform_data = &msm_device_adspdec_database
	},
};

static struct android_pmem_platform_data android_pmem_kernel_ebi1_pdata = {
	.name = PMEM_KERNEL_EBI1_DATA_NAME,
	/* if no allocator_type, defaults to PMEM_ALLOCATORTYPE_BITMAP,
	 * the only valid choice at this time. The board structure is
	 * set to all zeros by the C runtime initialization and that is now
	 * the enum value of PMEM_ALLOCATORTYPE_BITMAP, now forced to 0 in
	 * include/linux/android_pmem.h.
	 */
	.cached = 0,
};

static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 1,
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
	.name = "pmem_adsp",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
};

static struct android_pmem_platform_data android_pmem_audio_pdata = {
	.name = "pmem_audio",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
};

static struct platform_device android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = { .platform_data = &android_pmem_pdata },
};

static struct platform_device android_pmem_adsp_device = {
	.name = "android_pmem",
	.id = 1,
	.dev = { .platform_data = &android_pmem_adsp_pdata },
};

static struct platform_device android_pmem_audio_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &android_pmem_audio_pdata },
};

static struct platform_device android_pmem_kernel_ebi1_device = {
	.name = "android_pmem",
	.id = 4,
	.dev = { .platform_data = &android_pmem_kernel_ebi1_pdata },
};

static struct msm_handset_platform_data hs_platform_data = {
	.hs_name = "7k_handset",
	.pwr_key_delay_ms = 500, /* 0 will disable end key */
};

static struct platform_device hs_device = {
	.name   = "msm-handset",
	.id     = -1,
	.dev    = {
		.platform_data = &hs_platform_data,
	},
};

/* Added by yubin,  for aeneas home key, begin */
#include <linux/gpio_keys.h>
#define HOME_KEY_GPIO		41

static struct gpio_keys_button gpio_keys_buttons[] = {
	{
		.code           = KEY_HOME,
		.gpio           = HOME_KEY_GPIO,
		.desc           = "home_key",
		.active_low     = 1,
		.type		= EV_KEY,
		.wakeup		= 1
	},
};

static struct gpio_keys_platform_data gpio_keys_data = {
	.buttons        = gpio_keys_buttons,
	.nbuttons       = ARRAY_SIZE(gpio_keys_buttons),
	.rep		= 0,
};

static struct platform_device msm_gpio_keys = {
	.name           = "gpio-keys",
	.id             = -1,
	.dev            = {
		.platform_data  = &gpio_keys_data,
	},
};
/* Added by yubin,  for aeneas home key, end */

/* TSIF begin */
#if defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE)

#define TSIF_B_SYNC      GPIO_CFG(87, 5, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)
#define TSIF_B_DATA      GPIO_CFG(86, 3, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)
#define TSIF_B_EN        GPIO_CFG(85, 3, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)
#define TSIF_B_CLK       GPIO_CFG(84, 4, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA)

static const struct msm_gpio tsif_gpios[] = {
	{ .gpio_cfg = TSIF_B_CLK,  .label =  "tsif_clk", },
	{ .gpio_cfg = TSIF_B_EN,   .label =  "tsif_en", },
	{ .gpio_cfg = TSIF_B_DATA, .label =  "tsif_data", },
	{ .gpio_cfg = TSIF_B_SYNC, .label =  "tsif_sync", },
};

static struct msm_tsif_platform_data tsif_platform_data = {
	.num_gpios = ARRAY_SIZE(tsif_gpios),
	.gpios = tsif_gpios,
	.tsif_clk = "tsif_clk",
	.tsif_pclk = "tsif_pclk",
	.tsif_ref_clk = "tsif_ref_clk",
};
#endif /* defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE) */
/* TSIF end   */

#define LCDC_CONFIG_PROC          21
#define LCDC_UN_CONFIG_PROC       22
#define LCDC_API_PROG             0x30000066
#define LCDC_API_VERS             0x00010001

#define GPIO_OUT_132    132
#define GPIO_OUT_131    131
#define GPIO_OUT_103    103
#define GPIO_OUT_102    102
#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
#else
#define GPIO_OUT_88     88
#endif

static struct msm_rpc_endpoint *lcdc_ep;

static int msm_fb_lcdc_config(int on)
{
	int rc = 0;
	struct rpc_request_hdr hdr;

	if (on)
		pr_info("lcdc config\n");
	else
		pr_info("lcdc un-config\n");

	lcdc_ep = msm_rpc_connect_compatible(LCDC_API_PROG, LCDC_API_VERS, 0);
	if (IS_ERR(lcdc_ep)) {
		printk(KERN_ERR "%s: msm_rpc_connect failed! rc = %ld\n",
			__func__, PTR_ERR(lcdc_ep));
		return -EINVAL;
	}

	rc = msm_rpc_call(lcdc_ep,
				(on) ? LCDC_CONFIG_PROC : LCDC_UN_CONFIG_PROC,
				&hdr, sizeof(hdr),
				5 * HZ);
	if (rc)
		printk(KERN_ERR
			"%s: msm_rpc_call failed! rc = %d\n", __func__, rc);

	msm_rpc_close(lcdc_ep);
	return rc;
}

static int gpio_array_num[] = {
				GPIO_OUT_132, /* spi_clk */
				GPIO_OUT_131, /* spi_cs  */
				GPIO_OUT_103, /* spi_sdi */
#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
#else
				GPIO_OUT_102, /* spi_sdoi */
				GPIO_OUT_88
#endif
				};

#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
static void lcdc_ips1p5700_gpio_init(void)
{
	if (gpio_request(GPIO_OUT_132, "spi_clk"))
		pr_err("failed to request gpio spi_clk\n");
	if (gpio_request(GPIO_OUT_131, "spi_cs"))
		pr_err("failed to request gpio spi_cs\n");
	if (gpio_request(GPIO_OUT_103, "spi_sdi"))
		pr_err("failed to request gpio spi_sdi\n");
#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA	//	Teddy 2011-04-12
#else
	if (gpio_request(GPIO_OUT_102, "spi_sdoi"))
		pr_err("failed to request gpio spi_sdoi\n");
#endif	//	CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
}

static uint32_t lcdc_gpio_table[] = {
	GPIO_CFG(GPIO_OUT_132, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_OUT_131, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_OUT_103, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA	//	Teddy 2011-04-12
#else
	GPIO_CFG(GPIO_OUT_102, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#endif	//	CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
};

void config_lcdc_gpio_table(uint32_t *table, int len, unsigned enable)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n],
			enable ? GPIO_CFG_ENABLE : GPIO_CFG_DISABLE);
		if (rc) {
			printk(KERN_ERR "%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}

/*
static void lcdc_ips1p5700_config_gpios(int enable)
{
	config_lcdc_gpio_table(lcdc_gpio_table,
		ARRAY_SIZE(lcdc_gpio_table), enable);
}
*/
	
#else
static void lcdc_gordon_gpio_init(void)
{
	if (gpio_request(GPIO_OUT_132, "spi_clk"))
		pr_err("failed to request gpio spi_clk\n");
	if (gpio_request(GPIO_OUT_131, "spi_cs"))
		pr_err("failed to request gpio spi_cs\n");
	if (gpio_request(GPIO_OUT_103, "spi_sdi"))
		pr_err("failed to request gpio spi_sdi\n");
//	if (gpio_request(GPIO_OUT_102, "spi_sdoi"))
//		pr_err("failed to request gpio spi_sdoi\n");
//	if (gpio_request(GPIO_OUT_88, "gpio_dac"))
//		pr_err("failed to request gpio_dac\n");
}

static uint32_t lcdc_gpio_table[] = {
	GPIO_CFG(GPIO_OUT_132, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_OUT_131, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_OUT_103, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
//	GPIO_CFG(GPIO_OUT_102, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
//	GPIO_CFG(GPIO_OUT_88,  0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
};

static void config_lcdc_gpio_table(uint32_t *table, int len, unsigned enable)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n],
			enable ? GPIO_CFG_ENABLE : GPIO_CFG_DISABLE);
		if (rc) {
			printk(KERN_ERR "%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}

static void lcdc_gordon_config_gpios(int enable)
{
	config_lcdc_gpio_table(lcdc_gpio_table,
		ARRAY_SIZE(lcdc_gpio_table), enable);
}
#endif

#ifndef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
static char *msm_fb_lcdc_vreg[] = {
	"gp5"
};
#endif

static int msm_fb_lcdc_power_save(int on)
{
#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
	return 0;
#else
	struct vreg *vreg[ARRAY_SIZE(msm_fb_lcdc_vreg)];
	int i, rc = 0;

	for (i = 0; i < ARRAY_SIZE(msm_fb_lcdc_vreg); i++) {
		if (on) {
			vreg[i] = vreg_get(0, msm_fb_lcdc_vreg[i]);
			rc = vreg_enable(vreg[i]);
			if (rc) {
				printk(KERN_ERR "vreg_enable: %s vreg"
						"operation failed \n",
						msm_fb_lcdc_vreg[i]);
				goto bail;
			}
		} else {
			int tmp;
			vreg[i] = vreg_get(0, msm_fb_lcdc_vreg[i]);
			tmp = vreg_disable(vreg[i]);
			if (tmp) {
				printk(KERN_ERR "vreg_disable: %s vreg "
						"operation failed \n",
						msm_fb_lcdc_vreg[i]);
				if (!rc)
					rc = tmp;
			}
			tmp = gpio_tlmm_config(GPIO_CFG(GPIO_OUT_88, 0,
						GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL,
						GPIO_CFG_2MA), GPIO_CFG_ENABLE);
			if (tmp) {
				printk(KERN_ERR "gpio_tlmm_config failed\n");
				if (!rc)
					rc = tmp;
			}
			gpio_set_value(88, 0);
			mdelay(15);
			gpio_set_value(88, 1);
			mdelay(15);
		}
	}

	return rc;

bail:
	if (on) {
		for (; i > 0; i--)
			vreg_disable(vreg[i - 1]);
	}

	return rc;
#endif
}
static struct lcdc_platform_data lcdc_pdata = {
	.lcdc_gpio_config = msm_fb_lcdc_config,
	.lcdc_power_save   = msm_fb_lcdc_power_save,
};

static void lcdc_config_gpios(int enable)

{
	config_lcdc_gpio_table(lcdc_gpio_table,
		ARRAY_SIZE(lcdc_gpio_table), enable);
}

static struct msm_panel_common_pdata lcdc_panel_pdata = {
	.panel_config_gpio = lcdc_config_gpios,
	.gpio_num          = gpio_array_num,
};

static struct platform_device lcdc_panel_device = {
	//.name   = "lcdc_ips1p5700_qvga",
	//.name   = "lcdc_r61526_rgb",
	.name   = "lcdc_ili9486l_rgb",
	.id     = 0,
	.dev    = {
		.platform_data = &lcdc_panel_pdata,
	}
};

#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
#else
static struct msm_panel_common_pdata lcdc_gordon_panel_data = {
	.panel_config_gpio = lcdc_gordon_config_gpios,
	.gpio_num          = gpio_array_num,
};

static struct platform_device lcdc_gordon_panel_device = {
	.name   = "lcdc_gordon_vga",
	.id     = 0,
	.dev    = {
		.platform_data = &lcdc_gordon_panel_data,
	}
};
#endif

//add by peng.xiong begin
#define spi_clk	GPIO_OUT_132
#define spi_cs	GPIO_OUT_131
#define spi_sdi	GPIO_OUT_103

static void spi_write(unsigned char val, unsigned char index)
{
        int i,sdi;
        gpio_set_value(GPIO_OUT_103, index);
	gpio_set_value(GPIO_OUT_132, 0);
	udelay(0);
	gpio_set_value(GPIO_OUT_132, 1);
	udelay(0);
	for (i = 7; i >= 0; i--) {
		sdi = ((val >> i) & 0x1);
		gpio_set_value(GPIO_OUT_103, sdi);
		gpio_set_value(GPIO_OUT_132, 0);
		udelay(0);
		gpio_set_value(GPIO_OUT_132, 1);
		udelay(0);
	}

}

static int spi_read(void)
{
        int i,id =0;
        for(i=15;i>=0;i--){
		gpio_set_value(GPIO_OUT_132, 0);
		udelay(0);
		gpio_set_value(GPIO_OUT_132, 1);
		udelay(0);
	     if(i<=15)
	        id |= (gpio_get_value(GPIO_OUT_103)<<i);

	}
      return id;
}
//	

static int spi_write_hx8347h(unsigned char index, unsigned char val)
{
	unsigned char sda;	
	int i;
	unsigned char start_byte;
	gpio_set_value(spi_cs, 0);

//0x70: command; 0x72:data
	start_byte = index;//01110 0 0 0: 01110 not changed; 0: IM0; 0: RS, command; 0: RW, write to LCD
	for (i = 7; i >= 0; i--) {
		sda = ((start_byte >> i) & 0x1);
		gpio_set_value(spi_sdi, sda);
		gpio_set_value(spi_clk, 0);
		udelay(10);
		gpio_set_value(spi_clk, 1);
		udelay(10);
	}
	
	for (i = 7; i >= 0; i--) {
		sda = ((val >> i) & 0x1);
		gpio_set_value(spi_sdi, sda);
		gpio_set_value(spi_clk, 0);
		udelay(10);
		gpio_set_value(spi_clk, 1);
		udelay(10);
	}
	gpio_set_value(spi_cs, 1);

	return 0;
}


//spi write
static int spi_read_cmd(void)
{
	unsigned char sda;	
	int i, rc, tmp;
	unsigned int id = 0;
	unsigned char start_byte;
	gpio_set_value(spi_cs, 0);

	start_byte = 0x73;//01110 0 1 1: 01110 not changed; 0: IM0; 1: RS, index register; 1: RW, read from LCD
	for (i = 7; i >= 0; i--) {
		sda = ((start_byte >> i) & 0x1);
		gpio_set_value(spi_clk, 0);
		gpio_set_value(spi_sdi, sda);
		udelay(100);
		gpio_set_value(spi_clk, 1);
		udelay(100);
	}

//	GPIO_CFG(GPIO_OUT_103, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),

	//get the context of replying from lcd module.
	rc = gpio_tlmm_config(GPIO_CFG(spi_sdi,0, GPIO_CFG_INPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if(rc){
		printk(KERN_ERR "gpio_tlmm_config failed\n");
		return rc;
	}

        for(i=7;i>=0;i--){
		gpio_set_value(spi_clk, 0);
		 tmp=gpio_get_value(spi_sdi);
		udelay(100);
		gpio_set_value(spi_clk, 1);
		udelay(100);

		if(tmp==0)
			id|=0;
		else
			id|=(0x1 << i);
	}
//	printk(KERN_ERR"\n\n --------id is 0x%x--------------\n\n",id);
	gpio_set_value(spi_cs, 1);

	//get the context of replying from lcd module.
	rc = gpio_tlmm_config(GPIO_CFG(spi_sdi,0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	return id;
}
//add peng.xiong end

static struct resource msm_fb_resources[] = {
	{
		.flags  = IORESOURCE_DMA,
	}
};

static int msm_fb_detect_panel(const char *name)
{
	int ret = -EPERM;


	if (!strcmp(name, lcdc_panel_device.name))
		ret = 0;
	else
		ret = -ENODEV;


	return ret;
}

static int msm7x27_lcd_id_detect(void)
{
	int rc;
	int i;
	//int sdi;
	//unsigned int id = 0,cmd = 0;
    unsigned int id = 0;

	//config gpio pins of spi interface.
	lcdc_config_gpios(1);
	gpio_set_value(GPIO_OUT_132, 1);//clk
	gpio_set_value(GPIO_OUT_131, 1);//cs
	gpio_set_value(GPIO_OUT_103, 1);//sdi

	//reset lcd panel 
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));
	mdelay(3);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_HIGH));
	mdelay(100);

	//transform the inquire command of id.
	gpio_set_value(GPIO_OUT_131, 0);
        
       //R61526 need to set the 0xB0 command before we can read ID
        spi_write(0xB0,0);
        spi_write(0x3F,1);
        spi_write(0x3F,1);

        spi_write(0xBF,0);

	//get the context of replying from lcd module.
	rc = gpio_tlmm_config(GPIO_CFG(GPIO_OUT_103,0, GPIO_CFG_INPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if(rc){
		printk(KERN_ERR "gpio_tlmm_config failed\n");
		return rc;
	}

        //dummy read,and it is limited
        for(i=7;i>=0;i--){
		gpio_set_value(GPIO_OUT_132, 0);
		udelay(0);
		gpio_set_value(GPIO_OUT_132, 1);
		udelay(0);
	     if(i<=7)
	        id |= (gpio_get_value(GPIO_OUT_103)<<i);

	}

        //read 4 bytes to check ID
        id = spi_read();                //id = 0x0122
        lcd_id = spi_read()|(id<<16);   //id = 0x1526

        gpio_set_value(GPIO_OUT_131, 1);

	printk(KERN_ERR"\n\n---------------lcd module lcd id is 0x%x-----------\n\n",lcd_id);
	return lcd_id;
}

static int msm7x27_lcd_id_detect_hx8347h(void)
{
	//config gpio pins of spi interface.
	lcdc_config_gpios(1);

	//reset lcd panel 
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));
	mdelay(3);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_HIGH));
	mdelay(100);

       //hx8347h need to set the 0x00 command before we can read ID, 0x79

	gpio_set_value(spi_cs, 1);//cs
	udelay(10);
	gpio_set_value(spi_clk, 1);//clk
	gpio_set_value(spi_sdi, 1);//sdi
	udelay(10);
	
	gpio_set_value(spi_cs, 0);//cs

	spi_write_hx8347h(0x70,0x00);
	lcd_id = spi_read_cmd();		//lcd_id = 0x79

	printk(KERN_ERR"\n\nlcd module id is 0x%x\n\n",lcd_id);
	return lcd_id;
}

static struct msm_fb_platform_data msm_fb_pdata = {
	.detect_client = msm_fb_detect_panel,
	.mddi_prescan = 1,
};

static struct platform_device msm_fb_device = {
	.name   = "msm_fb",
	.id     = 0,
	.num_resources  = ARRAY_SIZE(msm_fb_resources),
	.resource       = msm_fb_resources,
	.dev    = {
		.platform_data = &msm_fb_pdata,
	}
};

#ifdef CONFIG_BT
static struct platform_device msm_bt_power_device = {
	.name = "bt_power",
};

enum {
	BT_WAKE,
	BT_RFR,
	BT_CTS,
	BT_RX,
	BT_TX,
	BT_PCM_DOUT,
	BT_PCM_DIN,
	BT_PCM_SYNC,
	BT_PCM_CLK,
	BT_HOST_WAKE,
};

static unsigned bt_config_power_on[] = {
	GPIO_CFG(42, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* WAKE */
	GPIO_CFG(43, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* RFR */
	GPIO_CFG(44, 2, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* CTS */
	GPIO_CFG(45, 2, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* Rx */
	GPIO_CFG(46, 3, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* Tx */
	GPIO_CFG(68, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* PCM_DOUT */
	GPIO_CFG(69, 1, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* PCM_DIN */
	GPIO_CFG(70, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* PCM_SYNC */
	GPIO_CFG(71, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* PCM_CLK */
	GPIO_CFG(83, 0, GPIO_CFG_INPUT,  GPIO_CFG_NO_PULL, GPIO_CFG_2MA),	/* HOST_WAKE */
};
static unsigned bt_config_power_off[] = {
	GPIO_CFG(42, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* WAKE */
	GPIO_CFG(43, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),	/* RFR */
	GPIO_CFG(44, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),	/* CTS */
	GPIO_CFG(45, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),	/* Rx */
	GPIO_CFG(46, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),	/* Tx */
	GPIO_CFG(68, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* PCM_DOUT */
	GPIO_CFG(69, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* PCM_DIN */
	GPIO_CFG(70, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* PCM_SYNC */
	GPIO_CFG(71, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* PCM_CLK */
	GPIO_CFG(83, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* HOST_WAKE */
};
//=======================================[modified by yubin begin
struct semaphore BT_Wifi_Power_Lock; //liuyu added
//BTorWIFI who call this function if BT 1; if wifi 0.
//on request to power on or off if on 1; if off 0.
#define BT_REQUEST 1
#define WIFI_REQUEST 0
#define POWER_ON 1
#define POWER_OFF 0

static short BT_Request=0;
static short WIFI_Request=0;
static int  BCM4325_power(int BTorWIFI, int on)
{
	static short bPowered=0;
	struct vreg *vreg_bt;
	int rc;
	
	
	down(&BT_Wifi_Power_Lock);
//	rc = gpio_tlmm_config(GPIO_CFG(81, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA), GPIO_ENABLE);

	vreg_bt = vreg_get(NULL, "gp4"); //liuyu changed
	if (IS_ERR(vreg_bt)) {
		printk(KERN_ERR "%s: vreg get failed (%ld)\n",
		       		__func__, PTR_ERR(vreg_bt));
		return PTR_ERR(vreg_bt);
	}
	
	if(on)
	{
		if(BTorWIFI)
			BT_Request=1; //BT request power on
		else
			WIFI_Request=1; //wifi request power on
		
		if(!bPowered) //if vreg_bt not powered on yet
		{
			rc = vreg_set_level(vreg_bt, 1800);
			if (rc) {
				printk(KERN_ERR "%s: vreg set level failed (%d)\n",
			       			__func__, rc);
				return -EIO;
			}
			rc = vreg_enable(vreg_bt);
			if (rc) {
				printk(KERN_ERR "%s: vreg enable failed (%d)\n",
			       			__func__, rc);
				return -EIO;
			}
			//msleep(200);
//			gpio_set_value(REG_ON_GPIO, 1); //turn on REG_ON
			//mpp_config_digital_out(21,       // BT_SHUTDOWN, MPP_22, 2.6v
			//			MPP_CFG(MPP_DLOGIC_LVL_MSMP,
			//			MPP_DLOGIC_OUT_CTRL_HIGH)); 
			bPowered=1;
		}

		// try to save power consumption while in idle mode.
		if(!BT_Request && WIFI_Request)            // WIFI open & BT closed, BT_RESET=low
			rc = mpp_config_digital_out(9,             // BT_RESET
					MPP_CFG(MPP_DLOGIC_LVL_MSMP,
					MPP_DLOGIC_OUT_CTRL_LOW));		
	}
	else
	{
		if(BTorWIFI)
			BT_Request=0; //BT request power off
		else
			WIFI_Request=0; //wifi request power off

		if((!BT_Request) && (!WIFI_Request)) //only when BT and wifi all requested power off
		{
			if(bPowered) //if powered on now
			{
			        vreg_set_level(vreg_bt, 0);	
//				gpio_set_value(REG_ON_GPIO, 0); // turn down REG_ON
                
				mpp_config_digital_out(9,                                // BT_RESET=high, avoid current leakage.
							MPP_CFG(MPP_DLOGIC_LVL_MSMP,
							MPP_DLOGIC_OUT_CTRL_HIGH)); 

				rc = vreg_disable(vreg_bt);
				if (rc) {
					printk(KERN_ERR "%s: vreg disable failed (%d)\n",
			       					__func__, rc);
					return -EIO;
				}
				bPowered=0;
			}
		}
	}

	up(&BT_Wifi_Power_Lock);

}
//]

static int bluetooth_power(int on)
{
//	struct vreg *vreg_bt;
	int pin, rc;

	printk(KERN_ERR "%s\n", __func__);
	if(on)
		printk("Turn on BT, GP4 got\n");
	else
		printk("Turn off BT, GP4 got\n");

	if (on) {
		for (pin = 0; pin < ARRAY_SIZE(bt_config_power_on); pin++) {
			rc = gpio_tlmm_config(bt_config_power_on[pin],
					      GPIO_CFG_ENABLE);
			if (rc) {
				printk(KERN_ERR
				       "%s: gpio_tlmm_config(%#x)=%d\n",
				       __func__, bt_config_power_on[pin], rc);
				return -EIO;
			}
		}

		gpio_set_value(42, 1); //liuyu added, wangjin deleted.

		//gpio_set_value(20, 1);   
		
		// BB_WAKES_BT, set high to avoid current linkage while bt enterred into reset mode.
		mpp_config_digital_out(13, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_HIGH));

/*		BTWakesMSMIRQ=MSM_GPIO_TO_INT(83);
		int res=request_irq(BTWakesMSMIRQ, BtWakesMSMIrqProc,
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
				 "BT Wakes MSM IRQ", NULL);
		if(res!=0)
			printk("IRQ for GPIO83 failed\n");
		set_irq_wake(BTWakesMSMIRQ, 1);*/
		mpp_config_digital_out(9,
					     MPP_CFG(MPP_DLOGIC_LVL_MSMP,
					     MPP_DLOGIC_OUT_CTRL_LOW));

		msleep(10);
		
		BCM4325_power(BT_REQUEST, POWER_ON);		


		mpp_config_digital_out(21,         // BT_SHUTDOWN, MPP_22, 2.6v
				MPP_CFG(MPP_DLOGIC_LVL_MSMP,
				MPP_DLOGIC_OUT_CTRL_HIGH));
		
		//gpio_set_value(REG_ON_GPIO, 1); //turn on REG_ON liuyu added

		msleep(10);
		
		//Turn up the BT_RST_N, MPP_10, low==>in reset mode, high===>normal work mode
		printk("========> Turn up the BT_RST_N to run out of reset status! \n");
		
		mpp_config_digital_out(9,
					     MPP_CFG(MPP_DLOGIC_LVL_MSMP,
					     MPP_DLOGIC_OUT_CTRL_HIGH));
		
		
	}
	else
	{

		gpio_set_value(42, 0); //liuyu added, wangjin deleted.

		// BB_WAKES_BT, set low to avoid current linkage after bt closed
		mpp_config_digital_out(13, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_LOW));
		
//		free_irq(BTWakesMSMIRQ, NULL);
		printk("Turn down BT32_K, BT_RST_N, REG_ON\n");
		mpp_config_digital_out(9,                                   // BT_RESET, high, to avoid current leakage.
					     MPP_CFG(MPP_DLOGIC_LVL_MSMP,
					     MPP_DLOGIC_OUT_CTRL_LOW));		

		mpp_config_digital_out(21,         // BT_SHUTDOWN, MPP_22, 0v
				MPP_CFG(MPP_DLOGIC_LVL_MSMP,
				MPP_DLOGIC_OUT_CTRL_LOW));

	  	BCM4325_power(BT_REQUEST, POWER_OFF);
		for (pin = 0; pin < ARRAY_SIZE(bt_config_power_off); pin++) {
			rc = gpio_tlmm_config(bt_config_power_off[pin],
					      GPIO_CFG_ENABLE);
			if (rc) {
				printk(KERN_ERR
				       "%s: gpio_tlmm_config(%#x)=%d\n",
				       __func__, bt_config_power_off[pin], rc);
				return -EIO;
			}
		}
		
	}
	return 0;
}


static void __init bt_power_init(void)
{
	int pin, rc = 0;
				
	for (pin = 0; pin < ARRAY_SIZE(bt_config_power_off); pin++) {
		rc = gpio_tlmm_config(bt_config_power_off[pin],
				      GPIO_CFG_ENABLE);
		if (rc) {
			printk(KERN_ERR
			       "%s: gpio_tlmm_config(%#x)=%d\n",
			       __func__, bt_config_power_off[pin], rc);
			return -EIO;
		}
	}

	mpp_config_digital_out(9,                     /* BT reset, MPP10   */
			     MPP_CFG(MPP_DLOGIC_LVL_MSMP,
			     MPP_DLOGIC_OUT_CTRL_LOW));	

	mpp_config_digital_out(21,            /* BT_SHUTDOWN, MPP22  */
			     MPP_CFG(MPP_DLOGIC_LVL_MSMP,
			     MPP_DLOGIC_OUT_CTRL_LOW));
	msm_bt_power_device.dev.platform_data = &bluetooth_power;
}
//=======================================[modified by yubin end
#else
#define bt_power_init(x) do {} while (0)
#endif

#ifdef CONFIG_ARCH_MSM7X27
static struct resource kgsl_resources[] = {
	{
		.name = "kgsl_reg_memory",
		.start = 0xA0000000,
		.end = 0xA001ffff,
		.flags = IORESOURCE_MEM,
	},
	{
		.name = "kgsl_yamato_irq",
		.start = INT_GRAPHICS,
		.end = INT_GRAPHICS,
		.flags = IORESOURCE_IRQ,
	},
};

static struct kgsl_platform_data kgsl_pdata;

static struct platform_device msm_device_kgsl = {
	.name = "kgsl",
	.id = -1,
	.num_resources = ARRAY_SIZE(kgsl_resources),
	.resource = kgsl_resources,
	.dev = {
		.platform_data = &kgsl_pdata,
	},
};
#endif

static struct platform_device msm_device_pmic_leds = {
	.name   = "pmic-leds",
	.id = -1,
};

static struct resource bluesleep_resources[] = {
	{
		.name	= "gpio_host_wake",
		.start	= 83,
		.end	= 83,
		.flags	= IORESOURCE_IO,
	},
	{
		.name	= "gpio_ext_wake",
		.start	= 42,
		.end	= 42,
		.flags	= IORESOURCE_IO,
	},
	{
		.name	= "host_wake",
		.start	= MSM_GPIO_TO_INT(83),
		.end	= MSM_GPIO_TO_INT(83),
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device msm_bluesleep_device = {
	.name = "bluesleep",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(bluesleep_resources),
	.resource	= bluesleep_resources,
};

static struct i2c_board_info i2c_devices[] = {
#ifdef CONFIG_MT9D112
	{
		I2C_BOARD_INFO("mt9d112", 0x78 >> 1),
	},
#endif
#ifdef CONFIG_S5K3E2FX
	{
		I2C_BOARD_INFO("s5k3e2fx", 0x20 >> 1),
	},
#endif
#ifdef CONFIG_MT9P012
	{
		I2C_BOARD_INFO("mt9p012", 0x6C >> 1),
	},
#endif
#ifdef CONFIG_MT9P012_KM
	{
		I2C_BOARD_INFO("mt9p012_km", 0x6C >> 2),
	},
#endif
#if defined(CONFIG_MT9T013) || defined(CONFIG_SENSORS_MT9T013)
	{
		I2C_BOARD_INFO("mt9t013", 0x6C),
	},
#endif
#ifdef CONFIG_VB6801
	{
		I2C_BOARD_INFO("vb6801", 0x20),
	},
#endif
#ifdef CONFIG_HI253
	{
		I2C_BOARD_INFO("hi253", 0x40 >> 1),
	},
#endif
/*Added by yubin, begin*/
#ifdef CONFIG_S5K5CAGA
	{
		I2C_BOARD_INFO("s5k5caga", 0x5A >> 1),
    },
#endif
/*Added by yubin, end*/
};

#ifdef CONFIG_MSM_CAMERA
static uint32_t camera_off_gpio_table[] = {
	/* parallel CAMERA interfaces */
#ifdef CONFIG_M7LAB_VENUS_CAMERA	//	20110316
	GPIO_CFG(0,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), /* PDN */
	GPIO_CFG(1,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* RESET */
#else
	GPIO_CFG(0,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT0 */
	GPIO_CFG(1,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT1 */
#endif	//	M7LAB_VENUS_CAMERA
	GPIO_CFG(2,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT2 */
	GPIO_CFG(3,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT3 */
	GPIO_CFG(4,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT4 */
	GPIO_CFG(5,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT5 */
	GPIO_CFG(6,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT6 */
	GPIO_CFG(7,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT7 */
	GPIO_CFG(8,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT8 */
	GPIO_CFG(9,  0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT9 */
	GPIO_CFG(10, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT10 */
	GPIO_CFG(11, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT11 */
	GPIO_CFG(12, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* PCLK */
	GPIO_CFG(13, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* HSYNC_IN */
	GPIO_CFG(14, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* VSYNC_IN */
	GPIO_CFG(15, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), /* MCLK */
};

static uint32_t camera_on_gpio_table[] = {
	/* parallel CAMERA interfaces */
#ifdef CONFIG_M7LAB_VENUS_CAMERA	//	20110316
	GPIO_CFG(0,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), /* PDN */
	GPIO_CFG(1,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* RESET */	//	Teddy 2011-04-06
#else
	GPIO_CFG(0,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT0 */
	GPIO_CFG(1,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT1 */
#endif	//	M7LAB_VENUS_CAMERA
	GPIO_CFG(2,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT2 */
	GPIO_CFG(3,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT3 */
	GPIO_CFG(4,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT4 */
	GPIO_CFG(5,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT5 */
	GPIO_CFG(6,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT6 */
	GPIO_CFG(7,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT7 */
	GPIO_CFG(8,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT8 */
	GPIO_CFG(9,  1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT9 */
	GPIO_CFG(10, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT10 */
	GPIO_CFG(11, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* DAT11 */
	GPIO_CFG(12, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), /* PCLK */
	GPIO_CFG(13, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* HSYNC_IN */
	GPIO_CFG(14, 1, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* VSYNC_IN */
	GPIO_CFG(15, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_16MA), /* MCLK */
	};

static void config_gpio_table(uint32_t *table, int len)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n], GPIO_CFG_ENABLE);
		if (rc) {
			printk(KERN_ERR "%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}

static struct vreg *vreg_gp2;
static struct vreg *vreg_gp3;
#ifdef CONFIG_M7LAB_VENUS_CAMERA	//	Teddy 2011-01-17
static struct vreg *vreg_gp5;
#endif	//	M7LAB_VENUS_CAMERA

static void msm_camera_vreg_config(int vreg_en)
{
	int rc;

#ifdef CONFIG_M7LAB_VENUS_CAMERA	//	20110316 Do not change any order
	if (vreg_gp2 == NULL) {
		vreg_gp2 = vreg_get(NULL, "gp2");
		if (IS_ERR(vreg_gp2)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp2", PTR_ERR(vreg_gp2));
			return;
		}

		rc = vreg_set_level(vreg_gp2, 1800);
		if (rc) {
			printk(KERN_ERR "%s: GP2 set_level failed (%d)\n",
				__func__, rc);
		}
	}

	if (vreg_gp3 == NULL) {
		vreg_gp3 = vreg_get(NULL, "gp3");
		if (IS_ERR(vreg_gp3)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp3", PTR_ERR(vreg_gp3));
			return;
		}

		rc = vreg_set_level(vreg_gp3, 2800);
		if (rc) {
			printk(KERN_ERR "%s: GP3 set level failed (%d)\n",
				__func__, rc);
		}
	}

	if (vreg_gp5 == NULL) {
		vreg_gp5 = vreg_get(NULL, "gp5");
		if (IS_ERR(vreg_gp5)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp3", PTR_ERR(vreg_gp5));
			return;
		}

		rc = vreg_set_level(vreg_gp5, 2800);
		if (rc) {
			printk(KERN_ERR "%s: GP3 set level failed (%d)\n",
				__func__, rc);
		}
	}

	if (vreg_en) {
		rc = vreg_enable(vreg_gp3);
		if (rc) {
			printk(KERN_ERR "%s: GP3 enable failed (%d)\n",
				__func__, rc);
		}

		rc = vreg_enable(vreg_gp5);
		if (rc) {
			printk(KERN_ERR "%s: GP5 enable failed (%d)\n",
				__func__, rc);
		}

		rc = vreg_enable(vreg_gp2);
		if (rc) {
			printk(KERN_ERR "%s: GP2 enable failed (%d)\n",
				__func__, rc);
		}
	} else {
		rc = vreg_disable(vreg_gp2);
		if (rc) {
			printk(KERN_ERR "%s: GP2 disable failed (%d)\n",
				 __func__, rc);
		}

		rc = vreg_disable(vreg_gp3);
		if (rc) {
			printk(KERN_ERR "%s: GP3 disable failed (%d)\n",
				__func__, rc);
		}
		rc = vreg_disable(vreg_gp5);
		if (rc) {
			printk(KERN_ERR "%s: GP5 disable failed (%d)\n",
				__func__, rc);
		}
	}
#else	// #ifdef CONFIG_M7LAB_VENUS_CAMERA
	if (vreg_gp2 == NULL) {
		vreg_gp2 = vreg_get(NULL, "gp2");
		if (IS_ERR(vreg_gp2)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp2", PTR_ERR(vreg_gp2));
			return;
		}

		rc = vreg_set_level(vreg_gp2, 2850);
		if (rc) {
			printk(KERN_ERR "%s: GP2 set_level failed (%d)\n",
				__func__, rc);
		}
	}

	if (vreg_gp3 == NULL) {
		vreg_gp3 = vreg_get(NULL, "gp3");
		if (IS_ERR(vreg_gp3)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp3", PTR_ERR(vreg_gp3));
			return;
		}

		rc = vreg_set_level(vreg_gp3, 2850);
		if (rc) {
			printk(KERN_ERR "%s: GP3 set level failed (%d)\n",
				__func__, rc);
		}
	}

	if (vreg_en) {
		rc = vreg_enable(vreg_gp2);
		if (rc) {
			printk(KERN_ERR "%s: GP2 enable failed (%d)\n",
				 __func__, rc);
		}

		rc = vreg_enable(vreg_gp3);
		if (rc) {
			printk(KERN_ERR "%s: GP3 enable failed (%d)\n",
				__func__, rc);
		}
	} else {
		rc = vreg_disable(vreg_gp2);
		if (rc) {
			printk(KERN_ERR "%s: GP2 disable failed (%d)\n",
				 __func__, rc);
		}

		rc = vreg_disable(vreg_gp3);
		if (rc) {
			printk(KERN_ERR "%s: GP3 disable failed (%d)\n",
				__func__, rc);
		}
	}
#endif	// #ifdef CONFIG_M7LAB_VENUS_CAMERA
}

static int config_camera_on_gpios(void)
{
	int vreg_en = 1;

#ifdef CONFIG_M7LAB_VENUS_CAMERA	//	Teddy 2011-01-21
#else
	if (machine_is_msm7x25_ffa() ||
	    machine_is_msm7x27_ffa())
#endif	//	CONFIG_M7LAB_VENUS_CAMERA
		msm_camera_vreg_config(vreg_en);

	config_gpio_table(camera_on_gpio_table,
		ARRAY_SIZE(camera_on_gpio_table));
	return 0;
}

static void config_camera_off_gpios(void)
{
	int vreg_en = 0;

#ifdef CONFIG_M7LAB_VENUS_CAMERA	//	Teddy 2011-01-21
#else
	if (machine_is_msm7x25_ffa() ||
	    machine_is_msm7x27_ffa())
#endif	//	CONFIG_M7LAB_VENUS_CAMERA
		msm_camera_vreg_config(vreg_en);

	config_gpio_table(camera_off_gpio_table,
		ARRAY_SIZE(camera_off_gpio_table));
}

static struct msm_camera_device_platform_data msm_camera_device_data = {
	.camera_gpio_on  = config_camera_on_gpios,
	.camera_gpio_off = config_camera_off_gpios,
	.ioext.mdcphy = MSM_MDC_PHYS,
	.ioext.mdcsz  = MSM_MDC_SIZE,
	.ioext.appphy = MSM_CLK_CTL_PHYS,
	.ioext.appsz  = MSM_CLK_CTL_SIZE,
};

int pmic_set_flash_led_current(enum pmic8058_leds id, unsigned mA)
{
	int rc;
	rc = pmic_flash_led_set_current(mA);
	return rc;
}

static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_PMIC,
	._fsrc.pmic_src.num_of_src = 1,
	._fsrc.pmic_src.low_current  = 60,
	._fsrc.pmic_src.high_current = 100,
	._fsrc.pmic_src.led_src_1 = 0,
	._fsrc.pmic_src.led_src_2 = 0,
	._fsrc.pmic_src.pmic_set_current = pmic_set_flash_led_current,
};

/*Added by yubin, begin*/
#ifdef CONFIG_S5K5CAGA
static struct msm_camera_sensor_flash_data flash_s5k5caga = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};
 
static struct msm_camera_sensor_info msm_camera_sensor_s5k5caga_data = {
	.sensor_name    = "s5k5caga",
	.sensor_reset   = 1,    // reset gpio
	.sensor_pwd     = 0,    // power control gpio, connect to sensor-ENB signal, low level valid.
	.vcm_pwd        = 0,    // no auto-focus function.
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_type     = MSM_CAMERA_FLASH_LED,       // new added for flash-enable.
	.flash_data     = &flash_s5k5caga
};

static struct platform_device msm_camera_sensor_s5k5caga = {
	.name      = "msm_camera_s5k5caga",
	.dev       = {
		.platform_data = &msm_camera_sensor_s5k5caga_data,
	},
};
#endif
/*Added by yubin, end*/
#ifdef CONFIG_MT9D112
static struct msm_camera_sensor_flash_data flash_mt9d112 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9d112_data = {
	.sensor_name    = "mt9d112",
	.sensor_reset   = 89,
	.sensor_pwd     = 85,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_data     = &flash_mt9d112
};

static struct platform_device msm_camera_sensor_mt9d112 = {
	.name      = "msm_camera_mt9d112",
	.dev       = {
		.platform_data = &msm_camera_sensor_mt9d112_data,
	},
};
#endif

#ifdef CONFIG_S5K3E2FX
static struct msm_camera_sensor_flash_data flash_s5k3e2fx = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_s5k3e2fx_data = {
	.sensor_name    = "s5k3e2fx",
	.sensor_reset   = 89,
	.sensor_pwd     = 85,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_data     = &flash_s5k3e2fx
};

static struct platform_device msm_camera_sensor_s5k3e2fx = {
	.name      = "msm_camera_s5k3e2fx",
	.dev       = {
		.platform_data = &msm_camera_sensor_s5k3e2fx_data,
	},
};
#endif

#ifdef CONFIG_MT9P012
static struct msm_camera_sensor_flash_data flash_mt9p012 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9p012_data = {
	.sensor_name    = "mt9p012",
	.sensor_reset   = 89,
	.sensor_pwd     = 85,
	.vcm_pwd        = 88,
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_data     = &flash_mt9p012
};

static struct platform_device msm_camera_sensor_mt9p012 = {
	.name      = "msm_camera_mt9p012",
	.dev       = {
		.platform_data = &msm_camera_sensor_mt9p012_data,
	},
};
#endif

#ifdef CONFIG_MT9P012_KM
static struct msm_camera_sensor_flash_data flash_mt9p012_km = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9p012_km_data = {
	.sensor_name    = "mt9p012_km",
	.sensor_reset   = 89,
	.sensor_pwd     = 85,
	.vcm_pwd        = 88,
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_data     = &flash_mt9p012_km
};

static struct platform_device msm_camera_sensor_mt9p012_km = {
	.name      = "msm_camera_mt9p012_km",
	.dev       = {
		.platform_data = &msm_camera_sensor_mt9p012_km_data,
	},
};
#endif

#ifdef CONFIG_MT9T013
static struct msm_camera_sensor_flash_data flash_mt9t013 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9t013_data = {
	.sensor_name    = "mt9t013",
	.sensor_reset   = 89,
	.sensor_pwd     = 85,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_data     = &flash_mt9t013
};

static struct platform_device msm_camera_sensor_mt9t013 = {
	.name      = "msm_camera_mt9t013",
	.dev       = {
		.platform_data = &msm_camera_sensor_mt9t013_data,
	},
};
#endif

#ifdef CONFIG_VB6801
static struct msm_camera_sensor_flash_data flash_vb6801 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_vb6801_data = {
	.sensor_name    = "vb6801",
	.sensor_reset   = 89,
	.sensor_pwd     = 88,
	.vcm_pwd        = 0,
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_data     = &flash_vb6801
};

static struct platform_device msm_camera_sensor_vb6801 = {
	.name      = "msm_camera_vb6801",
	.dev       = {
		.platform_data = &msm_camera_sensor_vb6801_data,
	},
};
#endif

#ifdef CONFIG_HI253	// FEATURE_M7LAB
static struct msm_camera_sensor_flash_data flash_hi253 = {
	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_hi253_data = {
#ifdef CONFIG_GTS_JUKE_CAMERA
	.sensor_name    = "hi253",
#else
	.sensor_name    = "mt9d112",  // by sinclair.lee You should change this later "hi253",
#endif
	.sensor_reset   = 1,
	.sensor_pwd     = 0,
	.vcm_pwd        = 97,	//	Teddy 2011-04-07
	.vcm_enable     = 0,
	.pdata          = &msm_camera_device_data,
	.flash_data     = &flash_hi253
};

static struct platform_device msm_camera_sensor_hi253 = {
	.name      = "msm_camera_hi253",
	.dev       = {
		.platform_data = &msm_camera_sensor_hi253_data,
	},
};
#endif
#endif

#ifdef CONFIG_M7LAB_VENUS_BATTERY	//	Teddy
#else
static u32 msm_calculate_batt_capacity(u32 current_voltage);
#endif	//	CONFIG_M7LAB_VENUS_BATTERY

static struct msm_psy_batt_pdata msm_psy_batt_data = {
#ifdef CONFIG_M7LAB_VENUS_BATTERY	//	Teddy 2011-10-04
	.voltage_min_design		= 0,	//	Teddy 2011-10-04
	.voltage_max_design		= 0,
#else
	.voltage_min_design		= 2800,
	.voltage_max_design		= 4300,
#endif	//	CONFIG_M7LAB_VENUS_BATTERY
	.avail_chg_sources   	= AC_CHG | USB_CHG ,
	.batt_technology        = POWER_SUPPLY_TECHNOLOGY_LION,
#ifdef CONFIG_M7LAB_VENUS_BATTERY	//	Teddy
#else
	.calculate_capacity	= &msm_calculate_batt_capacity,
#endif	//	CONFIG_M7LAB_VENUS_BATTERY
};

#ifdef CONFIG_M7LAB_VENUS_BATTERY	//	Teddy
#else
static u32 msm_calculate_batt_capacity(u32 current_voltage)
{
	u32 low_voltage   = msm_psy_batt_data.voltage_min_design;
	u32 high_voltage  = msm_psy_batt_data.voltage_max_design;

	return (current_voltage - low_voltage) * 100
		/ (high_voltage - low_voltage);
}
#endif	//	CONFIG_M7LAB_VENUS_BATTERY

static struct platform_device msm_batt_device = {
	.name 		    = "msm-battery",
	.id		    = -1,
	.dev.platform_data  = &msm_psy_batt_data,
};
//=======================================[modified by yubin begin

int msm_wifi_power(int on)
{
	
	int rc = 0;
	
	if(on)
	{				
		rc = gpio_tlmm_config(GPIO_CFG(21, 0, GPIO_CFG_INPUT,  GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	/* WL_WAKES_BB */  
						GPIO_CFG_ENABLE);
		if (rc) {
			printk(KERN_ERR  "%s: gpio_tlmm_config(GPIO_21/ WL_WAKES_BB)=%d\n",  __func__,  rc);
			return rc;
		}
			
		BCM4325_power(WIFI_REQUEST, POWER_ON);		

		rc = mpp_config_digital_out(15,        // WIFI_SHUTDOWN
			MPP_CFG(MPP_DLOGIC_LVL_MSMP,
			MPP_DLOGIC_OUT_CTRL_HIGH));

		if (rc) {
			printk(KERN_ERR"%s: mpp_config_digital_out return val: %d \n",
				   __func__, rc);
			return rc;
		}

		msleep(150);
	}
	else
	{
		int rc=0;

		// set WIFI_SHUTDOWN to 0v
		rc = mpp_config_digital_out(15,               // WIFI_SHUTDOWN
				MPP_CFG(MPP_DLOGIC_LVL_MSMP,
				MPP_DLOGIC_OUT_CTRL_LOW));
		msleep(100);
		if (rc) {
			printk(KERN_ERR"%s: mpp_config_digital_out return val %d \n",
				   __func__, rc);
			return rc;
		}		
		
        	BCM4325_power(WIFI_REQUEST, POWER_OFF);	
	}
	printk(KERN_INFO"%s %d\n", __FUNCTION__,on);
	return 0;
}

EXPORT_SYMBOL(msm_wifi_power);

int msm_wifi_reset(int on)
{

    int rc=0;
	
   if(on)
    {
    	// WLAN_SHUTDOWN high, MPP_16
	    rc = mpp_config_digital_out(15, /*17,*/
    			MPP_CFG(MPP_DLOGIC_LVL_MSMP,
			    MPP_DLOGIC_OUT_CTRL_HIGH));
    }
    else
    {
	    // WLAN_SHUTDOWN low, MPP_16
    	rc = mpp_config_digital_out(15, /*17,*/
			    MPP_CFG(MPP_DLOGIC_LVL_MSMP,
    			MPP_DLOGIC_OUT_CTRL_LOW));
    }
    if(rc<0)
    {
		printk("%s: set Wifi_RST pin error\n",__FUNCTION__);
    }
	return rc;
}

void (*wifi_status_cb)(int card_present, void *dev_id);
void *wifi_status_cb_devid;

EXPORT_SYMBOL(wifi_status_cb_devid);
int wifi_status_notify(void (*callback)(int card_present, void *dev_id), void *dev_id)
{	printk(KERN_ERR "%s: -----------------b\n",__func__);	

        if (wifi_status_cb)
                return -EAGAIN;
        wifi_status_cb = callback;
        wifi_status_cb_devid = dev_id;
        return 0;
}

int msm_wifi_set_carddetect(int val)
{
    printk(KERN_INFO"%s: %d\n", __func__, val);
    
	if (wifi_status_cb) {
        wifi_status_cb(val, wifi_status_cb_devid);
    } else
        printk(KERN_WARNING"%s: Nobody to notify\n", __func__);
    return 0;
}

static struct resource msm_wifi_resources[] = { 
    [0] = { 
      .name       = "bcm4330_wlan_irq",
      .start      = MSM_GPIO_TO_INT(21),
      .end        = MSM_GPIO_TO_INT(21),
      .flags          = IORESOURCE_IRQ | IORESOURCE_IRQ_LOWEDGE ,
    },
};

#include <linux/wlan_plat.h>
static struct wifi_platform_data msm_wifi_control = { 
    .set_power      = msm_wifi_power,
    .set_reset      = msm_wifi_reset,
    .set_carddetect = msm_wifi_set_carddetect,
//    .mem_prealloc   = msm_wifi_mem_prealloc,
};

static struct platform_device msm_wifi_device = { 
        .name           = "bcm4330_wlan",
        .id             = 1,
//        .num_resources  = ARRAY_SIZE(msm_wifi_resources),
//      .resource       = msm_wifi_resources,
        .dev            = { 
                .platform_data = &msm_wifi_control,
        },  
};


//=======================================]modified by yubin end


static struct platform_device *devices[] __initdata = {
	&msm_device_smd,
	&msm_device_dmov,
	&msm_device_nand,

#ifdef CONFIG_USB_MSM_OTG_72K
	&msm_device_otg,
#ifdef CONFIG_USB_GADGET
	&msm_device_gadget_peripheral,
#endif
#endif

#ifdef CONFIG_USB_FUNCTION
	&msm_device_hsusb_peripheral,
	&mass_storage_device,
#endif

#ifdef CONFIG_USB_ANDROID
	&usb_mass_storage_device,
	&rndis_device,
#ifdef CONFIG_USB_ANDROID_DIAG
	&usb_diag_device,
#endif
	&android_usb_device,
#endif
	&msm_device_i2c,
	&smc91x_device,
	&msm_device_tssc,
	&android_pmem_kernel_ebi1_device,
	&android_pmem_device,
	&android_pmem_adsp_device,
	&android_pmem_audio_device,
	&msm_fb_device,
#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
//by wangpl	&lcdc_ips1p5700_panel_device,
#else
	&lcdc_gordon_panel_device,
#endif

	&lcdc_panel_device,
	
	&msm_device_uart_dm1,
#ifdef CONFIG_BT
	&msm_bt_power_device,
#endif
	&msm_device_pmic_leds,
	&msm_device_snd,
	&msm_device_adspdec,
#ifdef CONFIG_MT9T013
	&msm_camera_sensor_mt9t013,
#endif
/*Added by yubin, begin*/
#ifdef CONFIG_S5K5CAGA
	&msm_camera_sensor_s5k5caga,
#endif
/*Added by yubin, end*/
#ifdef CONFIG_MT9D112
	&msm_camera_sensor_mt9d112,
#endif
#ifdef CONFIG_S5K3E2FX
	&msm_camera_sensor_s5k3e2fx,
#endif
#ifdef CONFIG_MT9P012
	&msm_camera_sensor_mt9p012,
#endif
#ifdef CONFIG_MT9P012_KM
	&msm_camera_sensor_mt9p012_km,
#endif
#ifdef CONFIG_VB6801
	&msm_camera_sensor_vb6801,
#endif
#ifdef CONFIG_HI253
	&msm_camera_sensor_hi253,
#endif
	&msm_bluesleep_device,
#ifdef CONFIG_ARCH_MSM7X27
	&msm_device_kgsl,
#endif
#if defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE)
	&msm_device_tsif,
#endif
	&hs_device,
	&msm_batt_device,
	&msm_gpio_keys,/* Added by yubin,  for aeneas home key */
};

static struct msm_panel_common_pdata mdp_pdata = {
	.gpio = 97,
};

static void __init msm_fb_add_devices(void)
{
	msm_fb_register_device("mdp", &mdp_pdata);
	msm_fb_register_device("pmdh", 0);
	msm_fb_register_device("lcdc", &lcdc_pdata);
}

extern struct sys_timer msm_timer;

static void __init msm7x2x_init_irq(void)
{
	msm_init_irq();
}

static struct msm_acpu_clock_platform_data msm7x2x_clock_data = {
	.acpu_switch_time_us = 50,
	.max_speed_delta_khz = 400000,
	.vdd_switch_time_us = 62,
	.max_axi_khz = 160000,
};

void msm_serial_debug_init(unsigned int base, int irq,
			   struct device *clk_device, int signal_irq);

#if (defined(CONFIG_MMC_MSM_SDC1_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC2_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC3_SUPPORT)\
	|| defined(CONFIG_MMC_MSM_SDC4_SUPPORT))

static unsigned long vreg_sts, gpio_sts;
static struct vreg *vreg_mmc;
static unsigned mpp_mmc = 2;

struct sdcc_gpio {
	struct msm_gpio *cfg_data;
	uint32_t size;
	struct msm_gpio *sleep_cfg_data;
};

static struct msm_gpio sdc1_cfg_data[] = {
	{GPIO_CFG(51, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc1_dat_3"},
	{GPIO_CFG(52, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc1_dat_2"},
	{GPIO_CFG(53, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc1_dat_1"},
	{GPIO_CFG(54, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc1_dat_0"},
	{GPIO_CFG(55, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc1_cmd"},
	{GPIO_CFG(56, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA), "sdc1_clk"},
};

static struct msm_gpio sdc2_cfg_data[] = {
	{GPIO_CFG(62, 2, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA), "sdc2_clk"},
	{GPIO_CFG(63, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc2_cmd"},
	{GPIO_CFG(64, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc2_dat_3"},
	{GPIO_CFG(65, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc2_dat_2"},
	{GPIO_CFG(66, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc2_dat_1"},
	{GPIO_CFG(67, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc2_dat_0"},
};

//=======================================[modified by yubin begin
static struct msm_gpio sdc2_sleep_cfg_data[] = {
	{GPIO_CFG(62, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), "sdc2_clk"},
	{GPIO_CFG(63, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), "sdc2_cmd"},
	{GPIO_CFG(64, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), "sdc2_dat_3"},
	{GPIO_CFG(65, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), "sdc2_dat_2"},
	{GPIO_CFG(66, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), "sdc2_dat_1"},
	{GPIO_CFG(67, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), "sdc2_dat_0"},};
//=======================================]modified by yubin end

static struct msm_gpio sdc3_cfg_data[] = {
	{GPIO_CFG(88, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA), "sdc3_clk"},
	{GPIO_CFG(89, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc3_cmd"},
	{GPIO_CFG(90, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc3_dat_3"},
	{GPIO_CFG(91, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc3_dat_2"},
	{GPIO_CFG(92, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc3_dat_1"},
	{GPIO_CFG(93, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc3_dat_0"},
};

static struct msm_gpio sdc4_cfg_data[] = {
	{GPIO_CFG(19, 3, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc4_dat_3"},
	{GPIO_CFG(20, 3, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc4_dat_2"},
	{GPIO_CFG(21, 4, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc4_dat_1"},
	{GPIO_CFG(107, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc4_cmd"},
	{GPIO_CFG(108, 1, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_8MA), "sdc4_dat_0"},
	{GPIO_CFG(109, 1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA), "sdc4_clk"},
};

static struct sdcc_gpio sdcc_cfg_data[] = {
	{
		.cfg_data = sdc1_cfg_data,
		.size = ARRAY_SIZE(sdc1_cfg_data),
		.sleep_cfg_data = NULL,
	},
	{
		.cfg_data = sdc2_cfg_data,
		.size = ARRAY_SIZE(sdc2_cfg_data),
		.sleep_cfg_data = sdc2_sleep_cfg_data,
	},
	{
		.cfg_data = sdc3_cfg_data,
		.size = ARRAY_SIZE(sdc3_cfg_data),
		.sleep_cfg_data = NULL,
	},
	{
		.cfg_data = sdc4_cfg_data,
		.size = ARRAY_SIZE(sdc4_cfg_data),
		.sleep_cfg_data = NULL,
	},
};

static void msm_sdcc_setup_gpio(int dev_id, unsigned int enable)
{
	int rc = 0;
	struct sdcc_gpio *curr;

	curr = &sdcc_cfg_data[dev_id - 1];
	if (!(test_bit(dev_id, &gpio_sts)^enable))
		return;

	if (enable) {
		set_bit(dev_id, &gpio_sts);
		rc = msm_gpios_request_enable(curr->cfg_data, curr->size);
		if (rc)
			printk(KERN_ERR "%s: Failed to turn on GPIOs for slot %d\n",
				__func__,  dev_id);
	} else {
		clear_bit(dev_id, &gpio_sts);
		if (curr->sleep_cfg_data) {
			msm_gpios_enable(curr->sleep_cfg_data, curr->size);
			msm_gpios_free(curr->sleep_cfg_data, curr->size);
			return;
		}
		msm_gpios_disable_free(curr->cfg_data, curr->size);
	}
}

static uint32_t msm_sdcc_setup_power(struct device *dv, unsigned int vdd)
{
	int rc = 0;
	struct platform_device *pdev;

	pdev = container_of(dv, struct platform_device, dev);
	msm_sdcc_setup_gpio(pdev->id, !!vdd);

	if (vdd == 0) {
#if 1//change 0 to  1 by yubin	//	Teddy 2011-09-30
		if (!vreg_sts)
			return 0;
#endif

		clear_bit(pdev->id, &vreg_sts);

		if (!vreg_sts) {
			if (machine_is_msm7x25_ffa() ||
					machine_is_msm7x27_ffa()) {
				rc = mpp_config_digital_out(mpp_mmc,
				     MPP_CFG(MPP_DLOGIC_LVL_MSMP,
				     MPP_DLOGIC_OUT_CTRL_LOW));
			} else
			//modified by yubin, begin
				if(pdev->id!=1){
			 		vreg_set_level(vreg_mmc, 0);
					rc = vreg_disable(vreg_mmc);
				}
				//vreg_set_level(vreg_mmc, 1500);
				//rc = vreg_disable(vreg_mmc);
			//modified by yubin, end
			if (rc)
				printk(KERN_ERR "%s: return val: %d \n",
					__func__, rc);
		}
		return 0;
	}

	if (!vreg_sts) {
		if (machine_is_msm7x25_ffa() || machine_is_msm7x27_ffa()) {
			rc = mpp_config_digital_out(mpp_mmc,
			     MPP_CFG(MPP_DLOGIC_LVL_MSMP,
			     MPP_DLOGIC_OUT_CTRL_HIGH));
		} else {
			rc = vreg_set_level(vreg_mmc, 2850);
			if (!rc)
				rc = vreg_enable(vreg_mmc);
		}
		if (rc)
			printk(KERN_ERR "%s: return val: %d \n",
					__func__, rc);
	}
	set_bit(pdev->id, &vreg_sts);
	return 0;
}
#undef CONFIG_M7LAB_VENUS_SDCARD//added by yubin
#ifdef CONFIG_M7LAB_VENUS_SDCARD	//	Teddy 2011-01-21
static unsigned int venus_sdcc_slot_status(struct device *dev)
{
	int status = (unsigned int) gpio_get_value(49);
	printk(KERN_ERR "%s: return val: %d \n", __func__, status);
	return !status;
}
#endif	//	CONFIG_M7LAB_VENUS_SDCARD

#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
static struct mmc_platform_data msm7x2x_sdc1_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd	= msm_sdcc_setup_power,
#ifdef CONFIG_M7LAB_VENUS_SDCARD	//	Teddy 2011-01-21
	.status		= venus_sdcc_slot_status,
	.status_irq	= MSM_GPIO_TO_INT(49),
	.irq_flags	= IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
#endif	//	CONFIG_M7LAB_VENUS_SDCARD
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.nonremovable	= 0,
#ifdef CONFIG_MMC_MSM_SDC1_DUMMY52_REQUIRED
	.dummy52_required = 1,
#endif
};
#endif
//modified by yubin, begin[
#if 0
#ifdef CONFIG_MMC_MSM_SDC2_SUPPORT
static struct mmc_platform_data msm7x2x_sdc2_data = {
	.ocr_mask	= MMC_VDD_28_29,
//Teddy	.translate_vdd	= msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
#ifdef CONFIG_MMC_MSM_SDIO_SUPPORT
#ifdef CONFIG_M7LAB_VENUS	//	Teddy 2011-03-30
	.sdiowakeup_irq = MSM_GPIO_TO_INT(21),
#else
	.sdiowakeup_irq = MSM_GPIO_TO_INT(66),
#endif	//	CONFIG_M7LAB_VENUS
#endif
#endif
#endif

#ifdef CONFIG_MMC_MSM_SDC2_SUPPORT
static struct mmc_platform_data msm7x2x_sdc2_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd	= msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
#ifdef CONFIG_MMC_MSM_SDIO_SUPPORT
	.sdiowakeup_irq = MSM_GPIO_TO_INT(66),

//modified by yubin, end]
#endif
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.nonremovable	= 0,
#ifdef CONFIG_MMC_MSM_SDC2_DUMMY52_REQUIRED
	.dummy52_required = 1,
#endif
};
#endif

#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
static struct mmc_platform_data msm7x2x_sdc3_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd	= msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.nonremovable	= 0,
#ifdef CONFIG_MMC_MSM_SDC3_DUMMY52_REQUIRED
	.dummy52_required = 1,
#endif
};
#endif

#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
static struct mmc_platform_data msm7x2x_sdc4_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd	= msm_sdcc_setup_power,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.nonremovable	= 0,
#ifdef CONFIG_MMC_MSM_SDC4_DUMMY52_REQUIRED
	.dummy52_required = 1,
#endif
};
#endif
//========================modified by yubin, begin[
#ifdef CONFIG_M7LAB_VENUS_SDCARD	//	Teddy 2011-01-21
static void sdcc_gpio_init(void)
{
	int rc = 0;
	if (gpio_request(49, "sdc1_status_irq"))
		pr_err("failed to request gpio sdc1_status_irq\n");
	rc = gpio_tlmm_config(GPIO_CFG(49, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP,
				GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	if (rc)
		printk(KERN_ERR "%s: Failed to configure GPIO %d\n",
				__func__, rc);
}
#endif	//	CONFIG_M7LAB_VENUS_SDCARD

// chengyingkai 20091130 add for wifi begin
static uint32_t msm_sdcc_wifi_setup_power(struct device *dv, unsigned int vdd)
{	
	struct platform_device *pdev;

	pdev = container_of(dv, struct platform_device, dev);
	msm_sdcc_setup_gpio(pdev->id, !!vdd);

	return 0;
}
//1203 add begin
#if 1
static struct sdio_embedded_func wifi_func = {
	.f_class        = 0x07/*SDIO_CLASS_WLAN*/,
 .f_maxblksize   = 512,
};
#endif

static struct embedded_sdio_data wifi_emb_data = {
	.cis    = {
		.vendor         = 0x02d0,
  .device         = 0x4325,
  .blksize        = 512,
  /*.max_dtr      = 24000000,  Max of chip - no worky on Trout */
  .max_dtr        =   48000000,
	},
 .cccr   = {
	 .multi_block    = 1,
  .low_speed      = 1,
  .wide_bus       = 1,
  .high_power     = 1,
  .high_speed     = 1,
 },
 .funcs  = &wifi_func,
 .num_funcs = 2,
};

//1203 add end
static struct mmc_platform_data msm7x27_sdcc_wifi_data = {
	.ocr_mask	= MMC_VDD_28_29,
	.translate_vdd	= msm_sdcc_wifi_setup_power,
 .embedded_sdio		= &wifi_emb_data,
    .register_status_notify = wifi_status_notify,
	.msmsdcc_fmin	= 144000,
	.msmsdcc_fmid	= 24576000,
	.msmsdcc_fmax	= 49152000,
	.mmc_bus_width  = MMC_CAP_4_BIT_DATA,
	//.msmsdcc_fmax	= 24576000,
	.nonremovable	= 0,
};
// chengyingkai 20091130 add for wifi end

//========================modified by yubin, end[
static void __init msm7x2x_init_mmc(void)
{
	if (!machine_is_msm7x25_ffa() && !machine_is_msm7x27_ffa()) {
		vreg_mmc = vreg_get(NULL, "mmc");
		if (IS_ERR(vreg_mmc)) {
			printk(KERN_ERR "%s: vreg get failed (%ld)\n",
			       __func__, PTR_ERR(vreg_mmc));
			return;
		}
	}

#ifdef CONFIG_M7LAB_VENUS_SDCARD	//	Teddy 2011-01-21
	sdcc_gpio_init();
#endif	//	CONFIG_M7LAB_VENUS_SDCARD
#ifdef CONFIG_MMC_MSM_SDC1_SUPPORT
	msm_add_sdcc(1, &msm7x2x_sdc1_data);
#endif

// chengyingkai add for OPAL wifi 20091130 begin
#ifdef CONFIG_MMC_MSM_SDC2_SUPPORT
	msm_add_sdcc(2, &msm7x27_sdcc_wifi_data);
#endif
// chengyingkai add for OPAL wifi 20091130 end


	if (machine_is_msm7x25_surf() || machine_is_msm7x27_surf()) {
#ifdef CONFIG_MMC_MSM_SDC3_SUPPORT
		msm_add_sdcc(3, &msm7x2x_sdc3_data);
#endif
#ifdef CONFIG_MMC_MSM_SDC4_SUPPORT
		msm_add_sdcc(4, &msm7x2x_sdc4_data);
#endif
	}
}
#else
#define msm7x2x_init_mmc() do {} while (0)
#endif


static struct msm_pm_platform_data msm7x25_pm_data[MSM_PM_SLEEP_MODE_NR] = {
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE].latency = 16000,

	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN].latency = 12000,

	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].latency = 2000,
};

static struct msm_pm_platform_data msm7x27_pm_data[MSM_PM_SLEEP_MODE_NR] = {
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE].supported = 1,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE].suspend_enabled = 1,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE].idle_enabled = 1,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE].latency = 16000,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE].residency = 20000,

	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN].supported = 1,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN].suspend_enabled = 1,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN].idle_enabled = 1,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN].latency = 12000,
	[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN].residency = 20000,

	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].supported = 1,
	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].suspend_enabled
		= 1,
	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].idle_enabled = 1,
	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].latency = 2000,
	[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].residency = 0,
};

static void
msm_i2c_gpio_config(int iface, int config_type)
{
	int gpio_scl;
	int gpio_sda;
	if (iface) {
#ifdef CONFIG_M7LAB_VENUS	//	Teddy 2011-03-30
		return;
#else
		gpio_scl = 95;
		gpio_sda = 96;
#endif	//	CONFIG_M7LAB_VENUS
	} else {
		gpio_scl = 60;
		gpio_sda = 61;
	}
	if (config_type) {
		gpio_tlmm_config(GPIO_CFG(gpio_scl, 1, GPIO_CFG_INPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(gpio_sda, 1, GPIO_CFG_INPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	} else {
		gpio_tlmm_config(GPIO_CFG(gpio_scl, 0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(gpio_sda, 0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_16MA), GPIO_CFG_ENABLE);
	}
}

static struct msm_i2c_platform_data msm_i2c_pdata = {
	.clk_freq = 400000,
	.rmutex  = 0,
	.pri_clk = 60,
	.pri_dat = 61,
#ifdef CONFIG_M7LAB_VENUS	//	Teddy 2011-03-30
#else
	.aux_clk = 95,
	.aux_dat = 96,
#endif	//	CONFIG_M7LAB_VENUS
	.msm_i2c_config_gpio = msm_i2c_gpio_config,
};

static void __init msm_device_i2c_init(void)
{
	if (gpio_request(60, "i2c_pri_clk"))
		pr_err("failed to request gpio i2c_pri_clk\n");
	if (gpio_request(61, "i2c_pri_dat"))
		pr_err("failed to request gpio i2c_pri_dat\n");
#ifdef CONFIG_M7LAB_VENUS	//	Teddy 2011-03-30
#else
	if (gpio_request(95, "i2c_sec_clk"))
		pr_err("failed to request gpio i2c_sec_clk\n");
	if (gpio_request(96, "i2c_sec_dat"))
		pr_err("failed to request gpio i2c_sec_dat\n");
#endif	//	CONFIG_M7LAB_VENUS

	if (cpu_is_msm7x27())
		msm_i2c_pdata.pm_lat =
		msm7x27_pm_data[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN]
		.latency;
	else
		msm_i2c_pdata.pm_lat =
		msm7x25_pm_data[MSM_PM_SLEEP_MODE_POWER_COLLAPSE_NO_XO_SHUTDOWN]
		.latency;

	msm_device_i2c.dev.platform_data = &msm_i2c_pdata;
}

#ifdef CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5206
#define FT5206_I2C_BUSID	2		// MSM-I2C(0), MSM-AUX-I2C(1), TSP(2)
#define FT5206_I2C_ADDRESS 0x38	// 0x70 for write, 0x71 for read
#define FT5206_I2C_IRQ_PIN	19
#define FT5206_I2C_SCL_PIN	93
#define FT5206_I2C_SDA_PIN	107

#include <linux/ft_i2c.h>

static int ft_tsp_power(int on)
{
	struct vreg *vreg_gp6;
	int rc;

	/* power on */
	vreg_gp6 = vreg_get(NULL, "gp6");
	if (IS_ERR(vreg_gp6)) {
		printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
			__func__, "gp6", PTR_ERR(vreg_gp6));
		return -1;
	}
	rc = vreg_set_level(vreg_gp6, 2800);	// 2.80V <- 3.05V 20110413
	if (rc) {
		printk(KERN_ERR "%s: GP6 set_level failed (%d)\n",
			__func__, rc);
	}

	if (on) {
		rc = vreg_enable(vreg_gp6);
		if (rc) {
			printk(KERN_ERR "%s: GP6 enable failed (%d)\n",
				 __func__, rc);
		}
//#ifndef CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5202_POWER_OFF	// 20110413
		gpio_tlmm_config(GPIO_CFG(FT5206_I2C_SCL_PIN, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(FT5206_I2C_SDA_PIN, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(FT5206_I2C_IRQ_PIN, 0,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
//#endif
	} else {
//#ifndef CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5202_POWER_OFF	// 20110413
		gpio_tlmm_config(GPIO_CFG(FT5206_I2C_SCL_PIN, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(FT5206_I2C_SDA_PIN, 0,
			GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_tlmm_config(GPIO_CFG(FT5206_I2C_IRQ_PIN, 0,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
//#endif
		rc = vreg_disable(vreg_gp6);
		if (rc) {
			printk(KERN_ERR "%s: GP6 disable failed (%d)\n",
				 __func__, rc);
		}
	}

	return 0;
}

static struct ft_i2c_platform_data ft_i2c_data[] = {
	{
		.version = 0x0000,
		.abs_x_min = 0,
		.abs_x_max = 320,
		.abs_y_min = 0,
		.abs_y_max = 480,
		.intr_gpio = FT5206_I2C_IRQ_PIN,
		.power = ft_tsp_power,
		.scl_pin = FT5206_I2C_SCL_PIN,
		.sda_pin = FT5206_I2C_SDA_PIN,
	}
};

static struct i2c_board_info ft_i2c_bdinfo[] = {
	{
		I2C_BOARD_INFO("ft5206", FT5206_I2C_ADDRESS),
		.irq = MSM_GPIO_TO_INT(FT5206_I2C_IRQ_PIN),
		.platform_data = &ft_i2c_data,
	}, 
};

static struct gpio_i2c_pin {
	unsigned int sda_pin;
	unsigned int scl_pin;
	unsigned int reset_pin;
	unsigned int irq_pin;
} ft_i2c_pin[] = {
	{
		.sda_pin	= FT5206_I2C_SDA_PIN,
		.scl_pin	= FT5206_I2C_SCL_PIN,
		.reset_pin	= 35,/* Modifided by yubin,  for aeneas */
		.irq_pin	= FT5206_I2C_IRQ_PIN,
	},
};

static struct i2c_gpio_platform_data ft_i2c_pdata = {
	.sda_pin = FT5206_I2C_SDA_PIN,
	.scl_pin = FT5206_I2C_SCL_PIN,
	.sda_is_open_drain = 0,
	.scl_is_open_drain = 0,
	.udelay = 2, /* 400K for Fast mode */
};

static struct platform_device ft_i2c_device = {
	.id = FT5206_I2C_BUSID,
	.name = "i2c-gpio",
	.dev.platform_data = &ft_i2c_pdata,
};

void msm7x27_gpio_i2c_pin(struct i2c_gpio_platform_data *i2c_adap_pdata,
		struct gpio_i2c_pin *gpio_i2c_pin,
		struct i2c_board_info *i2c_board_info_data)
{
	gpio_tlmm_config(GPIO_CFG(gpio_i2c_pin->sda_pin, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_tlmm_config(GPIO_CFG(gpio_i2c_pin->scl_pin, 0, GPIO_CFG_OUTPUT,
				GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
	gpio_set_value(gpio_i2c_pin->sda_pin, 1);
	gpio_set_value(gpio_i2c_pin->scl_pin, 1);

	if (gpio_i2c_pin->reset_pin) {
		gpio_tlmm_config(GPIO_CFG(gpio_i2c_pin->reset_pin, 0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		gpio_set_value(gpio_i2c_pin->reset_pin, 1);
	}

	if (gpio_i2c_pin->irq_pin) {
		gpio_tlmm_config(GPIO_CFG(gpio_i2c_pin->irq_pin, 0, GPIO_CFG_INPUT,
					GPIO_CFG_PULL_UP, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		i2c_board_info_data->irq =
			MSM_GPIO_TO_INT(gpio_i2c_pin->irq_pin);
	}
}

#endif

static void usb_mpp_init(void)
{
	unsigned rc;
	unsigned mpp_usb = 7;

#ifdef CONFIG_M7LAB_VENUS	//	Teddy 2011-01-17
	{
#else
	if (machine_is_msm7x25_ffa() || machine_is_msm7x27_ffa()) {
#endif	//	CONFIG_M7LAB_VENUS
		rc = mpp_config_digital_out(mpp_usb,
			MPP_CFG(MPP_DLOGIC_LVL_VDD,
				MPP_DLOGIC_OUT_CTRL_HIGH));
		if (rc)
			pr_err("%s: configuring mpp pin"
				"to enable 3.3V LDO failed\n", __func__);
	}
}

static void msm7x27_wlan_init(void)
{
	int rc = 0;
	/* TBD: if (machine_is_msm7x27_ffa_with_wcn1312()) */
	platform_device_register(&msm_wifi_device);//add by yubin
#if 0  //add by yubin
	if (machine_is_msm7x27_ffa()) {
		rc = mpp_config_digital_out(3, MPP_CFG(MPP_DLOGIC_LVL_MSMP,
				MPP_DLOGIC_OUT_CTRL_LOW));
		if (rc)
			printk(KERN_ERR "%s: return val: %d \n",
				__func__, rc);
	}
#endif //add by yubin
}

#if 0  //add by yubin
#ifdef CONFIG_M7LAB_VENUS_BRCM	//	Teddy 2011-04-13
#define WL_REGON_PIN	PM_MPP_22
#define WL_RESET_PIN	PM_MPP_18
#define WL_WAKE_PIN		PM_MPP_16

void bcm_wlan_power_off(int mode)
{
#if 1
#if 0
	struct vreg *vreg_wl;
	int	rc = 0;
#endif

	printk(KERN_ERR "%s : %d\n",__func__, mode);
	wifi_power = false;
	mpp_config_digital_out(WL_WAKE_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP,MPP_DLOGIC_OUT_CTRL_LOW));

	if( mode == 1 || mode == 2)
	{
		mpp_config_digital_out(WL_RESET_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_LOW));
		if( !bt_power )
			mpp_config_digital_out(WL_REGON_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_LOW));
	}
	else if( mode == 3 )//unused
	{
		mpp_config_digital_out(WL_RESET_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_LOW));
	}
	msm_sdcc_setup_gpio(2, 0);

#if 0
	vreg_wl = vreg_get(NULL, "gp4");
	if (IS_ERR(vreg_wl))
	{
		printk(KERN_ERR "%s: vreg get failed (%ld)\n",
		       __func__, PTR_ERR(vreg_wl));
		return;
	}

	if( !bt_power )
		rc = vreg_disable(vreg_wl);
	if (rc) {
		printk(KERN_ERR "%s: vreg disable failed (%d)\n", __func__, rc);
		return ;
	}
#endif
#endif
}
void bcm_wlan_power_on(int mode)
{
	struct vreg *vreg_wl;
	int rc = 0;

	printk(KERN_ERR "%s : %d\n",__func__, mode);

	wifi_power = true;
	vreg_wl = vreg_get(NULL, "gp4");
	if (IS_ERR(vreg_wl))
	{
		printk(KERN_ERR "%s: vreg get failed (%ld)\n",
		       __func__, PTR_ERR(vreg_wl));
		return;
	}

	/* units of mV, steps of 50 mV */
	rc = vreg_set_level(vreg_wl, 2600);
	if (rc)
	{
		printk(KERN_ERR "%s: vreg set level failed (%d)\n", __func__, rc);
		return;
	}

	msm_sdcc_setup_gpio(2, 1);
	mdelay(10);

	rc = vreg_enable(vreg_wl);
	if (rc)
	{
		printk(KERN_ERR "%s: vreg enable failed (%d)\n", __func__, rc);
		return;
	}

	if(mode == 1||mode == 2)
	{
		mpp_config_digital_out(WL_RESET_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_LOW));
		mdelay(100);
		mpp_config_digital_out(WL_REGON_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_HIGH));
		mpp_config_digital_out(WL_RESET_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_HIGH));
	}
	else if(mode == 3)//unused
	{
		mpp_config_digital_out(WL_RESET_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP, MPP_DLOGIC_OUT_CTRL_HIGH));
	}

#if 1
	rc = mpp_config_digital_out(WL_WAKE_PIN, MPP_CFG(MPP_DLOGIC_LVL_MSMP,MPP_DLOGIC_OUT_CTRL_HIGH));
	if(rc != 0)
		printk(KERN_ERR "%s: cannot set WL_WAKE to output %d\n", __func__, rc);
#endif
}
EXPORT_SYMBOL(bcm_wlan_power_off);
EXPORT_SYMBOL(bcm_wlan_power_on);
#endif	//	CONFIG_M7LAB_VENUS_BRCM
#endif //add by yubin

//leesh_sensor
#ifdef CONFIG_M7LAB_VENUS_SENSOR
#define AKM8963_I2C_NAME		"akm8963"
#define AKM8963_I2C_ADDR		0x0e
//------------------------------- Magentic sensor --------------------------------------
static struct i2c_board_info akm8963_sensor_i2c_bdinfo[] = {
	{
		I2C_BOARD_INFO(AKM8963_I2C_NAME/*akm8963*/, AKM8963_I2C_ADDR/*0x30*/),
	},
};

static struct gpio_i2c_pin akm8963_sensor_i2c_pin[] = {
	{
		.sda_pin	= 76,
		.scl_pin	= 91,
	},
};

static struct i2c_gpio_platform_data akm8963_sensor_i2c_pdata = {
	.sda_pin = 76,
	.scl_pin =91,
	.sda_is_open_drain = 0,
	.scl_is_open_drain = 0,
	.udelay = 2, /* 400K for Fast mode */
};

static struct platform_device akm8963_sensor_i2c_device = {
	.id = 5,
	.name = "i2c-gpio",
	.dev.platform_data = &akm8963_sensor_i2c_pdata,
};
//------------------------------- Accelometer sensor --------------------------------------
static struct i2c_board_info bma222_sensor_i2c_bdinfo[] = {
	{
		I2C_BOARD_INFO("bma222", BMA222_I2C_ADDR),
	},
};

static struct gpio_i2c_pin bma222_sensor_i2c_pin[] = {
	{
		.sda_pin	= 16,
		.scl_pin	= 17,
	},
};

static struct i2c_gpio_platform_data bma222_sensor_i2c_pdata = {
	.sda_pin = 16,
	.scl_pin =17,
	.sda_is_open_drain = 0,
	.scl_is_open_drain = 0,
	.udelay = 2, /* 400K for Fast mode */
};

static struct platform_device bma222_sensor_i2c_device = {
	.id = 6,
	.name = "i2c-gpio",
	.dev.platform_data = &bma222_sensor_i2c_pdata,
};
//------------------------------- proximety sensor --------------------------------------
static struct i2c_board_info taos_sensor_i2c_bdinfo[] = {
	{
		I2C_BOARD_INFO("taos", 0x39),
		//.irq = MSM_GPIO_TO_INT(94),
		//.platform_data = &ft_i2c_data,
	},
};

static struct gpio_i2c_pin taos_sensor_i2c_pin[] = {
	{
		.sda_pin	= 108,
		.scl_pin	= 109,
		.reset_pin = 0,
		.irq_pin	= 0,
	},
};

static struct i2c_gpio_platform_data taos_sensor_i2c_pdata = {
	.sda_pin = 108,
	.scl_pin =109,
	.sda_is_open_drain = 0,
	.scl_is_open_drain = 0,
	.udelay = 2, /* 400K for Fast mode */
};

static struct platform_device taos_sensor_i2c_device = {
	.id = 7,
	.name = "i2c-gpio",
	.dev.platform_data = &taos_sensor_i2c_pdata,
};
//------------------------------- Ligth sensor --------------------------------------
#endif	//	CONFIG_M7LAB_VENUS_SENSOR
//leesh_sensor

static void __init msm7x2x_init(void)
{
	unsigned smem_size;

#ifdef CONFIG_ARCH_MSM7X25
	msm_clock_init(msm_clocks_7x25, msm_num_clocks_7x25);
#elif defined(CONFIG_ARCH_MSM7X27)
	msm_clock_init(msm_clocks_7x27, msm_num_clocks_7x27);
#endif

#if defined(CONFIG_SMC91X)
	if (machine_is_msm7x25_ffa() || machine_is_msm7x27_ffa()) {
		smc91x_resources[0].start = 0x98000300;
		smc91x_resources[0].end = 0x980003ff;
		smc91x_resources[1].start = MSM_GPIO_TO_INT(85);
		smc91x_resources[1].end = MSM_GPIO_TO_INT(85);
		if (gpio_tlmm_config(GPIO_CFG(85, 0,
					      GPIO_CFG_INPUT,
					      GPIO_CFG_PULL_DOWN,
					      GPIO_CFG_2MA),
				     GPIO_CFG_ENABLE)) {
			printk(KERN_ERR
			       "%s: Err: Config GPIO-85 INT\n",
				__func__);
		}
	}
#endif

	if (cpu_is_msm7x27())
		msm7x2x_clock_data.max_axi_khz = 200000;

	msm_acpu_clock_init(&msm7x2x_clock_data);

#ifdef CONFIG_ARCH_MSM7X27
	/* This value has been set to 160000 for power savings. */
	/* OEMs may modify the value at their discretion for performance */
	/* The appropriate maximum replacement for 160000 is: */
	/* clk_get_max_axi_khz() */
	kgsl_pdata.high_axi_3d = 160000;

	/* 7x27 doesn't allow graphics clocks to be run asynchronously to */
	/* the AXI bus */
	kgsl_pdata.max_grp2d_freq = 0;
	kgsl_pdata.min_grp2d_freq = 0;
	kgsl_pdata.set_grp2d_async = NULL;
	kgsl_pdata.max_grp3d_freq = 0;
	kgsl_pdata.min_grp3d_freq = 0;
	kgsl_pdata.set_grp3d_async = NULL;
	kgsl_pdata.imem_clk_name = "imem_clk";
	kgsl_pdata.grp3d_clk_name = "grp_clk";
	kgsl_pdata.grp3d_pclk_name = "grp_pclk";
	kgsl_pdata.grp2d0_clk_name = NULL;
	kgsl_pdata.idle_timeout_3d = HZ/5;
	kgsl_pdata.idle_timeout_2d = 0;

#ifdef CONFIG_KGSL_PER_PROCESS_PAGE_TABLE
	kgsl_pdata.pt_va_size = SZ_32M;
	/* Maximum of 32 concurrent processes */
	kgsl_pdata.pt_max_count = 32;
#else
	kgsl_pdata.pt_va_size = SZ_128M;
	/* We only ever have one pagetable for everybody */
	kgsl_pdata.pt_max_count = 1;
#endif
#endif
	usb_mpp_init();

#ifdef CONFIG_USB_FUNCTION
	msm_hsusb_pdata.swfi_latency =
		msm7x27_pm_data
		[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].latency;

	msm_device_hsusb_peripheral.dev.platform_data = &msm_hsusb_pdata;
#endif

#ifdef CONFIG_USB_MSM_OTG_72K
	msm_device_otg.dev.platform_data = &msm_otg_pdata;
	if (machine_is_msm7x25_surf() || machine_is_msm7x25_ffa()) {
		msm_otg_pdata.pemp_level =
			PRE_EMPHASIS_WITH_20_PERCENT;
		msm_otg_pdata.drv_ampl = HS_DRV_AMPLITUDE_5_PERCENT;
		msm_otg_pdata.cdr_autoreset = CDR_AUTO_RESET_ENABLE;
		msm_otg_pdata.phy_reset = msm_otg_rpc_phy_reset;
	}
	if (machine_is_msm7x27_surf() || machine_is_msm7x27_ffa()) {
		msm_otg_pdata.pemp_level =
			PRE_EMPHASIS_WITH_10_PERCENT;
		msm_otg_pdata.drv_ampl = HS_DRV_AMPLITUDE_5_PERCENT;
		msm_otg_pdata.cdr_autoreset = CDR_AUTO_RESET_DISABLE;
		msm_otg_pdata.phy_reset_sig_inverted = 1;
	}

#ifdef CONFIG_USB_GADGET
	msm_otg_pdata.swfi_latency =
		msm7x27_pm_data
		[MSM_PM_SLEEP_MODE_RAMP_DOWN_AND_WAIT_FOR_INTERRUPT].latency;
	msm_device_gadget_peripheral.dev.platform_data = &msm_gadget_pdata;
	msm_gadget_pdata.is_phy_status_timer_on = 1;
#endif
#endif
#if defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE)
	msm_device_tsif.dev.platform_data = &tsif_platform_data;
#endif

/* Only one LCD for Aeneas now
//add by peng.xiong
	if(msm7x27_lcd_id_detect()==0x01221526)
	{
		lcdc_panel_device.name = "lcdc_r61526_rgb";//0x01221526
		
	}
    else 
	{	if(msm7x27_lcd_id_detect_hx8347h()==0x79)
		{
			lcdc_panel_device.name = "lcdc_hx8347h_rgb"; //0x79
		}
		else
    		{
			lcdc_panel_device.name = "lcdc_ips1p5700_qvga";//0xffffffff
		}
	}
//end add by peng.xiong	
*/
#ifdef CONFIG_M7LAB_JUKE_USB	//	Teddy 2011-07-18
	if( socinfo_get_npi_mode() )
	{
		android_usb_pdata.num_products = ARRAY_SIZE(usb_products_npi),
		android_usb_pdata.products = usb_products_npi,
		android_usb_pdata.serial_number = "";
		android_usb_pdata.product_id = 0x00F6;
	}
#endif	//	CONFIG_M7LAB_JUKE_USB
	platform_add_devices(devices, ARRAY_SIZE(devices));
#ifdef CONFIG_MSM_CAMERA
	config_camera_off_gpios(); /* might not be necessary */
#endif
	msm_device_i2c_init();
	i2c_register_board_info(0, i2c_devices, ARRAY_SIZE(i2c_devices));

	/* for GPIO I2C Test */
#ifdef CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5206
	msm7x27_gpio_i2c_pin(&ft_i2c_pdata, &ft_i2c_pin[0], &ft_i2c_bdinfo[0]);
	i2c_register_board_info(FT5206_I2C_BUSID, &ft_i2c_bdinfo[0], ARRAY_SIZE(ft_i2c_bdinfo));
	platform_device_register(&ft_i2c_device); 
#endif //  CONFIG_TOUCHSCREEN_ELAN_TKCB032G

#ifdef CONFIG_SURF_FFA_GPIO_KEYPAD
	if (machine_is_msm7x25_ffa() || machine_is_msm7x27_ffa())
		platform_device_register(&keypad_device_7k_ffa);
	else
		platform_device_register(&keypad_device_surf);
#endif
#ifdef CONFIG_FB_MSM_LCDC_IPS1P5700_QVGA
	lcdc_ips1p5700_gpio_init();
#else
	lcdc_gordon_gpio_init();
#endif
	msm_fb_add_devices();
#ifdef CONFIG_USB_EHCI_MSM
	msm7x2x_init_host();
#endif
	msm7x2x_init_mmc();
	init_MUTEX( &BT_Wifi_Power_Lock); /* add a semaphore for BT&WIFI power control *///yubin add
	bt_power_init();

//leesh_sensor
#ifdef CONFIG_M7LAB_VENUS_SENSOR
	//------------------------------- Magentic sensor --------------------------------------
	msm7x27_gpio_i2c_pin(&akm8963_sensor_i2c_pdata, &akm8963_sensor_i2c_pin[0], akm8963_sensor_i2c_bdinfo);
	i2c_register_board_info(5, akm8963_sensor_i2c_bdinfo, ARRAY_SIZE(akm8963_sensor_i2c_bdinfo));
	platform_device_register(&akm8963_sensor_i2c_device);

	//------------------------------- Accelometer sensor --------------------------------------
	msm7x27_gpio_i2c_pin(&bma222_sensor_i2c_pdata, &bma222_sensor_i2c_pin[0], bma222_sensor_i2c_bdinfo);
	i2c_register_board_info(6, bma222_sensor_i2c_bdinfo, ARRAY_SIZE(bma222_sensor_i2c_bdinfo));
	platform_device_register(&bma222_sensor_i2c_device);
	//------------------------------- proximity sensor --------------------------------------
	msm7x27_gpio_i2c_pin(&taos_sensor_i2c_pdata, &taos_sensor_i2c_pin[0], taos_sensor_i2c_bdinfo);
	i2c_register_board_info(7, taos_sensor_i2c_bdinfo, ARRAY_SIZE(taos_sensor_i2c_bdinfo));
	platform_device_register(&taos_sensor_i2c_device);
//leesh_sensor
#endif	//	CONFIG_M7LAB_VENUS_SENSOR

	if (cpu_is_msm7x27())
		msm_pm_set_platform_data(msm7x27_pm_data,
					ARRAY_SIZE(msm7x27_pm_data));
	else
		msm_pm_set_platform_data(msm7x25_pm_data,
					ARRAY_SIZE(msm7x25_pm_data));
	msm7x27_wlan_init();
#ifdef CONFIG_M7LAB_VENUS_VIBRATOR	//	Teddy
	//msm_init_pmic_vibrator(); //by peng.xiong
#endif	//	CONFIG_M7LAB_VENUS_VIBRATOR
	boot_reason = *(unsigned int *)
		(smem_get_entry(SMEM_POWER_ON_STATUS_INFO, &smem_size));
	printk(KERN_NOTICE "Boot Reason = 0x%02x\n", boot_reason);
}

static unsigned pmem_kernel_ebi1_size = PMEM_KERNEL_EBI1_SIZE;
static int __init pmem_kernel_ebi1_size_setup(char *p)
{
	pmem_kernel_ebi1_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_kernel_ebi1_size", pmem_kernel_ebi1_size_setup);

static unsigned pmem_mdp_size = MSM_PMEM_MDP_SIZE;
static int __init pmem_mdp_size_setup(char *p)
{
	pmem_mdp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_mdp_size", pmem_mdp_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;
static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned pmem_audio_size = MSM_PMEM_AUDIO_SIZE;
static int __init pmem_audio_size_setup(char *p)
{
	pmem_audio_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_audio_size", pmem_audio_size_setup);

static unsigned fb_size = MSM_FB_SIZE;
static int __init fb_size_setup(char *p)
{
	fb_size = memparse(p, NULL);
	return 0;
}
early_param("fb_size", fb_size_setup);

static void __init msm_msm7x2x_allocate_memory_regions(void)
{
	void *addr;
	unsigned long size;

	size = pmem_mdp_size;
	if (size) {
		addr = alloc_bootmem(size);
		android_pmem_pdata.start = __pa(addr);
		android_pmem_pdata.size = size;
		pr_info("allocating %lu bytes at %p (%lx physical) for mdp "
			"pmem arena\n", size, addr, __pa(addr));
	}

	size = pmem_adsp_size;
	if (size) {
		addr = alloc_bootmem(size);
		android_pmem_adsp_pdata.start = __pa(addr);
		android_pmem_adsp_pdata.size = size;
		pr_info("allocating %lu bytes at %p (%lx physical) for adsp "
			"pmem arena\n", size, addr, __pa(addr));
	}

	size = pmem_audio_size;
	if (size) {
		addr = alloc_bootmem(size);
		android_pmem_audio_pdata.start = __pa(addr);
		android_pmem_audio_pdata.size = size;
		pr_info("allocating %lu bytes (at %lx physical) for audio "
			"pmem arena\n", size , __pa(addr));
	}

	size = fb_size ? : MSM_FB_SIZE;
	addr = alloc_bootmem(size);
	msm_fb_resources[0].start = __pa(addr);
	msm_fb_resources[0].end = msm_fb_resources[0].start + size - 1;
	pr_info("allocating %lu bytes at %p (%lx physical) for fb\n",
		size, addr, __pa(addr));

	size = pmem_kernel_ebi1_size;
	if (size) {
		addr = alloc_bootmem_aligned(size, 0x100000);
		android_pmem_kernel_ebi1_pdata.start = __pa(addr);
		android_pmem_kernel_ebi1_pdata.size = size;
		pr_info("allocating %lu bytes at %p (%lx physical) for kernel"
			" ebi1 pmem arena\n", size, addr, __pa(addr));
	}
}

static void __init msm7x2x_map_io(void)
{
	msm_map_common_io();
	msm_msm7x2x_allocate_memory_regions();

	if (socinfo_init() < 0)
		BUG();

#ifdef CONFIG_CACHE_L2X0
	if (machine_is_msm7x27_surf() || machine_is_msm7x27_ffa()) {
		/* 7x27 has 256KB L2 cache:
			64Kb/Way and 4-Way Associativity;
			evmon/parity/share disabled. */
		if ((SOCINFO_VERSION_MAJOR(socinfo_get_version()) > 1)
			|| ((SOCINFO_VERSION_MAJOR(socinfo_get_version()) == 1)
			&& (SOCINFO_VERSION_MINOR(socinfo_get_version()) >= 3)))
			/* R/W latency: 4 cycles; */
			l2x0_init(MSM_L2CC_BASE, 0x0006801B, 0xfe000000);
		else
			/* R/W latency: 3 cycles; */
			l2x0_init(MSM_L2CC_BASE, 0x00068012, 0xfe000000);
	}
#endif
}

MACHINE_START(MSM7X27_SURF, "QCT MSM7x27 SURF")
#ifdef CONFIG_MSM_DEBUG_UART
	.phys_io        = MSM_DEBUG_UART_PHYS,
	.io_pg_offst    = ((MSM_DEBUG_UART_BASE) >> 18) & 0xfffc,
#endif
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= msm7x2x_map_io,
	.init_irq	= msm7x2x_init_irq,
	.init_machine	= msm7x2x_init,
	.timer		= &msm_timer,
MACHINE_END

MACHINE_START(MSM7X27_FFA, "QCT MSM7x27 FFA")
#ifdef CONFIG_MSM_DEBUG_UART
	.phys_io        = MSM_DEBUG_UART_PHYS,
	.io_pg_offst    = ((MSM_DEBUG_UART_BASE) >> 18) & 0xfffc,
#endif
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= msm7x2x_map_io,
	.init_irq	= msm7x2x_init_irq,
	.init_machine	= msm7x2x_init,
	.timer		= &msm_timer,
MACHINE_END

MACHINE_START(MSM7X25_SURF, "QCT MSM7x25 SURF")
#ifdef CONFIG_MSM_DEBUG_UART
	.phys_io        = MSM_DEBUG_UART_PHYS,
	.io_pg_offst    = ((MSM_DEBUG_UART_BASE) >> 18) & 0xfffc,
#endif
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= msm7x2x_map_io,
	.init_irq	= msm7x2x_init_irq,
	.init_machine	= msm7x2x_init,
	.timer		= &msm_timer,
MACHINE_END

MACHINE_START(MSM7X25_FFA, "QCT MSM7x25 FFA")
#ifdef CONFIG_MSM_DEBUG_UART
	.phys_io        = MSM_DEBUG_UART_PHYS,
	.io_pg_offst    = ((MSM_DEBUG_UART_BASE) >> 18) & 0xfffc,
#endif
	.boot_params	= PHYS_OFFSET + 0x100,
	.map_io		= msm7x2x_map_io,
	.init_irq	= msm7x2x_init_irq,
	.init_machine	= msm7x2x_init,
	.timer		= &msm_timer,
MACHINE_END
