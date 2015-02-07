/* arch/arm/mach-msm/qdsp5/snd.c
 *
 * interface to "snd" service on the baseband cpu
 *
 * Copyright (C) 2008 HTC Corporation
 * Copyright (c) 2009, Code Aurora Forum. All rights reserved.
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

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/msm_audio.h>
#include <linux/seq_file.h>
#include <asm/atomic.h>
#include <asm/ioctls.h>
#include <mach/board.h>
#include <mach/msm_rpcrouter.h>
#include <mach/debug_mm.h>
#include <linux/gpio.h>
#include <mach/pmic.h>

/* define to use audio test app */
#define USE_AUDIO_TEST_APP 1  //MG_shkim 110929 ฟ๘บน 

bool is_ext_amp_on;
int is_sound_playing;
int speaker_mode;
//speaker_mode==0 No_Speaker
//speaker_mode==1 Speaker
//speaker_mode==2 Speaker_Phone

struct snd_ctxt {
	struct mutex lock;
	int opened;
	struct msm_rpc_endpoint *ept;
	struct msm_snd_endpoints *snd_epts;
};

struct snd_sys_ctxt {
	struct mutex lock;
	struct msm_rpc_endpoint *ept;
};

static struct snd_sys_ctxt the_snd_sys;

static struct snd_ctxt the_snd;

#define RPC_SND_PROG	0x30000002
#define RPC_SND_CB_PROG	0x31000002

#define RPC_SND_VERS                    0x00020001

#define SND_SET_DEVICE_PROC 2
#define SND_SET_VOLUME_PROC 3
#define SND_AVC_CTL_PROC 29
#define SND_AGC_CTL_PROC 30
/* [[ GTS_20110610 donghee.lee. setLoopbackMode */
#define SND_SET_LOOPBACK_MODE_PROC 61
/* ]] GTS_20110610 donghee.lee. setLoopbackMode */

//USE_AUDIO_TEST_APP - leesh
#define AMP_MODE_SEL1 20
#define AMP_MODE_SEL2 21
#define AMP_PL_SEL 113
#define GPIO_EXT_AMP_EN 76

#ifdef CONFIG_M7LAB_VENUS //leesh USE_AUDIO_TEST_APP
#if 0
#include <linux/socinfo.h>

	//uint32_t venus_hw_id = socinfo_get_hw_id();
	uint32_t venus_hw_id = 0;
#endif
#endif
#define SNDDEV_DUMMY_DATA_UINT32_MAX 0xFFffFFff
                
