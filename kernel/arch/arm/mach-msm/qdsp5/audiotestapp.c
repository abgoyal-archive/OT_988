#define USE_AUDIO_TEST_APP 1	//MG_shkim 110929 ฟ๘บน

#ifdef USE_AUDIO_TEST_APP

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

static int current_snd_device = -1;

#define SND_VOC_GAIN_SET_CMD_PROC    37+1
#define SND_VOC_GAIN_GET_CMD_PROC    38+1
#define SND_PCM_FILTER_SET_CMD_PROC  39+1
#define SND_PCM_FILTER_GET_CMD_PROC  40+1
#define SND_AGC_SET_CMD_PROC         41+1
#define SND_AGC_GET_CMD_PROC         42+1
#define SND_AVC_SET_CMD_PROC         43+1
#define SND_AVC_GET_CMD_PROC         44+1
#define SND_EC_SET_CMD_PROC          45+1
#define SND_EC_GET_CMD_PROC          46+1
#define SND_NS_SET_CMD_PROC          47+1
#define SND_NS_GET_CMD_PROC          48+1
#define SND_RX_VOLUME_SET_CMD_PROC   49+1
#define SND_RX_VOLUME_GET_CMD_PROC   50+1

// added 
#define SND_EC_GAIN_SET_CMD_PROC     51+1
#define SND_EC_GAIN_GET_CMD_PROC     52+1

#define SND_GET_CURRENT_DEVICE       5292
#define SND_SET_CURRENT_DEVICE_N_VOL 5293
#define SND_SET_DEVICE_              5294

#define SNDDEV_DUMMY_DATA_UINT32_MAX 0xFFffFFff

typedef enum {    SND_DEVICE_DEFAULT = 0,
    SND_DEVICE_HANDSET = SND_DEVICE_DEFAULT+0,
    SND_DEVICE_HFK = SND_DEVICE_DEFAULT+1,
    SND_DEVICE_HEADSET = SND_DEVICE_DEFAULT+2, /* Mono headset */
    SND_DEVICE_STEREO_HEADSET = SND_DEVICE_DEFAULT+3, /* Stereo headset */
    SND_DEVICE_AHFK = SND_DEVICE_DEFAULT+4,
    SND_DEVICE_SDAC = SND_DEVICE_DEFAULT+5,
    SND_DEVICE_SPEAKER_PHONE = SND_DEVICE_DEFAULT+6,
    SND_DEVICE_TTY_HFK = SND_DEVICE_DEFAULT+7,
    SND_DEVICE_TTY_HEADSET = SND_DEVICE_DEFAULT+8,
    SND_DEVICE_TTY_VCO = SND_DEVICE_DEFAULT+9,
    SND_DEVICE_TTY_HCO = SND_DEVICE_DEFAULT+10,
    SND_DEVICE_BT_INTERCOM = SND_DEVICE_DEFAULT+11,
    SND_DEVICE_BT_HEADSET = SND_DEVICE_DEFAULT+12,
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
    SND_DEVICE_MAX = SND_DEVICE_DEFAULT+30,
    SND_DEVICE_CURRENT = SND_DEVICE_DEFAULT+31,
    /* DO NOT USE: Force this enum to be a 32bit type */
    SND_DEVICE_32BIT_DUMMY = SNDDEV_DUMMY_DATA_UINT32_MAX
} snd_device_type;

typedef enum {
    VOC_AUDIO_CAL_TX_VOLUME, //Tx Volume
    VOC_AUDIO_CAL_DTMF, //DTMF Gain
    VOC_AUDIO_CAL_CODEC_TX, //TX Codec Gain
    VOC_AUDIO_CAL_CODEC_RX, //RX Codec Gain
    VOC_AUDIO_CAL_SIDETONE, //Sidetone Gain
} voc_audio_cal_type;

typedef enum {
    VOC_AUDIO_RX_FIR_FILTER,
    VOC_AUDIO_TX_FIR_FILTER,
} voc_audio_pcm_filter_type;

typedef enum {
    VOC_EC_OFF = 0,
    VOC_EC_ESEC,
    VOC_EC_HEADSET,
    VOC_EC_AEC,
    VOC_EC_SPEAKER,
    VOC_EC_BT,
VOC_EC_DEFAULT
} voc_ec_type;

typedef enum {
    VOC_NS_OFF = 0,
    VOC_NS_ON,
    VOC_NS_DEFAULT
} voc_ns_type;

typedef enum {
    /* This method is required
     */
    SOUND_METHOD_VOICE = 0, /* Use the device's voice generator */
    /* Application-Specific Methods
     */
    SOUND_METHOD_KEY_BEEP, /* Use the device's keybeep generator */
    SOUND_METHOD_MESSAGE, /* Use the path's ringer, or voice generator */
    SOUND_METHOD_RING, /* Use the device's ring generator */
    SOUND_METHOD_MIDI, /* Use the device's Midi generator */
    SOUND_METHOD_AUX, /* Use the device's auxiliary generator if avail. */
    SOUND_METHOD_MAX, /* Always last in the list */
    /* DO NOT USE: Force this enum to be a 32bit type */
    SOUND_METHOD_32BIT_DUMMY = SNDDEV_DUMMY_DATA_UINT32_MAX
} snd_method_type;

typedef unsigned char Boolean;

/* start : copy form snd.c */

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

struct rpc_snd_set_volume_args {
	uint32_t device;
	uint32_t method;
	uint32_t volume;

	uint32_t cb_func;
	uint32_t client_data;
};

struct snd_set_volume_msg {
	struct rpc_request_hdr hdr;
	struct rpc_snd_set_volume_args args;
};

#define SND_SET_VOLUME_PROC 3
#define SND_AVC_CTL_PROC 29
#define SND_AGC_CTL_PROC 30

/*  end  : copy form snd.c */

extern int current_snd_device;
static struct snd_sys_ctxt *the_snd_sys_context;

extern long snd_agc_enable(unsigned long arg);
extern long snd_avc_enable(unsigned long arg);
extern int snd_sys_open(void);
extern int snd_sys_release(void);
extern int set_device_for_audio_test_app(struct snd_sys_ctxt *snd, int dev);

