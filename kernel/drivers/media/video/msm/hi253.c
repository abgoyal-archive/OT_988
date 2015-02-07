/*
 * Copyright (c) 2010, Hisense mobile technology Ltd. All Rights Reserved
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
#define CONFIG_MSM_CAMERA_DEBUG	// by sinclair.lee for debug FEATURE_M7LAB

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>
#include <mach/camera.h>
#include <mach/board.h>


#define CONFIG_HI253_DEBUG
#ifdef CONFIG_HI253_DEBUG
#define HI253_DBG(fmt, args...) printk(KERN_INFO fmt, ##args)
#else
#define HI253_DBG(fmt, args...) do { } while (0)
#endif

static int current_special_effect = -1;
static uint8_t temp_iso = 0;		// kykim exif iso :store temporal value :
static uint32_t temp_exp = 0;		// kykim exif exp :store temporal value
static uint8_t current_iso = 0;		// kykim exif iso :store current value :
static uint32_t current_exp = 0;		// kykim exif exp :store current value

struct hi253_work {
	struct work_struct work;
};
static struct  hi253_work *hi253_sensorw;

static struct  i2c_client *hi253_client;

struct hi253_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};
static struct hi253_ctrl *hi253_ctrl;

static DECLARE_WAIT_QUEUE_HEAD(hi253_wait_queue);
DECLARE_MUTEX(hi253_sem);

struct hi253_i2c_reg_conf {
	unsigned char  baddr;
	unsigned char  bdata;

};

/* Revision constants */
/* Revision constants */
#define PID_HI253		0x92	/* Product ID */
#define HI253_PID_REG	0x04	

/*****************************************************************************

 *  Register Settings Function

 *****************************************************************************/
 struct hi253_i2c_reg_conf hi253_reg_init_tab[] = 
{
/////// Start Sleep ///////kykim iso400 // kykim flicker
{0x03,0x00},
{0x01,0x71}, //Sleep on
{0x08,0x0f}, //HI-Z on
{0x01,0x70}, //sleep off
         
{0x03,0x00}, //Dummy 750us start
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00}, //Dummy 750us END
         
{0x03,0x00}, 
{0x0e,0x03}, //PLL On
{0x0e,0x73}, //PLL x2
         
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
         
{0x03,0x00},
{0x0e,0x00}, //PLL Off
{0x01,0x71}, //Sleep On
{0x08,0x00}, //Hi-Z Off
{0x01,0x73}, //Sleep On + Soft Reset
{0x01,0x71}, //Sleep On
         
{0x03,0x20}, //AE off
{0x10,0x0c}, 
         
{0x03,0x22}, //AWB off
{0x10,0x69},
         
{0x03,0x00},
{0x10,0x1b}, //pre 2
{0x11,0x93}, //90
{0x12,0x00}, //01 // kykim campp clk time margin 
         
{0x0b,0xaa},
{0x0c,0xaa},
{0x0d,0xaa},
         
{0x20,0x00}, //Windowing start point Y
{0x21,0x04}, //04}, //kykim metering
{0x22,0x00}, //Windowing start point X 
{0x23,0x02}, //02},   // kykim metering
{0x24,0x04},
{0x25,0xb0},
{0x26,0x06},
{0x27,0x40},

{0x40,0x02}, //Hblank 604
{0x41,0x5c}, 
{0x42,0x00}, //Vblank 20
{0x43,0x14},
         
{0x45,0x04},
{0x46,0x18},
{0x47,0xd8},
         
{0x80,0x2e},
{0x81,0x7e},
{0x82,0x90},
{0x83,0x00},
{0x84,0x0c},
{0x85,0x00},

{0x90,0x06}, //BLC_TIME_TH_ON
{0x91,0x06}, //BLC_TIME_TH_OFF 
{0x92,0xd8}, //BLC_AG_TH_ON
{0x93,0xd0}, //BLC_AG_TH_OFF

{0x94,0x75},
{0x95,0x70},
{0x96,0xdc},
{0x97,0xfe},
{0x98,0x38},

{0x99,0x00}, //43}, //OutDoor BLC
{0x9a,0x00},
{0x9b,0x00},
{0x9c,0x00},
{0xa0,0x00}, //Dark BLC
{0xa2,0x00},
{0xa4,0x00},
{0xa6,0x00},
{0xa8,0x00}, //Normal BLC
{0xaa,0x00},
{0xac,0x00},
{0xae,0x00},

{0x03,0x02}, //page 02
{0x12,0x03},
{0x13,0x03},
{0x16,0x00},
{0x17,0x8C},
{0x18,0x4c},
{0x19,0x40}, //00},//pclk delay 40 80 C0
{0x1a,0x39},
{0x1c,0x09},
{0x1d,0x40},
{0x1e,0x30},
{0x1f,0x10},
{0x20,0x77},
{0x21,0xde},
{0x22,0xa7},
{0x23,0x30},
{0x27,0x3c},
{0x2b,0x80},
{0x2e,0x11},
{0x2f,0xa1},
{0x30,0x05},
{0x50,0x20},
{0x52,0x01},
{0x53,0xc1},
{0x55,0x1c},
{0x56,0x11},
{0x5d,0xa2},
{0x5e,0x5a},
{0x60,0x87},
{0x61,0x99},
{0x62,0x88},
{0x63,0x97},
{0x64,0x88},
{0x65,0x97},
{0x67,0x0c},
{0x68,0x0c},
{0x69,0x0c},
{0x72,0x89},
{0x73,0x96},
{0x74,0x89},
{0x75,0x96},
{0x76,0x89},
{0x77,0x96},
{0x7c,0x85},
{0x7d,0xaf},
{0x80,0x01},
{0x81,0x7f},
{0x82,0x13},
{0x83,0x24},
{0x84,0x7d},
{0x85,0x81},
{0x86,0x7d},
{0x87,0x81},
{0x92,0x48},
{0x93,0x54},
{0x94,0x7d},
{0x95,0x81},
{0x96,0x7d},
{0x97,0x81},
{0xa0,0x02},
{0xa1,0x7b},
{0xa2,0x02},
{0xa3,0x7b},
{0xa4,0x7b},
{0xa5,0x02},
{0xa6,0x7b},
{0xa7,0x02},
{0xa8,0x85},
{0xa9,0x8c},
{0xaa,0x85},
{0xab,0x8c},
{0xac,0x10},
{0xad,0x16},
{0xae,0x10},
{0xaf,0x16},
{0xb0,0x99},
{0xb1,0xa3},
{0xb2,0xa4},
{0xb3,0xae},
{0xb4,0x9b},
{0xb5,0xa2},
{0xb6,0xa6},
{0xb7,0xac},
{0xb8,0x9b},
{0xb9,0x9f},
{0xba,0xa6},
{0xbb,0xaa},
{0xbc,0x9b},
{0xbd,0x9f},
{0xbe,0xa6},
{0xbf,0xaa},
{0xc4,0x2c},
{0xc5,0x43},
{0xc6,0x63},
{0xc7,0x79},
{0xc8,0x2d},
{0xc9,0x42},
{0xca,0x2d},
{0xcb,0x42},
{0xcc,0x64},
{0xcd,0x78},
{0xce,0x64},
{0xcf,0x78},
{0xd0,0x0a},
{0xd1,0x09},

{0xd4,0x06}, //DCDC_TIME_TH_ON 
{0xd5,0x06}, //DCDC_TIME_TH_OFF
{0xd6,0xd8},//78}, //DCDC_AG_TH_ON
{0xd7,0xd0},//70}, //DCDC_AG_TH_OFF

{0xe0,0xc4},
{0xe1,0xc4},
{0xe2,0xc4},
{0xe3,0xc4},
{0xe4,0x00},
{0xe8,0x80},
{0xe9,0x40},
{0xea,0x7f},
{0xf0,0x01},
{0xf1,0x01},
{0xf2,0x01},
{0xf3,0x01},
{0xf4,0x01},

{0x03,0x03},
{0x10,0x10},

{0x03,0x10},
{0x10,0x01}, //03 01}, LHC 
{0x12,0x30},
{0x13,0x02},
{0x20,0x00},
{0x30,0x00},
{0x31,0x00},
{0x32,0x00},
{0x33,0x00},
{0x34,0x30},
{0x35,0x00},
{0x36,0x00},
{0x38,0x00},
{0x3e,0x58},
{0x3f,0x00},
{0x40,0x00}, //03},
{0x41,0x00},
{0x48,0x80}, //83}, //contrast
{0x60,0x67},

{0x61,0x80}, //Sat B 
{0x62,0x75}, //80}, //Sat R
         
{0x63,0x50},
{0x64,0x41},
{0x66,0x42},
{0x67,0x20},
{0x6a,0x80}, //Cb+
{0x6b,0x75}, //80}, //Cb-
{0x6c,0x80}, //88}, //Cr+
{0x6d,0x80}, //Cr-

{0x03,0x11},
{0x10,0x3f},
{0x11,0x40},
{0x12,0xba},
{0x13,0xcb},
{0x26,0x31},
{0x27,0x34},
{0x28,0x0f},
{0x29,0x10},
{0x2b,0x30},
{0x2c,0x32},
{0x30,0x70},
{0x31,0x10},
{0x32,0x63},
{0x33,0x0a},
{0x34,0x07},
{0x35,0x04},
{0x36,0x70},
{0x37,0x18},
{0x38,0x63},
{0x39,0x0a},
{0x3a,0x07},
{0x3b,0x04},
{0x3c,0x80},
{0x3d,0x18},
{0x3e,0x80},
{0x3f,0x0c},
{0x40,0x09},
{0x41,0x06},
{0x42,0x80},
{0x43,0x18},
{0x44,0x80},
{0x45,0x13},
{0x46,0x10},
{0x47,0x11},
{0x48,0x90},
{0x49,0x40},
{0x4a,0x80},
{0x4b,0x13},
{0x4c,0x10},
{0x4d,0x11},
{0x4e,0x80},
{0x4f,0x30},
{0x50,0x80},
{0x51,0x13},
{0x52,0x10},
{0x53,0x13},
{0x54,0x11},
{0x55,0x17},
{0x56,0x20},
{0x57,0x20},
{0x58,0x20},
{0x59,0x30},
{0x5a,0x18},
{0x5b,0x00},
{0x5c,0x00},
{0x60,0x3f},
{0x62,0x50},
{0x70,0x06},

{0x03,0x12}, //page 12
{0x20,0x0f},
{0x21,0x0f},

{0x25,0x30},
{0x28,0x00},
{0x29,0x00},
{0x2a,0x00},
{0x30,0x50},
{0x31,0x18},
{0x32,0x32},
{0x33,0x40},
{0x34,0x50},
{0x35,0x70},
{0x36,0xa0},
{0x40,0xa0},
{0x41,0x40},
{0x42,0xa0},
{0x43,0x90}, 
{0x44,0x85}, 
{0x45,0x80},
{0x46,0xb0},
{0x47,0x55},
{0x48,0xa0},
{0x49,0x90}, 
{0x4a,0x85}, 
{0x4b,0x80},
{0x4c,0xb0},
{0x4d,0x40},
{0x4e,0x90},
{0x4f,0x90},
{0x50,0x90},
{0x51,0x80},
{0x52,0xb0},
{0x53,0x60},
{0x54,0xc0},
{0x55,0xc0},
{0x56,0xc0},
{0x57,0x80},
{0x58,0x90},
{0x59,0x40},
{0x5a,0xd0},
{0x5b,0xd0},
{0x5c,0xe0},
{0x5d,0x80},
{0x5e,0x88},
{0x5f,0x40},
{0x60,0xe0},
{0x61,0xe0},
{0x62,0xe0},
{0x63,0x80},
{0x70,0x13},
{0x71,0x01},
{0x72,0x15},
{0x73,0x01},
{0x74,0x25},
{0x75,0x15},
{0x80,0x15},
{0x81,0x25},
{0x82,0x50},
{0x85,0x1a},
{0x88,0x00},
{0x89,0x00},

{0x90,0x5c},

{0xD0,0x0c},
{0xD1,0x80},
{0xD2,0x67},
{0xD3,0x00},
{0xD4,0x00},
{0xD5,0x02},
{0xD6,0xff},
{0xD7,0x18},
{0x3b,0x06},
{0x3c,0x06},
{0xc5,0x30},
{0xc6,0x2a},

{0x03,0x13}, //Page 13 Edge setting
{0x10,0xab},
{0x11,0x7b},
{0x12,0x06},
{0x14,0x00},
{0x20,0x15},
{0x21,0x13},
{0x22,0x33},
{0x23,0x04},
{0x24,0x09},
{0x25,0x08},
{0x26,0x20},
{0x27,0x30},
{0x29,0x12},
{0x2a,0x50},
{0x2b,0x06},
{0x2c,0x07},
{0x2d,0x0c},
{0x2e,0x12},
{0x2f,0x12},
{0x50,0x10},
{0x51,0x14},
{0x52,0x12},
{0x53,0x0c},
{0x54,0x0f},
{0x55,0x0f},
{0x56,0x10},
{0x57,0x13},
{0x58,0x12},
{0x59,0x0c},
{0x5a,0x0f},
{0x5b,0x0f},
{0x5c,0x0a},
{0x5d,0x0b},
{0x5e,0x0a},
{0x5f,0x08},
{0x60,0x09},
{0x61,0x08},
{0x62,0x08},
{0x63,0x09},
{0x64,0x08},
{0x65,0x06},
{0x66,0x07},
{0x67,0x06},
{0x68,0x07},
{0x69,0x08},
{0x6a,0x07},
{0x6b,0x06},
{0x6c,0x07},
{0x6d,0x06},
{0x6e,0x07},
{0x6f,0x08},
{0x70,0x07},
{0x71,0x06},
{0x72,0x07},
{0x73,0x06},

{0x80,0x81}, //01}, 
{0x81,0x1f},
{0x82,0x01}, //2+center
{0x83,0x31},
{0x90,0x3f},
{0x91,0x3f},
{0x92,0x33},
{0x93,0x30},
{0x94,0x05},
{0x95,0x18},
{0x97,0x30},
{0x99,0x35},
{0xa0,0x01},
{0xa1,0x02},
{0xa2,0x01},
{0xa3,0x02},
{0xa4,0x02},
{0xa5,0x04},
{0xa6,0x04},
{0xa7,0x06},
{0xa8,0x05},
{0xa9,0x08},
{0xaa,0x05},
{0xab,0x08},

{0xb0,0x35}, //22},
{0xb1,0x35}, //2a},
{0xb2,0x35}, //28},
{0xb3,0x35}, //22},
{0xb4,0x35}, //2a},
{0xb5,0x35}, //28},
{0xb6,0x35}, //22},
{0xb7,0x35}, //2a},
{0xb8,0x35}, //28},
{0xb9,0x35}, //22},
{0xba,0x35}, //2a},
{0xbb,0x35}, //28},
{0xbc,0x30}, //27},
{0xbd,0x30},
{0xbe,0x2a},
{0xbf,0x30}, //27},
{0xc0,0x30},
{0xc1,0x2a},
{0xc2,0x1e},
{0xc3,0x24},
{0xc4,0x20},
{0xc5,0x1e},
{0xc6,0x24},
{0xc7,0x20},
{0xc8,0x18},
{0xc9,0x20},
{0xca,0x1e},
{0xcb,0x18},
{0xcc,0x20},
{0xcd,0x1e},
{0xce,0x18},
{0xcf,0x20},
{0xd0,0x1e},
{0xd1,0x18},
{0xd2,0x20},
{0xd3,0x1e},