typedef enum { 
    SND_DEVICE_DEFAULT = 0, 
    SND_DEVICE_HANDSET = SND_DEVICE_DEFAULT+0,                                                          
    SND_DEVICE_HFK = SND_DEVICE_DEFAULT+1,                                                                     
    //SND_DEVICE_HEADSET = SND_DEVICE_DEFAULT+2, /* Mono headset */                               
    //SND_DEVICE_STEREO_HEADSET = SND_DEVICE_DEFAULT+3, /* Stereo headset */                 
    SND_DEVICE_MONO_HEADSET = SND_DEVICE_DEFAULT+2, /* Mono headset */                               
    SND_DEVICE_HEADSET = SND_DEVICE_DEFAULT+3, /* Stereo headset */                 
    SND_DEVICE_AHFK = SND_DEVICE_DEFAULT+4,                                                                    
    SND_DEVICE_SDAC = SND_DEVICE_DEFAULT+5,                                                                     
    SND_DEVICE_SPEAKER_PHONE = SND_DEVICE_DEFAULT+6,                                                   
    SND_DEVICE_TTY_HFK = SND_DEVICE_DEFAULT+7,                                                                 
    SND_DEVICE_TTY_HEADSET = SND_DEVICE_DEFAULT+8,                                                         
    SND_DEVICE_TTY_VCO = SND_DEVICE_DEFAULT+9,                                                                  
    SND_DEVICE_TTY_HCO = SND_DEVICE_DEFAULT+10,                                                                 
    SND_DEVICE_BT_INTERCOM = SND_DEVICE_DEFAULT+11,                   
    //SND_DEVICE_BT_HEADSET = SND_DEVICE_DEFAULT+12,                        
    SND_DEVICE_BT = SND_DEVICE_DEFAULT+12,                        
    SND_DEVICE_BT_AG_LOCAL_AUDIO = SND_DEVICE_DEFAULT+13,               
    SND_DEVICE_USB = SND_DEVICE_DEFAULT+14,                                            
    SND_DEVICE_STEREO_USB = SND_DEVICE_DEFAULT+15,                                
    SND_DEVICE_IN_S_SADC_OUT_HANDSET = SND_DEVICE_DEFAULT+16, /* Input Mono SADD, Output Handset */                           
    SND_DEVICE_IN_S_SADC_OUT_HEADSET = SND_DEVICE_DEFAULT+17, /* Input Stereo SADD, Output Headset */                         
    SND_DEVICE_EXT_S_SADC_OUT_HANDSET = SND_DEVICE_DEFAULT+18, /* Input Stereo SADD, Output Handset */                       
    SND_DEVICE_EXT_S_SADC_OUT_HEADSET = SND_DEVICE_DEFAULT+19, /* Input Stereo SADD, Output Headset */                       
    SND_DEVICE_BT_A2DP_HEADSET = SND_DEVICE_DEFAULT+20, /* A BT device supporting A2DP */                                               
    SND_DEVICE_BT_A2DP_SCO_HEADSET = SND_DEVICE_DEFAULT+21, /* A BT headset supporting A2DP and SCO */                       
    /* Input Internal Codec Stereo SADC, Output External AUXPCM */                                                                                                     
    SND_DEVICE_TX_INT_SADC_RX_EXT_AUXPCM = SND_DEVICE_DEFAULT+22,                                                                                    
    SND_DEVICE_RX_EXT_SDAC_TX_INTERNAL = SND_DEVICE_DEFAULT+23,                                                                                    
    SND_DEVICE_BT_CONFERENCE = SND_DEVICE_DEFAULT+24,                                                                                                           
    SND_DEVICE_IN_S_SADC_OUT_SPEAKER_PHONE = SND_DEVICE_DEFAULT+25,                                                                     
    SND_DEVICE_SPEAKER = SND_DEVICE_DEFAULT+26,                                                                                                               
    SND_DEVICE_VOIP_HANDSET = SND_DEVICE_DEFAULT+27,                                                                                                     
    SND_DEVICE_VOIP_STEREO_HEADSET = SND_DEVICE_DEFAULT+28,                                                                                         
    SND_DEVICE_VOIP_SPEAKER_PHONE = SND_DEVICE_DEFAULT+29,                                                                                           
    SND_DEVICE_BT_STEREO_HEADSET = SND_DEVICE_DEFAULT+30,                                                                                           
#if 0 // MERCURY SND DEVICE add 20110225                                                                                           
    SND_DEVICE_MAX = SND_DEVICE_DEFAULT+31,                                                                                                                         
    SND_DEVICE_CURRENT = SND_DEVICE_DEFAULT+32,                                                                                                                 
#else
	SND_DEVICE_STEREO_HEADSET_MEDIA = SND_DEVICE_DEFAULT+31,
	SND_DEVICE_STEREO_HEADSET_DMB = SND_DEVICE_DEFAULT+32,
	SND_DEVICE_SPEAKER_DMB = SND_DEVICE_DEFAULT+33,
    SND_DEVICE_MAX = SND_DEVICE_DEFAULT+34,                                                                                                                         
    SND_DEVICE_CURRENT = SND_DEVICE_DEFAULT+35,
#endif //ERCURY SND DEVICE add 20110225                                                                               
    /* DO NOT USE: Force this enum to be a 32bit type */                                                                                                                    
    SND_DEVICE_32BIT_DUMMY = SNDDEV_DUMMY_DATA_UINT32_MAX                                                                                              
} snd_device_type;

struct rpc_snd_set_device_args {
	uint32_t device;
	uint32_t ear_mute;
	uint32_t mic_mute;

	uint32_t cb_func;
	uint32_t client_data;
};

struct rpc_snd_set_volume_args {
	uint32_t device;
	uint32_t method;
	uint32_t volume;

	uint32_t cb_func;
	uint32_t client_data;
};

struct rpc_snd_avc_ctl_args {
	uint32_t avc_ctl;
	uint32_t cb_func;
	uint32_t client_data;
};

struct rpc_snd_agc_ctl_args {
	uint32_t agc_ctl;
	uint32_t cb_func;
	uint32_t client_data;
};

struct snd_set_device_msg {
	struct rpc_request_hdr hdr;
	struct rpc_snd_set_device_args args;
};

struct snd_set_volume_msg {
	struct rpc_request_hdr hdr;
	struct rpc_snd_set_volume_args args;
};

struct snd_avc_ctl_msg {
	struct rpc_request_hdr hdr;
	struct rpc_snd_avc_ctl_args args;
};

struct snd_agc_ctl_msg {
	struct rpc_request_hdr hdr;
	struct rpc_snd_agc_ctl_args args;
};

struct snd_endpoint *get_snd_endpoints(int *size);

/* [[ GTS_20110610 donghee.lee. setLoopbackMode */
struct snd_set_loopback_param_rep {
	struct rpc_reply_hdr hdr;
	uint32_t get_mode;
}lrep;	

struct rpc_snd_set_loopback_mode_args {
     uint32_t mode;
     uint32_t cb_func;
     uint32_t client_data;
};

struct snd_set_loopback_mode_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_loopback_mode_args args;
};
/* ]] GTS_20110610 donghee.lee. setLoopbackMode */

/* start : for audio test app by djjeon */
#ifdef USE_AUDIO_TEST_APP

extern long testapp_snd_ioctl_postprocess_SND_SET_DEVICE(int rc, struct snd_ctxt *snd, int device);
extern int testapp_snd_probe(struct miscdevice *snd_misc, struct snd_sys_ctxt *the_snd_sys_context);