long testapp_snd_ioctl_postprocess_SND_SET_DEVICE(int rc, struct snd_ctxt *snd, int device)
{
    struct snd_set_volume_msg vmsg;

    if (rc >= 0) {
	current_snd_device = device;
	
	vmsg.args.device = cpu_to_be32(device);
	vmsg.args.method = cpu_to_be32(SND_METHOD_VOICE);

	printk(KERN_ERR "snd_set_volume vol.device=%d vol.method=%d vol.volume=%d\n", device,
	       SND_METHOD_VOICE, 6);
	
	vmsg.args.volume = cpu_to_be32(6);
	vmsg.args.cb_func = -1;
	vmsg.args.client_data = 0;
	
		    
	rc = msm_rpc_call(snd->ept,
			  SND_SET_VOLUME_PROC,
			  &vmsg, sizeof(vmsg), 5 * HZ);
	
	vmsg.args.method = cpu_to_be32(SND_METHOD_VOICE);
	vmsg.args.volume = cpu_to_be32(7);
	vmsg.args.cb_func = -1;
	vmsg.args.client_data = 0;

	printk(KERN_ERR "snd_set_volume vol.device=%d vol.method=%d vol.volume=%d\n", device,
	       SND_METHOD_VOICE, 7);
	
	rc = msm_rpc_call(snd->ept,
			  SND_SET_VOLUME_PROC,
			  &vmsg, sizeof(vmsg), 5 * HZ);
	
    } else {
	printk(KERN_ERR "[%s] set_device is error", __func__);
    }

    return rc;
}

void set_current_snd_device(int dev)
{
    current_snd_device = dev;
}

static char device_attr_output_buffer[PAGE_SIZE];

static int testapp_is_command(const char *buf)
{
    int ret;
    int cmd;

    if (buf == NULL) {
	printk(KERN_ERR "[%s] input buffer is null\n", __func__); 
	return -1;
    }

    ret = sscanf(buf, "%d", &cmd);
    if (ret != 1) {
	printk(KERN_ERR "[%s] It has not command number\n", __func__); 
	return -1;
    }

//leesh ->
	//printk(KERN_ERR "[%s] leesh cmd before= %d\n", __func__, cmd); 
	if((cmd >= (SND_VOC_GAIN_SET_CMD_PROC-1)) && (cmd <= (SND_EC_GAIN_GET_CMD_PROC-1)))
		cmd++;
	//printk(KERN_ERR "[%s] leesh cmd aftere= %d\n", __func__, cmd); 
//leesh <-

    switch (cmd) {
    case SND_AVC_CTL_PROC:
    case SND_AGC_CTL_PROC:
    case SND_VOC_GAIN_SET_CMD_PROC:
    case SND_VOC_GAIN_GET_CMD_PROC:
    case SND_PCM_FILTER_SET_CMD_PROC:
    case SND_PCM_FILTER_GET_CMD_PROC:
    case SND_AGC_SET_CMD_PROC:
    case SND_AGC_GET_CMD_PROC:
    case SND_AVC_SET_CMD_PROC:
    case SND_AVC_GET_CMD_PROC:
    case SND_EC_SET_CMD_PROC:
    case SND_EC_GET_CMD_PROC:
    case SND_NS_SET_CMD_PROC:
    case SND_NS_GET_CMD_PROC:
    case SND_RX_VOLUME_SET_CMD_PROC:
    case SND_RX_VOLUME_GET_CMD_PROC:
    case SND_EC_GAIN_SET_CMD_PROC:
    case SND_EC_GAIN_GET_CMD_PROC:
    case SND_GET_CURRENT_DEVICE:
    case SND_SET_CURRENT_DEVICE_N_VOL:
    case SND_SET_DEVICE_:
	break;

    default:
	return -1;
	break;
    }

    return cmd;
}

static int testapp_set_current_snd_device_n_volume(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    struct snd_set_volume_msg vmsg;
    struct msm_snd_volume_config vol;
    int rc = 0;
    int cmd;

    rc = sscanf(buf, "%d %d %d %d",
		&cmd, &vol.device, &vol.method, &vol.volume);
    if (rc != 4) {
	MM_ERR("Invalid arguments. Usage: <cmd> <device> <method> \
				<volume>\n");
	rc = -EINVAL;
	return rc;
    }

    if (vol.device == -1) {
	vol.device = current_snd_device;
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

    printk(KERN_DEBUG "snd_set_volume %d %d %d\n", vol.device, vol.method,
	   vol.volume);

    rc = msm_rpc_call(snd_sys->ept,
		      SND_SET_VOLUME_PROC,
		      &vmsg, sizeof(vmsg), 5 * HZ);
    return rc;
}

static int testapp_snd_avc_enable(char *buf)
{
    int cmd;
    int on;
    int ret;

    printk(KERN_DEBUG "[%s] enable avc [%s]\n", __func__, buf); 

    ret = sscanf(buf, "%d %d", &cmd, &on);
    if (ret != 2) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (on == 1) {
	ret = snd_avc_enable(1);
    } else if (on == 0) {
	ret = snd_avc_enable(0);
    }

    return ret;
}

static int testapp_snd_agc_enable(char *buf)
{
    int cmd;
    int on;
    int ret;

    printk(KERN_DEBUG "[%s] enable agc [%s]\n", __func__, buf); 

    ret = sscanf(buf, "%d %d", &cmd, &on);
    if (ret != 2) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (on == 1) {
	ret = snd_agc_enable(1);
    } else if (on == 0) {
	ret = snd_agc_enable(0);
    }

    return ret;
}

/* set voc gain */

struct rpc_snd_set_voc_gain_ctl_args {
    snd_device_type device;
    voc_audio_cal_type etype;
    uint16_t dummy;
    uint16_t gain;
};

struct snd_set_voc_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_voc_gain_ctl_args args;
};

struct snd_set_voc_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success; 
};

