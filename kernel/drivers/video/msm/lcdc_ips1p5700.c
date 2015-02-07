/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Code Aurora Forum nor
 *       the names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 *
 * Alternatively, provided that this notice is retained in full, this software
 * may be relicensed by the recipient under the terms of the GNU General Public
 * License version 2 ("GPL") and only version 2, in which case the provisions of
 * the GPL apply INSTEAD OF those given above.  If the recipient relicenses the
 * software under the GPL, then the identification text in the MODULE_LICENSE
 * macro must be changed to reflect "GPLv2" instead of "Dual BSD/GPL".  Once a
 * recipient changes the license terms to the GPL, subsequent recipients shall
 * not relicense under alternate licensing terms, including the BSD or dual
 * BSD/GPL terms.  In addition, the following license statement immediately
 * below and between the words START and END shall also then apply when this
 * software is relicensed under the GPL:
 *
 * START
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 and only version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * END
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <linux/delay.h>
#include <mach/gpio.h>
#include "msm_fb.h"

#include <mach/mpp.h>
#include <mach/pmic.h>

// #define NO_REVERSE_FOR_TEST	// Test only by sinclair.lee

#if 0	// sinclair.lee for the test.defined(CONFIG_M7LAB_VENUS_LCD) && !defined(CONFIG_FRAMEBUFFER_CONSOLE)
// To turn the LCD on during boot-up sequence without CONFIG_FRAMEBUFFER_CONSOLE by sinclair.lee 20110324
#define ENABLE_FB_OPEN_ON_BOOT
#endif

#define MIN_BRIGHT		1
#define MAX_BRIGHT		14
#define GPIO_BL			90
#define FRAME_CYCLE_TIME	20

#define IPS1P5700_HFP		2	/* front porch */
#define IPS1P5700_HSW		2	/* hsync width */
#define IPS1P5700_HBP		2	/* back porch */
                        	
#define IPS1P5700_VFP		3	/* front porch 40 20110527 move spurious line position to out of sight */
#define IPS1P5700_VSW		2	/* vsync width */
#define IPS1P5700_VBP		2	/* back porch 40 20110527 move spurious line position to out of sight */

#define GPIO_OUT_132		132
#define GPIO_OUT_131		131
#define GPIO_OUT_103		103
#ifdef CONFIG_M7LAB_VENUS	// sdo(pin 102) Not used in Venus
#else
#define GPIO_OUT_102		102
#endif

static int spi_cs;
static int spi_sclk;
#ifdef CONFIG_M7LAB_VENUS	// sdo(pin 102) Not used in Venus
#else
static int spi_sdo;
#endif
static int spi_sdi;
static int spi_dac;

struct ips1p5700_state_type{
	boolean disp_initialized;
	boolean display_on;
	boolean disp_powered_up;
};

static struct ips1p5700_state_type ips1p5700_state = { 0 };
static struct msm_panel_common_pdata *lcdc_ips1p5700_pdata;


static inline void spi_lcd_den(int value)
{
	gpio_set_value(spi_cs, value);
}


static inline void spi_lcd_dclk(int value)
{

	gpio_set_value(spi_sclk, value);
}

static inline void spi_lcd_dseri(int value)
{
	gpio_set_value(spi_sdi, value);
}

