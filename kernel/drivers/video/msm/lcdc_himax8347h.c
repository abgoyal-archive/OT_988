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

struct hx8347h_state_type {
	boolean disp_initialized;
	boolean display_on;
	boolean disp_powered_up;
};
static struct hx8347h_state_type hx8347h_state = { 0 };
const int hx8347h_bk_intensity[8] = {0,14,12,10,4,3,2,1};

static struct msm_panel_common_pdata *lcdc_hx8347h_pdata;

#define GPIO_LCD_BK 90
#define LCDC_BK_INTEN_MAX 7

static uint32_t set_gpio_bk_table[] = {
	GPIO_CFG(GPIO_LCD_BK, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA)
};

static uint32_t lcdc_gpio_table_sleep[] = {
	GPIO_CFG(132, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//sclk
	GPIO_CFG(131, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//cs
	GPIO_CFG(103, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//sdi
	GPIO_CFG(90, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//bk	
};

void config_lcdc_gpio_table(uint32_t * table, int len, unsigned enable);

//spi write
static int spi_write_cmd(unsigned char val)
{
	unsigned char sda;	
	int i;
	unsigned char start_byte;
	gpio_set_value(spi_cs, 0);

	start_byte = 0x70;//01110 0 0 0: 01110 not changed; 0: IM0; 0: RS, command; 0: RW, write to LCD
	for (i = 7; i >= 0; i--) {
		sda = ((start_byte >> i) & 0x1);
		gpio_set_value(spi_sdi, sda);
		gpio_set_value(spi_sclk, 0);
		udelay(10);
		gpio_set_value(spi_sclk, 1);
		udelay(10);
	}
	
	for (i = 7; i >= 0; i--) {
		sda = ((val >> i) & 0x1);
		gpio_set_value(spi_sdi, sda);
		gpio_set_value(spi_sclk, 0);
		udelay(10);
		gpio_set_value(spi_sclk, 1);
		udelay(10);
	}
	gpio_set_value(spi_cs, 1);

	return 0;
}


static int spi_write_data(unsigned char val)
{
	unsigned char sda;	
	int i;
	unsigned char start_byte;
	gpio_set_value(spi_cs, 0);

	start_byte = 0x72;//01110 0 1 0: 01110 not changed; 0: IM0; 1: RS, data; 0: RW, write to LCD
	for (i = 7; i >= 0; i--) {
		sda = ((start_byte >> i) & 0x1);
		gpio_set_value(spi_sdi, sda);
		gpio_set_value(spi_sclk, 0);
		udelay(10);
		gpio_set_value(spi_sclk, 1);
		udelay(10);
	}
	
	for (i = 7; i >= 0; i--) {
		sda = ((val >> i) & 0x1);
		gpio_set_value(spi_sdi, sda);
		gpio_set_value(spi_sclk, 0);
		udelay(10);
		gpio_set_value(spi_sclk, 1);
		udelay(10);
	}
	gpio_set_value(spi_cs, 1);

	return 0;
}


static int hx8347h_reg_data(unsigned char data)
{
	spi_write_data(data);
	return 0;
}

static int hx8347h_reg_cmd(unsigned char cmd)
{
	spi_write_cmd(cmd);
	return 0;
}

static int spi_init(void)
{	
	spi_sclk = *(lcdc_hx8347h_pdata->gpio_num);
	spi_cs = *(lcdc_hx8347h_pdata->gpio_num + 1);
	spi_sdi = *(lcdc_hx8347h_pdata->gpio_num + 2);
	
	gpio_set_value(spi_sclk, 1);
	gpio_set_value(spi_sdi, 1);

	gpio_set_value(spi_cs, 1);

	return 0;
}

static int hx8347h_reset(void)
{
	mdelay(10);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));
	mdelay(10);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_HIGH));
	mdelay(120);

	return 0;
}

