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
 *-------------------------------------------------------
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
//static int spi_sdo;
static int spi_sdi;
//static int spi_dac;

//begin by youweiqiang readID
#if 1
#define u32		unsigned int
#define OUTPUT 1
#define INPUT 0
#define spi_sdi_in gpio_get_value(103)

static void WriteSPI8bitX1(u32 Com);
static void ili9486l_reg_cmdSPI_9bit(u32 Com);
static void ili9486l_reg_dataSPI_9bit(u32 Reg);
static u32 ReadRSPI_9bit(u32 REG_INDEX,u32 pn);
static void ili9486l_read_id(void);



/*LCD backlight is controled by MC8416 chip,which has 4 level
*intensity.Check the mc8416 datasheet to get more infor.
*/
static void ili9486l_gpio_config( unsigned int pin, unsigned int dir )
{
	int rc;
	//int a=gpio_get_value(pin);
	if(dir==OUTPUT)
	{
		rc = gpio_tlmm_config(GPIO_CFG(pin,0, GPIO_CFG_OUTPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		if(rc){
			printk(KERN_ERR "\n-------config pin out ERROR---\n");
		}
	}
	else if(dir==INPUT)
	{
		rc = gpio_tlmm_config(GPIO_CFG(pin,0, GPIO_CFG_INPUT,
					GPIO_CFG_NO_PULL, GPIO_CFG_2MA), GPIO_CFG_ENABLE);
		if(rc){
			printk(KERN_ERR "\n-------config pin in ERROR---\n");
		}
	}
}

static void WriteSPI8bitX1(u32 Com)
{
	unsigned short i;
	gpio_set_value(spi_cs,0);
	for(i=0;i<8;i++)
	{   gpio_set_value(spi_sclk,0);
	if(Com&0x80) {gpio_set_value(spi_sdi,1);}
	else {gpio_set_value(spi_sdi,0);}
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sclk,1);
	Com=Com<<1;
	}
}

static void ili9486l_reg_cmdSPI_9bit(u32 Com)
{
	gpio_set_value(spi_cs,0); 
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sdi,0);
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sclk,1); 
	WriteSPI8bitX1(Com);
	gpio_set_value(spi_cs,1);
}

static void ili9486l_reg_dataSPI_9bit(u32 Reg)
{
	gpio_set_value(spi_cs,0); 
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sdi,1);
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sclk,1);
	WriteSPI8bitX1(Reg);
	gpio_set_value(spi_cs,1);
}

static u32 ReadRSPI_9bit(u32 REG_INDEX,u32 pn)
{
 	unsigned char i,j;
 	u32 p; 
 
	//ili9486l_reg_cmdSPI_9bit(0xB0);    //add to initial code.
	//ili9486l_reg_dataSPI_9bit(0x8F);
 
	ili9486l_reg_cmdSPI_9bit(0xFB);
	ili9486l_reg_dataSPI_9bit(0x10|(pn&0x0f));
 
 
	gpio_set_value(spi_cs,0); 
 
 
 
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sdi,0);
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sclk,1); 
	WriteSPI8bitX1(REG_INDEX);
	ili9486l_gpio_config(spi_sdi,INPUT); 
	
	for(i=0,p=0;i<8;i++)
	{ 
	gpio_set_value(spi_sclk,0);
	p=p<<1;
	j=spi_sdi_in;
	if(j&0x1)
		p=p|0x1;
	gpio_set_value(spi_sclk,1);
	}
	ili9486l_gpio_config(spi_sdi,OUTPUT); 

	
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sdi,0);
	gpio_set_value(spi_sclk,0);
	gpio_set_value(spi_sclk,1); 
	WriteSPI8bitX1(REG_INDEX);
	ili9486l_gpio_config(spi_sdi,INPUT); 
	

	for(i=0,p=0;i<8;i++)
	{ 
	gpio_set_value(spi_sclk,0);
	p=p<<1;
	j=spi_sdi_in;
	if(j&0x1)
		p=p|0x1;
	gpio_set_value(spi_sclk,1);
	}
	ili9486l_gpio_config(spi_sdi,OUTPUT); 


	gpio_set_value(spi_cs,1);
	return p;
}