static int testapp_snd_set_voc_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int type;
    int gain;
    int ret;
    struct snd_set_voc_gain_msg msg;
    struct snd_set_voc_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set voc gain [%s]\n", __func__, buf); 

    ret = sscanf(buf, "%d %d %d %d", &cmd, &dev, &type, &gain);
    if (ret != 4) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.etype = cpu_to_be32(type);
    msg.args.gain = cpu_to_be16(gain);
    
    ret = msm_rpc_call_reply(snd_sys->ept, 
		       SND_VOC_GAIN_SET_CMD_PROC,
		       &msg, sizeof(msg),
		       &rpl, sizeof(rpl),
		       5 * HZ);
  
    printk(KERN_DEBUG "[%s] set voc gain rpc call ret val[%d] "
	   "rpc return val[%d] sizeof rpl[%d]\n",
	   __func__, ret, rpl.success, sizeof(rpl)); 

    return ret;
}

/* get voc gain */

struct rpc_snd_get_voc_gain_ctl_args {
    snd_device_type device;
    voc_audio_cal_type etype;
};

struct snd_get_voc_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_voc_gain_ctl_args args;
};

struct snd_get_voc_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[2];
    uint16_t gain; /* TODO: */
};

static int testapp_snd_get_voc_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int type;
    int ret;
    struct snd_get_voc_gain_msg msg;
    struct snd_get_voc_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] get voc gain [%s]\n", __func__, buf); 

    ret = sscanf(buf, "%d %d %d", &cmd, &dev, &type);
    if (ret != 3) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.etype = cpu_to_be32(type);
    
    ret = msm_rpc_call_reply(snd_sys->ept, 
		       SND_VOC_GAIN_GET_CMD_PROC,
		       &msg, sizeof(msg),
		       &rpl, sizeof(rpl),
		       5 * HZ);
  
    printk(KERN_DEBUG "[%s] get voc gain rpc call ret val[%d] "
	   "rpc return val[%d] sizeofrpl[%d]\n",
	   __func__, ret,  be16_to_cpu(rpl.gain), sizeof(rpl)); 

    snprintf(device_attr_output_buffer, PAGE_SIZE,
	     "%d", be16_to_cpu(rpl.gain));

    return ret;
}

/* set pcm filter */

typedef struct {
    uint16_t dummy0;
    uint16_t tap0; /* Filter Tap, h[0] and h[12], Q14 */
    uint16_t dummy1;
    uint16_t tap1; /* Filter Tap, h[1] and h[11], Q14 */
    uint16_t dummy2;
    uint16_t tap2; /* Filter Tap, h[2] and h[10], Q14 */
    uint16_t dummy3;
    uint16_t tap3; /* Filter Tap, h[3] and h[9], Q14 */
    uint16_t dummy4;
    uint16_t tap4; /* Filter Tap, h[4] and h[8], Q14 */
    uint16_t dummy5;
    uint16_t tap5; /* Filter Tap, h[5] and h[7], Q14 */
    uint16_t dummy6;
    uint16_t tap6; /* Filter Tap, h[6], Q14. The filter is disabled if this */
} voc_cal_pcm_filter_type;

struct rpc_snd_set_pcm_filter_ctl_args {
    snd_device_type device;
    voc_audio_pcm_filter_type etype;
    voc_cal_pcm_filter_type data;
};

struct snd_set_pcm_filter_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_pcm_filter_ctl_args args;
};

struct snd_set_pcm_filter_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success;
};

static int testapp_snd_set_pcm_filter(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int type;
    int ret;
    int tap0;
    int tap1;
    int tap2;
    int tap3;
    int tap4;
    int tap5;
    int tap6;
    struct snd_set_pcm_filter_msg msg;
    struct snd_set_pcm_filter_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set pcm filter [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d %d %d %d %d %d %d %d",
		 &cmd, &dev, &type,
		 &tap0, &tap1, &tap2, &tap3, &tap4, &tap5, &tap6);
    if (ret != 10) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.etype = cpu_to_be32(type);
    msg.args.data.tap0 = cpu_to_be16(tap0);
    msg.args.data.tap1 = cpu_to_be16(tap1);
    msg.args.data.tap2 = cpu_to_be16(tap2);
    msg.args.data.tap3 = cpu_to_be16(tap3);
    msg.args.data.tap4 = cpu_to_be16(tap4);
    msg.args.data.tap5 = cpu_to_be16(tap5);
    msg.args.data.tap6 = cpu_to_be16(tap6);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_PCM_FILTER_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] set pcm filter rpc call ret val[%d]"
	   "sizeofrpl[%d]\n",
	   __func__, rpl.success, sizeof(rpl)); 

   return ret;
}

/* get pcm filter */

struct rpc_snd_get_pcm_filter_ctl_args {
    snd_device_type device;
    voc_audio_pcm_filter_type etype;
};

struct snd_get_pcm_filter_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_pcm_filter_ctl_args args;
};

struct snd_get_pcm_filter_rpl_msg {
    struct rpc_reply_hdr hdr;
    voc_cal_pcm_filter_type data;
};

static int testapp_snd_get_pcm_filter(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int type;
    int ret;
    struct snd_get_pcm_filter_msg msg;
    struct snd_get_pcm_filter_rpl_msg rpl/* [7] */;

    printk(KERN_DEBUG "[%s] get pcm filter [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d", &cmd, &dev, &type);
    if (ret != 3) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.etype = cpu_to_be32(type);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_PCM_FILTER_GET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] rpc call [%d]"
    	   "get pcm filter rpc call ret val"
    	   "[%d][%d][%d][%d][%d][%d][%d] "
    	   "sizeofrpl[%d]\n",
    	   __func__, ret,
    	   be16_to_cpu(rpl.data.tap0),
    	   be16_to_cpu(rpl.data.tap1),
    	   be16_to_cpu(rpl.data.tap2),
    	   be16_to_cpu(rpl.data.tap3),
    	   be16_to_cpu(rpl.data.tap4),
    	   be16_to_cpu(rpl.data.tap5),
    	   be16_to_cpu(rpl.data.tap6),
    	   sizeof(rpl));


   snprintf(device_attr_output_buffer, PAGE_SIZE,
    	    "%d %d %d %d %d %d %d",
   	    be16_to_cpu(rpl.data.tap0),
   	    be16_to_cpu(rpl.data.tap1),
   	    be16_to_cpu(rpl.data.tap2),
   	    be16_to_cpu(rpl.data.tap3),
   	    be16_to_cpu(rpl.data.tap4),
   	    be16_to_cpu(rpl.data.tap5),
   	    be16_to_cpu(rpl.data.tap6));

   return ret;
}