#endif 
/*  end  : for audio test app by djjeon */

static inline int check_mute(int mute)
{
	return (mute == SND_MUTE_MUTED ||
		mute == SND_MUTE_UNMUTED) ? 0 : -EINVAL;
}

static int get_endpoint(struct snd_ctxt *snd, unsigned long arg)
{
	int rc = 0, index;
	struct msm_snd_endpoint ept;

	if (copy_from_user(&ept, (void __user *)arg, sizeof(ept))) {
		MM_ERR("snd_ioctl get endpoint: invalid read pointer\n");
		return -EFAULT;
	}

	index = ept.id;
	if (index < 0 || index >= snd->snd_epts->num) {
		MM_ERR("snd_ioctl get endpoint: invalid index!\n");
		return -EINVAL;
	}

	ept.id = snd->snd_epts->endpoints[index].id;
	strncpy(ept.name,
		snd->snd_epts->endpoints[index].name,
		sizeof(ept.name));

	if (copy_to_user((void __user *)arg, &ept, sizeof(ept))) {
		MM_ERR("snd_ioctl get endpoint: invalid write pointer\n");
		rc = -EFAULT;
	}

	return rc;
}

// start : djjeon : for run audio test app : 2011-03-04
#ifdef USE_AUDIO_TEST_APP

extern void set_current_snd_device(int dev);

int set_device_for_audio_test_app(struct snd_sys_ctxt *snd, int dev)
{
    int ret = 0;
    struct snd_set_device_msg dmsg;
    struct snd_set_volume_msg vmsg;

    if (snd == NULL) {
	printk(KERN_ERR "[%s] snd is null\n", __func__);
	return -1;
    }

    printk(KERN_DEBUG "[%s] set device %d\n", __func__, dev);

    memset(&dmsg, 0, sizeof (dmsg));

    dmsg.args.device = cpu_to_be32(dev);
    dmsg.args.ear_mute = cpu_to_be32(1);
    dmsg.args.mic_mute = cpu_to_be32(1);
    dmsg.args.cb_func = -1;
    dmsg.args.client_data = 0;

    if(dev==SND_DEVICE_SPEAKER_PHONE){
	if(!is_ext_amp_on){
#ifdef CONFIG_M7LAB_VENUS //leesh - USE_AUDIO_TEST_APP
		if(0)//if( socinfo_get_hw_id() == HW_ID_REV_V1A )
		{
			gpio_set_value(80,1);
			gpio_set_value(81,1);
		}
		else
		{
			gpio_set_value(124,1);
			gpio_set_value(88,1);
		}		
#else
	    if (get_m7system_board_revision() <= M7SYSTEM_REV_PT2) {
		gpio_set_value(GPIO_EXT_AMP_EN,1);
	    } else {
		gpio_set_value(AMP_PL_SEL,1);
		gpio_set_value(AMP_MODE_SEL1,1);
	    }
#endif
	    is_ext_amp_on=1;
	    MM_INFO("EXT_AMP_ENABLED\n");
	}else{
	    MM_INFO("EXT_AMP_ALREADY_ENABLED\n");			
	}
	speaker_mode=2;
    }
    else if(dev==SND_DEVICE_SPEAKER){
	if(is_sound_playing){
	    if(!is_ext_amp_on){
#ifdef CONFIG_M7LAB_VENUS //leesh - USE_AUDIO_TEST_APP
		if(0)//if( socinfo_get_hw_id() == HW_ID_REV_V1A )
		{
			gpio_set_value(80,1);
			gpio_set_value(81,1);
		}
		else
		{
			gpio_set_value(124,1);
			gpio_set_value(88,1);
		}		
#else
		if (get_m7system_board_revision() <= M7SYSTEM_REV_PT2) {
		    gpio_set_value(GPIO_EXT_AMP_EN,1);
		} else {
		    gpio_set_value(AMP_PL_SEL,1);
		    gpio_set_value(AMP_MODE_SEL1,1);
		}
#endif
		is_ext_amp_on=1;
		MM_INFO("EXT_AMP_ENABLED\n");
	    }else{
		MM_INFO("EXT_AMP_ALREADY_ENABLED\n");			
	    }			
	}
	speaker_mode=1;			
    }
    else if(dev==SND_DEVICE_CURRENT)
	{
	    MM_INFO("Current Device : No Action\n");			
	}
    else{
	if(is_ext_amp_on){				
#ifdef CONFIG_M7LAB_VENUS //leesh - USE_AUDIO_TEST_APP
		if(0)//if( socinfo_get_hw_id() == HW_ID_REV_V1A )
		{
			gpio_set_value(80,0);
			gpio_set_value(81,0);
		}
		else
		{
			gpio_set_value(124,0);
			gpio_set_value(88,0);
		}		
#else
	    if (get_m7system_board_revision() <= M7SYSTEM_REV_PT2) {
		gpio_set_value(GPIO_EXT_AMP_EN,0);
	    } else {
		gpio_set_value(AMP_PL_SEL,0);
		gpio_set_value(AMP_MODE_SEL1,0);
	    }
#endif
	    is_ext_amp_on=0;
	    MM_INFO("EXT_AMP_DISABLED\n");
	}else{
	    MM_INFO("EXT_AMP_ALREADY_DISABLED\n");
	}
	speaker_mode=0;
    }
    MM_INFO("snd_set_device dev=%d\n", dev);

    ret = msm_rpc_call(snd->ept, 
     		      SND_SET_DEVICE_PROC, 
     		      &dmsg, sizeof(dmsg), 5 * HZ);  

    if (ret >= 0) {
	set_current_snd_device(dev);
	
	vmsg.args.device = cpu_to_be32(dev);
	vmsg.args.method = cpu_to_be32(SND_METHOD_VOICE);

	printk(KERN_ERR "snd_set_volume vol.device=%d vol.method=%d vol.volume=%d\n", dev,
	       SND_METHOD_VOICE, 6);
	
	vmsg.args.volume = cpu_to_be32(6);
	vmsg.args.cb_func = -1;
	vmsg.args.client_data = 0;
	
		    
	ret = msm_rpc_call(snd->ept,
			  SND_SET_VOLUME_PROC,
			  &vmsg, sizeof(vmsg), 5 * HZ);
	
	vmsg.args.method = cpu_to_be32(SND_METHOD_VOICE);
	vmsg.args.volume = cpu_to_be32(7);
	vmsg.args.cb_func = -1;
	vmsg.args.client_data = 0;

	printk(KERN_ERR "snd_set_volume vol.device=%d vol.method=%d vol.volume=%d\n", dev,
	       SND_METHOD_VOICE, 7);
	
	ret = msm_rpc_call(snd->ept,
			  SND_SET_VOLUME_PROC,
			  &vmsg, sizeof(vmsg), 5 * HZ);
	
    } else {
	printk(KERN_ERR "[%s] set_device is error", __func__);
    }

    return ret;
}