static void ili9486l_read_id(void)
{


	unsigned int d,i;

	ili9486l_reg_cmdSPI_9bit(0xB0); //add to initial code.

	ili9486l_reg_dataSPI_9bit(0x8F);


 //read reg:d3

	printk("\n\n----d3----------");

	for(i=0;i<4;i++){

		d=ReadRSPI_9bit(0xd3,i);

	printk(", %x , ",d);

	}

	printk("--------------\n\n");


 //read reg:e0

	printk("\n\n----e0----------");

	for(i=0;i<15;i++){

		d=ReadRSPI_9bit(0xe0,i);

	printk(", %x , ",d);

	}

	printk("--------------\n\n");


 //read reg:e1

	printk("\n\n----e1----------");

	for(i=0;i<15;i++){

		d=ReadRSPI_9bit(0xe1,i);

	printk(", %x , ",d);

	}

	printk("--------------\n\n");

}


#endif
//end by youweiqiang readID


struct ili9486l_state_type {
	boolean disp_initialized;
	boolean display_on;
	boolean disp_powered_up;
};
static struct ili9486l_state_type ili9486l_state = { 0 };

//const int ili9486l_bk_intensity[8] = {0,16,15,14,4,3,2,1};
//unsigned int ili9486l_bk_intensity[7] = {0,1,2,3,4,5,6,};
//const int ili9486l_bk_intensity[8] = {0,14,12,10,8,6,4,2};
/*modified by xu qijun,bug 301006,make backlight good effect in low backlight rang,
because backlight hardware is too light in low backlight rang*/
//const int ili9486l_bk_intensity[17] = {0,16,16,16,14,13,11,10,9,8,7,6,5,4,3,2,1};
const int ili9486l_bk_intensity[17] = {0,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1};


static struct msm_panel_common_pdata *lcdc_ili9486l_pdata;

#define GPIO_LCD_BK 90		//lcd backlight
#define LCDC_BK_INTEN_MAX 13 //Alvin 16->13

static uint32_t set_gpio_bk_table[] = {
	GPIO_CFG(GPIO_LCD_BK, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA)
};

static uint32_t spi_sdi_input[] = {
	GPIO_CFG(103, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA)
};

static uint32_t spi_sdi_output[] = {
	GPIO_CFG(103, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA)
};

static uint32_t lcdc_gpio_table_sleep[] = {
	GPIO_CFG(132, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//sclk
	GPIO_CFG(131, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//cs
	GPIO_CFG(103, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//sdi
	//GPIO_CFG(102, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA),       //sdo
	//GPIO_CFG(88, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),        //id

	GPIO_CFG(90, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),	//bk
	//GPIO_CFG(96, 0, GPIO_INPUT, GPIO_PULL_DOWN, GPIO_2MA) //reset 
};

void config_lcdc_gpio_table(uint32_t * table, int len, unsigned enable);

/*
*It used index to indentify command or data tyep.1:data.0:cmd.
*/
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

/*
*Just test,not used here!!!.
*/
static int spi_test(void)
{
	int retval = 0;
	int tmp, i;

	spi_write(0, 0x0C);	//get pixel format.

	gpio_set_value(spi_cs, 0);	//put cs low
	config_lcdc_gpio_table(spi_sdi_input, 1, 1);	//change spi_sdi to input

	for (i = 7; i >= 0; i--) {
		gpio_set_value(spi_sclk, 0);
		udelay(0);	//_nop_();        
		gpio_set_value(spi_sclk, 1);
		udelay(0);	//_nop_(); 
		tmp = gpio_get_value(spi_sdi);

		if (tmp == 0)
			retval |= 0;
		else
			retval |= (0x1 << i);
	}

	gpio_set_value(spi_cs, 1);
	config_lcdc_gpio_table(spi_sdi_output, 1, 1);

	return 0;
}

static int ili9486l_reg_data(unsigned char data)
{
	spi_write(1, data);
	return 0;
}

static int ili9486l_reg_cmd(unsigned char cmd)
{
	spi_write(0, cmd);
	return 0;
}

static int spi_init(void)
{
	/* gpio setting */
	spi_sclk = *(lcdc_ili9486l_pdata->gpio_num);
	spi_cs = *(lcdc_ili9486l_pdata->gpio_num + 1);
	spi_sdi = *(lcdc_ili9486l_pdata->gpio_num + 2);
	//spi_sdo  = *(lcdc_ili9486l_pdata->gpio_num + 3);    

	/* set the output so that we dont disturb the slave device */
	gpio_set_value(spi_sclk, 1);
	gpio_set_value(spi_sdi, 1);

	/* set the chip select de-asserted */
	gpio_set_value(spi_cs, 1);

	return 0;
}

static int ili9486l_reset(void)
{
	mdelay(50); 
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_HIGH));
	mdelay(1); 
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));
	mdelay(10); 
	mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_HIGH));
	mdelay(100); 

	return 0;
}