{0x03,0x14}, //Page 14 LSC
{0x10,0x11},
{0x14,0x80},
{0x15,0x80},
{0x16,0x80},
{0x17,0x80},
{0x18,0x80},

{0x19,0x80},
{0x20,0x80}, //85}, //0x60->0x85
{0x21,0x80},
{0x22,0x80},
{0x23,0x80},
{0x24,0x80},
{0x30,0xc8},
{0x31,0x2b},
{0x32,0x00},
{0x33,0x00},
{0x34,0x90},
{0x40,0x52}, //LensR
{0x50,0x38}, //LensG
{0x60,0x30}, //LensB
{0x70,0x38}, //LensG

{0x03,0x15}, //page 15 CMC
{0x10,0x0f},
{0x14,0x42},
{0x15,0x32},
{0x16,0x22},
{0x17,0x2f},

{0x30,0x8f},
{0x31,0x59},
{0x32,0x0a},
{0x33,0x15},
{0x34,0x5b},
{0x35,0x06},
{0x36,0x07},
{0x37,0x40},
{0x38,0x87},//86},
/*[[ tuned 0725 */
{0x40,0xa3},// 00},
{0x41,0x2a},// 00},
{0x42,0x87},// 00},
{0x43,0x88},// 00},
{0x44,0x0d},// 00},
{0x45,0x85},// 00},
{0x46,0x01},// 00},
{0x47,0x9b},// 00},
{0x48,0x1a},// 00},

{0x50,0x00},
{0x51,0x00},
{0x52,0x00},
{0x53,0x00},
{0x54,0x00},
{0x55,0x00},
{0x56,0x00},
{0x57,0x00},
{0x58,0x00},
/*]] tuned 0725 */
{0x80,0x03},
{0x85,0x40},
{0x87,0x02},
{0x88,0x00},
{0x89,0x00},
{0x8a,0x00},

{0x03,0x16}, //page16 gamma
{0x10,0x31},
{0x18,0x5e},
{0x19,0x5d},
{0x1a,0x0e},
{0x1b,0x01},
{0x1c,0xdc},
{0x1d,0xfe},

{0x30,0x00},
{0x31,0x0a},// 07},//00},
{0x32,0x18},// 12},//08},
{0x33,0x2c},// 27},//22},
{0x34,0x50},//45},
{0x35,0x6f},//5f},
{0x36,0x8a},//74},
{0x37,0x9e},//86},
{0x38,0xae},//96},
{0x39,0xb8},//a5},
{0x3a,0xc3},//b3},
{0x3b,0xcc},//bf},
{0x3c,0xd5},//ca},
{0x3d,0xde},//d4},
{0x3e,0xe6},//de},
{0x3f,0xee},//e6},
{0x40,0xf5},//ee},
{0x41,0xfc},//f4},
{0x42,0xff},

{0x50,0x00},  
{0x51,0x0a},// 07},  
{0x52,0x18},// 12},  
{0x53,0x2c},// 27},  
{0x54,0x50},  
{0x55,0x6f},  
{0x56,0x8a},  
{0x57,0x9e},  
{0x58,0xae},  
{0x59,0xb8},  
{0x5a,0xc3},  
{0x5b,0xcc},  
{0x5c,0xd5},  
{0x5d,0xde},  
{0x5e,0xe6},  
{0x5f,0xee},  
{0x60,0xf5},  
{0x61,0xfc},  
{0x62,0xff},  

{0x70,0x00},
{0x71,0x11}, //08},
{0x72,0x23}, //17},
{0x73,0x3f}, //2f},
{0x74,0x61}, //53},
{0x75,0x7a}, //6c},
{0x76,0x8d}, //81},
{0x77,0x9e}, //94},
{0x78,0xac}, //a4},
{0x79,0xba}, //b3},
{0x7a,0xc6}, //c0},
{0x7b,0xcf}, //cb},
{0x7c,0xda}, //d5},
{0x7d,0xe1}, //de},
{0x7e,0xe9}, //e6},
{0x7f,0xf0}, //ee},
{0x80,0xf5}, //f4},
{0x81,0xfa},
{0x82,0xff},

{0x03,0x17}, //page17
{0x10,0xf7},
{0xC4,0x54}, //FLK 200
{0xC5,0x46}, //FLK 240

{0x03,0x18},
{0x10,0x00}, //scale off   

{0x03,0x20}, //page20
{0x11,0x1c},
{0x18,0x30},
{0x1a,0x08},
{0x20,0x00},
{0x21,0x30},
{0x22,0x10},
{0x23,0x00},
{0x24,0x00},
{0x28,0xe7},
{0x29,0x0d},
{0x2a,0xff},

{0x2b,0x34},
{0x2c,0xc3},  //c2 -> c3}, 2012.01.05 // kykim metering
{0x2d,0xcf},
{0x2e,0x33},
{0x30,0x78},
{0x32,0x03},
{0x33,0x2e},
{0x34,0x30},
{0x35,0xd4},
{0x36,0xfe},
{0x37,0x32},
{0x38,0x04},
{0x39,0x22},
{0x3a,0xde},
{0x3b,0x22},
{0x3c,0xde},
{0x50,0x45},
{0x51,0x88},
{0x56,0x03},
{0x57,0xf7},
{0x58,0x14},
{0x59,0x88},
{0x5a,0x04},
// kykim metering 
	{0x60, 0x00}, //AEweight1	
	{0x61, 0x00}, //AEweight2
	{0x62, 0x00}, //AEweight3
	{0x63, 0x00}, //AEweight4
	{0x64, 0x05}, //0a}, //AEweight5
	{0x65, 0x50}, //a0}, //AEweight6
	{0x66, 0x05}, //0a}, //AEweight7
	{0x67, 0x50}, //a0}, //AEweight8
	{0x68, 0x05}, //0a}, //AEweight9
	{0x69, 0x50}, //a0}, //AEweight10
	{0x6a, 0x05}, //0a}, //AEweight11
	{0x6b, 0x50}, //a0}, //AEweight12
	{0x6c, 0x00}, //AEweight13
	{0x6d, 0x00}, //AEweight14
	{0x6e, 0x00}, //AEweight15
	{0x6f, 0x00}, //AEweight16

{0x70,0x40},  //73-> 40}, //Y level target  2012.01.05 // kykim metering
{0x71,0x89}, //Outdoor Y level

// haunting control

{0x76,0x43},
{0x77,0xe2}, 
{0x78,0x23}, //Yth1
{0x79,0x46}, //Yth2 
{0x7a,0x23}, 
{0x7b,0x22},
{0x7d,0x23},

{0x03, 0x20}, //Page 20 12/08
{0x83,0x02}, //EXP Normal 20.00 fps 
{0x84,0x49}, 
{0x85,0xb4}, 
{0x86,0x01}, //EXPMin 8403.36 fps
{0x87,0x65}, 
{0x88, 0x03}, //EXP Max 15.00 fps 
{0x89, 0x0c}, 
{0x8a, 0xf0}, 
{0x8B,0x75}, //EXP100 
{0x8C,0x24}, 
{0x8D,0x61}, //EXP120 
{0x8E,0x9e}, 
{0x9c, 0x10}, //EXP Limit 700.28 fps 
{0x9d, 0xbc}, 
{0x9e,0x01}, //EXP Unit 
{0x9f,0x65},  

{0xb0,0x14},
{0xb1,0x14}, // AGMin
{0xb2,0xe0},
{0xb3,0x1f},
{0xb4,0x14},
{0xb5,0x38}, //44},
{0xb6,0x26}, //2f},
{0xb7,0x20}, //28},
{0xb8,0x1d}, //25},
{0xb9,0x1b}, //22},
{0xba,0x1a}, //21},
{0xbb,0x19}, //20},
{0xbc,0x19}, //1f},
{0xbd,0x18}, //1f},

//AE_Adaptive Time option

{0xc0,0x14},
{0xc1,0x1f},
{0xc2,0x1f},
{0xc3,0x1f}, 
{0xc4,0x10}, 

{0xc8,0x80},
{0xc9,0x40},

{0x03,0x22}, //page22 awb
{0x10,0xfd},
{0x11,0x2e},
{0x19,0x01},
{0x20,0x30},
{0x21,0x80},
{0x24,0x01},
{0x30,0x80},
{0x31,0x80},
{0x38,0x11},
{0x39,0x34},
{0x40,0xf4}, //Y1 limit
{0x41,0x44}, //Cdiff
{0x42,0x33}, //CSUM
{0x43,0xf6},
{0x44,0x55},
{0x45,0x44},
{0x46,0x00},
{0x50,0xb2},
{0x51,0x81},
{0x52,0x98},

{0x80,0x2d}, //Rgain
{0x81,0x20}, //Ggain
{0x82,0x42}, //Bgain

{0x83,0x50}, //indoor gain
{0x84,0x10},
{0x85,0x60},
{0x86,0x23},

{0x87,0x49}, //Middle gain
{0x88,0x30},
{0x89,0x37},
{0x8a,0x28},

{0x8b,0x41}, //Bottom gain
{0x8c,0x30},
{0x8d,0x34},
{0x8e,0x28},
             //gain slope
{0x8f,0x5a}, //0x60 //BGAINPARA 1
{0x90,0x55}, //0x5C //BGAINPARA 2
{0x91,0x4e}, //0x53 //BGAINPARA 3
{0x92,0x43}, //0x43 //BGAINPARA 4
{0x93,0x3e}, //3A}, //BGAINPARA 5
{0x94,0x38}, //35}, //0x37 //BGAINPARA 6
{0x95,0x2E}, //BGAINPARA 7
{0x96,0x26}, //BGAINPARA 8
{0x97,0x23}, //BGAINPARA 9
{0x98,0x21}, //BGAINPARA 10
{0x99,0x20}, //BGAINPARA 11
{0x9a,0x20}, //BGAINPARA 12

{0x9b,0x66},
{0x9c,0x66},
{0x9d,0x48},
{0x9e,0x38},
{0x9f,0x30},
{0xa0,0x60},
{0xa1,0x34},
{0xa2,0x6f},
{0xa3,0xff},
{0xa4,0x14},
{0xa5,0x2c},
{0xa6,0xcf},
{0xad,0x40},
{0xae,0x4a},
{0xaf,0x28},
{0xb0,0x26},
{0xb1,0x00},
{0xb4,0xea},
{0xb8,0xa0},
{0xb9,0x00},

{0x03,0x20}, 
{0x18,0x30}, 

{0x10,0xcc}, //AE on 12/08

{0x03,0x22}, //AWB on
{0x10,0xff}, 

{0x03,0x00},
{0x0e,0x03}, //PLL Off
{0x0e,0x73}, //PLL x2

{0x03,0x00}, //Dummy 750us
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x03,0x00},
{0x01,0x70}, //sleep off

};
// kykim flicker
struct hi253_i2c_reg_conf hi253_reg_video_base_tab[] = 
{
{0x03, 0x00},
{0x01, 0x71}, //79}, //sleep on 12/08	 , {0x01, 0x71} -- > {0x01, 0x79}
{0x0e, 0x03}, //pll off

{0x03,0x20}, //Page 20																		
{0x10,0x0c}, //AE OFF												   
{0x03,0x22}, //Page 22											   
{0x10,0x69}, //AWB OFF 

{0x03,0x00}, //0 page
{0x10,0x1b}, //pre 2
{0x11,0x97}, //94 93}, 12/08
{0x12,0x00}, //01 04},

{0x20,0x00},
{0x21,0x04},
{0x22,0x00},
{0x23,0x02}, 

{0x40,0x02}, //Hblank 604
{0x41,0x5c},
{0x42,0x00}, //Vblank 20
{0x43,0x14},

{0x03, 0x18},
{0x10, 0x00}, //scale off		 
  
{0x03, 0x20},
{0x2a, 0xff}, //03}, //ff}, 12/08													  
{0x2b, 0x34}, //35}, //34}, 12/08														  
{0x2c, 0xc3}, 			// kykim metering	  c2->c3												  
{0x30, 0x78},

{0x03, 0x20}, //Page 20
{0x83, 0x01}, //EXP Normal 30.00 fps 
{0x84, 0x86}, 
{0x85, 0x78}, 
{0x86, 0x01}, //EXPMin 8403.36 fps
{0x87, 0x65}, 
{0x88, 0x02}, //EXP Max 17.14 fps 
{0x89, 0xab}, 
{0x8a, 0x52}, 
{0x8B, 0x75}, //EXP100 
{0x8C, 0x24}, 
{0x8D, 0x61}, //EXP120 
{0x8E, 0x9e}, 
{0x91, 0x03}, //EXP Fix 15.01 fps
{0x92, 0x0c}, 
{0x93, 0xf0}, 
{0x9c, 0x10}, //EXP Limit 700.28 fps 
{0x9d, 0xbc}, 
{0x9e, 0x01}, //EXP Unit 
{0x9f, 0x65}, 

{0x03, 0x17}, //Page 17 
{0xC4, 0x54}, //FLK200 
{0xC5, 0x46}, //FLK240 

//AntiBand Unlock
{0x03, 0x20}, //Page 20 
{0x2b, 0x34}, 
{0x30, 0x78}, 

//BLC 
{0x03, 0x00}, //PAGE 0
{0x90, 0x05}, //BLC_TIME_TH_ON
{0x91, 0x05}, //BLC_TIME_TH_OFF 
{0x92, 0xd8}, //BLC_AG_TH_ON
{0x93, 0xd0}, //BLC_AG_TH_OFF

//DCDC 
{0x03, 0x02}, //PAGE 2
{0xd4, 0x05}, //DCDC_TIME_TH_ON
{0xd5, 0x05}, //DCDC_TIME_TH_OFF 
{0xd6, 0xd8}, //DCDC_AG_TH_ON
{0xd7, 0xd0}, //DCDC_AG_TH_OFF

						   
{0x03,0x20}, 
{0x18,0x30}, //AE reset off		 
{0x10,0xcc}, //AE ON 12/08
{0x03,0x22}, 		 
{0x10,0xeb}, //AWB ON
	   
{0x03, 0x00}, 
{0x0e, 0x03}, //3)PLL off
{0x0e, 0x73}, //4)PLL x2, on
		  
{0x03,0x00}, //dummy1	 
{0x03,0x00}, //dummy2		 
{0x03,0x00}, //dummy3
{0x03,0x00}, //dummy4	 
{0x03,0x00}, //dummy5		 
{0x03,0x00}, //dummy6	
{0x03,0x00}, //dummy7	 
{0x03,0x00}, //dummy8		 
{0x03,0x00}, //dummy9
{0x03,0x00}, //dummy10		  
{0x01,0x70}, //78}, //Sleep Off, {0x01,0x70} -->{0x01,0x78} 12/08

};