#endif
//  end  : djjeon : for run audio test app : 2011-03-04

static long snd_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct snd_set_device_msg dmsg;
	struct snd_set_volume_msg vmsg;
	struct snd_avc_ctl_msg avc_msg;
	struct snd_agc_ctl_msg agc_msg;

	struct msm_snd_device_config dev;
	struct msm_snd_volume_config vol;
	struct snd_ctxt *snd = file->private_data;

	/* [[ GTS_20110610 donghee.lee. setLoopbackMode */		
	struct msm_snd_set_loopback_mode_param loopback;
	struct snd_set_loopback_mode_msg lbmsg;	
	/* ]] GTS_20110610 donghee.lee. setLoopbackMode */
	
	int rc = 0;
	static int hac_open = false;

	uint32_t avc, agc;

	mutex_lock(&snd->lock);
	switch (cmd) {
	case SND_SET_DEVICE:
		if (copy_from_user(&dev, (void __user *) arg, sizeof(dev))) {
			MM_ERR("set device: invalid pointer\n");
			rc = -EFAULT;
			break;
		}

		dmsg.args.device = cpu_to_be32(dev.device);
		dmsg.args.ear_mute = cpu_to_be32(dev.ear_mute);
		dmsg.args.mic_mute = cpu_to_be32(dev.mic_mute);
		if (check_mute(dev.ear_mute) < 0 ||
				check_mute(dev.mic_mute) < 0) {
			MM_ERR("set device: invalid mute status\n");
			rc = -EINVAL;
			break;
		}
		dmsg.args.cb_func = -1;
		dmsg.args.client_data = 0;

		if(dev.device==SND_DEVICE_SPEAKER_PHONE){
			if(!is_ext_amp_on){
#ifdef CONFIG_M7LAB_VENUS //leesh - USE_AUDIO_TEST_APP
				if(0)//if( socinfo_get_hw_id() == HW_ID_REV_V1A )
				{
					gpio_set_value(80,1);
					gpio_set_value(81,1);
				}
				else
				{
					gpio_set_value(124,1);
					gpio_set_value(88,1);
				}		
#else
				if (get_m7system_board_revision() <= M7SYSTEM_REV_PT2) {
					gpio_set_value(GPIO_EXT_AMP_EN,1);
				} else {
					gpio_set_value(AMP_PL_SEL,1);
					gpio_set_value(AMP_MODE_SEL1,1);
				}
#endif
				is_ext_amp_on=1;
				MM_INFO("EXT_AMP_ENABLED\n");
			}else{
				MM_INFO("EXT_AMP_ALREADY_ENABLED\n");			
			}
			speaker_mode=2;
		}
		else if(dev.device==SND_DEVICE_SPEAKER){
			if(is_sound_playing){
				if(!is_ext_amp_on){
#ifdef CONFIG_M7LAB_VENUS //leesh - USE_AUDIO_TEST_APP
					if(0)//if( socinfo_get_hw_id() == HW_ID_REV_V1A )
					{
						gpio_set_value(80,1);
						gpio_set_value(81,1);
					}
					else
					{
						gpio_set_value(124,1);
						gpio_set_value(88,1);
					}		
#else
					if (get_m7system_board_revision() <= M7SYSTEM_REV_PT2) {
						gpio_set_value(GPIO_EXT_AMP_EN,1);
					} else {
						gpio_set_value(AMP_PL_SEL,1);
						gpio_set_value(AMP_MODE_SEL1,1);
					}
#endif
					is_ext_amp_on=1;
					MM_INFO("EXT_AMP_ENABLED\n");
				}else{
					MM_INFO("EXT_AMP_ALREADY_ENABLED\n");			
				}			
			}
			speaker_mode=1;			
		}
		else if(dev.device==SND_DEVICE_CURRENT)
		{
			MM_INFO("Current Device : No Action\n");			
		}
		else{
			if(is_ext_amp_on){				
#ifdef CONFIG_M7LAB_VENUS //leesh - USE_AUDIO_TEST_APP
				if(0)//if( socinfo_get_hw_id() == HW_ID_REV_V1A )
				{
					gpio_set_value(80,0);
					gpio_set_value(81,0);
				}
				else
				{
					gpio_set_value(124,0);
					gpio_set_value(88,0);
				}		
#else
				if (get_m7system_board_revision() <= M7SYSTEM_REV_PT2) {
					gpio_set_value(GPIO_EXT_AMP_EN,0);
				} else {
					gpio_set_value(AMP_PL_SEL,0);
					gpio_set_value(AMP_MODE_SEL1,0);
				}
#endif
				is_ext_amp_on=0;
				MM_INFO("EXT_AMP_DISABLED\n");
			}else{
				MM_INFO("EXT_AMP_ALREADY_DISABLED\n");
			}
				speaker_mode=0;
		}
		MM_INFO("snd_set_device dev.device=%d dev.ear_mute=%d dev.mic_mute=%d\n", dev.device,
				dev.ear_mute, dev.mic_mute);

		rc = msm_rpc_call(snd->ept,
			SND_SET_DEVICE_PROC,
			&dmsg, sizeof(dmsg), 5 * HZ);

		if((dev.device == 0) || (dev.device == 16))
		{
			if (!hac_open)
			{
				printk("snd: ==JRD===hac_pa_enable()!\n");
				pmic_spkr_en(LEFT_SPKR, true);
				pmic_speaker_cmd(SPKR_ON);
				pmic_set_speaker_gain(SPKR_GAIN_00DB);
				pmic_spkr_en_left_chan(1);
				hac_open = true;
			}
		}else if (dev.device != 27){
			if (hac_open)
			{
				printk("snd: ==JRD===hac_pa_disable()!\n");
				if(pmic_spkr_en_left_chan(0) < 0)
					printk("snd: ==JRD===pmic_spkr_en_left_chan(0) : error!\n");
				if(pmic_speaker_cmd(SPKR_OFF) < 0)
					printk("snd: ==JRD===pmic_speaker_cmd() : SPKR_OFF error!\n");
				hac_open = false;
			}
		}
		/* start : for audio test app by djjeon */
#if 0//MG_shkim 110929 def USE_AUDIO_TEST_APP

		rc = testapp_snd_ioctl_postprocess_SND_SET_DEVICE(rc, snd, dev.device);

#endif
		/*  end  : for audio test app by djjeon */

		break;

	case SND_SET_VOLUME:
		if (copy_from_user(&vol, (void __user *) arg, sizeof(vol))) {
			MM_ERR("set volume: invalid pointer\n");
			rc = -EFAULT;
			break;
		}

		vmsg.args.device = cpu_to_be32(vol.device);
		vmsg.args.method = cpu_to_be32(vol.method);
		if (vol.method != SND_METHOD_VOICE) {
			MM_ERR("set volume: invalid method\n");
			rc = -EINVAL;
			break;
		}

		vmsg.args.volume = cpu_to_be32(vol.volume);
		vmsg.args.cb_func = -1;
		vmsg.args.client_data = 0;

		MM_INFO("snd_set_volume %d %d %d\n", vol.device,
				vol.method, vol.volume);

		rc = msm_rpc_call(snd->ept,
			SND_SET_VOLUME_PROC,
			&vmsg, sizeof(vmsg), 5 * HZ);
		break;

	case SND_AVC_CTL:
		if (get_user(avc, (uint32_t __user *) arg)) {
			rc = -EFAULT;
			break;
		} else if ((avc != 1) && (avc != 0)) {
			rc = -EINVAL;
			break;
		}

		avc_msg.args.avc_ctl = cpu_to_be32(avc);
		avc_msg.args.cb_func = -1;
		avc_msg.args.client_data = 0;

		MM_INFO("snd_avc_ctl %d\n", avc);

		rc = msm_rpc_call(snd->ept,
			SND_AVC_CTL_PROC,
			&avc_msg, sizeof(avc_msg), 5 * HZ);
		break;

	case SND_AGC_CTL:
		if (get_user(agc, (uint32_t __user *) arg)) {
			rc = -EFAULT;
			break;
		} else if ((agc != 1) && (agc != 0)) {
			rc = -EINVAL;
			break;
		}
		agc_msg.args.agc_ctl = cpu_to_be32(agc);
		agc_msg.args.cb_func = -1;
		agc_msg.args.client_data = 0;

		MM_INFO("snd_agc_ctl %d\n", agc);

		rc = msm_rpc_call(snd->ept,
			SND_AGC_CTL_PROC,
			&agc_msg, sizeof(agc_msg), 5 * HZ);
		break;

	case SND_GET_NUM_ENDPOINTS:
		if (copy_to_user((void __user *)arg,
				&snd->snd_epts->num, sizeof(unsigned))) {
			MM_ERR("get endpoint: invalid pointer\n");
			rc = -EFAULT;
		}
		break;

	case SND_GET_ENDPOINT:
		rc = get_endpoint(snd, arg);
		break;
/* [[ GTS_20110610 donghee.lee. setLoopbackMode */
	case SND_SET_LOOPBACK_MODE:
	printk(KERN_ERR "@@@@@@@@@@@@@@@@@@@@@@@@Snd.c\n");	
	if (copy_from_user(&loopback, (void __user *) arg, sizeof(loopback))) {
		pr_err("snd_ioctl set_loopback_mode: invalid pointer.\n");
		rc = -EFAULT;
		break;
	}

	lbmsg.args.mode = cpu_to_be32(loopback.mode);
	lbmsg.args.cb_func = -1;
	lbmsg.args.client_data = 0;


	pr_info("set_loopback_mode %d \n", loopback.mode);
	
	rc = msm_rpc_call(snd->ept,
		SND_SET_LOOPBACK_MODE_PROC,
		&lbmsg, sizeof(lbmsg), 5 * HZ);
	
	if (rc < 0){
		printk(KERN_ERR "%s:rpc err because of %d\n", __func__, rc);
	}
	else
	{
		loopback.get_param = be32_to_cpu(lrep.get_mode);
		printk(KERN_INFO "%s:loopback mode ->%d\n", __func__, loopback.get_param);
		if (copy_to_user((void __user *)arg, &loopback, sizeof(loopback))) {
			pr_err("snd_ioctl get loopback mode: invalid write pointer.\n");
			rc = -EFAULT;
		}
	}
	break;
/* ]] GTS_20110610 donghee.lee. setLoopbackMode */
	
	default:
		MM_ERR("unknown command\n");
		rc = -EINVAL;
		break;
	}
	mutex_unlock(&snd->lock);

	return rc;
}

