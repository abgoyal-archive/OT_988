/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
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
 *
 */

#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <mach/gpio.h>
#include <mach/board.h>
#include <mach/camera.h>
#include <mach/vreg.h>
#include <mach/clk.h>

#define CAMIF_CFG_RMSK 0x1fffff
#define CAM_SEL_BMSK 0x2
#define CAM_PCLK_SRC_SEL_BMSK 0x60000
#define CAM_PCLK_INVERT_BMSK 0x80000
#define CAM_PAD_REG_SW_RESET_BMSK 0x100000

#define EXT_CAM_HSYNC_POL_SEL_BMSK 0x10000
#define EXT_CAM_VSYNC_POL_SEL_BMSK 0x8000
#define MDDI_CLK_CHICKEN_BIT_BMSK  0x80

#define CAM_SEL_SHFT 0x1
#define CAM_PCLK_SRC_SEL_SHFT 0x11
#define CAM_PCLK_INVERT_SHFT 0x13
#define CAM_PAD_REG_SW_RESET_SHFT 0x14

#define EXT_CAM_HSYNC_POL_SEL_SHFT 0x10
#define EXT_CAM_VSYNC_POL_SEL_SHFT 0xF
#define MDDI_CLK_CHICKEN_BIT_SHFT  0x7
#define APPS_RESET_OFFSET 0x00000210

static struct clk *camio_vfe_mdc_clk;
static struct clk *camio_mdc_clk;
static struct clk *camio_vfe_clk;

static struct msm_camera_io_ext camio_ext;
static struct resource *appio, *mdcio;
void __iomem *appbase, *mdcbase;

static struct resource *appio, *mdcio;
void __iomem *appbase, *mdcbase;


/* used for camera power config */
static struct vreg *vreg_gp2=NULL;
static struct vreg *vreg_gp3=NULL;
static struct vreg *vreg_gp5=NULL;
static struct vreg *vreg_wlan=NULL;

int msm_camio_clk_enable(enum msm_camio_clk_type clktype)
{
	int rc = -1;
	struct clk *clk = NULL;

	switch (clktype) {
	case CAMIO_VFE_MDC_CLK:
		clk = camio_vfe_mdc_clk = clk_get(NULL, "vfe_mdc_clk");
		break;

	case CAMIO_MDC_CLK:
		clk = camio_mdc_clk = clk_get(NULL, "mdc_clk");
		break;

	case CAMIO_VFE_CLK:
		clk = camio_vfe_clk = clk_get(NULL, "vfe_clk");
		break;

	default:
		break;
	}

	if (!IS_ERR(clk)) {
		clk_enable(clk);
		rc = 0;
	}

	return rc;
}

int msm_camio_clk_disable(enum msm_camio_clk_type clktype)
{
	int rc = -1;
	struct clk *clk = NULL;

	switch (clktype) {
	case CAMIO_VFE_MDC_CLK:
		clk = camio_vfe_mdc_clk;
		break;

	case CAMIO_MDC_CLK:
		clk = camio_mdc_clk;
		break;

	case CAMIO_VFE_CLK:
		clk = camio_vfe_clk;
		break;

	default:
		break;
	}

	if (!IS_ERR(clk)) {
		clk_disable(clk);
		clk_put(clk);
		rc = 0;
	}

	return rc;
}

void msm_camio_clk_rate_set(int rate)
{
	struct clk *clk = camio_vfe_clk;

	if (clk != ERR_PTR(-ENOENT))
		clk_set_rate(clk, rate);
}