/* set agc gain */

typedef struct {
    uint16_t dummy0;
    uint16_t rx_agc_static_gain;
    uint16_t dummy1;
    uint16_t rx_agc_aig;
    uint16_t dummy2;
    uint16_t rx_agc_exp_thres;
    uint16_t dummy3;
    uint16_t rx_agc_exp_slope;
    uint16_t dummy4;
    uint16_t rx_agc_compr_thres;
    uint16_t dummy5;
    uint16_t rx_agc_compr_slope;
    uint16_t dummy6;
    uint16_t rx_avc_sensitivity;
    uint16_t dummy7;
    uint16_t rx_avc_headroom;
    uint16_t dummy8;
    uint16_t tx_agc_static_gain;
    uint16_t dummy9;
    uint16_t tx_agc_aig;
    uint16_t dummy10;
    uint16_t tx_agc_exp_thres;
    uint16_t dummy11;
    uint16_t tx_agc_exp_slope;
    uint16_t dummy12;
    uint16_t tx_agc_compr_thres;
    uint16_t dummy13;
    uint16_t tx_agc_compr_slope;
} voc_cal_agc_param_type;

struct rpc_snd_set_agc_gain_args {
    snd_device_type device;
    voc_cal_agc_param_type data;
};

struct snd_set_agc_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_agc_gain_args args;
};

struct snd_set_agc_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success;
};

static int testapp_snd_set_agc_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int rx_agc_static_gain;
    int rx_agc_aig;
    int rx_agc_exp_thres;
    int rx_agc_exp_slope;
    int rx_agc_compr_thres;
    int rx_agc_compr_slope;
    int rx_avc_sensitivity;
    int rx_avc_headroom;
    int tx_agc_static_gain;
    int tx_agc_aig;
    int tx_agc_exp_thres;
    int tx_agc_exp_slope;
    int tx_agc_compr_thres;
    int tx_agc_compr_slope;

    struct snd_set_agc_gain_msg msg;
    struct snd_set_agc_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set pcm filter [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		 &cmd, &dev,
		 &rx_agc_static_gain,
		 &rx_agc_aig,
		 &rx_agc_exp_thres,
		 &rx_agc_exp_slope,
		 &rx_agc_compr_thres,
		 &rx_agc_compr_slope,
		 &rx_avc_sensitivity,
		 &rx_avc_headroom,
		 &tx_agc_static_gain,
		 &tx_agc_aig,
		 &tx_agc_exp_thres,
		 &tx_agc_exp_slope,
		 &tx_agc_compr_thres,
		 &tx_agc_compr_slope);
		 
    if (ret != 16) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.data.rx_agc_static_gain = cpu_to_be16(rx_agc_static_gain);
    msg.args.data.rx_agc_aig         = cpu_to_be16(rx_agc_aig);
    msg.args.data.rx_agc_exp_thres   = cpu_to_be16(rx_agc_exp_thres);
    msg.args.data.rx_agc_exp_slope   = cpu_to_be16(rx_agc_exp_slope);
    msg.args.data.rx_agc_compr_thres = cpu_to_be16(rx_agc_compr_thres);
    msg.args.data.rx_agc_compr_slope = cpu_to_be16(rx_agc_compr_slope);
    msg.args.data.rx_avc_sensitivity = cpu_to_be16(rx_avc_sensitivity);
    msg.args.data.rx_avc_headroom    = cpu_to_be16(rx_avc_headroom);
    msg.args.data.tx_agc_static_gain = cpu_to_be16(tx_agc_static_gain);
    msg.args.data.tx_agc_aig         = cpu_to_be16(tx_agc_aig);
    msg.args.data.tx_agc_exp_thres   = cpu_to_be16(tx_agc_exp_thres);
    msg.args.data.tx_agc_exp_slope   = cpu_to_be16(tx_agc_exp_slope);
    msg.args.data.tx_agc_compr_thres = cpu_to_be16(tx_agc_compr_thres);
    msg.args.data.tx_agc_compr_slope = cpu_to_be16(tx_agc_compr_slope);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_AGC_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] set pcm filter rpc call ret val[%d]"
	   "sizeofrpl[%d]\n",
	   __func__, rpl.success, sizeof(rpl)); 

    return 0;
}

/* agc gain get */

struct rpc_snd_get_agc_gain_args {
    snd_device_type device;
};

struct snd_get_agc_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_agc_gain_args args;
};

struct snd_get_agc_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    voc_cal_agc_param_type data;    
};

static int testapp_snd_get_agc_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    struct snd_get_agc_gain_msg msg;
    struct snd_get_agc_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] get agc gain [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d", &cmd, &dev);
    if (ret != 2) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_AGC_GET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] rpc call [%d]"
    	   "get agc gain rpc call ret val"
    	   "[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d]"
    	   "sizeofrpl[%d]\n",
    	   __func__, ret,
	   be16_to_cpu(rpl.data.rx_agc_static_gain),
	   be16_to_cpu(rpl.data.rx_agc_aig),
	   be16_to_cpu(rpl.data.rx_agc_exp_thres),
	   be16_to_cpu(rpl.data.rx_agc_exp_slope),
	   be16_to_cpu(rpl.data.rx_agc_compr_thres),
	   be16_to_cpu(rpl.data.rx_agc_compr_slope),
	   be16_to_cpu(rpl.data.rx_avc_sensitivity),
	   be16_to_cpu(rpl.data.rx_avc_headroom),
	   be16_to_cpu(rpl.data.tx_agc_static_gain),
	   be16_to_cpu(rpl.data.tx_agc_aig),
	   be16_to_cpu(rpl.data.tx_agc_exp_thres),
	   be16_to_cpu(rpl.data.tx_agc_exp_slope),
	   be16_to_cpu(rpl.data.tx_agc_compr_thres),
	   be16_to_cpu(rpl.data.tx_agc_compr_slope),
    	   sizeof(rpl));

    snprintf(device_attr_output_buffer, PAGE_SIZE,
	     "%d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	     be16_to_cpu(rpl.data.rx_agc_static_gain),
	     be16_to_cpu(rpl.data.rx_agc_aig),
	     be16_to_cpu(rpl.data.rx_agc_exp_thres),
	     be16_to_cpu(rpl.data.rx_agc_exp_slope),
	     be16_to_cpu(rpl.data.rx_agc_compr_thres),
	     be16_to_cpu(rpl.data.rx_agc_compr_slope),
	     be16_to_cpu(rpl.data.rx_avc_sensitivity),
	     be16_to_cpu(rpl.data.rx_avc_headroom),
	     be16_to_cpu(rpl.data.tx_agc_static_gain),
	     be16_to_cpu(rpl.data.tx_agc_aig),
	     be16_to_cpu(rpl.data.tx_agc_exp_thres),
	     be16_to_cpu(rpl.data.tx_agc_exp_slope),
	     be16_to_cpu(rpl.data.tx_agc_compr_thres),
	     be16_to_cpu(rpl.data.tx_agc_compr_slope));

   return ret;
}