static int hx8347h_init(void)
{
printk(KERN_ERR"\n\n -------lcd hx8347h_init begin e e e e------.\n\n");
//int i;

hx8347h_reset();



hx8347h_reg_cmd(0xEA);hx8347h_reg_data(0x00); //PTBA[15:8]
hx8347h_reg_cmd(0xEB);hx8347h_reg_data(0x00); //PTBA[7:0]
hx8347h_reg_cmd(0xEC);hx8347h_reg_data(0x3C); //STBA[15:8]
hx8347h_reg_cmd(0xED);hx8347h_reg_data(0xC4); //STBA[7:0]
hx8347h_reg_cmd(0xE8);hx8347h_reg_data(0x30); //N_OPON[7:0] 50
hx8347h_reg_cmd(0xE9);hx8347h_reg_data(0x38); //I_OPON[7:0]
hx8347h_reg_cmd(0xF1);hx8347h_reg_data(0x01); //OTPS1B
hx8347h_reg_cmd(0xF2);hx8347h_reg_data(0x08); //GEN[7:0]
hx8347h_reg_cmd(0xDF);hx8347h_reg_data(0x52); //NVREF  52 
hx8347h_reg_cmd(0xE2);hx8347h_reg_data(0x02); //VREF  02
hx8347h_reg_cmd(0x2F);hx8347h_reg_data(0x22); //NW
//Gamma 2.2 Setting
hx8347h_reg_cmd(0x40);hx8347h_reg_data(0x00);
hx8347h_reg_cmd(0x41);hx8347h_reg_data(0x0c);
hx8347h_reg_cmd(0x42);hx8347h_reg_data(0x0c);
hx8347h_reg_cmd(0x43);hx8347h_reg_data(0x36);
hx8347h_reg_cmd(0x44);hx8347h_reg_data(0x38);
hx8347h_reg_cmd(0x45);hx8347h_reg_data(0x3f);
hx8347h_reg_cmd(0x46);hx8347h_reg_data(0x0e);
hx8347h_reg_cmd(0x47);hx8347h_reg_data(0x6f);
hx8347h_reg_cmd(0x48);hx8347h_reg_data(0x07);
hx8347h_reg_cmd(0x49);hx8347h_reg_data(0x0E);
hx8347h_reg_cmd(0x4A);hx8347h_reg_data(0x11);
hx8347h_reg_cmd(0x4B);hx8347h_reg_data(0x13);
hx8347h_reg_cmd(0x4C);hx8347h_reg_data(0x19);
hx8347h_reg_cmd(0x50);hx8347h_reg_data(0x00);
hx8347h_reg_cmd(0x51);hx8347h_reg_data(0x07);
hx8347h_reg_cmd(0x52);hx8347h_reg_data(0x09);
hx8347h_reg_cmd(0x53);hx8347h_reg_data(0x33);
hx8347h_reg_cmd(0x54);hx8347h_reg_data(0x33);
hx8347h_reg_cmd(0x55);hx8347h_reg_data(0x3f);
hx8347h_reg_cmd(0x56);hx8347h_reg_data(0x10);
hx8347h_reg_cmd(0x57);hx8347h_reg_data(0x71);
hx8347h_reg_cmd(0x58);hx8347h_reg_data(0x06);
hx8347h_reg_cmd(0x59);hx8347h_reg_data(0x0c);
hx8347h_reg_cmd(0x5A);hx8347h_reg_data(0x0e);
hx8347h_reg_cmd(0x5B);hx8347h_reg_data(0x11);
hx8347h_reg_cmd(0x5C);hx8347h_reg_data(0x18);
hx8347h_reg_cmd(0x5D);hx8347h_reg_data(0xFF);
//Power Voltage Setting
hx8347h_reg_cmd(0x1B);hx8347h_reg_data(0x1c); //VRH
hx8347h_reg_cmd(0x25);hx8347h_reg_data(0x1B); //NVRH
hx8347h_reg_cmd(0x1A);hx8347h_reg_data(0x06); //BT
hx8347h_reg_cmd(0x1D);hx8347h_reg_data(0x40); //FS0
hx8347h_reg_cmd(0x1E);hx8347h_reg_data(0x0f); //FS1
//VCOM
hx8347h_reg_cmd(0x23);hx8347h_reg_data(0x66); //for Flicker adjust //can reload from OTP
//Power on Setting
hx8347h_reg_cmd(0x18);hx8347h_reg_data(0x34); //I_RADJ N_RADJ Normal mode 60Hz
hx8347h_reg_cmd(0x19);hx8347h_reg_data(0x01); //OSC_EN='1' start Osc
hx8347h_reg_cmd(0x01);hx8347h_reg_data(0x00); //DP_STB='0' out deep sleep
hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xAC);
mdelay(5);
hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xA4);
mdelay(5);
hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xB4);
mdelay(150);
hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xF4);
mdelay(5);
hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xD4);
mdelay(5);