struct hi253_i2c_reg_conf hi253_reg_video_20fps_tab[] =
{
//["video"] 20 fps//
{0x03,0x00},
{0x01,0x71}, //sleep on	 
{0x0e,0x03}, //pll off

{0x03,0x20}, //Page 20																		
{0x10,0x0c}, //AE OFF												   
{0x03,0x22}, //Page 22											   
{0x10,0x69}, //AWB OFF 

{0x03,0x00}, //0 page
{0x10,0x1b}, //pre 2
{0x11,0x93}, //93}, 12/08 	//kykim metering
{0x12,0x00}, // 0x04 -> 0x00  // kykim campp

{0x20,0x00},
{0x21,0x04},
{0x22,0x00},
{0x23,0x02}, 

{0x40,0x02}, //Hblank 604
{0x41,0x5c},
{0x42,0x00}, //Vblank 20
{0x43,0x14},
  
{0x03,0x18},
{0x10,0x00}, //scale off		 
  
{0x03,0x20},
{0x2a,0xff}, //03}, 12/08														  
{0x2b,0x34}, //35}, 12/08														  
{0x2c,0xc3}, 		// kykim metering	 c2 -> c3													  
{0x30,0x78},

{0x03, 0x20}, //Page 20
{0x83, 0x01}, //EXP Normal 30.00 fps 
{0x84, 0x86}, 
{0x85, 0x78}, 
{0x86, 0x01}, //EXPMin 8403.36 fps
{0x87, 0x65}, 
{0x88, 0x02}, //EXP Max 24.00 fps 		//kykim metering 
{0x89, 0x49}, 
{0x8a, 0xb4}, 
{0x8B, 0x75}, //EXP100 
{0x8C, 0x24}, 
{0x8D, 0x61}, //EXP120 
{0x8E, 0x9e}, 
{0x91, 0x09}, //EXP Fix 20.01 fps
{0x92, 0x49}, 
{0x93, 0xb4}, 
{0x9c, 0x09}, //EXP Limit 1200.48 fps 
{0x9d, 0xc3}, 
{0x9e, 0x01}, //EXP Unit 
{0x9f, 0x65}, 

{0x03, 0x17}, //Page 17 
{0xC4, 0x54}, //FLK200 
{0xC5, 0x46}, //FLK240 

//AntiBand Unlock
{0x03, 0x20}, //Page 20 
{0x2b, 0x34}, 
{0x30, 0x78}, 

//BLC 
{0x03, 0x00}, //PAGE 0
{0x90, 0x05}, //BLC_TIME_TH_ON
{0x91, 0x05}, //BLC_TIME_TH_OFF 
{0x92, 0xd8}, //BLC_AG_TH_ON
{0x93, 0xd0}, //BLC_AG_TH_OFF

//DCDC 
{0x03, 0x02}, //PAGE 2
{0xd4, 0x05}, //DCDC_TIME_TH_ON
{0xd5, 0x05}, //DCDC_TIME_TH_OFF 
{0xd6, 0xd8}, //DCDC_AG_TH_ON
{0xd7, 0xd0}, //DCDC_AG_TH_OFF 
						   
{0x03,0x20}, 
{0x18,0x30}, //AE reset off		 
{0x10,0xcc}, //DC AE ON 12/08
{0x03,0x22}, 		 
{0x10,0xff}, //AWB ON
	   
{0x03,0x00}, 
{0x0e,0x03}, //3)PLL off
{0x0e,0x73}, //4)PLL x2, on
		  
{0x03,0x00}, //dummy1	 
{0x03,0x00}, //dummy2		 
{0x03,0x00}, //dummy3
{0x03,0x00}, //dummy4	 
{0x03,0x00}, //dummy5		 
{0x03,0x00}, //dummy6	
{0x03,0x00}, //dummy7	 
{0x03,0x00}, //dummy8		 
{0x03,0x00}, //dummy9
{0x03,0x00}, //dummy10		  
{0x01,0x70}, //Sleep Off

};

struct hi253_i2c_reg_conf hi253_reg_video_30fps_tab[] =
{
//["video"] 25 fps//

{0x03,0x00},
{0x01,0x71}, //sleep on	 
{0x0e,0x03}, //pll off

{0x03,0x20}, //Page 20																		
{0x10,0x0c}, //AE OFF												   
{0x03,0x22}, //Page 22											   
{0x10,0x69}, //AWB OFF 

{0x03,0x00}, //0 page
{0x10,0x1b}, //pre 2
{0x11,0x97}, //93}, 12/08 
{0x12,0x00}, 

{0x20,0x00},
{0x21,0x04},
{0x22,0x00},
{0x23,0x02}, 

{0x40,0x02}, //Hblank 604
{0x41,0x5c},
{0x42,0x00}, //Vblank 20
{0x43,0x14},
  
{0x03,0x18},
{0x10,0x00}, //scale off		 
  
{0x03,0x20},
{0x2a,0xff}, //03}, 12/08														  
{0x2b,0x34}, //35}, 12/08														  
{0x2c,0xc3}, 		// kykim metering													  
{0x30,0x78},

{0x03, 0x20}, //Page 20
{0x83, 0x01}, //EXP Normal 30.00 fps 
{0x84, 0x86}, 
{0x85, 0x78}, 
{0x86, 0x01}, //EXPMin 8403.36 fps
{0x87, 0x65}, 
{0x88, 0x01}, //EXP Max 30.00 fps 
{0x89, 0x86}, 
{0x8a, 0x78}, 
{0x8B, 0x75}, //EXP100 
{0x8C, 0x24}, 
{0x8D, 0x61}, //EXP120 
{0x8E, 0x9e}, 
{0x91, 0x01}, //EXP Fix 25.01 fps
{0x92, 0xd4}, 
{0x93, 0x90}, 
{0x9c, 0x10}, //EXP Limit 700.28 fps 
{0x9d, 0xbc}, 
{0x9e, 0x01}, //EXP Unit 
{0x9f, 0x65}, 

{0x03, 0x17}, //Page 17 
{0xC4, 0x54}, //FLK200 
{0xC5, 0x46}, //FLK240 

//AntiBand Unlock
{0x03, 0x20}, //Page 20 
{0x2b, 0x34}, 
{0x30, 0x78}, 

//BLC 
{0x03, 0x00}, //PAGE 0
{0x90, 0x03}, //BLC_TIME_TH_ON
{0x91, 0x03}, //BLC_TIME_TH_OFF 
{0x92, 0xd8}, //BLC_AG_TH_ON
{0x93, 0xd0}, //BLC_AG_TH_OFF

//DCDC 
{0x03, 0x02}, //PAGE 2
{0xd4, 0x03}, //DCDC_TIME_TH_ON
{0xd5, 0x03}, //DCDC_TIME_TH_OFF 
{0xd6, 0xd8}, //DCDC_AG_TH_ON
{0xd7, 0xd0}, //DCDC_AG_TH_OFF						   
						   
{0x03,0x20}, 
{0x18,0x30}, //AE reset off		 
{0x10,0xcc}, //dc}, //AE ON 12/08
{0x03,0x22}, 		 
{0x10,0xff}, //AWB ON
	   
{0x03,0x00}, 
{0x0e,0x03}, //3)PLL off
{0x0e,0x73}, //4)PLL x2, on
		  
{0x03,0x00}, //dummy1	 
{0x03,0x00}, //dummy2		 
{0x03,0x00}, //dummy3
{0x03,0x00}, //dummy4	 
{0x03,0x00}, //dummy5		 
{0x03,0x00}, //dummy6	
{0x03,0x00}, //dummy7	 
{0x03,0x00}, //dummy8		 
{0x03,0x00}, //dummy9
{0x03,0x00}, //dummy10		  
{0x01,0x70}, //Sleep Off
};
// kykim mmsvideo 2
struct hi253_i2c_reg_conf hi253_reg_video_10fps_tab[] =
{
	//["video"] Fixed 10 fps//
	//Auto 60Hz//
	{0x03,0x00},
	{0x01,0x71}, //sleep on  
	{0x0e,0x03}, //pll off
	
	{0x03,0x20}, //Page 20					
	{0x10,0x0c}, //AE OFF				
	{0x03,0x22}, //Page 22				
	{0x10,0x69}, //AWB OFF 
	
	{0x03,0x00}, //0 page
	{0x10,0x1b}, //pre 2
	{0x11,0x97},  
	{0x12,0x00}, 
	
	{0x20,0x00},
	{0x21,0x04},
	{0x22,0x00},
	{0x23,0x02}, 
	
	{0x40,0x02}, //Hblank 604
	{0x41,0x5c},
	{0x42,0x00}, //Vblank 20
	{0x43,0x14},
	  
	{0x03,0x18},
	{0x10,0x00}, //scale off   
	  
	{0x03,0x20},
	{0x2a,0xff},				 
	{0x2b,0x34},				 
	{0x2c,0xc3},			// kykim metering	 
	{0x30,0x78},
	
	{0x03, 0x20}, //Page 20
	{0x83, 0x01}, //EXP Normal 30.00 fps 
	{0x84, 0x86}, 
	{0x85, 0x78}, 
	{0x86, 0x01}, //EXPMin 8403.36 fps
	{0x87, 0x65}, 
	{0x88, 0x04}, //EXP Max 10.91 fps 
	{0x89, 0x31}, 
	{0x8a, 0xca}, 
	{0x8B, 0x75}, //EXP100 
	{0x8C, 0x24}, 
	{0x8D, 0x61}, //EXP120 
	{0x8E, 0x9e}, 
	{0x91, 0x04}, //EXP Fix 10.00 fps
	{0x92, 0x93}, 
	{0x93, 0x68}, 
	{0x9c, 0x10}, //EXP Limit 700.28 fps 
	{0x9d, 0xbc}, 
	{0x9e, 0x01}, //EXP Unit 
	{0x9f, 0x65}, 
	
	{0x03, 0x17}, //Page 17 
	{0xC4, 0x54}, //FLK200 
	{0xC5, 0x46}, //FLK240 
	
	//AntiBand Unlock
	{0x03, 0x20}, //Page 20 
	{0x2b, 0x34}, 
	{0x30, 0x78}, 
	
	//BLC 
	{0x03, 0x00}, //PAGE 0
	{0x90, 0x09}, //BLC_TIME_TH_ON
	{0x91, 0x09}, //BLC_TIME_TH_OFF 
	{0x92, 0xd8}, //BLC_AG_TH_ON
	{0x93, 0xd0}, //BLC_AG_TH_OFF
	
	//DCDC 
	{0x03, 0x02}, //PAGE 2
	{0xd4, 0x09}, //DCDC_TIME_TH_ON
	{0xd5, 0x09}, //DCDC_TIME_TH_OFF 
	{0xd6, 0xd8}, //DCDC_AG_TH_ON
	{0xd7, 0xd0}, //DCDC_AG_TH_OFF
			 
{0x03,0x20}, 
{0x18,0x30}, //AE reset off		 
	{0x10,0xcc}, //DC AE ON 12/08
	{0x03,0x22},	
	{0x10,0xff}, //AWB ON
		
	{0x03,0x00}, 
	{0x0e,0x03}, //3)PLL off
	{0x0e,0x73}, //4)PLL x2, on
		
	{0x03,0x00}, //dummy1  
	{0x03,0x00}, //dummy2	
	{0x03,0x00}, //dummy3
	{0x03,0x00}, //dummy4  
	{0x03,0x00}, //dummy5	
	{0x03,0x00}, //dummy6 
	{0x03,0x00}, //dummy7  
	{0x03,0x00}, //dummy8	
	{0x03,0x00}, //dummy9
	{0x03,0x00}, //dummy10	  
	{0x01,0x70}, //Sleep Off
};
// kykim mmsvideo 2
struct hi253_i2c_reg_conf hi253_reg_video_8fps_tab[] =
{
	//["video"] Fixed 8 fps//
	//Auto 60Hz//	
	{0x03,0x00},
	{0x01,0x71}, //sleep on  
	{0x0e,0x03}, //pll off
	
	{0x03,0x20}, //Page 20					
	{0x10,0x0c}, //AE OFF				
	{0x03,0x22}, //Page 22				
	{0x10,0x69}, //AWB OFF 
	
	{0x03,0x00}, //0 page
	{0x10,0x1b}, //pre 2
	{0x11,0x97},  
	{0x12,0x00}, 
	
	{0x20,0x00},
	{0x21,0x04},
	{0x22,0x00},
	{0x23,0x02}, 
	
	{0x40,0x02}, //Hblank 604
	{0x41,0x5c},
	{0x42,0x00}, //Vblank 20
	{0x43,0x14},
	  
	{0x03,0x18},
	{0x10,0x00}, //scale off   
	  
	{0x03,0x20},
	{0x2a,0xff},				 
	{0x2b,0x34},				 
	{0x2c,0xc3},			// kykim metering				 
	{0x30,0x78}, 
	
	{0x03, 0x20}, //Page 20
	{0x83, 0x01}, //EXP Normal 30.00 fps 
	{0x84, 0x86}, 
	{0x85, 0x78}, 
	{0x86, 0x01}, //EXPMin 8403.36 fps
	{0x87, 0x65}, 
	{0x88, 0x05}, //EXP Max 8.57 fps 
	{0x89, 0x56}, 
	{0x8a, 0xa4}, 
	{0x8B, 0x75}, //EXP100 
	{0x8C, 0x24}, 
	{0x8D, 0x61}, //EXP120 
	{0x8E, 0x9e}, 
	{0x91, 0x05}, //EXP Fix 8.00 fps
	{0x92, 0xb8}, 
	{0x93, 0x42}, 
	{0x9c, 0x10}, //EXP Limit 700.28 fps 
	{0x9d, 0xbc}, 
	{0x9e, 0x01}, //EXP Unit 
	{0x9f, 0x65}, 
	
	{0x03, 0x17}, //Page 17 
	{0xC4, 0x54}, //FLK200 
	{0xC5, 0x46}, //FLK240 
	
	//AntiBand Unlock
	{0x03, 0x20}, //Page 20 
	{0x2b, 0x34}, 
	{0x30, 0x78}, 
	
	//BLC 
	{0x03, 0x00}, //PAGE 0
	{0x90, 0x0b}, //BLC_TIME_TH_ON
	{0x91, 0x0b}, //BLC_TIME_TH_OFF 
	{0x92, 0xd8}, //BLC_AG_TH_ON
	{0x93, 0xd0}, //BLC_AG_TH_OFF
	
	//DCDC 
	{0x03, 0x02}, //PAGE 2
	{0xd4, 0x0b}, //DCDC_TIME_TH_ON
	{0xd5, 0x0b}, //DCDC_TIME_TH_OFF 
	{0xd6, 0xd8}, //DCDC_AG_TH_ON
	{0xd7, 0xd0}, //DCDC_AG_TH_OFF
	