static int snd_release(struct inode *inode, struct file *file)
{
	struct snd_ctxt *snd = file->private_data;
	int rc;

	mutex_lock(&snd->lock);
	rc = msm_rpc_close(snd->ept);
	if (rc < 0)
		MM_ERR("msm_rpc_close failed\n");
	snd->ept = NULL;
	snd->opened = 0;
	mutex_unlock(&snd->lock);
	return 0;
}

#ifndef USE_AUDIO_TEST_APP
static
#endif 
int snd_sys_release(void)
{
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	int rc = 0;

	mutex_lock(&snd_sys->lock);
	rc = msm_rpc_close(snd_sys->ept);
	if (rc < 0)
		MM_ERR("msm_rpc_close failed\n");
	snd_sys->ept = NULL;
	mutex_unlock(&snd_sys->lock);
	return rc;
}
static int snd_open(struct inode *inode, struct file *file)
{
	struct snd_ctxt *snd = &the_snd;
	int rc = 0;

	mutex_lock(&snd->lock);
	if (snd->opened == 0) {
		if (snd->ept == NULL) {
			snd->ept = msm_rpc_connect_compatible(RPC_SND_PROG,
					RPC_SND_VERS, 0);
			if (IS_ERR(snd->ept)) {
				rc = PTR_ERR(snd->ept);
				snd->ept = NULL;
				MM_ERR("failed to connect snd svc\n");
				goto err;
			}
		}
		file->private_data = snd;
		snd->opened = 1;
	} else {
		MM_ERR("snd already opened\n");
		rc = -EBUSY;
	}

err:
	mutex_unlock(&snd->lock);
	return rc;
}