hx8347h_reg_cmd(0x31);hx8347h_reg_data(0x02);
hx8347h_reg_cmd(0x32);hx8347h_reg_data(0x0e);// high active
hx8347h_reg_cmd(0x33);hx8347h_reg_data(0x08);
hx8347h_reg_cmd(0x34);hx8347h_reg_data(0x08);


//262k/65k color selection
hx8347h_reg_cmd(0x17);hx8347h_reg_data(0x06); //default 0x06 262k color // 0x05 65k color//0x03 4k color
//SET PANEL
hx8347h_reg_cmd(0x36);hx8347h_reg_data(0x0F); //SS_P GS_P REV_P BGR_P
//Display ON Setting
hx8347h_reg_cmd(0x28);hx8347h_reg_data(0x38); //GON=1  DTE=1 D=10
mdelay(40);
hx8347h_reg_cmd(0x28);hx8347h_reg_data(0x3C); //GON=1 DTE=1 D=11
//Set GRAM Area
hx8347h_reg_cmd(0x02);hx8347h_reg_data(0x00);
hx8347h_reg_cmd(0x03);hx8347h_reg_data(0x00); //Column Start
hx8347h_reg_cmd(0x04);hx8347h_reg_data(0x00);
hx8347h_reg_cmd(0x05);hx8347h_reg_data(0xEF); //Column End
hx8347h_reg_cmd(0x06);hx8347h_reg_data(0x00);
hx8347h_reg_cmd(0x07);hx8347h_reg_data(0x00); //Row Start
hx8347h_reg_cmd(0x08);hx8347h_reg_data(0x01);
hx8347h_reg_cmd(0x09);hx8347h_reg_data(0x3F); //Row End
hx8347h_reg_cmd(0x16);hx8347h_reg_data(0x40);
//hx8347h_reg_cmd(0x22); //Start GRAM write
mdelay(40);

printk(KERN_ERR"\n lcd hx8347h_init end.\n");

#if 0

printk(KERN_ERR"\n lcd hx8347h_init test begin.\n");

for(i=0; i<50; i++)
{
	hx8347h_reg_cmd(0x5a);
	mdelay(200);
}
printk(KERN_ERR"\n lcd hx8347h_init test end.\n");
#endif

	return 0;
	
}

#if 0
static int hx8347h_sleep_out(void)
{
 hx8347h_reg_cmd(0x19);hx8347h_reg_data(0x01);//OSC_EN='1', start Osc
 mdelay(6);
 hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xAC); 
 mdelay(5); 
 hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xA4); 
 mdelay(5); 
 hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xB4); 
 mdelay(100); 
 hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xF4); 
 mdelay(5); 
 hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xD4); 
 mdelay(5); 
 hx8347h_reg_cmd(0x28);hx8347h_reg_data(0x38);//GON=1, DTE=1, D=1000
 mdelay(40);
 hx8347h_reg_cmd(0x28);hx8347h_reg_data(0x3C);//GON=1, DTE=1, D=1100
 return 0;
}
#endif

static int hx8347h_sleep_enter(void)
{
	hx8347h_reg_cmd(0x28);hx8347h_reg_data(0x38);//GON=¡¯1¡¯ DTE=¡¯1¡¯ D[1:0]=¡¯10¡¯
  mdelay(40);
  hx8347h_reg_cmd(0x28);hx8347h_reg_data(0x20);//GON=¡¯1¡¯ DTE=¡¯0¡¯ D[1:0]=¡¯01¡¯
  mdelay(5);
  hx8347h_reg_cmd(0x1F);hx8347h_reg_data(0xA9);//GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=1
  mdelay(5);
  hx8347h_reg_cmd(0x19);hx8347h_reg_data(0x00);//OSC_EN=¡¯0¡¯
	return 0;
}