static void spi_write(int address, int data)
{
	unsigned int delay = 2;		// 20110709 50 is too much;
	unsigned char dev_id = 0x1C; /* 011100 */
	int i;


	spi_lcd_den(1);
	spi_lcd_dclk(1);
	spi_lcd_dseri(1);
	udelay(delay);

	spi_lcd_den(0);
	udelay(delay);

	/* device ID */
	for (i = 5; i >= 0; i--) {
		spi_lcd_dclk(0);

		if ((dev_id >> i) & 0x1)
			spi_lcd_dseri(1);
		else
			spi_lcd_dseri(0);

		udelay(delay);

		spi_lcd_dclk(1);
		udelay(delay);
	}

	/* RS/RW set index register */
	spi_lcd_dclk(0);
	spi_lcd_dseri(0);
	udelay(delay);

	spi_lcd_dclk(1);
	udelay(delay);

	spi_lcd_dclk(0);
	spi_lcd_dseri(0);
	udelay(delay);

	spi_lcd_dclk(1);
	udelay(delay);



	/* Write instruction - Address */
	for (i = 15; i >= 0; i--) {
		spi_lcd_dclk(0);

		if ((address >> i) & 0x1)
			spi_lcd_dseri(1);
		else
			spi_lcd_dseri(0);

		udelay(delay);

		spi_lcd_dclk(1);
		udelay(delay);
	}

	spi_lcd_dseri(1);
	udelay(delay * 10);

	spi_lcd_den(1);
	udelay(delay * 10);

	spi_lcd_den(0);
	udelay(delay);

	/* device ID */
	for (i = 5; i >= 0; i--) {
		spi_lcd_dclk(0);

		if ((dev_id >> i) & 0x1)
			spi_lcd_dseri(1);
		else
			spi_lcd_dseri(0);

		udelay(delay);

		spi_lcd_dclk(1);
		udelay(delay);

	}

	/* RS/RW set index register */
	spi_lcd_dclk(0);
	spi_lcd_dseri(1);
	udelay(delay);

	spi_lcd_dclk(1);
	udelay(delay);

	spi_lcd_dclk(0);
	spi_lcd_dseri(0);
	udelay(delay);

	spi_lcd_dclk(1);
	udelay(delay);
	
	/* Write instruction - Data*/
	for (i = 15; i >= 0; i--) {
		spi_lcd_dclk(0);

		if ((data >> i) & 0x1)
			spi_lcd_dseri(1);
		else
			spi_lcd_dseri(0);

		udelay(delay);

		spi_lcd_dclk(1);
		udelay(delay);

	}

	spi_lcd_den(1);
		
	udelay(delay);
}


static void spi_init(void)
{
	/* Setting the Default GPIO's  */
	spi_sclk = GPIO_OUT_132;	//*(lcdc_ips1p5700_pdata->gpio_num);
	spi_cs   = GPIO_OUT_131;	//*(lcdc_ips1p5700_pdata->gpio_num + 1);
	spi_sdi  = GPIO_OUT_103;	//*(lcdc_ips1p5700_pdata->gpio_num + 2);
#ifdef CONFIG_M7LAB_VENUS	// sdo(pin 102) Not used in Venus
#else
	spi_sdo  =GPIO_OUT_102;	 //*(lcdc_ips1p5700_pdata->gpio_num + 3);
#endif

	/* Set the output so that we dont disturb the slave device */
	gpio_set_value(spi_sclk, 1);
	gpio_set_value(spi_sdi, 1);

	/* Set the Chip Select De-asserted */
	gpio_set_value(spi_cs, 1);

}

static void ips1p5700_disp_backlight_on(int level)
{

	int i = 0;
	
	if (level > MAX_BRIGHT)
		level = MAX_BRIGHT;
	if (level < MIN_BRIGHT)
		level = MIN_BRIGHT;

	// tick count(output mA) : 1(20), 2(17), 3(14), 4(12), 5(10), 6(8.6), 7(7), 8(6), 9(5), 10(4.2), 11(3.6), 12(3), 13(2.4), 14(1.8)
	for (i = MAX_BRIGHT; i >= level; i--) {
		gpio_set_value(GPIO_BL, 0);
		udelay(2);
		gpio_set_value(GPIO_BL, 1);
		udelay(2);		
	}
	gpio_set_value(GPIO_BL, 1);
	udelay(600);	// should be over 500us
	
	printk("[DEBUG] ips1p5700_disp_backlight_on(%d) end \n", level);
}

static void ips1p5700_disp_backlight_off(void)
{
	gpio_set_value(GPIO_BL, 0);
	udelay(600);	// should be over 500us
}

static void lcdc_ips1p5700_panel_reset(void)
{
	/* Reset 1st */
	if(pmic_secure_mpp_control_digital_output(spi_dac, PM_MPP__DLOGIC__LVL_MSME, PM_MPP__DLOGIC_OUT__CTRL_HIGH))
 		printk("lcdc reset mpp setting error...\n");
	
	msleep_interruptible(10);	// 20111214 5->10 changed by sinclair.lee accept TCT suggestion
	pmic_secure_mpp_control_digital_output(spi_dac, PM_MPP__DLOGIC__LVL_MSME, PM_MPP__DLOGIC_OUT__CTRL_LOW);
	msleep_interruptible(10);
	pmic_secure_mpp_control_digital_output(spi_dac, PM_MPP__DLOGIC__LVL_MSME, PM_MPP__DLOGIC_OUT__CTRL_HIGH);
//	msleep_interruptible(100);	// 20111222 reblocked 20111214 added by sinclair.lee accept TCT suggestion
}