static int ili9486l_init(void)
{	

	//read ID begin by youweiqiang
	#if 0
	ili9486l_reset();
	ili9486l_read_id();
	#endif
	//read ID end by youweiqiang
	
	ili9486l_reset();
	

        ili9486l_reg_cmd(0xC0);
        ili9486l_reg_data(0x0D);
        ili9486l_reg_data(0x0B);

        ili9486l_reg_cmd(0xC1);
        ili9486l_reg_data(0x43);

        ili9486l_reg_cmd(0XC5);
        ili9486l_reg_data(0x00);
        ili9486l_reg_data(0x51);

        ili9486l_reg_cmd(0xB1);
        ili9486l_reg_data(0xB0);
        ili9486l_reg_data(0x11);

        ili9486l_reg_cmd(0xC2);
        ili9486l_reg_data(0x22);

        ili9486l_reg_cmd(0xB4);
        ili9486l_reg_data(0x02);

        ili9486l_reg_cmd(0xB7);
        ili9486l_reg_data(0x86);

        /*CPT 3.5,v VCI=2.6V Gamma1.9*/
        ili9486l_reg_cmd(0xE0);
        ili9486l_reg_data(0x0F);
        ili9486l_reg_data(0x18);
        ili9486l_reg_data(0x15);
        ili9486l_reg_data(0x0C);
        ili9486l_reg_data(0x0E);
        ili9486l_reg_data(0x06);
        ili9486l_reg_data(0x48);
        ili9486l_reg_data(0xB7);
        ili9486l_reg_data(0x3B);
        ili9486l_reg_data(0x0A);
        ili9486l_reg_data(0x15);
        ili9486l_reg_data(0x08);
        ili9486l_reg_data(0x1A);
        ili9486l_reg_data(0x12);
        ili9486l_reg_data(0x00);

        ili9486l_reg_cmd(0XE1);
        ili9486l_reg_data(0x0F);
        ili9486l_reg_data(0x33);
        ili9486l_reg_data(0x2D);
        ili9486l_reg_data(0x0B);
        ili9486l_reg_data(0x0C);
        ili9486l_reg_data(0x05);
        ili9486l_reg_data(0x46);
        ili9486l_reg_data(0x56);
        ili9486l_reg_data(0x37);
        ili9486l_reg_data(0x04);
        ili9486l_reg_data(0x0F);
        ili9486l_reg_data(0x04);
        ili9486l_reg_data(0x23);
        ili9486l_reg_data(0x20);
        ili9486l_reg_data(0x00);

        ili9486l_reg_cmd(0XF2);
        ili9486l_reg_data(0x18);
        ili9486l_reg_data(0xA3);
        ili9486l_reg_data(0x12);
        ili9486l_reg_data(0x02);
        ili9486l_reg_data(0xB2);
        ili9486l_reg_data(0x12);
        ili9486l_reg_data(0xFF);
        ili9486l_reg_data(0x10);
        ili9486l_reg_data(0x00);

        ili9486l_reg_cmd(0XF1);       
        ili9486l_reg_data(0x36); 
        ili9486l_reg_data(0x04); 
        ili9486l_reg_data(0x00); 
        ili9486l_reg_data(0x3C); 
        ili9486l_reg_data(0X0F); 
        ili9486l_reg_data(0x8F); 

        ili9486l_reg_cmd(0XF8);
        ili9486l_reg_data(0x21);
        ili9486l_reg_data(0x04);

        ili9486l_reg_cmd(0x36);
        ili9486l_reg_data(0x08);

        ili9486l_reg_cmd(0x3A);
        ili9486l_reg_data(0x66);

        ili9486l_reg_cmd(0XB6);
        ili9486l_reg_data(0x22);//  RGG interface
        ili9486l_reg_data(0x22);



	ili9486l_reg_cmd(0xB0);    
	ili9486l_reg_data(0x01); //according to display controller signal polarity 0x0F-->0x0E

        ili9486l_reg_cmd(0x2A); 
        ili9486l_reg_data(0x00);
        ili9486l_reg_data(0x00);
        ili9486l_reg_data(0x01);    
        ili9486l_reg_data(0x3F);

        ili9486l_reg_cmd(0x2B); 
        ili9486l_reg_data(0x00);
        ili9486l_reg_data(0x00);
        ili9486l_reg_data(0x01);    
        ili9486l_reg_data(0xDF);

        ili9486l_reg_cmd(0xF9);  
        ili9486l_reg_data(0x00);  
        ili9486l_reg_data(0x08);  



	ili9486l_reg_cmd(0x11);//exit sleep mode
	mdelay(120);

	
	ili9486l_reg_cmd(0x29); //Display on
	mdelay(20);

	//ili9486l_reg_cmd(0x2C); //memary write
	
	
	//read ID begin by youweiqiang
	#if 0
	
	printk(KERN_ERR"YWQ-------Initial Sequence");

	ili9486l_read_id();

	#endif
	//read ID end by youweiqiang
	
//////////////////////////////////////////////////
//inital picture data write into LCD memory RAM///
//////////////////////////////////////////////////
//end
//LED light on
	
	return 0;
}


