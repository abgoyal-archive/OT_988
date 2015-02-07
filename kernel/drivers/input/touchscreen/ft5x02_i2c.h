
/********************************************************************************
 *                                                                           	*
 *      			            Focaltech Systems (R)                    		*
 *                                                                           	*
 *                               All Rights Reserved                     		*
 *                                                                        		*
 *  THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS      	*
 *  THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS       	*
 *  SUBJECT TO LICENSE TERMS.                               				    *
 *                                                                           	*
 *******************************************************************************/
 
 /*******************************************************************************
 *
 * Filename:
 * ---------
 *   : I2C_PP_26Bytes.h 
 *
 * Project:
 * --------
 *  ctpm
 *
 * Description:
 * ------------
 * Define all the APIs and the needed data structures for I2C_PP_26Bytes.c.
 *   
 *   
 * Author: 
 * -------
 * Wang xinming  
 *
 * 2010-06-07
 *
 * Last changed:
 * ------------- 
 * Author:  
 *
 * Modtime:   
 *
 * Revision: 0.1
*******************************************************************************/

#ifndef _I2C_PP_26BYTE_H
#define _I2C_PP_26BYTE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char	FTS_BYTE;
typedef unsigned short  FTS_WORD;
typedef unsigned int    FTS_DWRD;
typedef signed int      FTS_BOOL;

#define FTS_NULL 	0x0
#define FTS_TRUE 	0x01
#define FTS_FALSE 	0x0

#define PROTOCOL_LEN 26

#define POINTER_CHECK(p)	if((p)==FTS_NULL){return FTS_FALSE;}

/*Note: please modify this MACRO if the slave address changed*/
#define I2C_CTPM_ADDRESS 	0x70

#define I2C_STARTTCH_READ 	0xF9
#define I2C_WORK_STARTREG 	0xFC


#define FT5X02_INT 19
#define FT5x02_WK 97

#define Mtr_Mode 0x01  //monitor mode
#define Hib_Mode 0x03 //hibernate mode


typedef enum
{
	ERR_OK,
	ERR_MODE,
	ERR_READID,
	ERR_ERASE,
	ERR_STATUS,
	ERR_ECC
}E_UPGRADE_ERR_TYPE;

 /* 
 Error status codes:
 */
#define CTPM_NOERROR			(0x01 << 0)
#define CTPM_ERR_PARAMETER		(0x01 << 1)
#define CTPM_ERR_PROTOCOL		(0x01 << 2)
#define CTPM_ERR_ECC			(0x01 << 3)
#define CTPM_ERR_MODE			(0x01 << 4)
#define CTPM_ERR_I2C			(0x01 << 5)
#define CTPM_ERR_SCAN			(0x01 << 6)

/*fts 26bytes protocol,support 5 points*/
#define CTPM_26BYTES_POINTS_MAX	0x05

/*the information of one touch point */
typedef struct
{
	/*x coordinate*/
	FTS_WORD	w_tp_x;

	/*y coordinate*/
	FTS_WORD 	w_tp_y;

	/*point id: start from 1*/
	FTS_BYTE 	bt_tp_id;

	/*0 means press down; 1 means put up*/
	FTS_BYTE 	bt_tp_property;

	/*the strength of the press*/
	FTS_WORD 	w_tp_strenth;
}ST_TOUCH_POINT, *PST_TOUCH_POINT;

typedef enum
{
	TYPE_Z00M_IN,
	TYPE_Z00M_OUT,
	TYPE_INVALIDE
}E_GESTURE_TYPE;

/*the information of one touch */
typedef struct
{
	/*the number of touch points*/
	FTS_BYTE 			bt_tp_num;

	/*touch gesture*/
	E_GESTURE_TYPE		bt_gesture;

	/*point to a list which stored 1 to 5 touch points information*/
	ST_TOUCH_POINT* 	pst_point_info;
}ST_TOUCH_INFO, *PST_TOUCH_INFO;

/*
[function]: 
	write a value to register.
[parameters]:
	e_reg_name[in]	:register name;
	pbt_buf[in]		:the returned register value;
[return]:
	FTS_TRUE	:success;
	FTS_FALSE	:io fail;
*/
extern FTS_BYTE fts_register_read(FTS_BYTE e_reg_name, FTS_BYTE* pbt_buf, FTS_BYTE bt_len);

/*
[function]: 
	get all the information of one touch.
[parameters]:
	pst_touch_info[out]	:stored all the information of one touch;	
[return]:
	CTPM_NOERROR		:success;
	CTPM_ERR_I2C		:io fail;
	CTPM_ERR_PROTOCOL	:protocol data error;
	CTPM_ERR_ECC		:ecc error.
*/
extern FTS_BYTE fts_ctpm_get_touch_info(ST_TOUCH_INFO* pst_touch_info);

/*
[function]: 
	burn the FW to ctpm.
[parameters]:(ref. SPEC)
	pbt_buf[in]	:point to Head+FW ;
	dw_lenth[in]:the length of the FW + 6(the Head length);	
	bt_ecc[in]	:the ECC of the FW
[return]:
	ERR_OK		:no error;
	ERR_MODE	:fail to switch to UPDATE mode;
	ERR_READID	:read id fail;
	ERR_ERASE	:erase chip fail;
	ERR_STATUS	:status error;
	ERR_ECC		:ecc error.
*/
extern E_UPGRADE_ERR_TYPE fts_ctpm_fw_upgrade(FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth);
extern char read_reg(unsigned char addr);
extern char write_reg(unsigned char addr, char v);
extern E_UPGRADE_ERR_TYPE fts_ctpm_fw_upgrade_with_i_file(void);
extern struct i2c_client *ft5x02_client;
#ifdef __cplusplus
}
#endif

#endif
