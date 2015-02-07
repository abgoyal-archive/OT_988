
/********************************************************************************
 *                                                                               *
 *                              Focaltech Systems (R)                            *
 *                                                                               *
 *                               All Rights Reserved                             *
 *                                                                                *
 *  THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS          *
 *  THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS           *
 *  SUBJECT TO LICENSE TERMS.                                                   *
 *                                                                               *
 *******************************************************************************/
 
 /*******************************************************************************
 *
 * Filename:
 * ---------
 *   : I2C_PP_Std.c 
 *
 * Project:
 * --------
 *  ctpm
 *
 * Description:
 * ------------
 * upgrade the CTPM firmware by Host side.
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

#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/wait.h>
#include <linux/time.h>
#include <linux/earlysuspend.h>
#include <linux/delay.h>
#include "ft5x02_i2c.h"
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>

#include <linux/io.h>
#include <linux/platform_device.h>
#include <mach/gpio.h>
extern void T0_Waitms (FTS_WORD ms);



/*the follow three funcions should be implemented by user*/
/**************************************************************************************/
/*
[function]: 
    callback: read data from ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[out]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/





void delay_ms(FTS_WORD  w_ms)
{
    unsigned int i;
    unsigned j;
    for (j = 0; j<w_ms; j++)
     {
        for (i = 0; i < 1000; i++)
        {
            udelay(1);
        }
     }
    
   
    //platform related, please implement this function
    //T0_Waitms(w_ms);
}
//#endif 
/*
[function]: 
    callback: write data to ctpm by i2c interface,implemented by special user;
[parameters]:
    bt_ctpm_addr[in]    :the address of the ctpm;
    pbt_buf[in]        :data buffer;
    dw_lenth[in]        :the length of the data buffer;
[return]:
    FTS_TRUE     :success;
    FTS_FALSE    :fail;
*/

FTS_BOOL i2c_write_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
	int ret;   
	//struct i2c_client *i2c_client;
    ret = i2c_master_send(ft5x02_client, pbt_buf, dw_lenth);
	if(ret <=  0)
{
	printk("write_interface error!\n");
	return 0;
}    
	return 1;
}
FTS_BOOL i2c_read_interface(FTS_BYTE bt_ctpm_addr, FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{
	int ret;   
	//struct i2c_client *i2c_client;
    ret = i2c_master_recv(ft5x02_client, pbt_buf, dw_lenth);
	if(ret <=  0)
{
	printk(" read_interface error!\n");
	return 0;
}    
	return 1;
}
/***************************************************************************************/

/*
[function]: 
	read out the register value.
[parameters]:
	e_reg_name[in]	:register name;
	pbt_buf[out]	:the returned register value;
	bt_len[in]		:length of pbt_buf, should be set to 2;		
[return]:
	FTS_TRUE	:success;
	FTS_FALSE	:io fail;
*/
FTS_BYTE fts_register_read(FTS_BYTE e_reg_name, FTS_BYTE *pbt_buf, FTS_BYTE bt_len)
{
	FTS_BYTE read_cmd[3]= {0};
	FTS_BYTE cmd_len 	= 0;

	read_cmd[0] = I2C_WORK_STARTREG;
	read_cmd[1] = e_reg_name+0x40;
	cmd_len = 2;	

	/*call the write callback function*/
	if(!i2c_write_interface(I2C_CTPM_ADDRESS, read_cmd, cmd_len))
	{
		return FTS_FALSE;
	}

	/*call the read callback function to get the register value*/		
	if(!i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len))
	{
		return FTS_FALSE;
	}
	return FTS_TRUE;
}

/***************************************************************************************/


/*
[function]: 
    write a value to register.
[parameters]:
    e_reg_name[in]    :register name;
    pbt_buf[in]        :the returned register value;
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL fts_register_write(FTS_BYTE e_reg_name, FTS_BYTE bt_value)
{

    unsigned char tmp[4], ecc = 0;
	int32_t rc = 0;

	memset(tmp, 0, 2);
	tmp[0] = 0xfc;
	ecc ^= 0xfc;
	tmp[1] = e_reg_name;
	ecc ^= e_reg_name;
	tmp[2] = bt_value;
	ecc ^= bt_value;
	tmp[3] = ecc;

	rc = i2c_write_interface(I2C_CTPM_ADDRESS, tmp, 4);
	if (rc != 1){
		printk("i2c_write_interface failed!\n");
		return rc;
	}
	return 0;

}


unsigned char CTPM_FW[]=
{
#include "ft5x02_firmware_upgrade.h"
};
/*
[function]: 
	send the opration head to ctpm.
[return]:
	FTS_TRUE	:success;
	FTS_FALSE	:io fail;
[remark]:
	if FT5x06,this function would be empty.
*/