//Exit Standby/Sleep mode
static int ili9486l_sleep_exit(void)
{
	//ili9486l_init();
	printk(KERN_ERR"YWQ-------ili9486l_sleep_exit");
	ili9486l_reg_cmd(0x11); //sleep out
	mdelay(150);
	ili9486l_reg_cmd(0x29); //Display on
	mdelay(50);
	ili9486l_reg_cmd(0x2C); //Start GRAM write

	return 0;
//
//Refresh 'dispay picture' again.
//
}

//Enter standby/Sleep mode
static int ili9486l_sleep_enter(void)
{
	printk(KERN_ERR"YWQ-------ili9486l_sleep_enter");
	ili9486l_reg_cmd(0x28); //Display off
	mdelay(50);
	ili9486l_reg_cmd(0x10); //sleep in
	mdelay(120);

	return 0;
}

static int aw9656_control(const int step)
{
	int i,n;
	static unsigned int nr_pulse;
	static int prev_nr_pulse;

	//gpio_set_value(GPIO_LCD_BK, 0);
	//udelay(500);
	
	if (step == 0)
	{
		printk(KERN_ERR"YWQ------step = 0\n");
  		gpio_set_value(GPIO_LCD_BK, 0);	//turn off the backlight
		mdelay(5);
		prev_nr_pulse = 0;
		return 0;
	}
	
	printk(KERN_ERR"ili9486l BL:%d\n",step);

	nr_pulse = ili9486l_bk_intensity[step];
	
    printk(KERN_ERR"nr_pulse=%d\n",n,nr_pulse);

	if(nr_pulse < prev_nr_pulse)
		n = 16 + nr_pulse -prev_nr_pulse;
	else
		n = nr_pulse - prev_nr_pulse;
	//if(nr_pulse>16)
	//	nr_pulse = 16;

	//gpio_set_value(GPIO_LCD_BK, 1);
	//udelay(20);
	
    for (i = 0; i < n; i++){
		gpio_set_value(GPIO_LCD_BK, 0);
		udelay(1);
		gpio_set_value(GPIO_LCD_BK, 1);
		udelay(1);
	}

	prev_nr_pulse = nr_pulse;
	
	return 0;
}

static int lcdc_ili9486l_bk_setting(const int step)
{
	aw9656_control(step);
	
	return 0;
}

static void lcdc_ili9486l_set_backlight(struct msm_fb_data_type *mfd)
{
	if (ili9486l_state.display_on==TRUE)
		lcdc_ili9486l_bk_setting(mfd->bl_level);
}

static void ili9486l_disp_on(void)
{
	if (ili9486l_state.disp_powered_up && !ili9486l_state.display_on) {
		ili9486l_init();
		ili9486l_state.display_on = TRUE;
	}
}

static void ili9486l_disp_powerup(void)
{
	if (!ili9486l_state.disp_powered_up && !ili9486l_state.display_on) {
		/* Reset the hardware first */
		/* Include DAC power up implementation here */
		ili9486l_state.disp_powered_up = TRUE;
	}
}

int UTS_get_disp_state()
{
	return ili9486l_state.disp_initialized;
}