/* set avc gain */

typedef struct {
    struct {
	uint16_t dummy;
	uint16_t flink_threshold_list;
    } data0[24];
    struct {
	uint16_t dummy;
	uint16_t flink_targetgain_list;
    } data1[12];
} voc_cal_avc_param_type;

struct rpc_snd_set_avc_gain_args {
    snd_device_type device;
    voc_cal_avc_param_type data;
};

struct snd_set_avc_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_avc_gain_args args;
};

struct snd_set_avc_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success;
};

static int testapp_snd_set_avc_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int fthl[24];
    int ftal[12];
    int i;
    struct snd_set_avc_gain_msg msg;
    struct snd_set_avc_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set avc [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d "
		 "%d %d %d %d %d %d %d %d %d %d %d %d "
		 "%d %d %d %d %d %d %d %d %d %d %d %d "
		 "%d %d %d %d %d %d %d %d %d %d %d %d",
		 &cmd, &dev,
		 &fthl[0], &fthl[1], &fthl[2], &fthl[3], &fthl[4], &fthl[5],
		 &fthl[6], &fthl[7], &fthl[8], &fthl[9], &fthl[10], &fthl[11], 
		 &fthl[12], &fthl[13], &fthl[14], &fthl[15], &fthl[16], &fthl[17], 
		 &fthl[18], &fthl[19], &fthl[20], &fthl[21], &fthl[22], &fthl[23], 
		 &ftal[0], &ftal[1], &ftal[2], &ftal[3], &ftal[4], &ftal[5], 
		 &ftal[6], &ftal[7], &ftal[8], &ftal[9], &ftal[10], &ftal[11]);

    if (ret != 38) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    for (i = 0; i < 24; i++) {
	msg.args.data.data0[i].flink_threshold_list = cpu_to_be16(fthl[i]);
	printk("%d - msg.args.data.data0[i].flink.threshold.. %d\n", i, fthl[i]);
    }
    for (i = 0; i < 12; i++) {
	msg.args.data.data1[i].flink_targetgain_list = cpu_to_be16(ftal[i]);
	printk("%d - msg.args.data.data0[i].flink.target. %d\n", i, ftal[i]);
    }

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_AVC_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s][%d] set avc rpc call ret val[%d]"
	   "sizeofrpl[%d]\n",
	   __func__, ret, rpl.success, sizeof(rpl)); 

    return 0;
}

/* avc gain get */

struct rpc_snd_get_avc_gain_args {
    snd_device_type device;
};

struct snd_get_avc_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_avc_gain_args args;
};

struct snd_get_avc_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    voc_cal_avc_param_type data;    
};

static int testapp_snd_get_avc_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    struct snd_get_avc_gain_msg msg;
    struct snd_get_avc_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] get avc gain [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d", &cmd, &dev);
    if (ret != 2) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_AVC_GET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] rpc call [%d]"
    	   "get avc gain rpc call ret val"
	   "[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d]"
	   "[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d]"
	   "[%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d][%d]"
    	   "sizeofrpl[%d]\n",
    	   __func__, ret,
	   be16_to_cpu(rpl.data.data0[0].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[1].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[2].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[3].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[4].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[5].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[6].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[7].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[8].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[9].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[10].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[11].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[12].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[13].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[14].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[15].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[16].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[17].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[18].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[19].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[20].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[21].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[22].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[23].flink_threshold_list),
	   be16_to_cpu(rpl.data.data1[0].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[1].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[2].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[3].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[4].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[5].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[6].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[7].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[8].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[9].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[10].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[11].flink_targetgain_list),
    	   sizeof(rpl));

    snprintf(device_attr_output_buffer, PAGE_SIZE,
	     "%d %d %d %d %d %d %d %d %d %d %d %d "
	     "%d %d %d %d %d %d %d %d %d %d %d %d "
	     "%d %d %d %d %d %d %d %d %d %d %d %d",
	   be16_to_cpu(rpl.data.data0[0].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[1].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[2].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[3].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[4].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[5].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[6].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[7].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[8].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[9].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[10].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[11].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[12].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[13].flink_threshold_list),
 	   be16_to_cpu(rpl.data.data0[14].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[15].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[16].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[17].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[18].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[19].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[20].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[21].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[22].flink_threshold_list),
	   be16_to_cpu(rpl.data.data0[23].flink_threshold_list),
	   be16_to_cpu(rpl.data.data1[0].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[1].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[2].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[3].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[4].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[5].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[6].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[7].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[8].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[9].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[10].flink_targetgain_list),
	   be16_to_cpu(rpl.data.data1[11].flink_targetgain_list));

   return ret;
}

/* set ec mode */

struct rpc_snd_set_ecmode_args {
    snd_device_type device;
    voc_ec_type echo_mode;
};

struct snd_set_ecmode_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_ecmode_args args;
};

struct snd_set_ecmode_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success;
};

