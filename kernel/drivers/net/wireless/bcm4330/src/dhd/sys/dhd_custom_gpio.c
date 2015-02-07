/*
* Customer code to add GPIO control during WLAN start/stop
* Copyright (C) 1999-2011, Broadcom Corporation
* 
*         Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2 (the "GPL"),
* available at http://www.broadcom.com/licenses/GPLv2.php, with the
* following added to such license:
* 
*      As a special exception, the copyright holders of this software give you
* permission to link this software with independent modules, and to copy and
* distribute the resulting executable under terms of your choice, provided that
* you also meet, for each linked independent module, the terms and conditions of
* the license of that module.  An independent module is a module which is not
* derived from this software.  The special exception does not apply to any
* modifications of the software.
* 
*      Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a license
* other than the GPL, without Broadcom's express prior written consent.
*
* $Id: dhd_custom_gpio.c,v 1.2.42.1 2010-10-19 00:41:09 Exp $
*/

#include <typedefs.h>
#include <linuxver.h>
#include <osl.h>
#include <bcmutils.h>

#include <dngl_stats.h>
#include <dhd.h>

#include <wlioctl.h>
#include <wl_iw.h>
#include <asm/gpio.h>
// Alvin
#include <mach/mpp.h>

#define WL_ERROR(x) printf x
#define WL_TRACE(x) printf x

#ifdef CUSTOMER_HW
extern  void bcm_wlan_power_off(int);
extern  void bcm_wlan_power_on(int);
#endif /* CUSTOMER_HW */
#if  defined(CONFIG_MACH_MAHIMAHI)
int wifi_set_carddetect(int on);
int wifi_set_power(int on, unsigned long msec);
int wifi_get_irq_number(unsigned long *irq_flags_ptr);
int wifi_get_mac_addr(unsigned char *buf);
#endif

int msm_wifi_set_carddetect(int on);
int msm_wifi_power(int on);
extern void bcm_detect_card(int n);


#if defined(OOB_INTR_ONLY)

#if defined(BCMLXSDMMC)
extern int sdioh_mmc_irq(int irq);
#endif /* (BCMLXSDMMC)  */

#ifdef CUSTOMER_HW3
#include <mach/gpio.h>
#endif

/* Customer specific Host GPIO defintion  */
static int dhd_oob_gpio_num = -1;

module_param(dhd_oob_gpio_num, int, 0644);
MODULE_PARM_DESC(dhd_oob_gpio_num, "DHD oob gpio number");

/* This function will return:
 *  1) return :  Host gpio interrupt number per customer platform
 *  2) irq_flags_ptr : Type of Host interrupt as Level or Edge
 *
 *  NOTE :
 *  Customer should check his platform definitions
 *  and his Host Interrupt spec
 *  to figure out the proper setting for his platform.
 *  Broadcom provides just reference settings as example.
 *
 */
int dhd_customer_oob_irq_map(unsigned long *irq_flags_ptr)
{
	int  host_oob_irq = 0;

#ifdef CONFIG_MACH_MAHIMAHI
	host_oob_irq = wifi_get_irq_number(irq_flags_ptr);

#else /* for NOT  CONFIG_MACH_MAHIMAHI */
#if defined(CUSTOM_OOB_GPIO_NUM)
	if (dhd_oob_gpio_num < 0) {
		dhd_oob_gpio_num = CUSTOM_OOB_GPIO_NUM;
	}
#endif

	if (dhd_oob_gpio_num < 0) {
		WL_ERROR(("%s: ERROR customer specific Host GPIO is NOT defined \n",
			__FUNCTION__));
		return (dhd_oob_gpio_num);
	}

	WL_ERROR(("%s: customer specific Host GPIO number is (%d)\n",
	         __FUNCTION__, dhd_oob_gpio_num));

//add by comtech Tom
#if defined CUSTOM_OOB_GPIO_NUM
	host_oob_irq = MSM_GPIO_TO_INT(dhd_oob_gpio_num);
	WL_TRACE(("chengpenghui===> host_oob_irq = %d ! \n", host_oob_irq));
#endif

#if defined CUSTOMER_HW
	host_oob_irq = MSM_GPIO_TO_INT(dhd_oob_gpio_num);
#elif defined CUSTOMER_HW3
	gpio_request(dhd_oob_gpio_num, "oob irq");
	host_oob_irq = gpio_to_irq(dhd_oob_gpio_num);
	gpio_direction_input(dhd_oob_gpio_num);
#endif /* CUSTOMER_HW */
#endif /* CONFIG_MACH_MAHIMAHI */

	return (host_oob_irq);
}
#endif /* defined(OOB_INTR_ONLY) */