static void ips1p5700_disp_powerup(void)
{
	if (!ips1p5700_state.disp_powered_up && !ips1p5700_state.display_on) {
		/* Reset the hardware first */		
#ifndef IPS1P5700_NO_DEEPSLEEP	// 20111214 added by sinclair.lee accept TCT suggestion
		lcdc_ips1p5700_panel_reset();
#endif
		/* Include DAC power up implementation here */
	      ips1p5700_state.disp_powered_up = TRUE;
	}
}

static void ips1p5700_disp_init(void)
{
		/* ips1p5700_dispmode setting */
		spi_lcd_den(1);
		spi_lcd_dclk(1);
		spi_lcd_dseri(1);
		
		spi_write(0x0000, 0x0000);
		spi_write(0x0000, 0x0000);
		spi_write(0x0060, 0x2700);
		spi_write(0x0090, 0x0010);	// 20110718 requested by FT. 0x001A
		// by sinclair.lee from 0x0808
		spi_write(0x0008, (IPS1P5700_VBP<<8)|IPS1P5700_VFP);	// to fix a bug 20110527 swap VBP <-> VFP position
		spi_write(0x0030, 0x0505);
		spi_write(0x0031, 0x8a10);
		spi_write(0x0032, 0x0907);
		spi_write(0x0033, 0x0308);
		spi_write(0x0034, 0x0000);
		spi_write(0x0035, 0x0803);
		spi_write(0x0036, 0x8709);
		spi_write(0x0037, 0x100a);
		spi_write(0x0038, 0x0505);
		spi_write(0x0039, 0x3333);
		spi_write(0x000A, 0x0008);
		spi_write(0x0010, 0x0730);
		spi_write(0x0011, 0x0127);	// 20110719 requested by FT. 0x0237);
		spi_write(0x0012, 0x0198);
		spi_write(0x000E, 0x0020);
		spi_write(0x0013, 0x1500);	// to fix a bug 20110527 0x1800);
		spi_write(0x002A, 0x0075);
		spi_write(0x0012, 0x01BC);	// to fix a bug 20110527 0x01B8);
		msleep_interruptible(120);	// 150 -> 120 20111222 according to Truly's recommendation
		printk("power on sequence interruptible ...\n");

		//display setting
		spi_write(0x0000, 0x0001);
		spi_write(0x0001, 0x0100);
		spi_write(0x0002, 0x0700);
#ifndef NO_REVERSE_FOR_TEST
		spi_write(0x0003, 0x9000);	// to fix a bug 20110527 0x1000);
#else
		spi_write(0x0003, 0x1030);
#endif
		spi_write(0x0004, 0x0000);
		spi_write(0x0009, 0x0001);
		spi_write(0x000A, 0x0008);//turn on framesignal 
		spi_write(0x000C, 0x0000);
		spi_write(0x000D, 0x0000);
		spi_write(0x000E, 0x0030);
		// 20110527 0x0012 works but unstable. Right after spurious line test I change it to 0x1B again. 
		spi_write(0x000F, 0x001B);	 // by sinclair.lee should be differ from the recommendation. VSYNC/HSYNC are active high 0x001A <-- 0x0002

		spi_write(0x0050, 0x0000);
		spi_write(0x0051, 0x00Ef);
		spi_write(0x0052, 0x0000);
		spi_write(0x0053, 0x013F);
		spi_write(0x0061, 0x0000);
		spi_write(0x0080, 0x0000);
		spi_write(0x0081, 0x0000);
		spi_write(0x0082, 0x0000);
		spi_write(0x006A, 0x0000);
		spi_write(0x0092, 0x0200);
		spi_write(0x0093, 0x0202);
		spi_write(0x0001, 0x0100);
		spi_write(0x0002, 0x0700);
#ifndef NO_REVERSE_FOR_TEST
		spi_write(0x0003, 0x9030);	// to fix a bug 20110527 0x9080
#else
		spi_write(0x0003, 0x9030);
#endif

		spi_write(0x0001, 0x0000);	// to fix a bug 20110527 added
		spi_write(0x0060, 0xa700);	// to fix a bug 20110527 added

		spi_write(0x0020, 0x0000);
		spi_write(0x0021, 0x0000);
		spi_write(0x0007, 0x0100); 
		msleep_interruptible(30);	// 50 -> 30 20111222 according to Truly's recommendation
		spi_write(0x000C, 0x0100);	// 20110718 Requested by FT 0x0110
		msleep_interruptible(30);	// 50 -> 30 20111222 according to Truly's recommendation
		spi_write(0x0022, 0x0000);		// ?? without data ??? not sure....
		msleep_interruptible(30);	// 40 -> 30 20111222 according to Truly's recommendation
}