//lcd backlight
static int sn3326_control(const int step)
{
	int i;
	static unsigned int nr_pulse;

	gpio_set_value(GPIO_LCD_BK, 0);
	udelay(500);

	printk(KERN_ERR"hx8347h BL:%d\n",step);

	nr_pulse = hx8347h_bk_intensity[step];
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
static int lcdc_hx8347h_bk_setting(const int step)
{
	if (step == 0)
		gpio_set_value(GPIO_LCD_BK, 0);
	 else
		sn3326_control(step);

	return 0;
}

static void lcdc_hx8347h_set_backlight(struct msm_fb_data_type *mfd)
{
	if (hx8347h_state.display_on==TRUE)
		lcdc_hx8347h_bk_setting(mfd->bl_level);
}

static void hx8347h_disp_on(void)
{
	if (hx8347h_state.disp_powered_up && !hx8347h_state.display_on) {
		hx8347h_init();
		hx8347h_state.display_on = TRUE;
	}
}

static void hx8347h_disp_powerup(void)
{
	if (!hx8347h_state.disp_powered_up && !hx8347h_state.display_on)		
		hx8347h_state.disp_powered_up = TRUE;	
}

static int lcdc_hx8347h_panel_on(struct platform_device *pdev)
{
	if (!hx8347h_state.disp_initialized) {
		lcdc_hx8347h_pdata->panel_config_gpio(1);
		config_lcdc_gpio_table(set_gpio_bk_table, 1, 1);		
		spi_init();
		hx8347h_disp_powerup();
		hx8347h_disp_on();
		hx8347h_state.disp_initialized = TRUE;
	}
	printk(KERN_ERR"\n --by wangpl-- lcd hx8347h panel on.\n");
	return 0;
}

static int lcdc_hx8347h_panel_off(struct platform_device *pdev)
{
	hx8347h_sleep_enter();
	config_lcdc_gpio_table(lcdc_gpio_table_sleep, ARRAY_SIZE(lcdc_gpio_table_sleep), 1);
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));

	if (hx8347h_state.disp_powered_up && hx8347h_state.display_on) {
		lcdc_hx8347h_pdata->panel_config_gpio(0);
		hx8347h_state.display_on = FALSE;
		hx8347h_state.disp_initialized = FALSE;
	}

	printk(KERN_ERR"\n --by wangpl-- lcd hx8347h panel off.\n");
	return 0;
}

static int __devinit hx8347h_probe(struct platform_device *pdev)
{
	if (pdev->id == 0) {
		lcdc_hx8347h_pdata = pdev->dev.platform_data;
		return 0;
	}	
	msm_fb_add_device(pdev);
	lcdc_hx8347h_bk_setting(3);
	
	printk(KERN_ERR"lcd hx8347h probe.\n");
	return 0;
}

static struct platform_driver this_driver = {
	.probe = hx8347h_probe,
	.driver = {
		   .name = "lcdc_hx8347h_rgb",
		   },
};

static struct msm_fb_panel_data hx8347h_panel_data = {
	.on = lcdc_hx8347h_panel_on,
	.off = lcdc_hx8347h_panel_off,
	.set_backlight = lcdc_hx8347h_set_backlight,
};

static struct platform_device this_device = {
	.name = "lcdc_hx8347h_rgb",
	.id = 1,
	.dev = {
		.platform_data = &hx8347h_panel_data,
		}
};

static int __init lcdc_hx8347h_panel_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

	printk(KERN_ERR"lcd hx8347h panel initialize.\n");

#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
	if (msm_fb_detect_client("lcdc_hx8347h_rgb"))
		return 0;
#endif
	ret = platform_driver_register(&this_driver);
	if (ret)
		return ret;

	pinfo = &hx8347h_panel_data.panel_info;
	pinfo->xres = 240;
	pinfo->yres = 320;
	pinfo->type = LCDC_PANEL;
	pinfo->pdest = DISPLAY_1;
	pinfo->wait_cycle = 0;
	pinfo->bl_max = LCDC_BK_INTEN_MAX;
	pinfo->bl_min = 0;
	pinfo->bpp = 18;
	pinfo->fb_num = 2;
//	pinfo->clk_rate = 4800000;   //pclk,,,,
	pinfo->clk_rate = 6000000;   //pclk,,,,
	
//timing ,can modify
	pinfo->lcdc.h_back_porch = 2;
	pinfo->lcdc.h_front_porch = 2; //4
	pinfo->lcdc.h_pulse_width = 2;
	pinfo->lcdc.v_back_porch = 2; // 4
	pinfo->lcdc.v_front_porch = 2; // 2
	pinfo->lcdc.v_pulse_width = 2; // 2
	pinfo->lcdc.border_clr = 0;
	pinfo->lcdc.underflow_clr = 0xff;
	pinfo->lcdc.hsync_skew = 4;


	ret = platform_device_register(&this_device);
	if (ret) {
		platform_driver_unregister(&this_driver);
	}

	return ret;
}
module_init(lcdc_hx8347h_panel_init);