static int lcdc_ili9486l_panel_on(struct platform_device *pdev)
{
	if (!ili9486l_state.disp_initialized) {
		lcdc_ili9486l_pdata->panel_config_gpio(1);
		config_lcdc_gpio_table(set_gpio_bk_table, 1, 1);
		//spi_dac = *(lcdc_ili9486l_pdata->gpio_num + 4);
		//gpio_set_value(spi_dac, 0);
		//udelay(15);
		//gpio_set_value(spi_dac, 1);
		spi_init();	/* lcd needs spi */
		ili9486l_disp_powerup();
		ili9486l_disp_on();
		ili9486l_state.disp_initialized = TRUE;
	}
	printk(KERN_ERR"lcdc ili9486l panel on.\n");
	return 0;
}

static int lcdc_ili9486l_panel_off(struct platform_device *pdev)
{
	ili9486l_sleep_enter();
	config_lcdc_gpio_table(lcdc_gpio_table_sleep, ARRAY_SIZE(lcdc_gpio_table_sleep), 1);
	//mpp_config_digital_out(20, MPP_CFG(MPP_DLOGIC_LVL_MSME, MPP_DLOGIC_OUT_CTRL_LOW));
	ili9486l_reset();

	if (ili9486l_state.disp_powered_up && ili9486l_state.display_on) {
		lcdc_ili9486l_pdata->panel_config_gpio(0);
		ili9486l_state.display_on = FALSE;
		ili9486l_state.disp_initialized = FALSE;
	}

	printk(KERN_ERR"lcdc ili9486l panel off.\n");
	return 0;
}

static int __devinit ili9486l_probe(struct platform_device *pdev)
{
	if (pdev->id == 0) {
		lcdc_ili9486l_pdata = pdev->dev.platform_data;
		return 0;
	}
	msm_fb_add_device(pdev);
	lcdc_ili9486l_bk_setting(3);
	printk(KERN_ERR"lcd ili9486l probe.\n");
	return 0;
}

static struct platform_driver this_driver = {
	.probe = ili9486l_probe,
	.driver = {
		   .name = "lcdc_ili9486l_rgb",
		   },
};

static struct msm_fb_panel_data ili9486l_panel_data = {
	.on = lcdc_ili9486l_panel_on,
	.off = lcdc_ili9486l_panel_off,
	.set_backlight = lcdc_ili9486l_set_backlight,
};

static struct platform_device this_device = {
	.name = "lcdc_ili9486l_rgb",
	.id = 1,
	.dev = {
		.platform_data = &ili9486l_panel_data,
		}
};

static int __init lcdc_ili9486l_panel_init(void)
{
	int ret;
	struct msm_panel_info *pinfo;

	printk(KERN_ERR"lcd ili9486l panel initialize.\n");
#ifdef CONFIG_FB_MSM_TRY_MDDI_CATCH_LCDC_PRISM
	if (msm_fb_detect_client("lcdc_ili9486l_rgb"))
		return 0;
#endif
	ret = platform_driver_register(&this_driver);
	if (ret)
		return ret;

	pinfo = &ili9486l_panel_data.panel_info;
	pinfo->xres = 320;
	pinfo->yres = 480;
	pinfo->type = LCDC_PANEL;
	pinfo->pdest = DISPLAY_1;
	pinfo->wait_cycle = 0;
	pinfo->bl_max = LCDC_BK_INTEN_MAX;
	pinfo->bl_min = 0;
	pinfo->bpp = 18;
	pinfo->fb_num = 2;
	//pinfo->clk_rate = 658240;
	//pinfo->clk_rate = 2000000;
	//pinfo->clk_rate = 3000000;
	//pinfo->clk_rate = 4000000;
	//pinfo->clk_rate = 4500000;
	//pinfo->clk_rate = 4800000;
	pinfo->clk_rate = 8000000;

	pinfo->lcdc.h_back_porch = 8;
	pinfo->lcdc.h_front_porch = 8;
	pinfo->lcdc.h_pulse_width = 8;
	pinfo->lcdc.v_back_porch = 4;
	pinfo->lcdc.v_front_porch = 4;
	pinfo->lcdc.v_pulse_width = 3;
	pinfo->lcdc.border_clr = 0;
	pinfo->lcdc.underflow_clr = 0xff;
	pinfo->lcdc.hsync_skew = 4;

	ret = platform_device_register(&this_device);
	if (ret) {
		platform_driver_unregister(&this_driver);
	}

	return ret;
}
module_init(lcdc_ili9486l_panel_init);