FTS_BOOL Send_OP_HEAD(void)
{
	FTS_BYTE write_cmd[2] = {0};

	write_cmd[0] = 0xfa;
	write_cmd[1] = 0xfa;
	return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, 2);

	return FTS_TRUE;
}


//#if CFG_SUPPORT_FLASH_UPGRADE 
/*
[function]: 
    send a command to ctpm.
[parameters]:
    btcmd[in]        :command code;
    btPara1[in]    :parameter 1;    
    btPara2[in]    :parameter 2;    
    btPara3[in]    :parameter 3;    
    num[in]        :the valid input parameter numbers, if only command code needed and no parameters followed,then the num is 1;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL cmd_write(FTS_BYTE btcmd,FTS_BYTE btPara1,FTS_BYTE btPara2,FTS_BYTE btPara3,FTS_BYTE num)
{
    FTS_BYTE write_cmd[4] = {0};

    write_cmd[0] = btcmd;
    write_cmd[1] = btPara1;
    write_cmd[2] = btPara2;
    write_cmd[3] = btPara3;
    return i2c_write_interface(I2C_CTPM_ADDRESS, write_cmd, num);
 //return ft5x02_i2c_txdata(I2C_CTPM_ADDRESS, write_cmd, num);
}

/*
[function]: 
    write data to ctpm , the destination address is 0.
[parameters]:
    pbt_buf[in]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/
FTS_BOOL byte_write(FTS_BYTE* pbt_buf, FTS_DWRD dw_len)
{
    POINTER_CHECK(pbt_buf);
    
    return i2c_write_interface(I2C_CTPM_ADDRESS, pbt_buf, dw_len);
}

/*
[function]: 
    read out data from ctpm,the destination address is 0.
[parameters]:
    pbt_buf[out]    :point to data buffer;
    bt_len[in]        :the data numbers;    
[return]:
    FTS_TRUE    :success;
    FTS_FALSE    :io fail;
*/

FTS_BOOL byte_read(FTS_BYTE* pbt_buf, FTS_BYTE bt_len)
{
    POINTER_CHECK(pbt_buf);
    
    return i2c_read_interface(I2C_CTPM_ADDRESS, pbt_buf, bt_len);
}


/*
[function]: 
    burn the FW to ctpm.
[parameters]:(ref. SPEC)
    pbt_buf[in]    :point to Head+FW ;
    dw_lenth[in]:the length of the FW + 6(the Head length);    
    bt_ecc[in]    :the ECC of the FW
[return]:
    ERR_OK        :no error;
    ERR_MODE    :fail to switch to UPDATE mode;
    ERR_READID    :read id fail;
    ERR_ERASE    :erase chip fail;
    ERR_STATUS    :status error;
    ERR_ECC        :ecc error.
*/


#define    FTS_PACKET_LENGTH        128

E_UPGRADE_ERR_TYPE fts_ctpm_fw_upgrade_with_i_file(void)
{

    FTS_BYTE*     pbt_buf = FTS_NULL;
    E_UPGRADE_ERR_TYPE  ret;
    unsigned char uc_temp;
    int len = 0;    
    
    
    //=========FW upgrade========================*/
    pbt_buf = CTPM_FW;
	len = (pbt_buf[126] << 8) + pbt_buf[127];
	printk("new firmware len is = %d\n", len);		
   /*call the upgrade function*/
    ret =  fts_ctpm_fw_upgrade(pbt_buf+122,len+6);

    delay_ms(1000);
	uc_temp = read_reg(0x3b);
	printk("new version is = 0x%x\n", uc_temp);	

      return ret;
}




