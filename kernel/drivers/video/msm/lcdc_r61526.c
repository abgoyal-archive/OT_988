/* Copyright (c) 2010, JRD Communication Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <linux/delay.h>
#include <mach/gpio.h>
#include "msm_fb.h"
#include <mach/mpp.h>
#include <linux/leds.h>
#include <mach/pmic.h>
#include <linux/workqueue.h>



static int spi_cs;
static int spi_sclk;
static int spi_sdi;

#define MAX_BRIGHT		14
#define LCDC_BK_INTEN_MAX 7

struct r61526_state_type {
	boolean disp_initialized;
	boolean display_on;
	boolean disp_powered_up;
};
static struct r61526_state_type r61526_state = { 0 };
const int r61526_bk_intensity[8] = {0,14,12,10,4,3,2,1};

static struct msm_panel_common_pdata *lcdc_r61526_pdata;

#define GPIO_LCD_BK 90

static uint32_t set_gpio_bk_table[] = {
	GPIO_CFG(GPIO_LCD_BK, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA)
};


static uint32_t lcdc_gpio_table_sleep[] = {
	GPIO_CFG(132, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//sclk
	GPIO_CFG(131, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//cs
	GPIO_CFG(103, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//sdi
	GPIO_CFG(90, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//bk	
};

extern void config_lcdc_gpio_table(uint32_t * table, int len, unsigned enable);

//spi write
static int spi_write(unsigned char index, unsigned char val)
{
	unsigned char sda;	
	int i;
	gpio_set_value(spi_cs, 0);

	gpio_set_value(spi_sdi, index);
	gpio_set_value(spi_sclk, 0);
	udelay(0);
	gpio_set_value(spi_sclk, 1);
	udelay(0);

	for (i = 7; i >= 0; i--) {
		sda = ((val >> i) & 0x1);
		gpio_set_value(spi_sdi, sda);
		gpio_set_value(spi_sclk, 0);
		udelay(0);
		gpio_set_value(spi_sclk, 1);
		udelay(0);
	}
	gpio_set_value(spi_cs, 1);

	return 0;
}

static int r61526_reg_data(unsigned char data)
{
	spi_write(1, data);
	return 0;
}

static int r61526_reg_cmd(unsigned char cmd)
{
	spi_write(0, cmd);
	return 0;
}

static int spi_init(void)
{	
	spi_sclk = *(lcdc_r61526_pdata->gpio_num);
	spi_cs = *(lcdc_r61526_pdata->gpio_num + 1);
	spi_sdi = *(lcdc_r61526_pdata->gpio_num + 2);
	
	gpio_set_value(spi_sclk, 1);
	gpio_set_value(spi_sdi, 1);

	gpio_set_value(spi_cs, 1);

	return 0;
}

static int r61526_reset(void)
{
	mdelay(10);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));
	mdelay(3);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_HIGH));
	mdelay(5);

	return 0;
}

static int r61526_init(void)
{
	r61526_reset();
	r61526_reg_cmd(0xB0);//Manufacturer Command Access Protect
	r61526_reg_data(0x3F);
	r61526_reg_data(0x3F);
	mdelay(50);

	r61526_reg_cmd(0xFE);    //?
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x21);
	r61526_reg_data(0xB4);

	r61526_reg_cmd(0xB3);//Frame Memory Access and Interface Setting
	r61526_reg_data(0x00);
	r61526_reg_data(0x10);//0X12--0929

	r61526_reg_cmd(0xE0);//NVM Access Control
	r61526_reg_data(0x00);//NVAE: NVM access enable register. NVM access is enabled when NVAE=1.
	r61526_reg_data(0x40);//FTT: NVM control bit.
	mdelay(50);

	r61526_reg_cmd(0xB3);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);

	r61526_reg_cmd(0xFE);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x21);
	r61526_reg_data(0x30);

	r61526_reg_cmd(0xB0);
	r61526_reg_data(0x3F);
	r61526_reg_data(0x3F);

	r61526_reg_cmd(0xB3);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);


	r61526_reg_cmd(0xC0);
	r61526_reg_data(0x06);//GIP REV  SM GS BGR SS   
	r61526_reg_data(0x4F);
	r61526_reg_data(0x00);
	r61526_reg_data(0x10);
	r61526_reg_data(0xA0);//BLV=0 LINE 
	r61526_reg_data(0x00);
	r61526_reg_data(0x04);
	r61526_reg_data(0x04);

//Frame frequency (fFRM)={fosc/(Clock per line¡Ádivision ratio¡Á(NL + FP + BP))} [Hz]
	r61526_reg_cmd(0xC1);
	r61526_reg_data(0x01);//BC0   Frame inversion(0)/Line inversion(1)
	r61526_reg_data(0x02);//DIV0[1:0]  800kHz
	r61526_reg_data(0x20);//RTN0[5:0]  38 clocks    21
	r61526_reg_data(0x08);//BP0[7:0]   8 lines   ,,,,BP,LCD back porch,normal mode
	r61526_reg_data(0x08);//FP0[7:0]   8 lines   ,,,,
	mdelay(20);

	r61526_reg_cmd(0xC3);  //PRTIAL MODE
	r61526_reg_data(0x01);//BC2
	r61526_reg_data(0x00);//DIV2[1:0]
	r61526_reg_data(0x21);//RTN2[5:0]
	r61526_reg_data(0x08);//BP2[7:0]    ,,,,partical mode,,back porch
	r61526_reg_data(0x08);//FP2[7:0]
	mdelay(20);

	r61526_reg_cmd(0xC4);
	r61526_reg_data(0x11);
	r61526_reg_data(0x01);
	r61526_reg_data(0x43);//00时全白画面VCL拉的太低，大概-1.59V
	r61526_reg_data(0x04);//00时全白画面VCL拉的太低，大概-1.59V，修改后全白画面VCL大概为-1.97V

	r61526_reg_cmd(0xC8);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x09);//0X0C--0929
	r61526_reg_data(0x05);//0X0B
	r61526_reg_data(0x10);//0X13
	r61526_reg_data(0x16);
	r61526_reg_data(0x13);
	r61526_reg_data(0x06);//0X08
	r61526_reg_data(0x08);//0X0C
	r61526_reg_data(0x03);//08
	r61526_reg_data(0x09);//06
	r61526_reg_data(0x03);//00
	r61526_reg_data(0x07);//08
	r61526_reg_data(0x09);//0C
	r61526_reg_data(0x09);
	r61526_reg_data(0x0E);//12
	r61526_reg_data(0x12);//17
	r61526_reg_data(0x12);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x10);
	r61526_reg_data(0x04);
	r61526_reg_data(0x0D);
	r61526_reg_data(0x00);

	r61526_reg_cmd(0xC9);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x0D);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x13);
	r61526_reg_data(0x16);
	r61526_reg_data(0x13);
	r61526_reg_data(0x08);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x03);
	r61526_reg_data(0x06);
	r61526_reg_data(0x00);
	r61526_reg_data(0x07);
	r61526_reg_data(0x09);
	r61526_reg_data(0x09);
	r61526_reg_data(0x0E);
	r61526_reg_data(0x12);
	r61526_reg_data(0x12);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x10);
	r61526_reg_data(0x04);
	r61526_reg_data(0x0D);
	r61526_reg_data(0x00);

	r61526_reg_cmd(0xCA);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x0D);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x13);
	r61526_reg_data(0x16);
	r61526_reg_data(0x13);
	r61526_reg_data(0x08);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x03);
	r61526_reg_data(0x06);
	r61526_reg_data(0x00);
	r61526_reg_data(0x07);
	r61526_reg_data(0x09);
	r61526_reg_data(0x09);
	r61526_reg_data(0x0E);
	r61526_reg_data(0x12);
	r61526_reg_data(0x12);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x10);
	r61526_reg_data(0x04);
	r61526_reg_data(0x0D);
	r61526_reg_data(0x00);

	r61526_reg_cmd(0xCB);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x0D);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x13);
	r61526_reg_data(0x16);
	r61526_reg_data(0x13);
	r61526_reg_data(0x08);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x03);
	r61526_reg_data(0x06);
	r61526_reg_data(0x00);
	r61526_reg_data(0x07);
	r61526_reg_data(0x09);
	r61526_reg_data(0x09);
	r61526_reg_data(0x0E);
	r61526_reg_data(0x12);
	r61526_reg_data(0x12);
	r61526_reg_data(0x0A);
	r61526_reg_data(0x10);
	r61526_reg_data(0x04);
	r61526_reg_data(0x0D);
	r61526_reg_data(0x00);

	r61526_reg_cmd(0xD0);
	r61526_reg_data(0x23);//BT[2:0]=110  VCI+VCI2¡Á2  :5     -(VCI2¡Á2):  -4
	r61526_reg_data(0x53);
	r61526_reg_data(0x82);//VC2[2:0]=010,VCI2=5V
	r61526_reg_data(0x30);//33,VREG=4.7V  

	r61526_reg_cmd(0xD1);
	r61526_reg_data(0x69);//VCOMH=VREG x 0.876      69
	r61526_reg_data(0x67);//VDV=VREG x 1.064

	r61526_reg_cmd(0xD2);//DC10[2:0], DC12[2:0]
	r61526_reg_data(0x03);
	r61526_reg_data(0x24);

	r61526_reg_cmd(0xD4);
	r61526_reg_data(0x03);
	r61526_reg_data(0x24);

	r61526_reg_cmd(0xE2);//NVM Load Control
	r61526_reg_data(0x3F);

	r61526_reg_cmd(0x35);//set_tear_on
	r61526_reg_data(0x00);

	r61526_reg_cmd(0x36);
	r61526_reg_data(0x00); //0x00

	r61526_reg_cmd(0x3A);//set_pixel_format
	r61526_reg_data(0x66);//55

	r61526_reg_cmd(0x2A);//set_column_address
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0xEF);

	r61526_reg_cmd(0x2B);//set_page_address:
	r61526_reg_data(0x00);
	r61526_reg_data(0x00);
	r61526_reg_data(0x01);
	r61526_reg_data(0x3F);

	r61526_reg_cmd(0x11);//exit_sleep_mode
	mdelay(150);

	r61526_reg_cmd(0x29);//set_display_on
	mdelay(50);

	r61526_reg_cmd(0xB4);
	r61526_reg_data(0x13);//00   12:DPI  11:VSYNC   10:Internal OSC

	r61526_reg_cmd(0x2C);//write_memory_start:
	mdelay(20);


	return 0;
	
}

static int r61526_sleep_enter(void)
{
	r61526_reg_cmd(0x28);
	r61526_reg_cmd(0x10);
	mdelay(120);
	return 0;
}

//lcd backlight
static int sn3326_control(const int step)
{
	int i;
	static unsigned int nr_pulse;

	gpio_set_value(GPIO_LCD_BK, 0);
	udelay(500);

	printk(KERN_ERR"r61526 BL:%d\n",step);

	nr_pulse = r61526_bk_intensity[step];
	if(nr_pulse>14)
		nr_pulse = 14;

	for (i = 0; i < nr_pulse; i++){
		gpio_set_value(GPIO_LCD_BK, 0);
		udelay(10);
		gpio_set_value(GPIO_LCD_BK, 1);
		udelay(10);
	}

	return 0;
}

//backlight
static int lcdc_r61526_bk_setting(const int step)
{
	if (step == 0)
		gpio_set_value(GPIO_LCD_BK, 0);
	 else
		sn3326_control(step);

	return 0;
}

static void lcdc_r61526_set_backlight(struct msm_fb_data_type *mfd)
{
	if (r61526_state.display_on==TRUE)
		lcdc_r61526_bk_setting(mfd->bl_level);
}

static void r61526_disp_on(void)
{
	if (r61526_state.disp_powered_up && !r61526_state.display_on) {
		r61526_init();
		r61526_state.display_on = TRUE;
	}
}

static void r61526_disp_powerup(void)
{
	if (!r61526_state.disp_powered_up && !r61526_state.display_on)		
		r61526_state.disp_powered_up = TRUE;	
}

static int lcdc_r61526_panel_on(struct platform_device *pdev)
{
	if (!r61526_state.disp_initialized) {
		lcdc_r61526_pdata->panel_config_gpio(1);
		config_lcdc_gpio_table(set_gpio_bk_table, 1, 1);		
		spi_init();
		r61526_disp_powerup();
		r61526_disp_on();
		r61526_state.disp_initialized = TRUE;
	}

	printk(KERN_ERR"\n\n lcd r61526 panel on.\n\n");
	return 0;
}

static int lcdc_r61526_panel_off(struct platform_device *pdev)
{
	r61526_sleep_enter();
	config_lcdc_gpio_table(lcdc_gpio_table_sleep, ARRAY_SIZE(lcdc_gpio_table_sleep), 1);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));

	if (r61526_state.disp_powered_up && r61526_state.display_on) {
		lcdc_r61526_pdata->panel_config_gpio(0);
		r61526_state.display_on = FALSE;
		r61526_state.disp_initialized = FALSE;
	}

	printk(KERN_ERR"\n\n lcd r61526 panel off.\n\n");
	return 0;
}

static int __devinit r61526_probe(struct platform_device *pdev)
{
	if (pdev->id == 0) {
		lcdc_r61526_pdata = pdev->dev.platform_data;
		return 0;
	}	
	msm_fb_add_device(pdev);
	lcdc_r61526_bk_setting(3);
	
	printk(KERN_ERR"lcd r61526 probe.\n");
	return 0;
}

static struct platform_driver this_driver = {
	.probe = r61526_probe,
	.driver = {
		   .name = "lcdc_r61526_rgb",
		   },
};

static struct msm_fb_panel_data r61526_panel_data = {
	.on = lcdc_r61526_panel_on,
	.off = lcdc_r61526_panel_off,
	.set_backlight = lcdc_r61526_set_backlight,
};

static struct platform_device this_device = {
	.name = "lcdc_r61526_rgb",
	.id = 1,
	.dev = {
		.platform_data = &r61526_panel_data,
		}
};

static int __init lcdc_r61526_panel_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

	printk(KERN_ERR"lcd r61526 panel initialize.\n");

//#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
	if (msm_fb_detect_client("lcdc_r61526_rgb"))
		return 0;
//#endif
	ret = platform_driver_register(&this_driver);
	if (ret)
		return ret;

	pinfo = &r61526_panel_data.panel_info;
	pinfo->xres = 240;
	pinfo->yres = 320;
	pinfo->type = LCDC_PANEL;
	pinfo->pdest = DISPLAY_1;
	pinfo->wait_cycle = 0;
	pinfo->bl_max = LCDC_BK_INTEN_MAX;
	pinfo->bl_min = 0;
	pinfo->bpp = 18;
	pinfo->fb_num = 2;
	pinfo->clk_rate = 4800000;   //pclk,,,,

//timing ,can modify
	pinfo->lcdc.h_back_porch = 8;
	pinfo->lcdc.h_front_porch = 8;
	pinfo->lcdc.h_pulse_width = 2;
	pinfo->lcdc.v_back_porch = 8;
	pinfo->lcdc.v_front_porch = 8;
	pinfo->lcdc.v_pulse_width = 5;
	pinfo->lcdc.border_clr = 0;
	pinfo->lcdc.underflow_clr = 0xff;
	pinfo->lcdc.hsync_skew = 4;


	ret = platform_device_register(&this_device);
	if (ret) {
		platform_driver_unregister(&this_driver);
	}

	return ret;
}
module_init(lcdc_r61526_panel_init);