static int testapp_snd_set_ec_mode(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int type;

    struct snd_set_ecmode_msg msg;
    struct snd_set_ecmode_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set ecmode [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d",
		 &cmd, &dev, &type);
		 
    if (ret != 3) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.echo_mode = cpu_to_be32(type);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_EC_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s][%d] set ecmode rpc call ret val[%d]"
	   "sizeofrpl[%d]\n",
	   __func__, ret, rpl.success, sizeof(rpl)); 

    return 0;
}

/* get ec mode */

struct rpc_snd_get_ecmode_args {
    snd_device_type device;
};

struct snd_get_ecmode_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_ecmode_args args;
};

struct rpc_snd_get_ecmode_rpl_args {
    voc_ec_type echo_mode;
};

struct snd_get_ecmode_rpl_msg {
    struct rpc_reply_hdr hdr;
    struct rpc_snd_get_ecmode_rpl_args args;
};

static int testapp_snd_get_ec_mode(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    struct snd_get_ecmode_msg msg;
    struct snd_get_ecmode_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] get ecmode [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d", &cmd, &dev);
    if (ret != 2) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_EC_GET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] rpc call [%d]"
    	   "get ec mode rpc call ret val"
    	   "[%d] sizeofrpl[%d]\n",
    	   __func__, ret,
	   be32_to_cpu(rpl.args.echo_mode),
    	   sizeof(rpl));

    snprintf(device_attr_output_buffer, PAGE_SIZE,
	     "%d",
	     be32_to_cpu(rpl.args.echo_mode));

   return ret;
}

/* set ns mode */

struct rpc_snd_set_ns_mode_args {
    snd_device_type device;
    voc_ns_type ns_mode;
};

struct snd_set_ns_mode_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_ns_mode_args args;
};

struct snd_set_ns_mode_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success;
};

static int testapp_snd_set_ns_mode(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int type;

    struct snd_set_ns_mode_msg msg;
    struct snd_set_ns_mode_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set ns mode [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d", &cmd, &dev, &type);
		 
    if (ret != 3) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.ns_mode = cpu_to_be32(type);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_NS_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] set ns mode rpc call ret val[%d]"
	   "sizeofrpl[%d]\n",
	   __func__, rpl.success, sizeof(rpl)); 

    return 0;
}

/* get ns mode */

struct rpc_snd_get_ns_mode_args {
    snd_device_type device;
};

struct snd_get_ns_mode_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_ecmode_args args;
};

struct rpc_snd_get_ns_mode_rpl_args {
    voc_ns_type ns_mode;
};

struct snd_get_ns_mode_rpl_msg {
    struct rpc_reply_hdr hdr;
    struct rpc_snd_get_ns_mode_rpl_args args;
};

static int testapp_snd_get_ns_mode(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    struct snd_get_ns_mode_msg msg;
    struct snd_get_ns_mode_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] get ns mode [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d", &cmd, &dev);
    if (ret != 2) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_NS_GET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] rpc call [%d]"
    	   "get ns mode rpc call ret val"
    	   "[%d] sizeofrpl[%d]\n",
    	   __func__, ret,
	   be32_to_cpu(rpl.args.ns_mode),
    	   sizeof(rpl));

    snprintf(device_attr_output_buffer, PAGE_SIZE,
	     "%d",
	     be32_to_cpu(rpl.args.ns_mode));

   return ret;
}

/* set rx volume */

struct rpc_snd_set_rx_volume_args {
    snd_device_type device;
    snd_method_type etype;
    char dummy0[3];
    int8_t nLevel;
    char dummy1[2];
    int16_t gain;

};

struct snd_set_rx_volume_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_rx_volume_args args;
};

struct snd_set_rx_volume_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success;
};

static int testapp_snd_set_rx_volume(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int type;
    int level;
    int gain;
    int i;

    struct snd_set_rx_volume_msg msg;
    struct snd_set_rx_volume_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set rx volume [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d %d %d", &cmd, &dev, &type, &level, &gain);
	
    if (ret != 5) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.etype = cpu_to_be32(type);
    msg.args.nLevel = level;
    msg.args.gain = cpu_to_be16((int16_t)gain);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_RX_VOLUME_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);

    /* retry: 3 times */
    for (i = 0; i < 3 && ret < 0; i++) {
	msleep(100);
	printk(KERN_DEBUG "rx volume retry %d\n", i);
	ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_RX_VOLUME_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);
	
    }
  
    printk(KERN_DEBUG "[%s] [%d]set rx volume rpc call ret val[%d]"
	   "sizeofrpl[%d] gain[%d][%d]\n",
	   __func__, ret, rpl.success, sizeof(rpl), gain, cpu_to_be16(gain)); 

    return 0;
}

/* get rx volume */

struct rpc_snd_get_rx_volume_args {
    snd_device_type device;
    snd_method_type etype;
    char dummy[3];
    int8_t nLevel;
};

struct snd_get_rx_volume_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_rx_volume_args args;
};

struct rpc_snd_get_rx_volume_rpl_args {
    char dummy[2];
    int16_t gain;
};

struct snd_get_rx_volume_rpl_msg {
    struct rpc_reply_hdr hdr;
    struct rpc_snd_get_rx_volume_rpl_args args;
};

static int testapp_snd_get_rx_volume(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int type;
    int level;
    struct snd_get_rx_volume_msg msg;
    struct snd_get_rx_volume_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] get rx volume [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d %d", &cmd, &dev, &type, &level);
    if (ret != 4) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }
    
    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.etype = cpu_to_be32(type);
    msg.args.nLevel = level;

    printk(KERN_DEBUG "[%s]] current_snd_device : %d %d %d\n", 
	   __func__, current_snd_device, type, level);

    ret = msm_rpc_call_reply(snd_sys->ept,
    			     SND_RX_VOLUME_GET_CMD_PROC,
    			     &msg, sizeof(msg),
    			     &rpl, sizeof(rpl),
    			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] rpc call [%d]"
    	   "get rx volume rpc call ret val"
    	   "[%d][%d] sizeofrp[%d]\n",
    	   __func__, ret,
    	   (int16_t)be16_to_cpu(rpl.args.gain),
   	   (int16_t)rpl.args.gain,
    	   sizeof(rpl));

    if (ret > 0) {
	snprintf(device_attr_output_buffer, PAGE_SIZE,
		 "%d",
		 (int16_t)be16_to_cpu(rpl.args.gain));
    }

    return ret;
}