#ifndef USE_AUDIO_TEST_APP
static
#endif 
int snd_sys_open(void)
{
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	int rc = 0;

	mutex_lock(&snd_sys->lock);
	if (snd_sys->ept == NULL) {
		snd_sys->ept = msm_rpc_connect_compatible(RPC_SND_PROG,
			RPC_SND_VERS, 0);
		if (IS_ERR(snd_sys->ept)) {
			rc = PTR_ERR(snd_sys->ept);
			snd_sys->ept = NULL;
			MM_ERR("failed to connect snd svc\n");
			goto err;
		}
	} else
		MM_DBG("snd already opened\n");

err:
	mutex_unlock(&snd_sys->lock);
	return rc;
}

static struct file_operations snd_fops = {
	.owner		= THIS_MODULE,
	.open		= snd_open,
	.release	= snd_release,
	.unlocked_ioctl	= snd_ioctl,
};

struct miscdevice snd_misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "msm_snd",
	.fops	= &snd_fops,
};


#ifndef USE_AUDIO_TEST_APP
static
#endif 
long snd_agc_enable(unsigned long arg)
{
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	struct snd_agc_ctl_msg agc_msg;
	int rc = 0;

	if ((arg != 1) && (arg != 0))
		return -EINVAL;

	agc_msg.args.agc_ctl = cpu_to_be32(arg);
	agc_msg.args.cb_func = -1;
	agc_msg.args.client_data = 0;

	MM_DBG("snd_agc_ctl %ld,%d\n", arg, agc_msg.args.agc_ctl);

	rc = msm_rpc_call(snd_sys->ept,
		SND_AGC_CTL_PROC,
		&agc_msg, sizeof(agc_msg), 5 * HZ);
	return rc;
}