	{0x03,0x20}, 
	{0x18,0x30}, //AE reset off   
	{0x10,0xcc}, //DC AE ON 12/08
{0x03,0x22}, 		 
{0x10,0xff}, //AWB ON
	   
{0x03,0x00}, 
{0x0e,0x03}, //3)PLL off
{0x0e,0x73}, //4)PLL x2, on
		  
{0x03,0x00}, //dummy1	 
{0x03,0x00}, //dummy2		 
{0x03,0x00}, //dummy3
{0x03,0x00}, //dummy4	 
{0x03,0x00}, //dummy5		 
{0x03,0x00}, //dummy6	
{0x03,0x00}, //dummy7	 
{0x03,0x00}, //dummy8		 
{0x03,0x00}, //dummy9
{0x03,0x00}, //dummy10		  
{0x01,0x70}, //Sleep Off
};

// kykim flicker
struct hi253_i2c_reg_conf hi253_reg_uxga_tab[] = 	
{	
{0x03,0x00},										   
{0x01,0x71}, //sleep On								
{0x0e,0x03}, //pll off  
																				
{0x03,0x20}, //Page 20								   
{0x10,0x0c}, //AE OFF								   
{0x03,0x22}, //Page 22								   
{0x10,0x69}, //AWB OFF		
		
{0x03,0x00},										   
{0x10,0x0c}, 
{0x12,0x00}, //pclk divider // kykim campp 
	
{0x20,0x00},
{0x21,0x0a}, //04}, 
{0x22,0x00},
{0x23,0x0a}, //02}, 							 
	
{0x03,0x11},
{0x3e,0x80}, //for capture
{0x44,0x80}, //for capture //dark1 ratio
{0x55,0x17}, //0418 0x0d}, //for capture //inddark1 skip th that over
{0x5b,0x3f}, //for capture //impulse_LPF
{0x62,0x10}, //for capture
	
{0x03,0x12},
{0x25,0x30}, //for capture
{0x30,0x50}, //110126 noise improve//0x30}, //for capture
{0x31,0x38}, //for capture
{0x32,0x42}, //for capture
{0x33,0x60}, //for capture
{0x34,0x70}, //for capture
{0x35,0x80}, //for capture
{0x4e,0x90}, //0418 0xb0}, //for capture
{0x4f,0x90}, //0418 0xb0}, //for capture
{0x50,0x90}, //0418 0xc0}, //for capture
		
{0x03,0x13},
{0x12,0x06}, //for capture
{0x5c,0x0d}, //for capture
{0x5d,0x0a}, //for capture
{0x5f,0x0d}, //for capture
{0x60,0x0c}, //for capture
{0x61,0x0b}, //for capture
{0x80,0xfd}, //0418//for capture
{0x82,0x0a}, //for capture
{0x94,0x05}, //110126 noise improve//0x13}, //for capture
{0x95,0x18}, //110126 noise improve//0x54}, //for capture
{0xa4,0x01}, //for capture
{0xa5,0x01}, //for capture
{0xbc,0x27}, //0418 0x55}, //for capture
{0xbd,0x30}, //0418 0x5a}, //for capture
{0xbe,0x2a}, //0418 0x57}, //for capture
{0xbf,0x27}, //0418 0x55}, //for capture
{0xc0,0x30}, //0418 0x5a}, //for capture
{0xc1,0x2a}, //0418 0x57}, //for capture

{0x03,0x18},  							   
{0x10,0x00}, 	

{0x03,0x00}, 							   
{0x0e,0x03}, //PLL off					   
{0x0e,0x73}, //PLL x2, on  
			
{0x03,0x00}, //dummy1	 
{0x03,0x00}, //dummy2		 
{0x03,0x00}, //dummy3
{0x03,0x00}, //dummy4	 
{0x03,0x00}, //dummy5		 
{0x03,0x00}, //dummy6	
{0x03,0x00}, //dummy7	 
{0x03,0x00}, //dummy8		 
{0x03,0x00}, //dummy9
{0x03,0x00}, //dummy10				   
			
{0x03,0x00},//Page 0
{0x01,0x70},//Sensor sleep off, Output Drivability Max	
};


struct hi253_i2c_reg_conf hi253_reg_flip_mirror_off[] = 
{
	//mirror b[0]->x, flip  b[1]->y 
	{0x03, 0x00},
	{0x11, 0x90},
};

struct hi253_i2c_reg_conf hi253_reg_mirror[] = 
{
	//mirror b[0]->x, flip  b[1]->y 
	{0x03, 0x00},
	{0x11, 0x91},
};

struct hi253_i2c_reg_conf hi253_reg_flip[] = 
{
	//mirror b[0]->x, flip  b[1]->y 
	{0x03, 0x00},
	{0x11, 0x92},
};

struct hi253_i2c_reg_conf hi253_reg_flip_mirror[] = 
{
	//mirror b[0]->x, flip  b[1]->y 
	{0x03, 0x00},
	{0x11, 0x93},
};

struct hi253_i2c_reg_conf hi253_reg_evoff_n2ev[] =
{
	/* -2.0EV */
	{0x03, 0x10},
	{0x13, 0x0a},
	{0x4a, 0x40},
};

struct hi253_i2c_reg_conf hi253_reg_evoff_n1ev[] =
{	
	/* -1.0EV */
	{0x03, 0x10},
	{0x13, 0x0a},
	{0x4a, 0x60},
};

struct hi253_i2c_reg_conf hi253_reg_evoff_0ev[] =
{
	/* default */
	{0x03, 0x10},
	{0x13, 0x0a},
	{0x4a, 0x80},
};

struct hi253_i2c_reg_conf hi253_reg_evoff_1ev[] =
{
	/* 1.0EV */
	{0x03, 0x10},
	{0x13, 0x0a},
	{0x4a, 0xa0},
};

struct hi253_i2c_reg_conf hi253_reg_evoff_2ev[] =
{
	/* 2.0EV */
	{0x03, 0x10},
	{0x13, 0x0a},
	{0x4a, 0xc0},
};

// [[ GTSystem_exposure_compensation
struct hi253_i2c_reg_conf hi253_reg_ae_n2[] = 
{
	//GTSystem_exposure_compensation -2
	{0x03, 0x20},
	{0x70, 0x5e},
	{0x78, 0x23},
	{0x79, 0x38},
	{0x76, 0x32},
	{0x77, 0xc1},
};

struct hi253_i2c_reg_conf hi253_reg_ae_n1[] = 
{
	//GTSystem_exposure_compensation -1
	{0x03, 0x20},
	{0x70, 0x66},
	{0x78, 0x23},
	{0x79, 0x3d},
	{0x76, 0x43},
	{0x77, 0xd2},

};

struct hi253_i2c_reg_conf hi253_reg_ae_0[] = 
{
	//GTSystem_exposure_compensation 0
	{0x03, 0x20},
	{0x70, 0x76},
	{0x78, 0x23},
	{0x79, 0x46},
	{0x76, 0x43},
	{0x77, 0xf2},

};

struct hi253_i2c_reg_conf hi253_reg_ae_1[] = 
{
	//GTSystem_exposure_compensation 1
	{0x03, 0x20},
	{0x70, 0x86},
	{0x78, 0x24},
	{0x79, 0x50},
	{0x76, 0x53},
	{0x77, 0x02},

};

struct hi253_i2c_reg_conf hi253_reg_ae_2[] = 
{
	//GTSystem_exposure_compensation 2
	{0x03, 0x20},
	{0x70, 0x8e},
	{0x78, 0x24},
	{0x79, 0x55},
	{0x76, 0x53},
	{0x77, 0x02},
};
// ]] GTSystem_exposure_compensation

struct hi253_i2c_reg_conf hi253_reg_wb_auto[] =
{
	/* Auto: */
//	GTSystem_whitebalance Auto
	{0x03, 0x22},	
	{0x11, 0x2e},
	{0x80, 0x2d},			
	{0x82, 0x42},			
	{0x83, 0x50},
	{0x84, 0x10},
	{0x85, 0x60},
	{0x86, 0x23},
};

struct hi253_i2c_reg_conf hi253_reg_wb_sunny[] =
{
	/* Sunny: *///riguang
//GTSystem_whitebalance Daylight
	{0x03, 0x22},
	{0x11, 0x28},		  
	{0x80, 0x59},
	{0x82, 0x29},
	{0x83, 0x60},
	{0x84, 0x50},
	{0x85, 0x2f},
	{0x86, 0x23},
};

struct hi253_i2c_reg_conf hi253_reg_wb_cloudy[] =
{
	/* Cloudy: *///yintian
//	GTSystem_whitebalance cloudy	// kykim tuneWB
	{0x03, 0x22},	
	{0x11, 0x28},	
	{0x80, 0x6f}, //71},	
	{0x82, 0x22}, //2b},				
	{0x83, 0x72},	
	{0x84, 0x6a}, //70},	
	{0x85, 0x25}, //2b},	
	{0x86, 0x20}, //28},
};

struct hi253_i2c_reg_conf hi253_reg_wb_office[] =
{
	/* Office: *///yingguang
//	GTSystem_whitebalance Fluorescent
	{0x03, 0x22},	
	{0x11, 0x28},	
	{0x80, 0x30}, //41},	
	{0x82, 0x45}, //42},				
	{0x83, 0x3a}, //44},	
	{0x84, 0x2a}, //34},	
	{0x85, 0x4a}, //46},	
	{0x86, 0x40}, //3a},
};

struct hi253_i2c_reg_conf hi253_reg_wb_home[] =
{
	/* Home: *///baizhideng
//	GTSystem_whitebalance Incandescence
	{0x03, 0x22},	
	{0x11, 0x28},	
	{0x80, 0x20}, //29},	
	{0x82, 0x54},				
	{0x83, 0x23}, //2e},	
	{0x84, 0x18}, //23},	
	{0x85, 0x58},	
	{0x86, 0x4f},
};

struct hi253_i2c_reg_conf hi253_reg_contrast_6[] =
{
	/*GTSystem_contrast contrast +3: */
	{0x03, 0x10},
	{0x48, 0xb0},

};
struct hi253_i2c_reg_conf hi253_reg_contrast_5[] =
{
	/*GTSystem_contrast contrast +2: */
	{0x03, 0x10},
	{0x48, 0xa0},

};

struct hi253_i2c_reg_conf hi253_reg_contrast_4[] =
{
	/*GTSystem_contrast contrast +1: */
	{0x03, 0x10},
	{0x48, 0x90},
};

struct hi253_i2c_reg_conf hi253_reg_contrast_3[] =
{
	/*GTSystem_contrast contrast 0: */
	{0x03, 0x10},
	{0x48, 0x80},
};

struct hi253_i2c_reg_conf hi253_reg_contrast_2[] =
{

	/*GTSystem_contrast contrast -1: */
	{0x03, 0x10},
	{0x48, 0x70},
};

struct hi253_i2c_reg_conf hi253_reg_contrast_1[] =
{
	/*GTSystem_contrast contrast -2: */
	{0x03, 0x10},
	{0x48, 0x60},
};

struct hi253_i2c_reg_conf hi253_reg_contrast_0[] =
{
	/*GTSystem_contrast contrast -3: */
	{0x03, 0x10},
	{0x48, 0x50},
};

struct hi253_i2c_reg_conf hi253_reg_brightness_6[] =
{
	/*GTSystem_brightness Brightness +3: */
	{0x03, 0x10},
	{0x40, 0x30},
};

struct hi253_i2c_reg_conf hi253_reg_brightness_5[] =
{
	/*GTSystem_brightness Brightness +2: */
	{0x03, 0x10},
	{0x40, 0x20},
};

struct hi253_i2c_reg_conf hi253_reg_brightness_4[] =
{
	/*GTSystem_brightness Brightness +1: */
	{0x03, 0x10},
	{0x40, 0x10},

};

struct hi253_i2c_reg_conf hi253_reg_brightness_3[] =
{
	/*GTSystem_brightness Brightness 0: */
	{0x03, 0x10},
	{0x40, 0x00},

};

struct hi253_i2c_reg_conf hi253_reg_brightness_2[] =
{
	/*GTSystem_brightness Brightness -1: */
	{0x03, 0x10},
	{0x40, 0x90},

};

struct hi253_i2c_reg_conf hi253_reg_brightness_1[] =
{
	/*GTSystem_brightness Brightness -2: */
	{0x03, 0x10},
	{0x40, 0xa0},
};

struct hi253_i2c_reg_conf hi253_reg_brightness_0[] =
{
	/*GTSystem_brightness Brightness -3: */
	{0x03, 0x10},
	{0x40, 0xc0},
};


struct hi253_i2c_reg_conf hi253_reg_effect_normal[] =
{
	/* normal: */
//	GTSystem nomal
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x30},	//GTSystem_brightness 0x12 -- adding of subtracting brightness : b[4]
	{0x13, 0x0a},	//GTSystem_contrast 0x13(Page Mode = 10) -- B[1] Control the contrast bit
	{0x44, 0x80},
	{0x45, 0x80},
};

struct hi253_i2c_reg_conf hi253_reg_effect_black_white[] =
{
	/* B&W: */
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x03},
	{0x13, 0x0a},
	{0x40, 0x00},
	{0x44, 0x80},
	{0x45, 0x80},
};

struct hi253_i2c_reg_conf hi253_reg_effect_negative[] =
{
	/* Negative: */
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x08},
	{0x13, 0x0a},
	{0x14, 0x00},
};

struct hi253_i2c_reg_conf hi253_reg_effect_old_movie[] =
{
	/* Sepia(antique): */
//	GTSystem Sepia
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x33},
	{0x13, 0x0a},
	{0x44, 0x25},
	{0x45, 0xa6},
};

struct hi253_i2c_reg_conf hi253_reg_effect_bluish[] =
{
	/* Bluish: */
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x03},
	{0x40, 0x00},
	{0x13, 0x0a},
	{0x44, 0xb0},
	{0x45, 0x40},
};

struct hi253_i2c_reg_conf hi253_reg_effect_gray_scale[] =
{
//	GTSystem Grayscale
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x03},
	{0x13, 0x02},
	{0x44, 0x80},
	{0x45, 0x80},
	{0x47, 0x7f},
};

struct hi253_i2c_reg_conf hi253_reg_effect_sepiagreen[] = 
{
//	GTSystem Sepiagreen
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x13},
	{0x13, 0x02},
	{0x44, 0x30},
	{0x45, 0x50},
	{0x47, 0x7f},
};
struct hi253_i2c_reg_conf hi253_reg_effect_sepiablue[] =
{
	//GTSystem /* Sepiablue: */
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x03},
	{0x13, 0x02},
	{0x44, 0xb0},
	{0x45, 0x40},
	{0x47, 0x7f},
};
struct hi253_i2c_reg_conf hi253_reg_effect_colorinv[] =
{
	//GTSystem /* Color_inv: */
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x08},
	{0x13, 0x02},
	{0x14, 0x00},
	{0x44, 0x80},
	{0x45, 0x80},
	{0x47, 0x7f},
};
struct hi253_i2c_reg_conf hi253_reg_effect_grayinv[] =
{
	//GTSystem /* Gray_inv: */
	{0x03, 0x10},
	{0x11, 0x03},
	{0x12, 0x0b},
	{0x13, 0x02},
	{0x14, 0x00},
	{0x44, 0x80},
	{0x45, 0x80},
	{0x47, 0x7f},
};
struct hi253_i2c_reg_conf hi253_reg_effect_embossment[] =
{
	//GTSystem /* Embossement: */
	{0x03, 0x10},
	{0x11, 0x23},
	{0x12, 0x30},
	{0x13, 0x02},
	{0x44, 0x80},
	{0x45, 0x80},
	{0x47, 0x7f},
};
//GTSystem
struct hi253_i2c_reg_conf hi253_reg_effect_sketch[] =
{
	//GTSystem /* Sketch: */
	{0x03, 0x10},
	{0x11, 0x13},
	{0x12, 0x38},
	{0x13, 0x02},
	{0x44, 0x80},
	{0x45, 0x80},
	{0x47, 0x7f},
};