/* set ec mode */

typedef enum {
    NLPP_LIMIT, /* 0x0800 0x7FFF 0.0625 1.0 Q0.15 */
    NLPP_GAIN, /* 0x0000 0x7FFF -Infinity dB +24 dB Q4.11 (1*/
    NLMS_LIMIT, /* 0x0800 0x7FFF 0.0625 1.0 Q0.15 */
    MODE, /* see defines above */
    TUNING_MODE, /* */
    ECHO_PATH_DELAY, /* in ms */
    OUTPUTGAIN, /* 0x0000 0x7FFF -dB +24dB Q4.11 */
    INPUTGAIN, /* 0x0000 0x7FFF -dB +12dB Q2.13 */
    NLMS_TWOALPHA, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    NLMS_ERL, /* 0x0000 0x7FFF -Infinity dB +54 dB Q9.6 (1*/
    NLMS_TAPS, /* 12 200 12 200 */
    NLMS_PRESET_COEFS, /* 0 2 0 2 */
    NLMS_OFFSET, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    NLMS_ERL_BG, /* 0x0000 0x7FFF -Infinity dB +54 dB Q9.6 */
    NLMS_TAPS_BG, /* 12 200 12 200 */
    PCD_THRESHOLD, /* 0x0000 0x7FFF 0 1 Q0.15 */
    MINIMUM_ERL, /* 0x0000 0x7FFF -Infinity dB +54 dB Q9.6 (1*/
    ERL_STEP, /* 0x0000 0x7FFF 0 2 Q1.14 */
    MAX_NOISE_FLOOR, /* */
    DET_THRESHOLD, /* */
    SPDET_FAR, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    SPDET_MIC, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    SPDET_XCLIP, /* 0x0000 0x7FFF 0 6.15 dBm0 (3*/
    DENS_TAIL_ALPHA, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    DENS_TAIL_PORTION, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    DENS_GAMMA_E_ALPHA, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    DENS_GAMMA_E_DT, /* 0x0000 0x7FFF 0 128 Q7.8 */
    DENS_GAMMA_E_LOW, /* 0x0000 0x7FFF 0 128 Q7.8 */
    DENS_GAMMA_E_RESCUE, /* 0x0000 0x7FFF 0 128 Q7.8 */
    DENS_GAMMA_E_HIGH, /* 0x0000 0x7FFF 0 128 Q7.8 */
    DENS_SPDET_NEAR, /* 0x0000 0x7FFF 0 128 Q7.8 */
    DENS_SPDET_ACT, /* 0x0000 0x7FFF 0 128 Q7.8 */
    DENS_GAMMA_N, /* 0x0000 0x7FFF 0 128 Q7.8 */
    DENS_NFE_BLOCKSIZE, /* 0x0000 0x7FFF 0 32768 */
    DENS_LIMIT_NS, /* 0x0000 0x7FFF 0 1.0 Q0.15 */
    DENS_NL_ATTEN, /* 0x0000 0x7FFF -Infinity dB +24 dB Q4.11 (1*/
    DENS_CNI_LEVEL, /* 0x0000 0x7FFF 0 2.0 Q1.14 */
    WB_ECHO_RATIO /* 0x0000 0x7FFF -Infinity dB +21 dB Q7.8 (2*/
}voc_ec_gain_type;

struct rpc_snd_set_ec_gain_args {
    snd_device_type device;
    voc_ec_gain_type gain_type;
    char dummy1[2];
    uint16_t echo_gain;
};

struct snd_set_ec_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_set_ec_gain_args args;
};

struct snd_set_ec_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    char dummy[3];
    Boolean success;
};

static int testapp_snd_set_ec_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int type;
    int gain;

    struct snd_set_ec_gain_msg msg;
    struct snd_set_ec_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] set ec gain [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d %d", &cmd, &dev, &type, &gain);
		 
    if (ret != 4) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }

    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.gain_type = cpu_to_be32(type);
    msg.args.echo_gain = cpu_to_be16((uint16_t)gain);

    ret = msm_rpc_call_reply(snd_sys->ept, 
			     SND_EC_GAIN_SET_CMD_PROC,
			     &msg, sizeof(msg),
			     &rpl, sizeof(rpl),
			     5 * HZ);

    printk(KERN_DEBUG "[%s] ret [%d], set set ec gain rpc call ret val [%d]"
	   "sizeofrpl[%d]\n",
	   __func__, ret, rpl.success, sizeof(rpl)); 

    return 0;
}

/* get ec mode */

struct rpc_snd_get_ec_gain_args {
    snd_device_type device;
    voc_ec_gain_type gain_type;
};

struct snd_get_ec_gain_msg {
    struct rpc_request_hdr hdr;
    struct rpc_snd_get_ec_gain_args args;
};

struct rpc_snd_get_ec_gain_rpl_args {
    char dummy[2];
    uint16_t gain;
};

struct snd_get_ec_gain_rpl_msg {
    struct rpc_reply_hdr hdr;
    struct rpc_snd_get_ec_gain_rpl_args args;
};

static int testapp_snd_get_ec_gain(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;
    int type;
    struct snd_get_ec_gain_msg msg;
    struct snd_get_ec_gain_rpl_msg rpl;

    printk(KERN_DEBUG "[%s] get ec gain [%s]\n", __func__, buf); 
   
    ret = sscanf(buf, "%d %d %d", &cmd, &dev, &type);
    if (ret != 3) {
	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__); 
	return -1;
    }
    
    if (dev == -1) {
	dev = current_snd_device;
    }

    msg.args.device = cpu_to_be32(dev);
    msg.args.gain_type = cpu_to_be32(type);

    ret = msm_rpc_call_reply(snd_sys->ept,
    			     SND_EC_GAIN_GET_CMD_PROC,
    			     &msg, sizeof(msg),
    			     &rpl, sizeof(rpl),
    			     5 * HZ);
  
    printk(KERN_DEBUG "[%s] rpc call [%d]"
    	   "get ec gain rpc call ret val"
    	   "[%d] sizeofrp[%d]\n",
    	   __func__, ret,
    	   (uint16_t)be16_to_cpu(rpl.args.gain),
    	   sizeof(rpl));

    if (ret > 0) {
	snprintf(device_attr_output_buffer, PAGE_SIZE,
		 "%d",
		 (uint16_t)be16_to_cpu(rpl.args.gain));
    }

    return ret;
}