/* Customer function to control hw specific wlan gpios */
void
dhd_customer_gpio_wlan_ctrl(int onoff)
{
	switch (onoff) {
		case WLAN_RESET_OFF:
			WL_TRACE(("%s: call customer specific GPIO to insert WLAN RESET\n",
				__FUNCTION__));
#ifdef CUSTOMER_HW
			bcm_wlan_power_off(2);
#endif /* CUSTOMER_HW */
#ifdef CONFIG_MACH_MAHIMAHI
			wifi_set_power(0, 0);
#endif
			
			msm_wifi_power(0);
			
			WL_ERROR(("=========== WLAN placed in RESET ========\n"));
		break;

		case WLAN_RESET_ON:
			WL_TRACE(("%s: callc customer specific GPIO to remove WLAN RESET\n",
				__FUNCTION__));
#ifdef CUSTOMER_HW
			bcm_wlan_power_on(2);
#endif /* CUSTOMER_HW */
#ifdef CONFIG_MACH_MAHIMAHI
			wifi_set_power(1, 0);
#endif
			msm_wifi_power(1);

			WL_ERROR(("=========== WLAN going back to live  ========\n"));
		break;

		case WLAN_POWER_OFF:
			WL_TRACE(("%s: call customer specific GPIO to turn off WL_REG_ON\n",
				__FUNCTION__));
#ifdef CUSTOMER_HW
			bcm_wlan_power_off(1);
#endif /* CUSTOMER_HW */

			msm_wifi_power(0);
			bcm_detect_card(0);
		break;

		case WLAN_POWER_ON:
			WL_TRACE(("%s: call customer specific GPIO to turn on WL_REG_ON\n",
				__FUNCTION__));
#ifdef CUSTOMER_HW
			bcm_wlan_power_on(1);
#endif /* CUSTOMER_HW */
			/* Lets customer power to get stable */
			OSL_DELAY(200);
			msm_wifi_power(1);
			bcm_detect_card(1);
		break;
	}
}

#ifdef GET_CUSTOM_MAC_ENABLE

//JRD function.
/* FIXME:hard code.
 * filename: /data/misc/wifi/config
 * 	this file will be updated by tool [nvcmd] when boot-up. 
 * 	refer to script [init.rc].
 * string format: [00:12:34:AB:cd:ef]
 * 	with hex value.
 * */
#define MAC_LEN		6
#define MAC_STR_LEN	17
#define MAC_CHAR_NUM	2

static inline int is_in_range(unsigned char c, 
		unsigned char low, unsigned char high)
{
	if((c > high) || (c < low)){
		return 0;
	}else{
		return 1;
	}
}

//convert a hex string to long integer.
static int hex_strtol(const char *p)
{
	int i;
	int acc = 0;
	unsigned char c;
	
	for(i=0; i<MAC_CHAR_NUM; i++){
		c = (unsigned char)(p[i]);
		if (is_in_range(c, '0', '9'))
			c -= '0';
		else if (is_in_range(c, 'a', 'f'))
			c -= ('a' - 10);
		else if (is_in_range(c, 'A', 'F'))
			c -= ('A' - 10);
		else
			break;

		acc = (acc << 4) + c;
	}

	return acc;
}

static int str2wa(const char *str, struct ether_addr *wa)
{
	struct ether_addr tmp;
	int l;
	const char *ptr = str;
	int i;
	int result = 0;
	
	for (i = 0; i < MAC_LEN; i++) {
		l = hex_strtol(ptr);
		if((l > 255) || (l < 0)){
			result = -1;
			break;	
		}

		tmp.octet[i] = (uint8_t)l;

		if(i == MAC_LEN - 1){
			break; //done
		}
		
		ptr = strchr(ptr, ':');
		if(ptr == NULL){
			result = -1;
			break;
		}
		ptr++;
	}

	if(result == 0){
		memcpy((char *)wa, (char*)(&tmp), MAC_LEN);
	}

	return result;
}