struct hi253_i2c_reg_conf hi253_reg_effect_posterize[] =
{
    {0x03, 0x10},
    {0x11, 0x0b}, 
    {0x12, 0x00}, 
    {0x13, 0x00}, 
    {0x40, 0x00}, 
    {0x46, 0xf0},
};

struct hi253_i2c_reg_conf hi253_reg_effect_aqua[] =
{
    {0x03, 0x10},
    {0x11, 0x03}, 
    {0x12, 0x23}, 
    {0x13, 0x00}, 
    {0x40, 0x00}, 
    {0x44, 0x80},
    {0x45, 0x04}, 
    {0x47, 0x7f}, 
    {0x03, 0x13}, 
    {0x20, 0x07},
    {0x21, 0x07},
};
// kykim effect night shot mode on/off 		// kykim flicker
struct hi253_i2c_reg_conf hi253_reg_effect_nightshot_on[] =
{
//////////// Night shot On //////////////////////
    {0x03, 0x00}, 
    {0x11, 0x93}, 	// fixed fps disable
    // BLC
    {0x03, 0x00}, 	// page 0
    {0x90, 0x14}, 	// blc time th on
    {0x91, 0x14}, 	// blc time th off 
    {0x92, 0xd8},	// blc ag th on 
    {0x93, 0xd0}, 	// blc ag th off
   //DCDC
    {0x03, 0x02}, 	//page 2
    {0xd4, 0x14}, 	// DCDC time th on
    {0xd5, 0x14},	//DCDC time th off
    {0xd6, 0xd8},	// DCDC AG TH_on
	{0xd7, 0xd0},	//DCDC AG TH_off

    {0x03, 0x20}, 	
    {0x10, 0x0c}, 	//ae off
    {0x03, 0x22}, 
    {0x10, 0x69},	//awb off

    {0x03, 0x20}, 
    {0x18, 0x38}, 	//ae reset on

    {0x03, 0x20}, //Page 20 12/08
    {0x83, 0x04}, //EXP Normal 10.00 fps 
    {0x84, 0x93}, 
    {0x85, 0x68}, 
    {0x86, 0x01}, //EXPMin 8403.36 fps
    {0x87, 0x65}, 
    {0x88, 0x09}, 	//EXP Max 5.00 fps
    {0x89, 0x26}, 
    {0x8a, 0xd0}, 
    {0x8B, 0x75}, //EXP100 
    {0x8C, 0x24}, 
    {0x8D, 0x61}, //EXP120 
    {0x8E, 0x9e}, 
    {0x9c, 0x10}, //EXP Limit 700.28 fps 
    {0x9d, 0xbc}, 
    {0x9e, 0x01}, //EXP Unit 
    {0x9f, 0x65}, 

    {0x03, 0x20}, 
    {0x10, 0xcc},       //dc}, //ae on 12/08
    {0x03, 0x22}, 
    {0x10, 0xff},	//awb on

	{0x03, 0x00}, 
	{0x01, 0x70},	//sleep off

	{0x03, 0x20}, 
	{0x18, 0x30},	//AE reset off
    
    {0x03, 0x00},	//dummy 12/08
    {0x03, 0x00}, 	//dummy
    {0x03, 0x00}, 	//dummy
    {0x03, 0x00}, 	//dummy
};
// kykim effect night shot mode on/off
struct hi253_i2c_reg_conf hi253_reg_effect_nightshot_off[] =
{
/////////////////////////////////////////////////
//NightShot Off
///////////////////////////////////////////////// kykim nightshot2
    {0x03, 0x00}, 
    {0x11, 0x93}, 	// fixed fps disable
    // BLC
    {0x90, 0x06}, //BLC_TIME_TH_ON
    {0x91, 0x06}, //BLC_TIME_TH_OFF 
	{0x92, 0xd8}, //BLC_AG_TH_ON
	{0x93, 0xd0}, //BLC_AG_TH_OFF
   //DCDC
	{0x03, 0x02}, //PAGE 2
    {0xd4, 0x06}, //DCDC_TIME_TH_ON
    {0xd5, 0x06}, //DCDC_TIME_TH_OFF 
	{0xd6, 0xd8}, //DCDC_AG_TH_ON
	{0xd7, 0xd0}, //DCDC_AG_TH_OFF

	{0x03, 0x20},
    {0x10, 0x0c}, 	//ae off
	
    {0x03, 0x22}, 
    {0x10, 0x69},	//awb off
    
    {0x03, 0x20}, 
	{0x18, 0x38}, //AE reset on //091030
    
    {0x03, 0x20}, //Page 20
    {0x83, 0x02}, //EXP Normal 20.00 fps 
    {0x84, 0x49}, 
    {0x85, 0xb4}, 
    {0x86, 0x01}, //EXPMin 8403.36 fps
    {0x87, 0x65}, 
    {0x88, 0x03}, //EXP Max 15.00 fps 
    {0x89, 0x0c}, 
    {0x8a, 0xf0}, 
    {0x8B, 0x75}, //EXP100 
    {0x8C, 0x24}, 
    {0x8D, 0x61}, //EXP120 
    {0x8E, 0x9e}, 
    {0x9c, 0x10}, //EXP Limit 700.28 fps 
    {0x9d, 0xbc}, 
    {0x9e, 0x01}, //EXP Unit 
    {0x9f, 0x65},  

	{0x03, 0x20},
    {0x10, 0xcc},  //dc}, //AE on 12/08
    {0x03, 0x22}, 
	{0x10, 0xff}, //AWB on

	{0x03, 0x00}, 
	{0x01, 0x70},	//sleep off

	{0x03, 0x20}, 
	{0x18, 0x30},	//AE reset off

	{0x03, 0x00},	//dummy
	{0x03, 0x00}, 	//dummy
	{0x03, 0x00}, 	//dummy
	{0x03, 0x00}, 	//dummy
};


//[[ GTSystem_saturation
struct hi253_i2c_reg_conf hi253_reg_saturation_6[] =
{
	{0x03, 0x10},
	{0x61, 0x4c},
	{0x62, 0x4c},
};
struct hi253_i2c_reg_conf hi253_reg_saturation_5[] =
{
	{0x03, 0x10},
	{0x61, 0x5c},
	{0x62, 0x5c},
};
struct hi253_i2c_reg_conf hi253_reg_saturation_4[] =
{
	{0x03, 0x10},
	{0x61, 0x6c},
	{0x62, 0x6c},
};
struct hi253_i2c_reg_conf hi253_reg_saturation_3[] =
{
	{0x03, 0x10},
	{0x61, 0x7c},
	{0x62, 0x7c},
};
struct hi253_i2c_reg_conf hi253_reg_saturation_2[] =
{
	{0x03, 0x10},
	{0x61, 0x8c},
	{0x62, 0x8c},
};
struct hi253_i2c_reg_conf hi253_reg_saturation_1[] =
{
	{0x03, 0x10},
	{0x61, 0x9c},
	{0x62, 0x9c},
};
struct hi253_i2c_reg_conf hi253_reg_saturation_0[] =
{
	{0x03, 0x10},
	{0x61, 0xac},
	{0x62, 0xac},
};
//]] GTSystem_saturation

//GTSystem Auto Exposure Mode

struct hi253_i2c_reg_conf hi253_reg_metering_average[] =
{
	{0x03, 0x20},
	{0x10, 0x0c}, //AE off
		
	{0x03, 0x20}, //Weight
	{0x60, 0x00}, //AEweight1
	{0x61, 0x00}, //AEweight2
	{0x62, 0x00}, //AEweight3
	{0x63, 0x00}, //AEweight4
	{0x64, 0x00}, //AEweight5
	{0x65, 0x00}, //AEweight6
	{0x66, 0x00}, //AEweight7
	{0x67, 0x00}, //AEweight8
	{0x68, 0x00}, //AEweight9
	{0x69, 0x00}, //AEweight10
	{0x6a, 0x00}, //AEweight11
	{0x6b, 0x00}, //AEweight12
	{0x6c, 0x00}, //AEweight13
	{0x6d, 0x00}, //AEweight14
	{0x6e, 0x00}, //AEweight15
	{0x6f, 0x00}, //AEweight16
		
	{0x10, 0xcc}, //AE on 12/08
};

struct hi253_i2c_reg_conf hi253_reg_metering_center[] =
{
	{0x03, 0x20},
	{0x10, 0x0c}, //AE off
		
	{0x03, 0x20}, //Weight
	{0x60, 0x00}, //AEweight1
	{0x61, 0x00}, //AEweight2
	{0x62, 0x00}, //AEweight3
	{0x63, 0x00}, //AEweight4
	{0x64, 0x0a}, //AEweight5
	{0x65, 0xa0}, //AEweight6
	{0x66, 0x0a}, //AEweight7
	{0x67, 0xa0}, //AEweight8
	{0x68, 0x0a}, //AEweight9
	{0x69, 0xa0}, //AEweight10
	{0x6a, 0x0a}, //AEweight11
	{0x6b, 0xa0}, //AEweight12
	{0x6c, 0x00}, //AEweight13
	{0x6d, 0x00}, //AEweight14
	{0x6e, 0x00}, //AEweight15
	{0x6f, 0x00}, //AEweight16
		
	{0x10, 0xcc}, //AE on 12/08
};

struct hi253_i2c_reg_conf hi253_reg_metering_spot[] =
{
	{0x03, 0x20},
	{0x10, 0x0c}, //AE off
		
	{0x03, 0x20}, //Weight
	{0x60, 0x00}, //AEweight1
	{0x61, 0x00}, //AEweight2
	{0x62, 0x00}, //AEweight3
	{0x63, 0x00}, //AEweight4
	{0x64, 0x00}, //AEweight5
	{0x65, 0x00}, //AEweight6
	{0x66, 0x0f}, //AEweight7
	{0x67, 0xf0}, //AEweight8
	{0x68, 0x0f}, //AEweight9
	{0x69, 0xf0}, //AEweight10
	{0x6a, 0x00}, //AEweight11
	{0x6b, 0x00}, //AEweight12
	{0x6c, 0x00}, //AEweight13
	{0x6d, 0x00}, //AEweight14
	{0x6e, 0x00}, //AEweight15
	{0x6f, 0x00}, //AEweight16
		
	{0x10, 0xcc}, //AE on 12/08
};


// GTSystem_lens_shade
struct hi253_i2c_reg_conf hi253_reg_lens_shade_disable[] =
{
	{0x03, 0x14},
	{0x10, 0x10},
};

struct hi253_i2c_reg_conf hi253_reg_lens_shade_enable[] =
{
	{0x03, 0x14},
	{0x10, 0x11},
};

//[[ GTSystem_anti_banding
struct hi253_i2c_reg_conf hi253_reg_anti_banding_50hz[] =
{
	{0x03, 0x00}, //Page 0
	{0x40, 0x02}, //Hblank 604
	{0x41, 0x5c}, 
	{0x42, 0x00}, //Vblank 20
	{0x43, 0x14}, 

	{0x03, 0x20}, //Page 20
	{0x83, 0x01}, //EXP Normal 25.00 fps 
	{0x84, 0xd4}, 
	{0x85, 0x90}, 
	{0x86, 0x01}, //EXPMin 8403.36 fps
	{0x87, 0x65}, 
	{0x88, 0x02}, //EXP Max 16.67 fps 
	{0x89, 0xbe}, 
	{0x8a, 0xd8}, 
	{0x8B, 0x75}, //EXP100 
	{0x8C, 0x24}, 
	{0x8D, 0x61}, //EXP120 
	{0x8E, 0x9e}, 
	{0x9c, 0x09}, //EXP Limit 1200.48 fps 
	{0x9d, 0xc3}, 
	{0x9e, 0x01}, //EXP Unit 
	{0x9f, 0x65}, 

	//AntiBand Unlock
	{0x03, 0x20}, //Page 20 
	{0x2b, 0x34}, 
	{0x30, 0x78}, 

	//BLC 
	{0x03, 0x00}, //PAGE 0
	{0x90, 0x06}, //BLC_TIME_TH_ON
	{0x91, 0x06}, //BLC_TIME_TH_OFF 
	{0x92, 0x78}, //BLC_AG_TH_ON
	{0x93, 0x70}, //BLC_AG_TH_OFF

	//DCDC 
	{0x03, 0x02}, //PAGE 2
	{0xd4, 0x06}, //DCDC_TIME_TH_ON
	{0xd5, 0x06}, //DCDC_TIME_TH_OFF 
	{0xd6, 0x78}, //DCDC_AG_TH_ON
	{0xd7, 0x70}, //DCDC_AG_TH_OFF

};

struct hi253_i2c_reg_conf hi253_reg_anti_banding_60hz[] =
{
{0x03, 0x00}, //Page 0
{0x40, 0x02}, //Hblank 604
{0x41, 0x5c}, 
{0x42, 0x00}, //Vblank 20
{0x43, 0x14}, 

{0x03, 0x20}, //Page 20
{0x83, 0x01}, //EXP Normal 30.00 fps 
{0x84, 0x86}, 
{0x85, 0x78}, 
{0x86, 0x01}, //EXPMin 8403.36 fps
{0x87, 0x65}, 
{0x88, 0x03}, //EXP Max 15.00 fps 
{0x89, 0x0c}, 
{0x8a, 0xf0}, 
{0x8B, 0x75}, //EXP100 
{0x8C, 0x24}, 
{0x8D, 0x61}, //EXP120 
{0x8E, 0x9e}, 
{0x9c, 0x09}, //EXP Limit 1200.48 fps 
{0x9d, 0xc3}, 
{0x9e, 0x01}, //EXP Unit 
{0x9f, 0x65}, 

//AntiBand Unlock
{0x03, 0x20}, //Page 20 
{0x2b, 0x34}, 
{0x30, 0x78}, 

//BLC 
{0x03, 0x00}, //PAGE 0
{0x90, 0x08}, //BLC_TIME_TH_ON
{0x91, 0x08}, //BLC_TIME_TH_OFF 
{0x92, 0x78}, //BLC_AG_TH_ON
{0x93, 0x70}, //BLC_AG_TH_OFF

//DCDC 
{0x03, 0x02}, //PAGE 2
{0xd4, 0x08}, //DCDC_TIME_TH_ON
{0xd5, 0x08}, //DCDC_TIME_TH_OFF 
{0xd6, 0x78}, //DCDC_AG_TH_ON
{0xd7, 0x70}, //DCDC_AG_TH_OFF

};