static void ips1p5700_disp_on(void)
{
	if (ips1p5700_state.disp_powered_up && !ips1p5700_state.display_on) {
		ips1p5700_disp_init();
		printk("display on sequence interruptible ...\n");
 
		ips1p5700_state.display_on = TRUE;
	}
}

// will be never called
static void lcdc_ips1p5700_panel_exit_from_sleep(struct platform_device *pdev)
{
#ifndef IPS1P5700_NO_DEEPSLEEP	// final version
	ips1p5700_disp_init();
#else	// #ifndef IPS1P5700_NO_DEEPSLEEP	// final version
	spi_lcd_den(1);
	spi_lcd_dclk(1);
	spi_lcd_dseri(1);

#ifndef IPS1P5700_NO_DEEPSLEEP
	{
		int i;

		for(i=0; i<6; i++)
		{
			spi_lcd_den(0);			// 40ns < t
			spi_lcd_den(1);			// 25ns < t
			if(i == 1)
				msleep_interruptible(2);	// 1ms < t
		}

		spi_write(0xA4, 0x0001);		// CALB=1
		msleep_interruptible(2);		// 0.2ms < t
		spi_write(0x10, 0x0730);
	}
#endif

	spi_write(0x0012, 0x01BC);		// ?? without data ??? not sure....
	msleep_interruptible(130);
	spi_write(0x0007, 0x0100);		// ?? without data ??? not sure....
	msleep_interruptible(50);
#endif	// #ifndef IPS1P5700_NO_DEEPSLEEP	// final version
}

static int lcdc_ips1p5700_panel_on(struct platform_device *pdev)
{
	
	printk("[DEBUG] lcdc_ips1p5700_panel_on start \n");
	if (!ips1p5700_state.disp_initialized) {
		spi_init();	/* LCD needs SPI */				
		spi_dac = PM_MPP_21;

#ifdef IPS1P5700_NO_DEEPSLEEP	// final version 20111213 panel_off already issue reset
		/* Reset 1st */
		lcdc_ips1p5700_panel_reset();
#endif

		ips1p5700_disp_powerup();

		ips1p5700_disp_on();

		ips1p5700_state.disp_initialized = TRUE;
	}
	else
	{	// will be never called
		lcdc_ips1p5700_panel_exit_from_sleep(pdev);
	}
	printk("[DEBUG] lcdc_ips1p5700_panel_on end \n");

	return 0;
}


static int lcdc_ips1p5700_panel_off(struct platform_device *pdev)
{
	printk("[DEBUG] lcdc_ips1p5700_panel_off start \n");
	if (ips1p5700_state.disp_powered_up && ips1p5700_state.display_on) {
		spi_lcd_den(1);
		spi_lcd_dclk(1);
		spi_lcd_dseri(1);

#ifndef IPS1P5700_NO_DEEPSLEEP	// final version
		lcdc_ips1p5700_panel_reset();
		spi_write(0x10, 0x0004);
 		printk("lcd power off sequence done...\n");
		msleep_interruptible(100);
#else	// #ifndef IPS1P5700_NO_DEEPSLEEP	// final version
		// display off sequence
		spi_write(0x07, 0x0000);
		msleep_interruptible(20);
		spi_write(0x12, 0x0180);
 
 		printk("lcd power off sequence done...\n");
		msleep_interruptible(90);

#ifndef IPS1P5700_NO_DEEPSLEEP
		spi_write(0x10, 0x0714);
#endif
#endif	// #ifndef IPS1P5700_NO_DEEPSLEEP	// final version

		//lcdc_ips1p5700_pdata->panel_config_gpio(0);
		ips1p5700_state.display_on = FALSE;
		ips1p5700_state.disp_initialized = FALSE;
	}
	printk("[DEBUG] lcdc_ips1p5700_panel_off end \n");
	return 0;
}