#ifndef USE_AUDIO_TEST_APP
static
#endif 
long snd_avc_enable(unsigned long arg)
{
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	struct snd_avc_ctl_msg avc_msg;
	int rc = 0;

	if ((arg != 1) && (arg != 0))
		return -EINVAL;

	avc_msg.args.avc_ctl = cpu_to_be32(arg);

	avc_msg.args.cb_func = -1;
	avc_msg.args.client_data = 0;

	MM_DBG("snd_avc_ctl %ld,%d\n", arg, avc_msg.args.avc_ctl);

	rc = msm_rpc_call(snd_sys->ept,
		SND_AVC_CTL_PROC,
		&avc_msg, sizeof(avc_msg), 5 * HZ);
	return rc;
}

static ssize_t snd_agc_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t status;
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	int rc = 0;

	rc = snd_sys_open();
	if (rc)
		return rc;

	mutex_lock(&snd_sys->lock);

	if (sysfs_streq(buf, "enable"))
		status = snd_agc_enable(1);
	else if (sysfs_streq(buf, "disable"))
		status = snd_agc_enable(0);
	else
		status = -EINVAL;

	mutex_unlock(&snd_sys->lock);
	rc = snd_sys_release();
	if (rc)
		return rc;

	return status ? : size;
}

static ssize_t snd_avc_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t status;
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	int rc = 0;

	rc = snd_sys_open();
	if (rc)
		return rc;

	mutex_lock(&snd_sys->lock);

	if (sysfs_streq(buf, "enable"))
		status = snd_avc_enable(1);
	else if (sysfs_streq(buf, "disable"))
		status = snd_avc_enable(0);
	else
		status = -EINVAL;

	mutex_unlock(&snd_sys->lock);
	rc = snd_sys_release();
	if (rc)
		return rc;

	return status ? : size;
}

static long snd_vol_enable(const char *arg)
{
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	struct snd_set_volume_msg vmsg;
	struct msm_snd_volume_config vol;
	int rc = 0;

	rc = sscanf(arg, "%d %d %d", &vol.device, &vol.method, &vol.volume);
	if (rc != 3) {
		MM_ERR("Invalid arguments. Usage: <device> <method> \
				<volume>\n");
		rc = -EINVAL;
		return rc;
	}

	vmsg.args.device = cpu_to_be32(vol.device);
	vmsg.args.method = cpu_to_be32(vol.method);
	if (vol.method != SND_METHOD_VOICE) {
		MM_ERR("snd_ioctl set volume: invalid method\n");
		rc = -EINVAL;
		return rc;
	}

	vmsg.args.volume = cpu_to_be32(vol.volume);
	vmsg.args.cb_func = -1;
	vmsg.args.client_data = 0;

	MM_DBG("snd_set_volume %d %d %d\n", vol.device, vol.method,
			vol.volume);

	rc = msm_rpc_call(snd_sys->ept,
		SND_SET_VOLUME_PROC,
		&vmsg, sizeof(vmsg), 5 * HZ);
	return rc;
}

static long snd_dev_enable(const char *arg)
{
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	struct snd_set_device_msg dmsg;
	struct msm_snd_device_config dev;
	int rc = 0;

	rc = sscanf(arg, "%d %d %d", &dev.device, &dev.ear_mute, &dev.mic_mute);
	if (rc != 3) {
		MM_ERR("Invalid arguments. Usage: <device> <ear_mute> \
				<mic_mute>\n");
		rc = -EINVAL;
		return rc;
	}
	dmsg.args.device = cpu_to_be32(dev.device);
	dmsg.args.ear_mute = cpu_to_be32(dev.ear_mute);
	dmsg.args.mic_mute = cpu_to_be32(dev.mic_mute);
	if (check_mute(dev.ear_mute) < 0 ||
			check_mute(dev.mic_mute) < 0) {
		MM_ERR("snd_ioctl set device: invalid mute status\n");
		rc = -EINVAL;
		return rc;
	}
	dmsg.args.cb_func = -1;
	dmsg.args.client_data = 0;

	MM_INFO("snd_set_device %d %d %d\n", dev.device, dev.ear_mute,
			dev.mic_mute);

	rc = msm_rpc_call(snd_sys->ept,
		SND_SET_DEVICE_PROC,
		&dmsg, sizeof(dmsg), 5 * HZ);
	return rc;
}