struct hi253_i2c_reg_conf hi253_reg_anti_banding_auto[] =
{
{0x03, 0x00}, //Page 0
{0x40, 0x02}, //Hblank 604
{0x41, 0x5c}, 
{0x42, 0x00}, //Vblank 20
{0x43, 0x14}, 

{0x03, 0x20}, //Page 20
{0x83, 0x01}, //EXP Normal 25.00 fps 
{0x84, 0xd4}, 
{0x85, 0x90}, 
{0x86, 0x01}, //EXPMin 8403.36 fps
{0x87, 0x65}, 
{0x88, 0x02}, //EXP Max 16.67 fps 
{0x89, 0xbe}, 
{0x8a, 0xd8}, 
{0x8B, 0x75}, //EXP100 
{0x8C, 0x24}, 
{0x8D, 0x61}, //EXP120 
{0x8E, 0x9e}, 
{0x9c, 0x09}, //EXP Limit 1200.48 fps 
{0x9d, 0xc3}, 
{0x9e, 0x01}, //EXP Unit 
{0x9f, 0x65}, 

{0x03, 0x17}, //Page 17 
{0xC4, 0x54}, //FLK200 
{0xC5, 0x46}, //FLK240 

//AntiBand Unlock
{0x03, 0x20}, //Page 20 
{0x2b, 0x34}, 
{0x30, 0x78}, 

//BLC 
{0x03, 0x00}, //PAGE 0
{0x90, 0x06}, //BLC_TIME_TH_ON
{0x91, 0x06}, //BLC_TIME_TH_OFF 
{0x92, 0x78}, //BLC_AG_TH_ON
{0x93, 0x70}, //BLC_AG_TH_OFF

//DCDC 
{0x03, 0x02}, //PAGE 2
{0xd4, 0x06}, //DCDC_TIME_TH_ON
{0xd5, 0x06}, //DCDC_TIME_TH_OFF 
{0xd6, 0x78}, //DCDC_AG_TH_ON
{0xd7, 0x70}, //DCDC_AG_TH_OFF

};

//]] GTSystem_anti_banding

/************************************************************************

			general function

************************************************************************/

static int hi253_reset(const struct msm_camera_sensor_info *dev)
{
	int rc = 0;

	rc = gpio_request(dev->sensor_reset, "hi253");

	if (!rc) {
		rc = gpio_direction_output(dev->sensor_reset, 0);
		msleep(20);
		gpio_set_value_cansleep(dev->sensor_reset, 1);
		msleep(20);
	}

	return rc;
}

/************************************************************************

			general function

************************************************************************/
static int hi253_i2c_rxdata(unsigned short saddr, unsigned char *rxdata,
	int length)
{
	struct i2c_msg msgs[] = {
		{
			.addr   = saddr,
			.flags = 0,
			.len   = 1,
			.buf   = rxdata,
		},
		{
			.addr   = saddr,
			.flags = I2C_M_RD,
			.len   = length,
			.buf   = rxdata,
		},
	};

	if (i2c_transfer(hi253_client->adapter, msgs, 2) < 0) {
		CDBG("hi253_i2c_rxdata failed!\n");
		return -EIO;
	}

	return 0;
}

static int hi253_i2c_txdata(unsigned short saddr,
	unsigned char *txdata, int length)
{
	struct i2c_msg msg[] = {
		{
		.addr  = saddr,
		.flags = 0,
		.len = length,
		.buf = txdata,
		},
	};

	if (i2c_transfer(hi253_client->adapter, msg, 1) < 0) {
		CDBG("hi253_i2c_txdata failed\n");
		return -EIO;
	}

	return 0;
}

static int hi253_i2c_write_b(unsigned short saddr, unsigned char baddr,
	unsigned char bdata)
{
	int rc = 0;
	unsigned char buf[3];

	memset(buf, 0, sizeof(buf));
	buf[0] = baddr;
	buf[1] = bdata;

	rc = hi253_i2c_txdata(saddr, buf, 2);

	if (rc < 0)
		printk("i2c_write_w failed, addr = 0x%x, val = 0x%x!\n", baddr, bdata);

	return rc;
}

static int hi253_i2c_write_table(struct hi253_i2c_reg_conf *reg_cfg_tbl, int num)
{
	int i;
	int rc = 0;
	for (i = 0; i < num; i++) 
	{
		rc = hi253_i2c_write_b(hi253_client->addr, 
				reg_cfg_tbl->baddr, reg_cfg_tbl->bdata);		
		if (rc < 0) 
		{
			printk("##################hi253_i2c_write_table %d!###############\n", i);
			//break;
			rc = hi253_i2c_write_b(hi253_client->addr,
					reg_cfg_tbl->baddr, reg_cfg_tbl->bdata);
		}
		reg_cfg_tbl++;
	}

//	return rc;
	return 0;
}

static int hi253_i2c_read(unsigned short saddr, unsigned char raddr,
	unsigned char *rdata)
{
	int rc = 0;

	if (!rdata)
		return -EIO;

	rc = hi253_i2c_rxdata(saddr, &raddr, 1);
	if (rc < 0)
		return rc;

	*rdata = raddr;

	if (rc < 0)
		CDBG("hi253_i2c_read failed!\n");

	return rc;
}

static int hi253_version_revision(unsigned char *pid)
{
	hi253_i2c_read(hi253_client->addr, HI253_PID_REG, pid);

    printk("****************************************************\n");
	printk("The value of hi253 ID is:pid=%x\n", *pid);
	printk("****************************************************\n");
	
	return 0;
}
// kykim exif exp
static int hi253_exposure_read(uint32_t *EXP_TIME)
{
	#define EXPOSURE_PAGE 		0x20
	#define EXPOSURE_REG_H		0x80
	#define EXPOSURE_REG_M		0x81
	#define EXPOSURE_REG_L		0x82
	int rc =0;
	uint8_t temp_exp =0;
	*EXP_TIME =0;
	rc = hi253_i2c_write_b(hi253_client->addr,0x03, EXPOSURE_PAGE);		  // kykim exif exp :30 ->03
	if (rc < 0) 
	{
		printk("##################hi253_i2c_write_table###############\n");
		//break;
		rc = hi253_i2c_write_b(hi253_client->addr,0x03, EXPOSURE_PAGE);  // kykim exif exp :30 ->03
	}
	if(rc < 0){
		printk("****************************************************\n");
		printk("Fail to change 20 page mode");
		printk("****************************************************\n");
	}
	rc = hi253_i2c_read(hi253_client->addr, EXPOSURE_REG_H, &temp_exp);	
	printk("Exposure Time High =%x\n", temp_exp);
	*EXP_TIME = (temp_exp << 16)|(*EXP_TIME);

	hi253_i2c_read(hi253_client->addr, EXPOSURE_REG_M, &temp_exp);
	printk("Exposure Time Mid =%x\n", temp_exp);
	*EXP_TIME = (temp_exp << 8)|(*EXP_TIME);
	
	hi253_i2c_read(hi253_client->addr, EXPOSURE_REG_L, &temp_exp);
	printk("Exposure Time Low =%x\n", temp_exp);
	*EXP_TIME = temp_exp |(*EXP_TIME);

    printk("****************************************************\n");
	printk("Exposure Time=%x\n", *EXP_TIME);
	printk("****************************************************\n");
	
	return rc;
}
// kykim mmsvideo
static int hi253_set_mmsvideo(uint32_t mms_fps)
{
	int rc =0;

	rc = hi253_i2c_write_table(&hi253_reg_video_8fps_tab[0], 
				ARRAY_SIZE(hi253_reg_video_8fps_tab));
	if(rc < 0){
		printk("****************************************************\n");
		printk("Fail to set mms video mode");
		printk("****************************************************\n");
	}
    printk("****************************************************\n");
	printk("Success mms video mode fps=%d\n", mms_fps);
	printk("****************************************************\n");
	
	return rc;
}

// kykim exif iso read
static int hi253_iso_read(uint8_t *ISO)
{
	#define ISO_PAGE 		0x20
	#define ISO_REG			0xb0
	int rc =0;
	rc = hi253_i2c_write_b(hi253_client->addr,0x03, ISO_PAGE);	  // kykim exif exp :30 ->03	
	if (rc < 0) 
	{
		printk("##################hi253_i2c_write_table###############\n");
		//break;
		rc = hi253_i2c_write_b(hi253_client->addr,0x03, ISO_PAGE);  // kykim exif exp :30 ->03
	}
	if(rc < 0){
		printk("****************************************************\n");
		printk("Fail to change 20 page mode");
		printk("****************************************************\n");
	}
	rc = hi253_i2c_read(hi253_client->addr, ISO_REG, ISO);

    printk("****************************************************\n");
	printk("ISO Value=%x\n", *ISO);
	printk("****************************************************\n");
	// kykim test exp 
	//{	uint32_t testExp = 0;
	//	hi253_exposure_read(&testExp);
	//}
	
	return rc;
}

static int hi253_reg_init(void)
{
    int rc = 0;
	HI253_DBG(KERN_INFO "hi253: sensor init\n");

	rc = hi253_i2c_write_table(&hi253_reg_init_tab[0],
			ARRAY_SIZE(hi253_reg_init_tab));
	if (rc < 0)
		return rc;
	
	HI253_DBG(KERN_INFO "hi253: sensor init end\n");
	
	return 0;
}


/*****************************************************************************

                 Settings
 
******************************************************************************/
static int hi253_set_effect(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);
	switch(value) {
	case CAMERA_EFFECT_OFF:
		rc = hi253_i2c_write_table(&hi253_reg_effect_normal[0],
		        ARRAY_SIZE(hi253_reg_effect_normal));
		break;			
	case CAMERA_EFFECT_MONO:
		rc = hi253_i2c_write_table(&hi253_reg_effect_black_white[0],
		        ARRAY_SIZE(hi253_reg_effect_black_white));
		break;			
	case CAMERA_EFFECT_NEGATIVE:
		rc = hi253_i2c_write_table(&hi253_reg_effect_negative[0],
		        ARRAY_SIZE(hi253_reg_effect_negative));
		break;			
#ifdef CONFIG_M7LAB_VENUS_CAMERA	// by sinclair.lee 20110316
	case CAMERA_EFFECT_SOLARIZE:
#else
	case CAMERA_EFFECT_BLUISH:
#endif
		rc = hi253_i2c_write_table(&hi253_reg_effect_bluish[0],
		        ARRAY_SIZE(hi253_reg_effect_bluish));
		break;			
	case CAMERA_EFFECT_SEPIA:
		rc = hi253_i2c_write_table(&hi253_reg_effect_old_movie[0],
		        ARRAY_SIZE(hi253_reg_effect_old_movie));
		break;			
// [[ GTSystem_new_effect
	case CAMERA_EFFECT_GRAYSCALE:
		rc = hi253_i2c_write_table(&hi253_reg_effect_gray_scale[0],
		        ARRAY_SIZE(hi253_reg_effect_black_white));
		break;			

	case CAMERA_EFFECT_SEPIABLUE:
		rc = hi253_i2c_write_table(&hi253_reg_effect_sepiablue[0],
		        ARRAY_SIZE(hi253_reg_effect_old_movie));
		break;			
	case CAMERA_EFFECT_SKETCH:
		rc = hi253_i2c_write_table(&hi253_reg_effect_sketch[0],
		        ARRAY_SIZE(hi253_reg_effect_old_movie));
		break;
	case CAMERA_EFFECT_COLORINV:
		rc = hi253_i2c_write_table(&hi253_reg_effect_colorinv[0],
		        ARRAY_SIZE(hi253_reg_effect_old_movie));
		break;
	case CAMERA_EFFECT_GRAYINV:
		rc = hi253_i2c_write_table(&hi253_reg_effect_grayinv[0],
		        ARRAY_SIZE(hi253_reg_effect_old_movie));
		break;
	case CAMERA_EFFECT_EMBOSSMENT:
		rc = hi253_i2c_write_table(&hi253_reg_effect_embossment[0],
		        ARRAY_SIZE(hi253_reg_effect_old_movie));
		break;

	case CAMERA_EFFECT_SEPIAGREEN:
		rc = hi253_i2c_write_table(&hi253_reg_effect_sepiagreen[0],
		        ARRAY_SIZE(hi253_reg_effect_bluish));
		break;			

	case CAMERA_EFFECT_POSTERIZE:
		rc = hi253_i2c_write_table(&hi253_reg_effect_posterize[0],
		        ARRAY_SIZE(hi253_reg_effect_posterize));
		break;			

	case CAMERA_EFFECT_AQUA:
		rc = hi253_i2c_write_table(&hi253_reg_effect_aqua[0],
		        ARRAY_SIZE(hi253_reg_effect_aqua));
		break;			

	case CAMERA_EFFECT_NIGHTSHOT:		// kykim nightshot
		rc = hi253_i2c_write_table(&hi253_reg_effect_nightshot_on[0],
				ARRAY_SIZE(hi253_reg_effect_nightshot_on));
		break;			

// ]] GTSystem_new_effect		
	default:
		rc = hi253_i2c_write_table(&hi253_reg_effect_normal[0],
		        ARRAY_SIZE(hi253_reg_effect_normal));
		break;			
	}

	current_special_effect = value;
	HI253_DBG("%s  rc=%d\n", __func__, rc);  // kykim videoerr

	return rc;
}
// kykim nightshot2
static int hi253_set_bestshot(int value)
{
	static int nightshot_on= 0;
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);
	switch(value) {
	case CAMERA_BESTSHOT_OFF:
			if(nightshot_on == 1){
				HI253_DBG("CAMERA_BESTSHOT_NIGHT OFF\n" );
		rc = hi253_i2c_write_table(&hi253_reg_effect_nightshot_off[0],
		        ARRAY_SIZE(hi253_reg_effect_nightshot_off));		
				nightshot_on = 0;
			}
		break;			
	case CAMERA_BESTSHOT_NIGHT :		//:
		HI253_DBG("CAMERA_BESTSHOT_NIGHT\n" );	
		rc = hi253_i2c_write_table(&hi253_reg_effect_nightshot_on[0],
		        ARRAY_SIZE(hi253_reg_effect_nightshot_on));
			nightshot_on = 1;
		break;			
	default:
			rc = 0;		
		break;			
	}
	HI253_DBG("%s  rc=%d\n", __func__, rc); 

	return rc;
}

static int hi253_set_contrast(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);
	switch (value) {
#ifdef CONFIG_GTS_JUKE_CAMERA
	case 0:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_0[0],
				ARRAY_SIZE(hi253_reg_contrast_0));
		break;
	case 1:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_1[0],
				ARRAY_SIZE(hi253_reg_contrast_1));
		break;
	case 2:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_2[0],
				ARRAY_SIZE(hi253_reg_contrast_2));
		break;
	case 3:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_3[0],
				ARRAY_SIZE(hi253_reg_contrast_3));
		break;
	case 4:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_4[0],
				ARRAY_SIZE(hi253_reg_contrast_4));
		break;
	case 5:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_5[0],
				ARRAY_SIZE(hi253_reg_contrast_5));
		break;
	case 6:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_6[0],
				ARRAY_SIZE(hi253_reg_contrast_6));
		break;
default:
	rc = hi253_i2c_write_table(&hi253_reg_contrast_2[0],
			ARRAY_SIZE(hi253_reg_contrast_2));
	 break;
}

if (current_special_effect != -1) {
	hi253_set_effect(current_special_effect);
}