int msm_camio_enable(struct platform_device *pdev)
{
	int rc = 0;
	struct msm_camera_sensor_info *sinfo = pdev->dev.platform_data;
	struct msm_camera_device_platform_data *camdev = sinfo->pdata;

	camio_ext = camdev->ioext;

	appio = request_mem_region(camio_ext.appphy,
		camio_ext.appsz, pdev->name);
	if (!appio) {
		rc = -EBUSY;
		goto enable_fail;
	}

	appbase = ioremap(camio_ext.appphy,
		camio_ext.appsz);
	if (!appbase) {
		rc = -ENOMEM;
		goto apps_no_mem;
	}

	msm_camio_clk_enable(CAMIO_VFE_CLK);
	msm_camio_clk_enable(CAMIO_MDC_CLK);
	return 0;
apps_no_mem:
	release_mem_region(camio_ext.appphy, camio_ext.appsz);
enable_fail:
	return rc;
}
//[modifed by yubin, for s5k5caga, begin
#define AENEAS_S5K5CAGA
#ifdef AENEAS_S5K5CAGA
static void msm_camera_vreg_enable(struct platform_device *pdev)
{
	int rc;
        /******************************************
        DVDD voltage 1.8
         ******************************************/
       mdelay(1);
	if (vreg_gp2 == NULL) {
		vreg_gp2 = vreg_get(NULL, "gp2");
		if (IS_ERR(vreg_gp2)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp2", PTR_ERR(vreg_gp2));
			goto msm_camera_vreg_enable_exit1;
		}

		//if (!strcmp(pdev->name, "msm_camera_s5k5caga")) {
			printk("%s %s %d==set s5k5caga(3M yuv) DVDD to 1800(1.8v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp2, 1800);			
		//}

		if (rc) {
			printk(KERN_ERR "%s: GP2 set_level failed (%d)\n",
				__func__, rc);
			goto msm_camera_vreg_enable_exit2;
		}

		rc = vreg_enable(vreg_gp2);
		if (rc) {
			printk(KERN_ERR "%s: GP2 enable failed (%d)\n",
				 __func__, rc);
			goto msm_camera_vreg_enable_exit3;
		}
	}


        /******************************************
        AVDD voltage 2.8
         ******************************************/
       mdelay(1);
	if (vreg_gp3 == NULL) {
		vreg_gp3 = vreg_get(NULL, "gp3");
		if (IS_ERR(vreg_gp3)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp3", PTR_ERR(vreg_gp3));
			goto msm_camera_vreg_enable_exit4;
		}

		printk("%s %s %d==set s5k5caga(3M yuv) AVDD to 2800(2.8v) ======\n", __FILE__, __func__, __LINE__);
		rc = vreg_set_level(vreg_gp3, 2800);
		if (rc) {
			printk(KERN_ERR "%s: GP3 set level failed (%d)\n",
				__func__, rc);
			goto msm_camera_vreg_enable_exit5;
		}

		rc = vreg_enable(vreg_gp3);
		if (rc) {
			printk(KERN_ERR "%s: GP3 enable failed (%d)\n",
				__func__, rc);
			goto msm_camera_vreg_enable_exit6;
		}
	}

        /******************************************
        IOVDD voltage 2.6
         ******************************************/
	mdelay(1);
	if (vreg_wlan == NULL) {
		vreg_wlan = vreg_get(NULL, "wlan");
		if (IS_ERR(vreg_wlan)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "wlan", PTR_ERR(vreg_wlan));
			goto msm_camera_vreg_enable_exit7;
		}

		printk("%s %s %d==set s5k5caga(3M yuv) IOVDD to 2600(2.6v) ======\n", __FILE__, __func__, __LINE__);
		rc = vreg_set_level(vreg_wlan, 2600);
		if (rc) {
			printk(KERN_ERR "%s: wlan set level failed (%d)\n",
				__func__, rc);
			goto msm_camera_vreg_enable_exit8;
		}

		rc = vreg_enable(vreg_wlan);
		if (rc) {
			printk(KERN_ERR "%s: vreg_wlan enable failed (%d)\n",
				__func__, rc);
			goto msm_camera_vreg_enable_exit9;
		}	
	}

	printk("========> Already opened camera power! \n");

	mdelay(1);//for mclk
	return;

msm_camera_vreg_enable_exit9:
msm_camera_vreg_enable_exit8:
			vreg_put(vreg_wlan);
msm_camera_vreg_enable_exit7:
			vreg_disable(vreg_gp3);
			vreg_wlan = NULL;

msm_camera_vreg_enable_exit6:
msm_camera_vreg_enable_exit5:
			vreg_put(vreg_gp3);
msm_camera_vreg_enable_exit4:
			vreg_disable(vreg_gp2);
			vreg_gp3 = NULL;

msm_camera_vreg_enable_exit3:
msm_camera_vreg_enable_exit2:
			vreg_put(vreg_gp2);
msm_camera_vreg_enable_exit1:
			vreg_gp2 = NULL;
			return;
}