static int lcdc_ips1p5700_backlight_value = -1;
static void lcdc_ips1p5700_set_backlight(struct msm_fb_data_type *mfd)
{
	int bl_level = mfd->bl_level;

	printk("[DEBUG] lcdc_ips1p5700_set_backlight start(%d) \n", bl_level);
	if (bl_level <= 1) {
		/* keep back light OFF */
		ips1p5700_disp_backlight_off();
		lcdc_ips1p5700_backlight_value = 0;
	} else {
		/* keep back light ON */
		if(lcdc_ips1p5700_backlight_value == 0)
			msleep_interruptible(100);
		ips1p5700_disp_backlight_on(bl_level);
		lcdc_ips1p5700_backlight_value = bl_level;
	}
	printk("[DEBUG] lcdc_ips1p5700_set_backlight(%d) end \n", bl_level);
}

#ifdef ENABLE_FB_OPEN_ON_BOOT
static int ips1p5700_fb_notified = 0;
static int ips1p5700_fb_event_notify(struct notifier_block *self,
			      unsigned long action, void *data)
{
	struct fb_event *event = data;
	struct fb_info *info = event->info;
	int ret = 0;

	if (ips1p5700_fb_notified == 0) {
		if (action == FB_EVENT_FB_REGISTERED && info->fbops->fb_open) {
			ret = info->fbops->fb_open(info, 0);
			ips1p5700_fb_notified = 1;
		}
	} else if (ips1p5700_fb_notified == 1) {
		struct msm_fb_data_type *mfd = (struct msm_fb_data_type *)info->par;
		if (3 < mfd->ref_cnt) {		// boot animation part
			if (info->fbops->fb_release) {
				ret = info->fbops->fb_release(info, 0);
			}
			ips1p5700_fb_notified = 2;
		}
	}

	return ret;
}


static struct notifier_block ips1p5700_fb_event_notifier = {
	.notifier_call	= ips1p5700_fb_event_notify,
};
#endif

static int __devinit ips1p5700_probe(struct platform_device *pdev)
{
	if (pdev->id == 0) {
		lcdc_ips1p5700_pdata = pdev->dev.platform_data;
		return 0;
	}
	msm_fb_add_device(pdev);
	return 0;
}

static struct platform_driver this_driver = {
	.probe  = ips1p5700_probe,
	.driver = {
		.name   = "lcdc_ips1p5700_qvga",
	},
};

static struct msm_fb_panel_data ips1p5700_panel_data = {
	.on = lcdc_ips1p5700_panel_on,
	.off = lcdc_ips1p5700_panel_off,
	.set_backlight = lcdc_ips1p5700_set_backlight,
};

static struct platform_device this_device = {
	.name   = "lcdc_ips1p5700_qvga",
	.id	= 1,
	.dev	= {
		.platform_data = &ips1p5700_panel_data,
	}
};

static int __init lcdc_ips1p5700_panel_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

//#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
	if (msm_fb_detect_client("lcdc_ips1p5700_qvga"))
		return 0;
//#endif
	ret = platform_driver_register(&this_driver);
	if (ret)
		return ret;

	pinfo = &ips1p5700_panel_data.panel_info;
	pinfo->xres = 240;
	pinfo->yres = 320;
	MSM_FB_SINGLE_MODE_PANEL(pinfo);
	pinfo->type = LCDC_PANEL;
	pinfo->pdest = DISPLAY_1;
	pinfo->wait_cycle = 0;
	pinfo->bpp = 18;
	pinfo->fb_num = 2;
	pinfo->clk_rate = 4800000; //20110715 Truly's recommendation
	pinfo->bl_max = MAX_BRIGHT;
	pinfo->bl_min = MIN_BRIGHT;
	
	pinfo->lcdc.h_back_porch = IPS1P5700_HBP;
	pinfo->lcdc.h_front_porch = IPS1P5700_HFP;
	pinfo->lcdc.h_pulse_width = IPS1P5700_HSW;
	pinfo->lcdc.v_back_porch = IPS1P5700_VBP;
	pinfo->lcdc.v_front_porch = IPS1P5700_VFP;
	pinfo->lcdc.v_pulse_width = IPS1P5700_VSW;
	pinfo->lcdc.border_clr = 0;     		/* blk */
	pinfo->lcdc.underflow_clr = 0xff;		/* blue */
	pinfo->lcdc.hsync_skew = 0;

#ifdef ENABLE_FB_OPEN_ON_BOOT
	fb_register_client(&ips1p5700_fb_event_notifier);
#endif

	ret = platform_device_register(&this_device);
	if (ret)
		platform_driver_unregister(&this_driver);

	return ret;
}

module_init(lcdc_ips1p5700_panel_init);