#else
	case CAMERA_CONTRAST_LOWEST:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_0[0],
		        ARRAY_SIZE(hi253_reg_contrast_0));
		break;
	case CAMERA_CONTRAST_LOW:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_1[0],
		        ARRAY_SIZE(hi253_reg_contrast_1));
		break;
	case CAMERA_CONTRAST_MIDDLE:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_2[0],
		        ARRAY_SIZE(hi253_reg_contrast_2));
		break;
	case CAMERA_CONTRAST_HIGH:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_3[0],
		        ARRAY_SIZE(hi253_reg_contrast_3));
		break;
	case CAMERA_CONTRAST_HIGHEST:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_4[0],
		        ARRAY_SIZE(hi253_reg_contrast_4));
		break;
	default:
		rc = hi253_i2c_write_table(&hi253_reg_contrast_2[0],
		        ARRAY_SIZE(hi253_reg_contrast_2));
		 break;
	}

	if (current_special_effect != -1) {
        hi253_set_effect(current_special_effect);
	}
#endif

	return rc;
}

static int hi253_set_brightness(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);
	switch (value) {
	   
#ifdef CONFIG_GTS_JUKE_CAMERA
	case 0:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_0[0],
		        ARRAY_SIZE(hi253_reg_brightness_0));
		break;
	case 1:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_1[0],
		        ARRAY_SIZE(hi253_reg_brightness_1));
		break;
	case 2: 
		rc = hi253_i2c_write_table(&hi253_reg_brightness_2[0],
		        ARRAY_SIZE(hi253_reg_brightness_2));
		break;
	case 3:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_3[0],
		        ARRAY_SIZE(hi253_reg_brightness_3));
		break;
	case 4:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_4[0],
		        ARRAY_SIZE(hi253_reg_brightness_4));
		break;
	case 5:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_5[0],
		        ARRAY_SIZE(hi253_reg_brightness_4));
		break;
	case 6:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_6[0],
		        ARRAY_SIZE(hi253_reg_brightness_4));
		break;
	default:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_3[0],
		        ARRAY_SIZE(hi253_reg_brightness_3));
		 break;
	}

	if (current_special_effect != -1) {
        hi253_set_effect(current_special_effect);
	}
#else
	case CAMERA_BRIGHTNESS_LOWEST:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_0[0],
		        ARRAY_SIZE(hi253_reg_brightness_0));
		break;
	case CAMERA_BRIGHTNESS_LOW:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_1[0],
		        ARRAY_SIZE(hi253_reg_brightness_1));
		break;
	case CAMERA_BRIGHTNESS_MIDDLE: 
		rc = hi253_i2c_write_table(&hi253_reg_brightness_2[0],
		        ARRAY_SIZE(hi253_reg_brightness_2));
		break;
	case CAMERA_BRIGHTNESS_HIGH:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_3[0],
		        ARRAY_SIZE(hi253_reg_brightness_3));
		break;
	case CAMERA_BRIGHTNESS_HIGHEST:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_4[0],
		        ARRAY_SIZE(hi253_reg_brightness_4));
		break;
	default:
		rc = hi253_i2c_write_table(&hi253_reg_brightness_2[0],
		        ARRAY_SIZE(hi253_reg_brightness_2));
		 break;
	}

	if (current_special_effect != -1) {
        hi253_set_effect(current_special_effect);
	}
#endif
	return rc;
}

static int hi253_set_exposure(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);
	switch(value) {
#ifdef CONFIG_GTS_JUKE_CAMERA
	case -12:
		/* -2.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_n2ev[0],
				ARRAY_SIZE(hi253_reg_evoff_n2ev));
		break;
	case -6:	
		/* -1.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_n1ev[0],
				ARRAY_SIZE(hi253_reg_evoff_n1ev));
		break;
	case 0:
		/* 0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_0ev[0],
				ARRAY_SIZE(hi253_reg_evoff_0ev));
		break;
	case 6:
		/* 1.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_1ev[0],
				ARRAY_SIZE(hi253_reg_evoff_1ev));
		break;
	case 12:
		/* 2.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_2ev[0],
				ARRAY_SIZE(hi253_reg_evoff_2ev));
		break;
	default:
		rc = hi253_i2c_write_table(&hi253_reg_evoff_0ev[0],
				ARRAY_SIZE(hi253_reg_evoff_0ev));
		break;
#else
	case CAMERA_EXPOSURE_LOWEST:
		/* -2.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_n2ev[0],
		        ARRAY_SIZE(hi253_reg_evoff_n2ev));
		break;
	case CAMERA_EXPOSURE_LOW:
		/* -1.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_n1ev[0],
		        ARRAY_SIZE(hi253_reg_evoff_n1ev));
		break;
	case CAMERA_EXPOSURE_MIDDLE:
		/* 0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_0ev[0],
		        ARRAY_SIZE(hi253_reg_evoff_0ev));
		break;
	case CAMERA_EXPOSURE_HIGH:
		/* 1.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_1ev[0],
		        ARRAY_SIZE(hi253_reg_evoff_1ev));
		break;
	case CAMERA_EXPOSURE_HIGHEST:
		/* 2.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_evoff_2ev[0],
		        ARRAY_SIZE(hi253_reg_evoff_2ev));
		break;
	default:
		rc = hi253_i2c_write_table(&hi253_reg_evoff_0ev[0],
		        ARRAY_SIZE(hi253_reg_evoff_0ev));
		break;
#endif
	}
	return rc;
}

static int hi253_set_exposure_mode(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);
	switch(value) {
	case 0:
		/* -2.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_metering_average[0],
				ARRAY_SIZE(hi253_reg_metering_average));
		break;
	case 1:	
		/* -1.0EV */
		rc = hi253_i2c_write_table(&hi253_reg_metering_center[0],
				ARRAY_SIZE(hi253_reg_metering_center));
		break;
	case 2:
		/* 0EV */
		rc = hi253_i2c_write_table(&hi253_reg_metering_spot[0],
				ARRAY_SIZE(hi253_reg_metering_spot));
		break;
	
	default:
		rc = hi253_i2c_write_table(&hi253_reg_metering_average[0],
				ARRAY_SIZE(hi253_reg_metering_average));
		break;
	}
	return rc;
}

static int hi253_set_white_balance(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);

	switch((camera_wb_type)value) {
	case CAMERA_WB_AUTO:			/* Auto */
		rc = hi253_i2c_write_table(&hi253_reg_wb_auto[0],
		        ARRAY_SIZE(hi253_reg_wb_auto));
		break;

	case CAMERA_WB_INCANDESCENT:			/* Incandescent */
		rc = hi253_i2c_write_table(&hi253_reg_wb_home[0],
		        ARRAY_SIZE(hi253_reg_wb_home));
		break;			

	case CAMERA_WB_DAYLIGHT:			/* Sunny */
		rc = hi253_i2c_write_table(&hi253_reg_wb_sunny[0],
		        ARRAY_SIZE(hi253_reg_wb_sunny));
		break;

	case CAMERA_WB_FLUORESCENT: 		/* Fluorescent */
		rc = hi253_i2c_write_table(&hi253_reg_wb_office[0],
		        ARRAY_SIZE(hi253_reg_wb_office));
		break;			

	case CAMERA_WB_CLOUDY_DAYLIGHT:			/* ext: Cloudy */
		rc = hi253_i2c_write_table(&hi253_reg_wb_cloudy[0],
		        ARRAY_SIZE(hi253_reg_wb_cloudy));
		break;
	default:
		rc = hi253_i2c_write_table(&hi253_reg_wb_auto[0],
		        ARRAY_SIZE(hi253_reg_wb_auto));
		break;						
	}	

	return rc;
}

//GTSystem_saturation
static int hi253_set_saturation(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);

	switch(value) {
	case 0:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_6[0],
		        ARRAY_SIZE(hi253_reg_saturation_0));
		break;

	case 1:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_5[0],
		        ARRAY_SIZE(hi253_reg_saturation_1));
		break;			

	case 2:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_4[0],
		        ARRAY_SIZE(hi253_reg_saturation_2));
		break;

	case 3:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_3[0],
		        ARRAY_SIZE(hi253_reg_saturation_3));
		break;			

	case 4:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_2[0],
		        ARRAY_SIZE(hi253_reg_saturation_4));
		break;
	case 5:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_1[0],
		        ARRAY_SIZE(hi253_reg_saturation_5));
		break;
	case 6:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_0[0],
		        ARRAY_SIZE(hi253_reg_saturation_6));
		break;
	default:
		rc = hi253_i2c_write_table(&hi253_reg_saturation_3[0],
		        ARRAY_SIZE(hi253_reg_saturation_3));
		break;						
	}	
	return rc;
}

// GTSystem_lens_shade
static int hi253_set_lens_shade(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);

	switch(value) {
	case 0:
		rc = hi253_i2c_write_table(&hi253_reg_lens_shade_disable[0],
		        ARRAY_SIZE(hi253_reg_lens_shade_disable));
		break;

	case 1:
		rc = hi253_i2c_write_table(&hi253_reg_lens_shade_enable[0],
		        ARRAY_SIZE(hi253_reg_lens_shade_enable));
		break;			

	default:
		rc = hi253_i2c_write_table(&hi253_reg_lens_shade_disable[0],
		        ARRAY_SIZE(hi253_reg_lens_shade_disable));
		break;						
	}	
	return rc;
}

//GTSystem_anti_banding
static int hi253_set_anti_banding(int value)
{
    int rc = 0;
	HI253_DBG("%s  value=%d\n", __func__, value);

	switch(value) {
	case 1:
		rc = hi253_i2c_write_table(&hi253_reg_anti_banding_60hz[0],
		        ARRAY_SIZE(hi253_reg_anti_banding_60hz));
		break;

	case 2:
		rc = hi253_i2c_write_table(&hi253_reg_anti_banding_50hz[0],
		        ARRAY_SIZE(hi253_reg_anti_banding_50hz));
		break;	

	case 3:
		rc = hi253_i2c_write_table(&hi253_reg_anti_banding_auto[0],
		        ARRAY_SIZE(hi253_reg_anti_banding_auto));
		break;	

	default:
		rc = hi253_i2c_write_table(&hi253_reg_anti_banding_auto[0],
		        ARRAY_SIZE(hi253_reg_anti_banding_auto));
		break;						
	}	
	return rc;
}

#if 0
static int hi253_set_flip_mirror(int flip, int mirror)
{
    int rc = 0;
	HI253_DBG("%s  flip=%d, mirror=%d\n", __func__, flip, mirror);
	
	if ((1 == flip) && (1 == mirror)) {
		rc = hi253_i2c_write_table(&hi253_reg_flip_mirror[0],
			    ARRAY_SIZE(hi253_reg_flip_mirror));	
	}
	else if ((1 == flip) && (0 == mirror)) {
		rc = hi253_i2c_write_table(&hi253_reg_flip[0],
			    ARRAY_SIZE(hi253_reg_flip));
	}
	else if ((0 == flip) && (1 == mirror)) {
		rc = hi253_i2c_write_table(&hi253_reg_mirror[0],
			    ARRAY_SIZE(hi253_reg_mirror));
	}
	else {
		rc = hi253_i2c_write_table(&hi253_reg_flip_mirror_off[0],
			    ARRAY_SIZE(hi253_reg_flip_mirror_off));
	}
	
	return rc;
}
#endif

static void sensor_power_ctrl(const struct msm_camera_sensor_info *sinfo, int pwr, int flag)
{
#ifdef CONFIG_M7LAB_VENUS_CAMERA	// by sinclair.lee 20110316
	if(pwr)
	{
		gpio_set_value(sinfo->sensor_pwd, 0);
	}
	else	//	Teddy
	{
		gpio_set_value(sinfo->sensor_pwd, 1);
	}
#else
	struct msm_camera_device_platform_data *camdev = sinfo->pdata;
	
	if (camdev->camera_power_ctrl) {
		camdev->camera_power_ctrl(pwr, flag);
	}
#endif
}

int HI253_pv_HI253_exposure_lines=0x015f00;
int g_snapshot = 1;
/*
*/
static int hi253_set_sensor_mode(int mode)
{
	int rc = 0;
//	unsigned char reg;
#if 0 //gts_comzo
	int HI253_cp_HI253_exposure_lines;
	unsigned char reg1;
	unsigned char reg2;
	unsigned char reg3;
	//unsigned char reg11;
	//unsigned char reg22;
	//unsigned char reg33;
//
#endif
	switch (mode) {
	case SENSOR_PREVIEW_MODE:
    {		
    	HI253_DBG("hi253_set_sensor_mode begin\n");
		HI253_DBG("ep=%x, g_snapshot=%d\n",HI253_pv_HI253_exposure_lines,g_snapshot);
		if(g_snapshot&&(HI253_pv_HI253_exposure_lines>0xffff)) //
		{
			HI253_DBG("--------------------reset exposure value---------------------\n");
			hi253_i2c_write_b(hi253_client->addr, 0x03, 0x20);				
			hi253_i2c_write_b(hi253_client->addr, 0x83, HI253_pv_HI253_exposure_lines >> 16);			
			hi253_i2c_write_b(hi253_client->addr, 0x84, (HI253_pv_HI253_exposure_lines >> 8) & 0x000000FF);			
			hi253_i2c_write_b(hi253_client->addr, 0x85, HI253_pv_HI253_exposure_lines & 0x000000FF);
			g_snapshot = 0;
		}		
#if 0
		rc = hi253_i2c_write_table(&hi253_reg_video_base_tab[0],
					 ARRAY_SIZE(hi253_reg_video_base_tab));
#endif
	    rc = hi253_i2c_write_table(&hi253_reg_video_20fps_tab[0],  //kykim videofps:// kykim flicker
			  		ARRAY_SIZE(hi253_reg_video_20fps_tab));

//	    rc = hi253_i2c_write_table(&hi253_reg_flip_mirror_on[0],  //flip and mirror
//			  ARRAY_SIZE(hi253_reg_flip_mirror_on));

		HI253_DBG("hi253_set_sensor_mode end\n");

    	break;
    }
	case SENSOR_RAW_SNAPSHOT_MODE:
	case SENSOR_SNAPSHOT_MODE:	
    {
    	
 #if 0   	//gts_comzo
    	HI253_DBG("hi253_set_sensor_mode snapshot begin\n");
		
//		msleep(150);

		hi253_i2c_write_b(hi253_client->addr, 0x03, 0x20);
		hi253_i2c_write_b(hi253_client->addr, 0x10, 0x1C);		
	
		hi253_i2c_read(hi253_client->addr, 0x80, &reg1);
		hi253_i2c_read(hi253_client->addr, 0x81, &reg2);
		hi253_i2c_read(hi253_client->addr, 0x82, &reg3);
		HI253_DBG("reg1=0x%x\n", reg1);//
		HI253_DBG("reg2=0x%x\n", reg2);//
		HI253_DBG("reg3=0x%x\n", reg3);//
		HI253_pv_HI253_exposure_lines =(reg1 << 16)|(reg2<< 8)|reg3;
		HI253_DBG("HI253_pv_HI253_exposure_lines=0x%x\n", HI253_pv_HI253_exposure_lines);//
		
		HI253_cp_HI253_exposure_lines = HI253_pv_HI253_exposure_lines/2;
		HI253_DBG("HI253_pv_HI253_exposure_lines=0x%x\n", HI253_cp_HI253_exposure_lines);//
		
		if(HI253_cp_HI253_exposure_lines<1)
		HI253_cp_HI253_exposure_lines=1;
		
		g_snapshot = 1;
		hi253_i2c_write_b(hi253_client->addr, 0x03, 0x20);				
		hi253_i2c_write_b(hi253_client->addr, 0x83, HI253_cp_HI253_exposure_lines >> 16);			
		hi253_i2c_write_b(hi253_client->addr, 0x84, (HI253_cp_HI253_exposure_lines >> 8) & 0x000000FF);			
		hi253_i2c_write_b(hi253_client->addr, 0x85, HI253_cp_HI253_exposure_lines & 0x000000FF);				
#endif 
		hi253_i2c_write_table(&hi253_reg_uxga_tab[0],ARRAY_SIZE(hi253_reg_uxga_tab));
// kykim exif iso read after shot :
		rc = hi253_iso_read(&temp_iso);//cfg_data.cfg.iso); 		
		rc = hi253_exposure_read(&temp_exp);//cfg_data.cfg.exp); 					
		HI253_DBG("KYKIM AFTER SHOT:ISO=%x, EXP=%x \n", temp_iso, temp_exp);
		if( temp_iso) current_iso = temp_iso;		
		if( temp_exp) current_exp = temp_exp;
			
		msleep(150);
		HI253_DBG("hi253_set_sensor_mode snapshot end\n");
    	break;
    }
#if 0
	case SENSOR_MMS_VIDEO_MODE:		// kykim mmsvideo 2
	{		
    	HI253_DBG("KYKIM]]] SET mms video mode begin\n");
		HI253_DBG("ep=%x, g_snapshot=%d\n",HI253_pv_HI253_exposure_lines,g_snapshot);
		if(g_snapshot&&(HI253_pv_HI253_exposure_lines>0xffff)) //
		{
			HI253_DBG("--------------------reset exposure value---------------------\n");
			hi253_i2c_write_b(hi253_client->addr, 0x03, 0x20);				
			hi253_i2c_write_b(hi253_client->addr, 0x83, HI253_pv_HI253_exposure_lines >> 16);			
			hi253_i2c_write_b(hi253_client->addr, 0x84, (HI253_pv_HI253_exposure_lines >> 8) & 0x000000FF);			
			hi253_i2c_write_b(hi253_client->addr, 0x85, HI253_pv_HI253_exposure_lines & 0x000000FF);
			g_snapshot = 0;
		}		
	    rc = hi253_i2c_write_table(&hi253_reg_video_8fps_tab[0],  
			  		ARRAY_SIZE(hi253_reg_video_8fps_tab));

		HI253_DBG("HI253 SENSOR_MMS_VIDEO_MODE [[X]]" );

    	break;
    }
#endif
	default:
		return -EINVAL;
	}

	return rc;
}