static void msm_camera_vreg_disable(struct platform_device *pdev)
{
	msleep(1);
	if (vreg_wlan) {                          /* wlan voltage  */
		vreg_disable(vreg_wlan);
		vreg_put(vreg_wlan);
		vreg_wlan = NULL;
		printk("======> vreg_wlan closed! \n");
	}
	msleep(1);
	if (vreg_gp2) {                             /* DVDD voltage  */
		vreg_disable(vreg_gp2);                
		vreg_put(vreg_gp2);
		vreg_gp2 = NULL;
		printk("======> vreg_gp2 DVDD closed! \n");
	}
	msleep(1);
	if (vreg_gp3) {                            /* AVDD voltage  */
		vreg_disable(vreg_gp3);
		vreg_put(vreg_gp3);
		vreg_gp3 = NULL;
		printk("======> vreg_gp3 AVDD closed! \n");
	}
	msleep(1);
}


#else
#if 0                // for tequila & NFC hw
static void msm_camera_vreg_enable(struct platform_device *pdev)
{
	int rc;
	
	if (vreg_gp3 == NULL) {
		vreg_gp3 = vreg_get(NULL, "gp3");
		if (IS_ERR(vreg_gp3)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp3", PTR_ERR(vreg_gp3));
			vreg_gp3 = NULL;
			return;
		}

		printk("%s %s %d==set camera AVDD to 2800(2.8v) ======\n", __FILE__, __func__, __LINE__);
		rc = vreg_set_level(vreg_gp3, 2800);    /* modified by wangjin, changed from 2.6v to 2.8v  */
		if (rc) {
			printk(KERN_ERR "%s: GP3 set level failed (%d)\n",
				__func__, rc);
			goto gp3_put;
		}

		rc = vreg_enable(vreg_gp3);        /* AVDD voltage  */
		if (rc) {
			printk(KERN_ERR "%s: GP3 enable failed (%d)\n",
				__func__, rc);
			goto gp3_put;
		}
	}

	mdelay(50);

	if (vreg_gp2 == NULL) {
		vreg_gp2 = vreg_get(NULL, "gp2");
		if (IS_ERR(vreg_gp2)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp2", PTR_ERR(vreg_gp2));
			vreg_gp2 = NULL;
			goto gp3_disable;
		}

		if (!strcmp(pdev->name, "msm_camera_hi253")) {
			printk("%s %s %d==set hi253(2M yuv) DVDD to 1800(1.8v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp2, 1800);			
		}
		else{
			printk("%s %s %d==set ov5647(5M bayer) DVDD to 1500(1.5v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp2, 1500);			
		}
		if (rc) {
			printk(KERN_ERR "%s: GP2 set_level failed (%d)\n",
				__func__, rc);
			goto gp2_put;
		}

		rc = vreg_enable(vreg_gp2);        /* DVDD voltage  */
		if (rc) {
			printk(KERN_ERR "%s: GP2 enable failed (%d)\n",
				 __func__, rc);
			goto gp2_put;
		}
	}

	if (!strcmp(pdev->name, "msm_camera_ov5647")) {       // only 5M sensor supports Focus function.
		if (vreg_gp5 == NULL) {
			vreg_gp5 = vreg_get(NULL, "gp5");
			if (IS_ERR(vreg_gp5)) {
				printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
					__func__, "gp5", PTR_ERR(vreg_gp5));
				vreg_gp5 = NULL;
				goto gp3_disable;
			}

			printk("%s %s %d==set camera VCM_VDD to 2800(2.8v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp5, 2800);
			if (rc) {
				printk(KERN_ERR "%s: GP5 set level failed (%d)\n",
					__func__, rc);
				goto gp5_put;
			}

			rc = vreg_enable(vreg_gp5);       /* VCM_VDD voltage  */
			if (rc) {
				printk(KERN_ERR "%s: GP5 enable failed (%d)\n",
					__func__, rc);
				goto gp5_put;
			}
		}
	}

	if (vreg_wlan == NULL) {
		/* open one vreg_wlan and set to 2.6v, modified by wangjin  */
		vreg_wlan = vreg_get(NULL, "wlan");
		
		if (IS_ERR(vreg_wlan)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "wlan", PTR_ERR(vreg_wlan));
			goto gp5_disable;
		}

		printk("%s %s %d==set vreg_wlan to 2600(2.6v) ======\n", __FILE__, __func__, __LINE__);
		rc = vreg_set_level(vreg_wlan, 2600);
		if (rc) {
			printk(KERN_ERR "%s: wlan set level failed (%d)\n",
				__func__, rc);
			goto wlan_put;
		}

		rc = vreg_enable(vreg_wlan);       /* wlan voltage  */
		if (rc) {
			printk(KERN_ERR "%s: vreg_wlan enable failed (%d)\n",
				__func__, rc);
			goto wlan_put;
		}	
	}

	printk("========> Already opened camera power! \n");
	return;

wlan_put:
	vreg_put(vreg_wlan);
	vreg_wlan = NULL;

gp5_disable:
	if (!strcmp(pdev->name, "msm_camera_ov5647")) {       // only 5M sensor supports Focus function.
		vreg_disable(vreg_gp5);
	}
gp5_put:
	if (!strcmp(pdev->name, "msm_camera_ov5647")) {       // only 5M sensor supports Focus function.
		vreg_put(vreg_gp5);
		vreg_gp5 = NULL;
	}


gp2_disable:
	vreg_disable(vreg_gp2);
gp2_put:
	vreg_put(vreg_gp2);
	vreg_gp2 = NULL;


gp3_disable:
	vreg_disable(vreg_gp3);
gp3_put:
	vreg_put(vreg_gp3);
	vreg_gp3 = NULL;

}

#else           // for venus hw
static void msm_camera_vreg_enable(struct platform_device *pdev)
{
	int rc;
//modified by yubin, begin, temp
	if (vreg_gp2 == NULL) {
		printk("dvdd 1.8>>>>>>>>>>>>>>>>>>>>>>>>>>");
		vreg_gp2 = vreg_get(NULL, "gp2");
		if (IS_ERR(vreg_gp2)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp2", PTR_ERR(vreg_gp2));
			vreg_gp2 = NULL;
			goto gp3_disable;
		}

		if (!strcmp(pdev->name, "msm_camera_hi253")) {
			printk("%s %s %d==set hi253(2M yuv) DVDD to 1800(1.8v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp2, 1800);			
		}
		else if (!strcmp(pdev->name, "msm_camera_s5k5caga")) {
			printk("%s %s %d==set hi253(2M yuv) DVDD to 1800(1.8v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp2, 1800);			
		}
		else{
			printk("%s %s %d==set ov5647(5M bayer) DVDD to 1500(1.5v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp2, 1500);			
		}
		if (rc) {
			printk(KERN_ERR "%s: GP2 set_level failed (%d)\n",
				__func__, rc);
			goto gp2_put;
		}

		rc = vreg_enable(vreg_gp2);        /* DVDD voltage  */
		if (rc) {
			printk(KERN_ERR "%s: GP2 enable failed (%d)\n",
				 __func__, rc);
			goto gp2_put;
		}
	}

	mdelay(1);

	if (vreg_gp3 == NULL) {
		printk("avdd 2.8>>>>>>>>>>>>>>>>>>>>>>>>>>");
		vreg_gp3 = vreg_get(NULL, "gp3");
		if (IS_ERR(vreg_gp3)) {
			printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
				__func__, "gp3", PTR_ERR(vreg_gp3));
			vreg_gp3 = NULL;
			return;
		}

		printk("%s %s %d==set camera AVDD to 2800(2.8v) ======\n", __FILE__, __func__, __LINE__);
		rc = vreg_set_level(vreg_gp3, 2800);    /* modified by wangjin, changed from 2.6v to 2.8v  */
		if (rc) {
			printk(KERN_ERR "%s: GP3 set level failed (%d)\n",
				__func__, rc);
			goto gp3_put;
		}

		rc = vreg_enable(vreg_gp3);        /* AVDD voltage  */
		if (rc) {
			printk(KERN_ERR "%s: GP3 enable failed (%d)\n",
				__func__, rc);
			goto gp3_put;
		}
	}
//modified by yubin, end, temp
	if (!strcmp(pdev->name, "msm_camera_ov5647")) {       // only 5M sensor supports Focus function.
		if (vreg_gp5 == NULL) {
			vreg_gp5 = vreg_get(NULL, "gp5");
			if (IS_ERR(vreg_gp5)) {
				printk(KERN_ERR "%s: vreg_get(%s) failed (%ld)\n",
					__func__, "gp5", PTR_ERR(vreg_gp5));
				vreg_gp5 = NULL;
				goto gp2_disable;
			}

			printk("%s %s %d==set camera VCM_VDD to 2800(2.8v) ======\n", __FILE__, __func__, __LINE__);
			rc = vreg_set_level(vreg_gp5, 2800);
			if (rc) {
				printk(KERN_ERR "%s: GP5 set level failed (%d)\n",
					__func__, rc);
				goto gp5_put;
			}

			rc = vreg_enable(vreg_gp5);       /* VCM_VDD voltage  */
			if (rc) {
				printk(KERN_ERR "%s: GP5 enable failed (%d)\n",
					__func__, rc);
				goto gp5_put;
			}
		}
	}	

	printk("========> Already opened camera power! \n");
	return;

gp5_put:
	if (!strcmp(pdev->name, "msm_camera_ov5647")) {       // only 5M sensor supports Focus function.
		vreg_put(vreg_gp5);
		vreg_gp5 = NULL;
	}

gp2_disable:
	vreg_disable(vreg_gp2);
gp2_put:
	vreg_put(vreg_gp2);
	vreg_gp2 = NULL;


gp3_disable:
	vreg_disable(vreg_gp3);
gp3_put:
	vreg_put(vreg_gp3);
	vreg_gp3 = NULL;

}
#endif


#if 0         // for tequila & NFC hw
static void msm_camera_vreg_disable(struct platform_device *pdev)
{	

	if (vreg_gp2) {                             /* DVDD voltage  */
		vreg_disable(vreg_gp2);                
		vreg_put(vreg_gp2);
		vreg_gp2 = NULL;
		printk("======> vreg_gp2 DVDD closed! \n");
	}

	if (vreg_gp3) {                            /* AVDD voltage  */
		vreg_disable(vreg_gp3);
		vreg_put(vreg_gp3);
		vreg_gp3 = NULL;
		printk("======> vreg_gp3 AVDD closed! \n");
	}

	if (!strcmp(pdev->name, "msm_camera_ov5647")) {       // only 5M sensor supports Focus function.
		if (vreg_gp5) {                            /* VCM_VDD voltage  */
			vreg_disable(vreg_gp5);
			vreg_put(vreg_gp5);
			vreg_gp5 = NULL;
			printk("======> vreg_gp5 VCM_VDD closed! \n");
		}
	}

	if (vreg_wlan) {                          /* wlan voltage  */
		vreg_disable(vreg_wlan);
		vreg_put(vreg_wlan);
		vreg_wlan = NULL;
		printk("======> vreg_wlan closed! \n");
	}
}

#else         // for venus hw
static void msm_camera_vreg_disable(struct platform_device *pdev)
{	

	if (vreg_gp2) {                             /* DVDD voltage  */
		vreg_disable(vreg_gp2);                
		vreg_put(vreg_gp2);
		vreg_gp2 = NULL;
		printk("======> vreg_gp2 DVDD closed! \n");
	}

	if (vreg_gp3) {                            /* AVDD voltage  */
		vreg_disable(vreg_gp3);
		vreg_put(vreg_gp3);
		vreg_gp3 = NULL;
		printk("======> vreg_gp3 AVDD closed! \n");
	}

	if (!strcmp(pdev->name, "msm_camera_ov5647")) {       // only 5M sensor supports Focus function.
		if (vreg_gp5) {                            /* VCM_VDD voltage  */
			vreg_disable(vreg_gp5);
			vreg_put(vreg_gp5);
			vreg_gp5 = NULL;
			printk("======> vreg_gp5 VCM_VDD closed! \n");
		}
	}
}
#endif
#endif//]modifed by yubin, for s5k5caga, end

int msm_camio_sensor_clk_on(struct platform_device *pdev)
{
	struct msm_camera_sensor_info *sinfo = pdev->dev.platform_data;
	struct msm_camera_device_platform_data *camdev = sinfo->pdata;
	int32_t rc = 0;
//[modifed by yubin, for s5k5caga, begin
printk("\n===================msm_camio_sensor_clk_on===================\n");
//]modifed by yubin, for s5k5caga, end
	camio_ext = camdev->ioext;
	mdcio = request_mem_region(camio_ext.mdcphy,
		camio_ext.mdcsz, pdev->name);
	if (!mdcio)
		rc = -EBUSY;
	mdcbase = ioremap(camio_ext.mdcphy,
		camio_ext.mdcsz);
	if (!mdcbase) {
		rc = -EINVAL;
		goto mdc_no_mem;
	}
	camdev->camera_gpio_on();
	msm_camera_vreg_enable(pdev);               // open camera power
	return msm_camio_clk_enable(CAMIO_VFE_MDC_CLK);

mdc_no_mem:
	release_mem_region(camio_ext.mdcphy, camio_ext.mdcsz);
	return rc;
}

int msm_camio_sensor_clk_off(struct platform_device *pdev)
{
	struct msm_camera_sensor_info *sinfo = pdev->dev.platform_data;
	struct msm_camera_device_platform_data *camdev = sinfo->pdata;
//[modifed by yubin, for s5k5caga, begin
	int rc;
printk("\n===================msm_camio_sensor_clk_off===================\n");

	
	camdev->camera_gpio_off();
	
	rc=msm_camio_clk_disable(CAMIO_VFE_MDC_CLK);
	
	msm_camera_vreg_disable(pdev);                 // close camera power
	iounmap(mdcbase);
	release_mem_region(camio_ext.mdcphy, camio_ext.mdcsz);
	return rc;
//]modifed by yubin, for s5k5caga, end
}

void msm_camio_disable(struct platform_device *pdev)
{
	iounmap(appbase);
	release_mem_region(camio_ext.appphy, camio_ext.appsz);
	msm_camio_clk_disable(CAMIO_VFE_CLK);
	msm_camio_clk_disable(CAMIO_MDC_CLK);
}

void msm_disable_io_gpio_clk(struct platform_device *pdev)
{
	return;
}

void msm_camio_camif_pad_reg_reset(void)
{
	uint32_t reg;
	uint32_t mask, value;

	/* select CLKRGM_VFE_SRC_CAM_VFE_SRC:  internal source */
	msm_camio_clk_sel(MSM_CAMIO_CLK_SRC_INTERNAL);

	reg = (readl(mdcbase)) & CAMIF_CFG_RMSK;

	mask = CAM_SEL_BMSK |
		CAM_PCLK_SRC_SEL_BMSK |
		CAM_PCLK_INVERT_BMSK;

	value = 1 << CAM_SEL_SHFT |
		3 << CAM_PCLK_SRC_SEL_SHFT |
		0 << CAM_PCLK_INVERT_SHFT;

	writel((reg & (~mask)) | (value & mask), mdcbase);
	msleep(10);

	reg = (readl(mdcbase)) & CAMIF_CFG_RMSK;
	mask = CAM_PAD_REG_SW_RESET_BMSK;
	value = 1 << CAM_PAD_REG_SW_RESET_SHFT;
	writel((reg & (~mask)) | (value & mask), mdcbase);
	msleep(10);

	reg = (readl(mdcbase)) & CAMIF_CFG_RMSK;
	mask = CAM_PAD_REG_SW_RESET_BMSK;
	value = 0 << CAM_PAD_REG_SW_RESET_SHFT;
	writel((reg & (~mask)) | (value & mask), mdcbase);
	msleep(10);

	msm_camio_clk_sel(MSM_CAMIO_CLK_SRC_EXTERNAL);
	msleep(10);
}

void msm_camio_vfe_blk_reset(void)
{
	uint32_t val;

	/* do apps reset */
	val = readl(appbase + 0x00000210);
	val |= 0x1;
	writel(val, appbase + 0x00000210);
	mdelay(10);

	val = readl(appbase + 0x00000210);
	val &= ~0x1;
	writel(val, appbase + 0x00000210);
	mdelay(10);

	/* do axi reset */
	val = readl(appbase + 0x00000208);
	val |= 0x1;
	writel(val, appbase + 0x00000208);
	mdelay(10);

	val = readl(appbase + 0x00000208);
	val &= ~0x1;
	writel(val, appbase + 0x00000208);
	mdelay(10);
}

void msm_camio_camif_pad_reg_reset_2(void)
{
	uint32_t reg;
	uint32_t mask, value;

	reg = (readl(mdcbase)) & CAMIF_CFG_RMSK;
	mask = CAM_PAD_REG_SW_RESET_BMSK;
	value = 1 << CAM_PAD_REG_SW_RESET_SHFT;
	writel((reg & (~mask)) | (value & mask), mdcbase);
	mdelay(10);

	reg = (readl(mdcbase)) & CAMIF_CFG_RMSK;
	mask = CAM_PAD_REG_SW_RESET_BMSK;
	value = 0 << CAM_PAD_REG_SW_RESET_SHFT;
	writel((reg & (~mask)) | (value & mask), mdcbase);
	mdelay(10);
}

void msm_camio_clk_sel(enum msm_camio_clk_src_type srctype)
{
	struct clk *clk = NULL;

	clk = camio_vfe_clk;

	if (clk != NULL && clk != ERR_PTR(-ENOENT)) {
		switch (srctype) {
		case MSM_CAMIO_CLK_SRC_INTERNAL:
			clk_set_flags(clk, 0x00000100 << 1);
			break;

		case MSM_CAMIO_CLK_SRC_EXTERNAL:
			clk_set_flags(clk, 0x00000100);
			break;

		default:
			break;
		}
	}
}



int msm_camio_probe_on(struct platform_device *pdev)
{
	struct msm_camera_sensor_info *sinfo = pdev->dev.platform_data;
	struct msm_camera_device_platform_data *camdev = sinfo->pdata;
	camdev->camera_gpio_on();
	msm_camera_vreg_enable(pdev);           // open all  camera power
	//printk("========> %s called! \n", __func__);
	return msm_camio_clk_enable(CAMIO_VFE_CLK);
}

int msm_camio_probe_off(struct platform_device *pdev)
{
	struct msm_camera_sensor_info *sinfo = pdev->dev.platform_data;
	struct msm_camera_device_platform_data *camdev = sinfo->pdata;
	camdev->camera_gpio_off();
	msm_camera_vreg_disable(pdev);                // close all camera power
	//printk("=========> %s called! \n", __func__);
	return msm_camio_clk_disable(CAMIO_VFE_CLK);
}