static int jrd_get_mac_addr(struct ether_addr *eaddr)
{
	struct file *fp = NULL;
	char fn[100] = {0};
	char dp[MAC_STR_LEN + 1] = {0};
	long l,len;
	loff_t pos;
	mm_segment_t old_fs;
	//FIXME:hard code.
	strcpy(fn, "/data/misc/wifi/config");
	fp = filp_open(fn, O_RDONLY, 0);
	if(IS_ERR(fp)){
		printk(KERN_INFO "Unable to open '%s'.\n", fn);
		goto err_open;
	}

	l = fp->f_path.dentry->d_inode->i_size;
	if(l != MAC_STR_LEN){
		printk(KERN_INFO "Invalid macaddr file '%s' %d\n", fn,l);
		goto err_format;
	}

	pos = 0;
	old_fs=get_fs();
	set_fs(KERNEL_DS);
	len=vfs_read(fp, dp, l, &pos);
	set_fs(old_fs);	
	if(len!= l)
	{
		printk(KERN_INFO "Failed to read '%s'  %d  %s.\n", fn,len,dp );
		goto err_format;
	}

	dp[MAC_STR_LEN] = '\0';
	str2wa(dp, eaddr);
	
	filp_close(fp, NULL);
	return 0;

err_format:
	filp_close(fp, NULL);
err_open:
	return -1;
}


/* Function to get custom MAC address */
int
dhd_custom_get_mac_address(unsigned char *buf)
{
	WL_TRACE(("%s Enter\n", __FUNCTION__));
	if (!buf)
		return -EINVAL;

	/* Customer access to MAC address stored outside of DHD driver */
	
	//JRD mac
	{
		//Friday Dec 10th, 2010.
		struct ether_addr ea_jrd = {{0x00, 0x20, 0x10, 0xDE, 0xC1, 0x0F}};
		struct ether_addr ea_example = {{0x00, 0x20, 0x10, 0xDE, 0xC1, 0x0F}};
		jrd_get_mac_addr(&ea_jrd);
        if((ea_jrd.octet[0]==0x00 && ea_jrd.octet[1]==0x00 && ea_jrd.octet[2]==0x00 ) || (ea_jrd.octet[0]==0xFF && ea_jrd.octet[1]==0xFF && ea_jrd.octet[2]==0xFF ) ){
		bcopy((char *)&ea_example, buf, sizeof(struct ether_addr));
		}else{
		bcopy((char *)&ea_jrd, buf, sizeof(struct ether_addr));
		}
	}

#ifdef EXAMPLE_GET_MAC
	/* EXAMPLE code */
	{
		struct ether_addr ea_example = {{0x00, 0x11, 0x22, 0x33, 0x44, 0xFF}};
		bcopy((char *)&ea_example, buf, sizeof(struct ether_addr));
	}
#endif /* EXAMPLE_GET_MAC */

	return 0;
}
#endif /* GET_CUSTOM_MAC_ENABLE */

/* Customized Locale table : OPTIONAL feature */
const struct cntry_locales_custom translate_custom_table[] = {
/* Table should be filled out based on custom platform regulatory requirement */
#ifdef EXAMPLE_TABLE
	{"US", "US", 69}, /* input ISO "US" to : US regrev 69 */
	{"CA", "US", 69}, /* input ISO "CA" to : US regrev 69 */
	{"EU", "EU", 05}, /* input ISO "CA" to : US regrev 69 */
	{"FR", "EU", 05},
	{"DE", "EU", 05},
	{"IR", "EU", 05},
	{"UK", "EU", 05}, /* input ISO "UK" to : EU regrev 05 */
	{"KR", "XY", 03},
	{"AU", "XY", 03},
	{"CN", "XY", 03}, /* input ISO "CN" to : XY regrev 03 */
	{"TW", "XY", 03},
	{"AR", "XY", 03}
#endif /* EXMAPLE_TABLE */
};


/* Customized Locale convertor
*  input : ISO 3166-1 country abbreviation
*  output: customized cspec
*/
void get_customized_country_code(char *country_iso_code, wl_country_t *cspec)
{
	int size, i;

	size = ARRAYSIZE(translate_custom_table);

	if (cspec == 0)
		 return;

	if (size == 0)
		 return;

	for (i = 0; i < size; i++) {
	if (strcmp(country_iso_code, translate_custom_table[i].iso_abbrev) == 0) {
		memcpy(cspec->ccode,  translate_custom_table[i].custom_locale, WLC_CNTRY_BUF_SZ);
		cspec->rev = translate_custom_table[i].custom_locale_rev;
			break;
		}
	}
	return;
}