E_UPGRADE_ERR_TYPE  fts_ctpm_fw_upgrade(FTS_BYTE* pbt_buf, FTS_DWRD dw_lenth)
{

    FTS_BYTE reg_val[2] = {0};
	FTS_BYTE reg_name[2] = {0};
    FTS_DWRD i = 0;
	FTS_BYTE write_cmd[4] = {0};
    int ret=0;
    int packnum=0;
    msleep(300); //make sure CTP bootup normally

	/*********Step 1:switch mode from WORK to UPDATE*****/
	printk("Step 1\n"); 
	reg_name[0] = 0x3c; /*mode register*/
	reg_val[0] = read_reg(reg_name[0]);
	if(reg_val[0] == 1)
	{
    /*write 0xaa to register 0xfc*/
		write_cmd[0] = 0xaa;
	
		write_reg(reg_name[0],write_cmd[0]);
     
        delay_ms(10);
	  	write_reg(reg_name[0],write_cmd[0]);

		/*check the register 0x3c again*/
		reg_val[0] = read_reg(reg_name[0]);
		if(reg_val[0] == 0xaa)
		{
            delay_ms(10);
	
			/*write 0x55 to register 0x3c*/
			write_cmd[0] = 0x55;
			write_reg(reg_name[0],write_cmd[0]);
		}
		else
		{
            printk("Step 2: Enter update mode. %x\n",reg_val[0] );
			return ERR_MODE;
		}
	}
	else
	{
        printk("Step 1: Enter update mode. %x\n",reg_val[0] );
		return ERR_MODE;
	}
     printk("Step 2: Enter update mode. \n");
   
    delay_ms(100);
    i = 0;
    /*********Step 3:check READ-ID***********************/
    /*send the opration head*/
    do{
        if(i > 10)
        { 
	        printk("step2 :error!\n");
            return ERR_READID; 
        }
		Send_OP_HEAD();
		/*read out the CTPM ID*/
        cmd_write(0x90,0x00,0x00,0x00,4);
	    delay_ms(1);
        byte_read(reg_val,2);

        i++;
        printk("Step 3: CTPM ID,ID1 = 0x %x,ID2 = 0x %x\n", reg_val[0], reg_val[1]);
    }while(reg_val[0] != 0x79 || reg_val[1] != 0x02);

	/*********Step 3:write enable**************************/
    Send_OP_HEAD();
	cmd_write(0x06,0x00,0x00,0x00,1);
	/*********Step 4:erase*******************************/
	Send_OP_HEAD();
	cmd_write(0x60,0x00,0x00,0x00,1);
    printk("Step 4\n"); 
	/*********Step 5:check Status *************************/
	delay_ms(300);/*wait about 50 ms for erase 19k ram, and about 300ms for erase 27K flash*/ 
	i = 0;
	do
	{
		delay_ms(10);/*wait 10 ms*/ 
		if(i > 3)
		{
			return ERR_ERASE; 
		}		
		Send_OP_HEAD();
		cmd_write(0x05,0x00,0x00,0x00,1);
		byte_read(reg_val,1);
		i++;
	}while((reg_val[0]&0x01) != 0); 
    printk("Step 5\n"); 
	/*********Step 6:write firmware(FW) to ctpm flash*********/
	Send_OP_HEAD();
	packnum = dw_lenth/128;
	printk("start...%x,%x,%d\n",pbt_buf[0],dw_lenth,packnum);

	for(i=0;i<packnum;i++)

	{
		ret= byte_write(pbt_buf+128*i,128);

		if(ret<0)
			printk("write erro=%d,i=%d\n",ret,i);
	}
	byte_write(pbt_buf+128*i,dw_lenth%128);
    printk("Step 6\n"); 
	/*********Step 7:write disable**************************/
	Send_OP_HEAD();
	cmd_write(0x04,0x00,0x00,0x00,1);
    printk("Step 7\n"); 
	/*********Step 8:check status**************************/
    delay_ms(20);

	Send_OP_HEAD();
	cmd_write(0x05,0x00,0x00,0x00,1);
	byte_read(reg_val,1);
	if((reg_val[0] & 0x40) != 0 || (reg_val[0]&0x01) != 0) 
	{
		return ERR_STATUS;
	}
    printk("Step 8\n"); 
	/*********Step 9:read out checksum***********************/
	/*send the opration head*/
	Send_OP_HEAD();
    cmd_write(0xcc,0x00,0x00,0x00,1);
    byte_read(reg_val,1);
    printk("Step 9:  ecc read 0x%x, new firmware 0x%x. \n", reg_val[0], CTPM_FW[3]);

    if(reg_val[0] != CTPM_FW[3])
    {
        printk("step 9 :ecc error!\n");
        return ERR_ECC;
    }

	/*********Step 10:reset the new FW***********************/
	Send_OP_HEAD();
	cmd_write(0x07,0x00,0x00,0x00,1);

    return ERR_OK;
}
//#endif




#ifdef __cplusplus
}
#endif