static int hi253_sensor_init_probe(const struct msm_camera_sensor_info *data)
{
	int rc = 0;
	unsigned char cm_rev, cm_pid;
	int timeout;
	int status;

	CDBG("init entry \n");

	rc = hi253_reset(data);
	if (rc < 0) {
		CDBG("reset failed!\n");
		goto init_probe_fail;
	}

	msleep(10);
	/* read out version */
	timeout = 3;
	do
	{
		cm_pid = cm_rev = 0;
		status = hi253_version_revision(&cm_pid);

		/* Check to make sure we are working with an hi253 */
		if (cm_pid != PID_HI253) {
			HI253_DBG("hi253: pid(%x) is not correct, try again!\n", cm_pid);
			msleep(10);
			rc = hi253_reset(data);
		}
		if (--timeout == 0) {
			return -EIO;
		}
	} while(cm_pid != PID_HI253);

	if (rc < 0)
		goto init_probe_fail;

	return rc;

init_probe_fail:
	return rc;
}

static int hi253_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	hi253_ctrl = kzalloc(sizeof(struct hi253_ctrl), GFP_KERNEL);
	if (!hi253_ctrl) {
		CDBG("hi253_init failed!\n");
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		hi253_ctrl->sensordata = data;

	/* Input MCLK = 24MHz */ // GTSystem_clk_rate
	msm_camio_clk_rate_set(24000000);
	mdelay(5);

	msm_camio_camif_pad_reg_reset();

	//pwr para for power on/off and sensor DVDD volt
	//0x1801: DVDD 1.8V, power on
	sensor_power_ctrl(data, 0x1801, 1);

	rc = hi253_sensor_init_probe(data);
	if (rc < 0) {
		CDBG("hi253_sensor_init failed!\n");
		goto init_fail;
	}

	rc = hi253_reg_init();

init_done:
	return rc;

init_fail:
	kfree(hi253_ctrl);
	return rc;
}

static int hi253_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&hi253_wait_queue);
	return 0;
}

static int hi253_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	struct sensor_cfg_data *tmp_data;		// kykim exif iso
	int rc = 0;

	tmp_data = (struct sensor_cfg_data *)argp;   // kykim exif iso

	if (copy_from_user(&cfg_data,
			(void *)argp,
			sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	/* down(&hi253_sem); */

	CDBG("hi253_ioctl, cfgtype = %d, mode = %d\n",
		cfg_data.cfgtype, cfg_data.mode);
	HI253_DBG("hi253_ioctl, cfgtype = %d, mode = %d\n",
		cfg_data.cfgtype, cfg_data.mode);

	switch (cfg_data.cfgtype) {
	case CFG_SET_MODE:
		rc = hi253_set_sensor_mode(cfg_data.mode);
		break;

	case CFG_SET_EFFECT:		
		rc = hi253_set_effect(cfg_data.cfg.effect);
		break;

	case CFG_SET_BESTSHOT:		// kykim nightshot
		rc = hi253_set_bestshot(cfg_data.cfg.bestshot);
		break;

	case CFG_SET_BRIGHTNESS:
		rc = hi253_set_brightness(cfg_data.cfg.brightness);
		break;

	case CFG_SET_CONTRAST:
		rc = hi253_set_contrast(cfg_data.cfg.contrast);
		break;

	case CFG_SET_WB:
		rc = hi253_set_white_balance(cfg_data.cfg.wb);
		break;

	case CFG_SET_EXP_GAIN:
		rc = hi253_set_exposure(cfg_data.cfg.exposure);
		break;
		
	case CFG_SET_EXPOSURE_MODE: //GTSystem_exposure_mode
		rc = hi253_set_exposure_mode(cfg_data.cfg.exposure_mode);
		break;
		
	case CFG_SET_SATURATION:	//GTSystem_saturation
		rc = hi253_set_saturation(cfg_data.cfg.saturation);
		break;

	case CFG_SET_LENS_SHADING:
		rc = hi253_set_lens_shade(cfg_data.cfg.lens_shading);
		break;

	case CFG_SET_ANTIBANDING:
		rc = hi253_set_anti_banding(cfg_data.cfg.antibanding);
		break;
/*
	case CFG_SET_FLIP_MIRROR:
		rc = hi253_set_flip_mirror(cfg_data.flip, cfg_data.mirror);
		break;
*/
	case CFG_GET_ISO :		// kykim exif iso read
		{		
			rc = hi253_iso_read(&temp_iso);//cfg_data.cfg.iso);				
			if( temp_iso) current_iso = temp_iso;		
			cfg_data.cfg.iso = current_iso;
			(*tmp_data).cfg.iso = current_iso;			
			printk("Temp ISO Value=%x, current_iso= %x", temp_iso, current_iso );			
//	printk("Temp data pointer=%p, cfg_data_pointer =%p", tmp_data, &cfg_data);
		}
		break;

	case CFG_GET_EXP :		// kykim exif exp read
	{				
		rc = hi253_exposure_read(&temp_exp);//cfg_data.cfg.exp); 
		if( temp_exp) current_exp = temp_exp;
		cfg_data.cfg.exp = current_exp;
		(*tmp_data).cfg.exp = current_exp;
		printk("Temp EXP Value=%x, config_EXP= %x", temp_exp, cfg_data.cfg.exp );
	}
		break;
	case CFG_SET_MMSVIDEO :		// kykim mmsvideo
	{				
		rc = hi253_set_mmsvideo(cfg_data.cfg.mmsfps);
		printk("HI253 set mms video fps=%d", cfg_data.cfg.mmsfps );
	}
	break;
	case CFG_GET_AF_MAX_STEPS:
	default:
		rc = -EINVAL;
		break;
	}
	HI253_DBG("%s  rc=%d\n", __func__, rc);	// kykim videoerr

	/* up(&hi253_sem); */

	return rc;
}


#ifdef CONFIG_M7LAB_VENUS_CAMERA	// by sinclair.lee 20110316
#else
static int hi253attr_clk_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return 0;
}

static int hi253attr_clk_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t n)
{
	unsigned char cm_pid;
	
	hi253_version_revision(&cm_pid);
	
	return n;
}

static DEVICE_ATTR(clk, S_IRUGO|S_IWUSR, hi253attr_clk_show, hi253attr_clk_store);


static struct attribute *hi253_sysfs_attr[] = {
	&dev_attr_clk.attr,
	NULL
};

static int hi253_sysfs_add(struct kobject *kobj)
{
	int i, n, ret;
	n = ARRAY_SIZE(hi253_sysfs_attr);
	for (i = 0; i < n; ++i) {
		if( hi253_sysfs_attr[i] ) {
			ret = sysfs_create_file(kobj, hi253_sysfs_attr[i]);
			if (ret)
				return ret;
		}
	}
	return 0;
}

static int hi253_sysfs_rm(struct kobject *kobj)
{
	int i, n;
	n = ARRAY_SIZE(hi253_sysfs_attr);
	for (i = 0; i < n; i++) {
		if( hi253_sysfs_attr[i] )
			sysfs_remove_file(kobj, hi253_sysfs_attr[i]);
	}
	return 0;
}
#endif

static int hi253_sensor_release(void)
{
	int rc = 0;

	gpio_set_value_cansleep(hi253_ctrl->sensordata->sensor_reset, 0);
	msleep(20);
	gpio_free(hi253_ctrl->sensordata->sensor_reset);

#ifdef CONFIG_M7LAB_VENUS_CAMERA	// by sinclair.lee 20110316
	if(hi253_ctrl && hi253_ctrl->sensordata)
		sensor_power_ctrl(hi253_ctrl->sensordata, 0x0000, 1);
	kfree(hi253_ctrl);
#else
	/* down(&hi253_sem); */

	kfree(hi253_ctrl);
	/* up(&hi253_sem); */

	hi253_sysfs_rm(&hi253_client->dev.kobj);

	//pwr para for power on/off and sensor DVDD volt
	//0x0000: power off
	sensor_power_ctrl(data, 0x0000, 1);
#endif

	return rc;
}

static int hi253_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int rc = 0;
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C))
	{
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	hi253_sensorw =
		kzalloc(sizeof(struct hi253_work), GFP_KERNEL);

	if (!hi253_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, hi253_sensorw);
	hi253_init_client(client);
	hi253_client = client;

	CDBG("hi253_probe succeeded!\n");

#ifdef CONFIG_M7LAB_VENUS_CAMERA	// by sinclair.lee 20110316
#else
	hi253_sysfs_add(&client->dev.kobj);
#endif

	return 0;

probe_failure:
	kfree(hi253_sensorw);
	hi253_sensorw = NULL;
	CDBG("hi253_probe failed!\n");
	return rc;
}

#ifdef MODULE
static int hi253_i2c_remove(struct i2c_client *client)
{
	dev_dbg(&client->dev, "%s: enter.\n", __func__);

	return 0;
}
#endif

static const struct i2c_device_id hi253_i2c_id[] = {
	{ "hi253", 0},
	{ },
};

static struct i2c_driver hi253_i2c_driver = {
	.id_table = hi253_i2c_id,
	.probe  = hi253_i2c_probe,
	.remove = __exit_p(hi253_i2c_remove),
	.driver = {
		.name = "hi253",
	},
};

static int hi253_sensor_probe(const struct msm_camera_sensor_info *info,
				struct msm_sensor_ctrl *s)
{
	int rc = i2c_add_driver(&hi253_i2c_driver);
	if (rc < 0 || hi253_client == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}

#ifdef CONFIG_M7LAB_VENUS_CAMERA	// by sinclair.lee 20110316
	rc = gpio_request(info->sensor_pwd, "hi253");
	if (!rc)
		gpio_direction_output(info->sensor_pwd, 1);
	else
	{
		CDBG("hi253_ctrl sensor_pwd gpio request failed!\n");
		goto probe_done;
	}

	if (info->vcm_enable)	// not sure???	//	Teddy
	{
		rc = gpio_request(info->vcm_pwd,
						"hi253");
		if (!rc)
			gpio_direction_output(info->vcm_pwd, 1);
		else
		{
			CDBG("hi253_ctrl vcm_pwd gpio request failed!\n");
			goto probe_done;
		}
		msleep(20);
	}
#endif

	/* Input MCLK = 24MHz */
	msm_camio_clk_rate_set(24000000);
	mdelay(5);

	//pwr para for power on/off and sensor DVDD volt
	//0x1801: DVDD 1.8V, power on
	sensor_power_ctrl(info, 0x1801, 1);

	rc = hi253_sensor_init_probe(info);
	if (rc < 0)
	{
		gpio_free(info->sensor_reset);
		goto probe_done;
	}

	s->s_init = hi253_sensor_init;
	s->s_release = hi253_sensor_release;
	s->s_config  = hi253_sensor_config;
#ifdef CONFIG_GTS_JUKE_CAMERA	// GTSystem 2011.05.23
	s->s_camera_type = BACK_CAMERA_2D;
	s->s_mount_angle  = 90;
#else
	s->s_camera_type = FRONT_CAMERA_2D;
	s->s_mount_angle  = 0;
#endif
	gpio_set_value_cansleep(info->sensor_reset, 0);
	msleep(20);
	gpio_free(info->sensor_reset);

probe_done:
	CDBG("%s %s:%d\n", __FILE__, __func__, __LINE__);
	
	//pwr para for power on/off and sensor DVDD volt
	//0x0000: power off
    sensor_power_ctrl(info, 0x0000, 1);
	return rc;
}

static int __hi253_probe(struct platform_device *pdev)
{
	return msm_camera_drv_start(pdev, hi253_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __hi253_probe,
	.driver = {
		.name = "msm_camera_hi253",
		.owner = THIS_MODULE,
	},
};

static int __init hi253_init(void)
{
	return platform_driver_register(&msm_camera_driver);
}

static void __exit hi253_exit(void)
{
	platform_driver_unregister(&msm_camera_driver);
}

#ifdef MODULE
EXPORT_SYMBOL(hi253_init);
EXPORT_SYMBOL(hi253_exit);
#endif

module_init(hi253_init);
module_exit(hi253_exit);

MODULE_AUTHOR("Sinclair.Lee <sinclair.lee@m7system.com>");
MODULE_DESCRIPTION("Hynix hi253 driver");
MODULE_LICENSE("GPL");