static int testapp_snd_get_current_device(char *buf)
{
    printk(KERN_DEBUG "[%s] get current_snd_device %d\n",
	   __func__, current_snd_device);

    snprintf(device_attr_output_buffer, PAGE_SIZE, "%d", current_snd_device);

    return 0;
}


static int testapp_snd_set_device(char *buf)
{
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int cmd;
    int dev;
    int ret;

    ret = sscanf(buf, "%d %d", &cmd, &dev);

    printk(KERN_DEBUG "[%s] %d device select\n", __func__, dev);
		 
    if (ret != 2 || dev == -1) {
    	printk(KERN_ERR "[%s] invalid argument(s)\n", __func__);
    	return -1;
    }

    if (snd_sys == NULL) {
    	printk(KERN_ERR "[%s]] snd_sys == NULLull\n", __func__);
	return -1;
    }

    ret = set_device_for_audio_test_app(the_snd_sys_context, dev);

    if (ret < 0) {
	printk(KERN_ERR "[%s] set device is error - errorcode[%d]\n",
	       __func__, ret);
    }

    return ret;
}    

static int testapp_do_command(int cmd, char *buf)
{
    int ret;

    switch (cmd) {
    case SND_AVC_CTL_PROC:
	ret = testapp_snd_avc_enable(buf);
	break;

    case SND_AGC_CTL_PROC:
	ret = testapp_snd_agc_enable(buf);
	break;

    case SND_VOC_GAIN_SET_CMD_PROC:
	ret = testapp_snd_set_voc_gain(buf);
	break;

    case SND_VOC_GAIN_GET_CMD_PROC:
	ret = testapp_snd_get_voc_gain(buf);
	break;

    case SND_PCM_FILTER_SET_CMD_PROC:
	ret = testapp_snd_set_pcm_filter(buf);
	break;

    case SND_PCM_FILTER_GET_CMD_PROC:
	ret = testapp_snd_get_pcm_filter(buf);
	break;

    case SND_AGC_SET_CMD_PROC:
	ret = testapp_snd_set_agc_gain(buf);
	break;

    case SND_AGC_GET_CMD_PROC:
	ret = testapp_snd_get_agc_gain(buf);
	break;

    case SND_AVC_SET_CMD_PROC:
	ret = testapp_snd_set_avc_gain(buf);
	break;

    case SND_AVC_GET_CMD_PROC:
	ret = testapp_snd_get_avc_gain(buf);
	break;

    case SND_EC_SET_CMD_PROC:
	ret = testapp_snd_set_ec_mode(buf);
	break;

    case SND_EC_GET_CMD_PROC:
	ret = testapp_snd_get_ec_mode(buf);
	break;

    case SND_NS_SET_CMD_PROC:
	ret = testapp_snd_set_ns_mode(buf);
	break;

    case SND_NS_GET_CMD_PROC:
	ret = testapp_snd_get_ns_mode(buf);
	break;

    case SND_RX_VOLUME_SET_CMD_PROC:
	ret = testapp_snd_set_rx_volume(buf);
	break;

    case SND_RX_VOLUME_GET_CMD_PROC:
	ret = testapp_snd_get_rx_volume(buf);
	break;

    case SND_EC_GAIN_SET_CMD_PROC:
	ret = testapp_snd_set_ec_gain(buf);
	break;

    case SND_EC_GAIN_GET_CMD_PROC:
	ret = testapp_snd_get_ec_gain(buf);
	break;

    case SND_GET_CURRENT_DEVICE:
	ret = testapp_snd_get_current_device(buf);
	break;

    case SND_SET_CURRENT_DEVICE_N_VOL:
	ret = testapp_set_current_snd_device_n_volume(buf);
	break;
    
    case SND_SET_DEVICE_:
	ret = testapp_snd_set_device(buf);
	break;

    default:
	printk(KERN_ERR "[%s] invalid command [%s]\n", __func__, buf); 
	ret = -1;
	break;
    }

    return ret;
}

static ssize_t snd_test_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
    ssize_t status;
    struct snd_sys_ctxt *snd_sys = the_snd_sys_context;
    int rc;
    int cmd;

    rc = snd_sys_open();
    if (rc) {
	printk(KERN_ERR "[%s] fail to open snd_sys_open()\n", __func__); 
	return rc;
    }

    cmd = testapp_is_command((char *)buf);
     if (cmd == -1) {
	printk(KERN_ERR "[%s] invalid command\n", __func__);
	return -EINVAL;
    }
    
    mutex_lock(&snd_sys->lock);
    status = testapp_do_command(cmd, (char *)buf);
    mutex_unlock(&snd_sys->lock);

    rc = snd_sys_release();
    if (rc) {
	printk(KERN_ERR "[%s] fail to open snd_sys_release()\n", __func__); 
	return rc;
    }

    return status ? : size;
}

static ssize_t snd_test_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
    int size;
   
    size = strlen(device_attr_output_buffer);
    if (size > PAGE_SIZE) {
	size = PAGE_SIZE;
    }

    strncpy(buf, device_attr_output_buffer, size);

    return size;
}

static DEVICE_ATTR(testapp, S_IRWXUGO,
		   snd_test_show, snd_test_store);

int testapp_snd_probe(struct miscdevice *snd_misc, struct snd_sys_ctxt *the_snd_sys)
{
    int rc;
    rc = device_create_file(snd_misc->this_device, &dev_attr_testapp);
    the_snd_sys_context = the_snd_sys;

    return rc;
}

#endif /* #if USE_AUDIO_TEST_APP */