static ssize_t snd_dev_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t status;
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	int rc = 0;

	rc = snd_sys_open();
	if (rc)
		return rc;

	mutex_lock(&snd_sys->lock);
	status = snd_dev_enable(buf);
	mutex_unlock(&snd_sys->lock);

	rc = snd_sys_release();
	if (rc)
		return rc;

	return status ? : size;
}

static ssize_t snd_vol_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t status;
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	int rc = 0;

	rc = snd_sys_open();
	if (rc)
		return rc;

	mutex_lock(&snd_sys->lock);
	status = snd_vol_enable(buf);
	mutex_unlock(&snd_sys->lock);

	rc = snd_sys_release();
	if (rc)
		return rc;

	return status ? : size;
}

static DEVICE_ATTR(agc, S_IWUSR | S_IRUGO,
		NULL, snd_agc_store);

static DEVICE_ATTR(avc, S_IWUSR | S_IRUGO,
		NULL, snd_avc_store);

static DEVICE_ATTR(device, S_IWUSR | S_IRUGO,
		NULL, snd_dev_store);

static DEVICE_ATTR(volume, S_IWUSR | S_IRUGO,
		NULL, snd_vol_store);

static int snd_probe(struct platform_device *pdev)
{
	struct snd_ctxt *snd = &the_snd;
	struct snd_sys_ctxt *snd_sys = &the_snd_sys;
	int rc = 0;
#if 0 //leesh - USE_AUDIO_TEST_APP
	if (get_m7system_board_revision() >= M7SYSTEM_REV_ES) {
		gpio_tlmm_config(GPIO_CFG(AMP_MODE_SEL1, 0, GPIO_OUTPUT, GPIO_PULL_DOWN, GPIO_2MA), GPIO_ENABLE);
		gpio_tlmm_config(GPIO_CFG(AMP_MODE_SEL2, 0, GPIO_OUTPUT, GPIO_PULL_DOWN, GPIO_2MA), GPIO_ENABLE);
		gpio_tlmm_config(GPIO_CFG(AMP_PL_SEL, 0, GPIO_OUTPUT, GPIO_PULL_DOWN, GPIO_2MA), GPIO_ENABLE);

//		gpio_set_value(AMP_PL_SEL,1);
	}
#endif
	mutex_init(&snd->lock);
	mutex_init(&snd_sys->lock);
	snd_sys->ept = NULL;
	snd->snd_epts = (struct msm_snd_endpoints *)pdev->dev.platform_data;
	rc = misc_register(&snd_misc);
	if (rc)
		return rc;

	rc = device_create_file(snd_misc.this_device, &dev_attr_agc);
	if (rc) {
		misc_deregister(&snd_misc);
		return rc;
	}

	rc = device_create_file(snd_misc.this_device, &dev_attr_avc);
	if (rc) {
		device_remove_file(snd_misc.this_device,
						&dev_attr_agc);
		misc_deregister(&snd_misc);
		return rc;
	}

	rc = device_create_file(snd_misc.this_device, &dev_attr_device);
	if (rc) {
		device_remove_file(snd_misc.this_device,
						&dev_attr_agc);
		device_remove_file(snd_misc.this_device,
						&dev_attr_avc);
		misc_deregister(&snd_misc);
		return rc;
	}

	rc = device_create_file(snd_misc.this_device, &dev_attr_volume);
	if (rc) {
		device_remove_file(snd_misc.this_device,
						&dev_attr_agc);
		device_remove_file(snd_misc.this_device,
						&dev_attr_avc);
		device_remove_file(snd_misc.this_device,
						&dev_attr_device);
		misc_deregister(&snd_misc);
	}

/* start : for test app by 7core */

#ifdef USE_AUDIO_TEST_APP //MG_shkim 110916

	rc = testapp_snd_probe(&snd_misc, snd_sys);
	if (rc) {
		device_remove_file(snd_misc.this_device,
						&dev_attr_agc);
		device_remove_file(snd_misc.this_device,
						&dev_attr_avc);
		device_remove_file(snd_misc.this_device,
						&dev_attr_device);
		device_remove_file(snd_misc.this_device,
                   			        &dev_attr_volume);
		misc_deregister(&snd_misc);
	}

#endif

/*  end  : for test app by 7core */

	return rc;
}

static struct platform_driver snd_plat_driver = {
	.probe = snd_probe,
	.driver = {
		.name = "msm_snd",
		.owner = THIS_MODULE,
	},
};

static int __init snd_init(void)
{
	return platform_driver_register(&snd_plat_driver);
}

module_init(snd_init);
