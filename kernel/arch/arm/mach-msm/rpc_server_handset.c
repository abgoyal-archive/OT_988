/* arch/arm/mach-msm/rpc_server_handset.c
 *
 * Copyright (c) 2008-2010, Code Aurora Forum. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can find it at http://www.fsf.org.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/switch.h>

#include <asm/mach-types.h>

#include <mach/msm_rpcrouter.h>
#include <mach/board.h>
#include <mach/rpc_server_handset.h>

#define DRIVER_NAME	"msm-handset"

#define HS_SERVER_PROG 0x30000062
#define HS_SERVER_VERS 0x00010001

#define HS_RPC_PROG 0x30000091

#define HS_PROCESS_CMD_PROC 0x02
#define HS_SUBSCRIBE_SRVC_PROC 0x03
#define HS_REPORT_EVNT_PROC    0x05
#define HS_EVENT_CB_PROC	1
#define HS_EVENT_DATA_VER	1

#define RPC_KEYPAD_NULL_PROC 0
#define RPC_KEYPAD_PASS_KEY_CODE_PROC 2
#define RPC_KEYPAD_SET_PWR_KEY_STATE_PROC 3

#define HS_PWR_K		0x6F	/* Power key */
#define HS_END_K		0x51	/* End key or Power key */
#define HS_HEADPHONE_K		0x7E	/*(Mono) Headset Connected*/ //gtsystem_2011-07-25_Mono Headset connected
#define HS_STEREO_HEADSET_K	0x82
#define HS_HEADSET_SWITCH_K	0x84
#define HS_HEADSET_SWITCH_2_K	0xF0
#define HS_HEADSET_SWITCH_3_K	0xF1
#define HS_HEADSET_HEADPHONE_K	0xF6
#define HS_HEADSET_MICROPHONE_K 0xF7

#define   HS_0_K           0x30	/* '0' key, ASCII '0'                  0x30    */
#define   HS_1_K           0x31	/* '1' key, ASCII '1'                  0x31    */
#define   HS_2_K           0x32	/* '2' key, ASCII '2'                  0x32    */
#define   HS_3_K           0x33	/* '3' key, ASCII '3'                  0x33    */
#define   HS_4_K           0x34	/* '4' key, ASCII '4'                  0x34    */
#define   HS_5_K           0x35	/* '5' key, ASCII '5'                  0x35    */
#define   HS_6_K           0x36	/* '6' key, ASCII '6'                  0x36    */
#define   HS_7_K           0x37	/* '7' key, ASCII '7'                  0x37    */
#define   HS_8_K           0x38	/* '8' key, ASCII '8'                  0x38    */
#define   HS_9_K           0x39	/* '9' key, ASCII '9'                  0x39    */
#define HS_ASTERISK_K	0x2A    //*		
#define HS_POUND_K	0x23    //#		
#define HS_SEND_K	0x50		
#define HS_CLEAR_K	0x52		
#define HS_LEFT_K	0x09   //LEFT		
#define HS_RIGHT_K	0x0A   //RIGHT		
#define HS_UP_K	0x54   //UP		
#define HS_DOWN_K	0x55   //DOWN		
#define HS_SELECT_K	0x70   //SELECT		
#define HS_MESSAGE_K	0x0B   //MESSAGE		
#define HS_VOLUME_UP_K	0x8F   //VOL UP		
#define HS_VOLUME_DOWN_K	0x90   //VOL DOWN		
#define HS_POWER_K	0x6F   //POWER		
#define HS_PTT_K 	0x87   //PTT		
#define HS_SPEAKER_K	0x86   //SPEAKER		
#define HS_LEFT_SOFT_KEY_K	0x9E   //LEFT SOFT		
#define HS_RIGHT_SOFT_KEY_K	0x9F   //RIGHT SOFT		
#define HS_TASK_K	0xA0   //TASK		
#define HS_VIDEO_K	0xA1   //VIDEO		
#define HS_CAMERA_K	0xA2   //CAMERA		
#define HS_WEB_K	0xA5   //WEB		
#define HS_LEFT_SHIFT_K	0xA5		
#define HS_SPACE_K	0x20		
#define HS_DOT_K	0x2E		
#define HS_A_K	0x41		
#define HS_B_K	0x42		
#define HS_C_K	0x43		
#define HS_D_K	0x44		
#define HS_E_K	0x45		
#define HS_F_K	0x46		
#define HS_G_K	0x47		
#define HS_H_K	0x48		
#define HS_I_K	0x49		
#define HS_J_K	0x4A		
#define HS_K_K	0x4B		
#define HS_L_K	0x4C		
#define HS_M_K	0x4D		
#define HS_N_K	0x4E		
#define HS_O_K	0x4F		
#define HS_P_K	0x5B		
#define HS_Q_K	0x5C		
#define HS_R_K	0x66		
#define HS_S_K	0x5D		
#define HS_T_K	0x5E		
#define HS_U_K	0x67		
#define HS_V_K	0x56		
#define HS_W_K	0x57		
#define HS_X_K	0x58		
#define HS_Y_K	0x59		
#define HS_Z_K	0x5A		
#define HS_MENU_K 0x60
#define HS_HOME_K 0x61
#define HS_SEARCH_K 0x62
#define HS_RETURN_K 0x63
#define HS_ENTER_K 0x64
#define HS_FUNCTION_K 0x65

#define HS_POWERUP_CMD 0x80
#define HS_POWERDOWN_CMD 0x81

/* DEFINE GPIO MATRIX KEYPAD */
#define GPIO_KEY_0 0xd7
#define GPIO_KEY_1 0x11
#define GPIO_KEY_2 0x12
#define GPIO_KEY_3 0x13
#define GPIO_KEY_4 0x1f
#define GPIO_KEY_5 0x20
#define GPIO_KEY_6 0x21
#define GPIO_KEY_7 0x2c
#define GPIO_KEY_8 0x2d
#define GPIO_KEY_9 0x2e
#define GPIO_KEY_ASTERISK 0x22
#define GPIO_KEY_POUND 0x2f
#define GPIO_KEY_SEND 0xe7
#define GPIO_KEY_END 
#define GPIO_KEY_CLEAR 0x6f
#define GPIO_KEY_LEFT 0x34
#define GPIO_KEY_RIGHT 0x7f
#define GPIO_KEY_UP 0x32
#define GPIO_KEY_DOWN 0xfa
#define GPIO_KEY_SELECT 
#define GPIO_KEY_MESSAGE
#define GPIO_KEY_VOLUME_UP 0x73
#define GPIO_KEY_VOLUME_DOWN 0x72
#define GPIO_KEY_POWER 
#define GPIO_KEY_PTT 
#define GPIO_KEY_SPEAKER 
#define GPIO_KEY_LEFT_SOFT_KEY 
#define GPIO_KEY_RIGHT_SOFT_KEY 
#define GPIO_KEY_TASK 
#define GPIO_KEY_VIDEO 
#define GPIO_KEY_CAMERA 
#define GPIO_KEY_WEB 
#define GPIO_KEY_LEFT_SHIFT 0x33
#define GPIO_KEY_SPACE 0x39
#define GPIO_KEY_DOT 0xd6
#define GPIO_KEY_A 0x1e
#define GPIO_KEY_B 0x2f
#define GPIO_KEY_C 0x2d
#define GPIO_KEY_D 0x20
#define GPIO_KEY_E 0x12
#define GPIO_KEY_F 0x21
#define GPIO_KEY_G 0x22
#define GPIO_KEY_H 0x23
#define GPIO_KEY_I 0x17
#define GPIO_KEY_J 0x24
#define GPIO_KEY_K 0x25
#define GPIO_KEY_L 0x26
#define GPIO_KEY_M 0x31
#define GPIO_KEY_N 0x30
#define GPIO_KEY_O 0x18
#define GPIO_KEY_P 0x19
#define GPIO_KEY_Q 0x10
#define GPIO_KEY_R 0x13
#define GPIO_KEY_S 0x1f
#define GPIO_KEY_T 0x14
#define GPIO_KEY_U 0x16
#define GPIO_KEY_V 0x2e
#define GPIO_KEY_W 0x11
#define GPIO_KEY_X 0x2c
#define GPIO_KEY_Y 0x15
#define GPIO_KEY_Z 0x2a
#define GPIO_KEY_MENU 0x8b
#define GPIO_KEY_HOME 0x66
#define GPIO_KEY_SEARCH 0xd9
#define GPIO_KEY_RETURN 0x9e
#define GPIO_KEY_ENTER 0x1c
#define GPIO_KEY_FUNCTION 0x1d0

#define CMD_POWERUP 0xFF80
#define CMD_POWERDOWN 0xFF81

#define KEY_1_0xF2 242
#define KEY_3_0xF4 244

#define KEY_STAR 224
#define KEY_POUND 225
#define KEY_CALL 227

#define HS_REL_K		0xFF	/* key release */

#define SW_HEADPHONE_INSERT_W_MIC 1 /* HS with mic */

#define KEY(hs_key, input_key) ((hs_key << 24) | input_key)

enum hs_event {
	HS_EVNT_EXT_PWR = 0,	/* External Power status        */
	HS_EVNT_HSD,		/* Headset Detection            */
	HS_EVNT_HSTD,		/* Headset Type Detection       */
	HS_EVNT_HSSD,		/* Headset Switch Detection     */
	HS_EVNT_KPD,
	HS_EVNT_FLIP,		/* Flip / Clamshell status (open/close) */
	HS_EVNT_CHARGER,	/* Battery is being charged or not */
	HS_EVNT_ENV,		/* Events from runtime environment like DEM */
	HS_EVNT_REM,		/* Events received from HS counterpart on a
				remote processor*/
	HS_EVNT_DIAG,		/* Diag Events  */
	HS_EVNT_LAST,		 /* Should always be the last event type */
	HS_EVNT_MAX		/* Force enum to be an 32-bit number */
};

enum hs_src_state {
	HS_SRC_STATE_UNKWN = 0,
	HS_SRC_STATE_LO,
	HS_SRC_STATE_HI,
};

struct hs_event_data {
	uint32_t	ver;		/* Version number */
	enum hs_event	event_type;     /* Event Type	*/
	enum hs_event	enum_disc;     /* discriminator */
	uint32_t	data_length;	/* length of the next field */
	enum hs_src_state	data;    /* Pointer to data */
	uint32_t	data_size;	/* Elements to be processed in data */
};

enum hs_return_value {
	HS_EKPDLOCKED     = -2,	/* Operation failed because keypad is locked */
	HS_ENOTSUPPORTED  = -1,	/* Functionality not supported */
	HS_FALSE          =  0, /* Inquired condition is not true */
	HS_FAILURE        =  0, /* Requested operation was not successful */
	HS_TRUE           =  1, /* Inquired condition is true */
	HS_SUCCESS        =  1, /* Requested operation was successful */
	HS_MAX_RETURN     =  0x7FFFFFFF/* Force enum to be a 32 bit number */
};

struct hs_key_data {
	uint32_t ver;        /* Version number to track sturcture changes */
	uint32_t code;       /* which key? */
	uint32_t parm;       /* key status. Up/down or pressed/released */
};

enum hs_subs_srvc {
	HS_SUBS_SEND_CMD = 0, /* Subscribe to send commands to HS */
	HS_SUBS_RCV_EVNT,     /* Subscribe to receive Events from HS */
	HS_SUBS_SRVC_MAX
};

enum hs_subs_req {
	HS_SUBS_REGISTER,    /* Subscribe   */
	HS_SUBS_CANCEL,      /* Unsubscribe */
	HS_SUB_STATUS_MAX
};

enum hs_event_class {
	HS_EVNT_CLASS_ALL = 0, /* All HS events */
	HS_EVNT_CLASS_LAST,    /* Should always be the last class type   */
	HS_EVNT_CLASS_MAX
};

enum hs_cmd_class {
	HS_CMD_CLASS_LCD = 0, /* Send LCD related commands              */
	HS_CMD_CLASS_KPD,     /* Send KPD related commands              */
	HS_CMD_CLASS_LAST,    /* Should always be the last class type   */
	HS_CMD_CLASS_MAX
};

/*
 * Receive events or send command
 */
union hs_subs_class {
	enum hs_event_class	evnt;
	enum hs_cmd_class	cmd;
};

struct hs_subs {
	uint32_t                ver;
	enum hs_subs_srvc	srvc;  /* commands or events */
	enum hs_subs_req	req;   /* subscribe or unsubscribe  */
	uint32_t		host_os;
	enum hs_subs_req	disc;  /* discriminator    */
	union hs_subs_class      id;
};

struct hs_event_cb_recv {
	uint32_t cb_id;
	uint32_t hs_key_data_ptr;
	struct hs_key_data key;
};
enum hs_ext_cmd_type {
	HS_EXT_CMD_KPD_SEND_KEY = 0, /* Send Key */
	HS_EXT_CMD_KPD_BKLT_CTRL, /* Keypad backlight intensity	*/
	HS_EXT_CMD_LCD_BKLT_CTRL, /* LCD Backlight intensity */
	HS_EXT_CMD_DIAG_KEYMAP, /* Emulating a Diag key sequence */
	HS_EXT_CMD_DIAG_LOCK, /* Device Lock/Unlock */
	HS_EXT_CMD_GET_EVNT_STATUS, /* Get the status for one of the drivers */
	HS_EXT_CMD_KPD_GET_KEYS_STATUS,/* Get a list of keys status */
	HS_EXT_CMD_KPD_SET_PWR_KEY_RST_THOLD, /* PWR Key HW Reset duration */
	HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD, /* Set pwr key threshold duration */
	HS_EXT_CMD_LAST, /* Should always be the last command type */
	HS_EXT_CMD_MAX = 0x7FFFFFFF /* Force enum to be an 32-bit number */
};

struct hs_cmd_data_type {
	uint32_t hs_cmd_data_type_ptr; /* hs_cmd_data_type ptr length */
	uint32_t ver; /* version */
	enum hs_ext_cmd_type id; /* command id */
	uint32_t handle; /* handle returned from subscribe proc */
	enum hs_ext_cmd_type disc_id1; /* discriminator id */
	uint32_t input_ptr; /* input ptr length */
	uint32_t input_val; /* command specific data */
	uint32_t input_len; /* length of command input */
	enum hs_ext_cmd_type disc_id2; /* discriminator id */
	uint32_t output_len; /* length of output data */
	uint32_t delayed; /* execution context for modem
				true - caller context
				false - hs task context*/
};

//gtsystem_2011-07-25_Mono Headset connected
static const uint32_t hs_key_map[] = {
//	KEY(HS_PWR_K, KEY_POWER),
	KEY(HS_END_K, KEY_POWER),	//	Teddy 2011-10-24
	KEY(HS_HEADPHONE_K, SW_HEADPHONE_INSERT),
	KEY(HS_STEREO_HEADSET_K, SW_HEADPHONE_INSERT_W_MIC),
	KEY(HS_HEADSET_HEADPHONE_K, SW_HEADPHONE_INSERT),
	KEY(HS_HEADSET_MICROPHONE_K, SW_MICROPHONE_INSERT),
	KEY(HS_HEADSET_SWITCH_K, KEY_MEDIA),
	KEY(HS_HEADSET_SWITCH_2_K, KEY_VOLUMEUP),
	KEY(HS_HEADSET_SWITCH_3_K, KEY_VOLUMEDOWN),
//	KEY(HS_ASTERISK_K, GPIO_KEY_ASTERISK),
//	KEY(HS_POUND_K, GPIO_KEY_POUND),
//	KEY(HS_SEND_K, GPIO_KEY_SEND),
//	KEY(HS_END_K, GPIO_KEY_END),
	KEY(HS_CLEAR_K, GPIO_KEY_CLEAR),
	KEY(HS_LEFT_K, GPIO_KEY_LEFT),
	KEY(HS_RIGHT_K, GPIO_KEY_RIGHT),
	KEY(HS_UP_K, GPIO_KEY_UP),
	KEY(HS_DOWN_K, GPIO_KEY_DOWN),
//	KEY(HS_SELECT_K, GPIO_KEY_SELECT),
//	KEY(HS_MESSAGE_K, GPIO_KEY_MESSAGE),
	KEY(HS_VOLUME_UP_K, GPIO_KEY_VOLUME_UP),
	KEY(HS_VOLUME_DOWN_K, GPIO_KEY_VOLUME_DOWN),
//	KEY(HS_POWER_K, GPIO_KEY_POWER),
//	KEY(HS_PTT_K , GPIO_KEY_PTT ),
//	KEY(HS_SPEAKER_K, GPIO_KEY_SPEAKER),
//	KEY(HS_LEFT_SOFT_KEY_K, GPIO_KEY_LEFT_SOFT_KEY),
//	KEY(HS_RIGHT_SOFT_KEY_K, GPIO_KEY_RIGHT_SOFT_KEY),
//	KEY(HS_TASK_K, GPIO_KEY_TASK),
//	KEY(HS_VIDEO_K, GPIO_KEY_VIDEO),
//	KEY(HS_CAMERA_K, GPIO_KEY_CAMERA),
//	KEY(HS_WEB_K, GPIO_KEY_WEB),
	KEY(HS_LEFT_SHIFT_K, GPIO_KEY_LEFT_SHIFT),
//	KEY(HS_SPACE_K, GPIO_KEY_SPACE),

	KEY(HS_0_K, KEY_0),
	KEY(HS_1_K, KEY_1_0xF2),
	KEY(HS_2_K, KEY_2),
	KEY(HS_3_K, KEY_3_0xF4),
	KEY(HS_4_K, KEY_4),
	KEY(HS_5_K, KEY_5),
	KEY(HS_6_K, KEY_6),
	KEY(HS_7_K, KEY_7),
	KEY(HS_8_K, KEY_8),
	KEY(HS_9_K, KEY_9),

	KEY(HS_ASTERISK_K, KEY_STAR),
	KEY(HS_POUND_K, KEY_POUND),
	KEY(HS_SEND_K, KEY_CALL),
	KEY(HS_END_K, KEY_END),
	KEY(HS_SPACE_K, KEY_SPACE),
	KEY(HS_DOT_K, KEY_DOT),

	KEY(HS_A_K, KEY_A),
	KEY(HS_B_K, KEY_B),
	KEY(HS_C_K, KEY_C),
	KEY(HS_D_K, KEY_D),
	KEY(HS_E_K, KEY_E),
	KEY(HS_F_K, KEY_F),
	KEY(HS_G_K, KEY_G),
	KEY(HS_H_K, KEY_H),
	KEY(HS_I_K, KEY_I),
	KEY(HS_J_K, KEY_J),
	KEY(HS_K_K, KEY_K),
	KEY(HS_L_K, KEY_L),
	KEY(HS_M_K, KEY_M),
	KEY(HS_N_K, KEY_N),
	KEY(HS_O_K, KEY_O),
	KEY(HS_P_K, KEY_P),
	KEY(HS_Q_K, KEY_Q),
	KEY(HS_R_K, KEY_R),
	KEY(HS_S_K, KEY_S),
	KEY(HS_T_K, KEY_T),
	KEY(HS_U_K, KEY_U),
	KEY(HS_V_K, KEY_V),
	KEY(HS_W_K, KEY_W),
	KEY(HS_X_K, KEY_X),
	KEY(HS_Y_K, KEY_Y),
	KEY(HS_Z_K, KEY_Z),

	KEY(HS_MENU_K, KEY_MENU),
	KEY(HS_HOME_K, KEY_HOME),
	KEY(HS_SEARCH_K, KEY_SEARCH),
	KEY(HS_RETURN_K, KEY_BACK),
	
	KEY(HS_ENTER_K, GPIO_KEY_ENTER),
	KEY(HS_FUNCTION_K, GPIO_KEY_FUNCTION),
	KEY(HS_POWERUP_CMD, CMD_POWERUP),
	KEY(HS_POWERDOWN_CMD, CMD_POWERDOWN),
	0
};

enum {
	NO_DEVICE	= 0,
	MSM_HEADSET	= 1,
};
/* Add newer versions at the top of array */
static const unsigned int rpc_vers[] = {
	0x00030001,
	0x00020001,
	0x00010001,
};
/* hs subscription request parameters */
struct hs_subs_rpc_req {
	uint32_t hs_subs_ptr;
	struct hs_subs hs_subs;
	uint32_t hs_cb_id;
	uint32_t hs_handle_ptr;
	uint32_t hs_handle_data;
};

static struct hs_subs_rpc_req *hs_subs_req;

struct msm_handset {
	struct input_dev *ipdev;
	struct switch_dev sdev;
	struct msm_handset_platform_data *hs_pdata;
	bool mic_on, hs_on;
};

static struct msm_rpc_client *rpc_client;
static struct msm_handset *hs;

static int hs_find_key(uint32_t hscode)
{
	int i, key;

	key = KEY(hscode, 0);

	for (i = 0; hs_key_map[i] != 0; i++) {
		if ((hs_key_map[i] & 0xff000000) == key)
			return hs_key_map[i] & 0x00ffffff;
	}
	return -1;
}

static void update_state(void)
{
	int state;

	if (hs->mic_on && hs->hs_on)
		state = 1 << 0;
	else if (hs->hs_on)
		state = 1 << 1;
	else if (hs->mic_on)
		state = 1 << 2;
	else
		state = 0;

	switch_set_state(&hs->sdev, state);
}
extern void emulate_input_report_key(unsigned int keycode, int pressed);
extern int UTS_get_disp_state();
static int UTS_POWER_NUM = 0;

/*
 * tuple format: (key_code, key_param)
 *
 * old-architecture:
 * key-press = (key_code, 0)
 * key-release = (0xff, key_code)
 *
 * new-architecutre:
 * key-press = (key_code, 0)
 * key-release = (key_code, 0xff)
 */
static void report_hs_key(uint32_t key_code, uint32_t key_parm)
{
	int key, temp_key_code;

	if (key_code == HS_REL_K)
		key = hs_find_key(key_parm);
	else
		key = hs_find_key(key_code);

	temp_key_code = key_code;

	if (key_parm == HS_REL_K)
		key_code = key_parm;
	
	printk("[JJONG] %s : %d -> key_code = %x, key_param = %x, key = %x\n", __func__, __LINE__, key_code, key_parm, key);
	switch (key) {
	/* 0 - 9 */
	case KEY_0:
	case KEY_1_0xF2:
	case KEY_2:
	case KEY_3_0xF4:
	case KEY_4:
	case KEY_5:
	case KEY_6:
	case KEY_7:
	case KEY_8:
	case KEY_9:

	/* * # CALL ENDCALL SPACE */
	case KEY_STAR:
	case KEY_POUND:
	case KEY_CALL:
	case KEY_END:
	case KEY_SPACE:
	case KEY_DOT:

	/* a - z */
	case KEY_A:
	case KEY_B:
	case KEY_C:
	case KEY_D:
	case KEY_E:
	case KEY_F:
	case KEY_G:
	case KEY_H:
	case KEY_I:
	case KEY_J:
	case KEY_K:
	case KEY_L:
	case KEY_M:
	case KEY_N:
	case KEY_O:
	case KEY_P:
	case KEY_Q:
	case KEY_R:
	case KEY_S:
	case KEY_T:
	case KEY_U:
	case KEY_V:
	case KEY_W:
	case KEY_X:
	case KEY_Y:
	case KEY_Z:

	/* MENU HOME SEARCH BACK */
	case KEY_MENU:
	case KEY_HOME:
	case KEY_SEARCH:
	case KEY_BACK:
	
	case KEY_POWER:
//	case KEY_END:
	case KEY_MEDIA:
//	case KEY_VOLUMEUP:
//	case KEY_VOLUMEDOWN:
		input_report_key(hs->ipdev, key, (key_code != HS_REL_K));
		break;
//	case GPIO_KEY_SEND:
//	case GPIO_KEY_END:
//	case GPIO_KEY_CLEAR:
//	case GPIO_KEY_LEFT:
//	case GPIO_KEY_RIGHT:
//	case GPIO_KEY_UP:
//	case GPIO_KEY_DOWN:
//	case GPIO_KEY_SELECT:
//	case GPIO_KEY_MESSAGE:
	case GPIO_KEY_VOLUME_UP:
	case GPIO_KEY_VOLUME_DOWN:
//	case GPIO_KEY_POWER:
//	case GPIO_KEY_PTT :
//	case GPIO_KEY_SPEAKER:
//	case GPIO_KEY_LEFT_SOFT_KEY:
//	case GPIO_KEY_RIGHT_SOFT_KEY:
//	case GPIO_KEY_TASK:
//	case GPIO_KEY_VIDEO:
//	case GPIO_KEY_CAMERA:
//	case GPIO_KEY_WEB:
//	case GPIO_KEY_LEFT_SHIFT:
//	case GPIO_KEY_SPACE:
//	case GPIO_KEY_DOT:
//	case GPIO_KEY_HOME:
//	case GPIO_KEY_SEARCH:
//	case GPIO_KEY_RETURN:
//	case GPIO_KEY_ENTER:
//	case GPIO_KEY_FUNCTION:
		emulate_input_report_key(key, (key_code != HS_REL_K));
		break;
	case CMD_POWERUP:
		if(UTS_POWER_NUM == 1){
			
			input_report_key(hs->ipdev, KEY_POWER, (key_code != HS_REL_K));			
			UTS_POWER_NUM = 0;

		}else if(UTS_POWER_NUM == 0){
			if(!UTS_get_disp_state())
			{
				
				input_report_key(hs->ipdev, KEY_POWER, (key_code != HS_REL_K));
				UTS_POWER_NUM = 1;
			}
		}
		break;
	case CMD_POWERDOWN:	
		if(UTS_POWER_NUM == 1){
			
			input_report_key(hs->ipdev, KEY_POWER, (key_code != HS_REL_K));			
			UTS_POWER_NUM = 0;

		}else if(UTS_POWER_NUM == 0){
			if(UTS_get_disp_state())
			{
				input_report_key(hs->ipdev, KEY_POWER, (key_code != HS_REL_K));
				UTS_POWER_NUM = 1;
			}
		}
		break;
	case SW_HEADPHONE_INSERT_W_MIC:
		hs->mic_on = hs->hs_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, SW_HEADPHONE_INSERT,
							hs->hs_on);
		input_report_switch(hs->ipdev, SW_MICROPHONE_INSERT,
							hs->mic_on);
		update_state();
		break;

	case SW_HEADPHONE_INSERT:
		hs->hs_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, key, hs->hs_on);
		update_state();
		break;
	case SW_MICROPHONE_INSERT:
		hs->mic_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, key, hs->mic_on);
		update_state();
		break;
	case -1:
		printk(KERN_ERR "[JJONG] %s: No mapping for remote handset event %x\n",
				 __func__, temp_key_code);
		return;
	}
	input_sync(hs->ipdev);
}

static int handle_hs_rpc_call(struct msm_rpc_server *server,
			   struct rpc_request_hdr *req, unsigned len)
{
	struct rpc_keypad_pass_key_code_args {
		uint32_t key_code;
		uint32_t key_parm;
	};

	switch (req->procedure) {
	case RPC_KEYPAD_NULL_PROC:
		return 0;

	case RPC_KEYPAD_PASS_KEY_CODE_PROC: {
		struct rpc_keypad_pass_key_code_args *args;

		args = (struct rpc_keypad_pass_key_code_args *)(req + 1);
		args->key_code = be32_to_cpu(args->key_code);
		args->key_parm = be32_to_cpu(args->key_parm);

		report_hs_key(args->key_code, args->key_parm);

		return 0;
	}

	case RPC_KEYPAD_SET_PWR_KEY_STATE_PROC:
		/* This RPC function must be available for the ARM9
		 * to function properly.  This function is redundant
		 * when RPC_KEYPAD_PASS_KEY_CODE_PROC is handled. So
		 * input_report_key is not needed.
		 */
		return 0;
	default:
		return -ENODEV;
	}
}

static struct msm_rpc_server hs_rpc_server = {
	.prog		= HS_SERVER_PROG,
	.vers		= HS_SERVER_VERS,
	.rpc_call	= handle_hs_rpc_call,
};

static int process_subs_srvc_callback(struct hs_event_cb_recv *recv)
{
	if (!recv)
		return -ENODATA;

	report_hs_key(be32_to_cpu(recv->key.code), be32_to_cpu(recv->key.parm));

	return 0;
}

static void process_hs_rpc_request(uint32_t proc, void *data)
{
	if (proc == HS_EVENT_CB_PROC)
		process_subs_srvc_callback(data);
	else
		pr_err("%s: unknown rpc proc %d\n", __func__, proc);
}

static int hs_rpc_report_event_arg(struct msm_rpc_client *client,
					void *buffer, void *data)
{
	struct hs_event_rpc_req {
		uint32_t hs_event_data_ptr;
		struct hs_event_data data;
	};

	struct hs_event_rpc_req *req = buffer;

	req->hs_event_data_ptr	= cpu_to_be32(0x1);
	req->data.ver		= cpu_to_be32(HS_EVENT_DATA_VER);
	req->data.event_type	= cpu_to_be32(HS_EVNT_HSD);
	req->data.enum_disc	= cpu_to_be32(HS_EVNT_HSD);
	req->data.data_length	= cpu_to_be32(0x1);
	req->data.data		= cpu_to_be32(*(enum hs_src_state *)data);
	req->data.data_size	= cpu_to_be32(sizeof(enum hs_src_state));

	return sizeof(*req);
}

static int hs_rpc_report_event_res(struct msm_rpc_client *client,
					void *buffer, void *data)
{
	enum hs_return_value result;

	result = be32_to_cpu(*(enum hs_return_value *)buffer);
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	if (result == HS_SUCCESS)
		return 0;

	return 1;
}

void report_headset_status(bool connected)
{
	int rc = -1;
	enum hs_src_state status;

	if (connected == true)
		status = HS_SRC_STATE_HI;
	else
		status = HS_SRC_STATE_LO;

	rc = msm_rpc_client_req(rpc_client, HS_REPORT_EVNT_PROC,
				hs_rpc_report_event_arg, &status,
				hs_rpc_report_event_res, NULL, -1);

	if (rc)
		pr_err("%s: couldn't send rpc client request\n", __func__);
}
EXPORT_SYMBOL(report_headset_status);

static int hs_rpc_pwr_cmd_arg(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	struct hs_cmd_data_type *hs_pwr_cmd = buffer;

	hs_pwr_cmd->hs_cmd_data_type_ptr = cpu_to_be32(0x01);

	hs_pwr_cmd->ver = cpu_to_be32(0x03);
	hs_pwr_cmd->id = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->handle = cpu_to_be32(hs_subs_req->hs_handle_data);
	hs_pwr_cmd->disc_id1 = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->input_ptr = cpu_to_be32(0x01);
	hs_pwr_cmd->input_val = cpu_to_be32(hs->hs_pdata->pwr_key_delay_ms);
	hs_pwr_cmd->input_len = cpu_to_be32(0x01);
	hs_pwr_cmd->disc_id2 = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->output_len = cpu_to_be32(0x00);
	hs_pwr_cmd->delayed = cpu_to_be32(0x00);

	return sizeof(*hs_pwr_cmd);
}

static int hs_rpc_pwr_cmd_res(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	uint32_t result;

	result = be32_to_cpu(*((uint32_t *)buffer));
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	return 0;
}

static int hs_rpc_register_subs_arg(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	hs_subs_req = buffer;

	hs_subs_req->hs_subs_ptr	= cpu_to_be32(0x1);
	hs_subs_req->hs_subs.ver	= cpu_to_be32(0x1);
	hs_subs_req->hs_subs.srvc	= cpu_to_be32(HS_SUBS_RCV_EVNT);
	hs_subs_req->hs_subs.req	= cpu_to_be32(HS_SUBS_REGISTER);
	hs_subs_req->hs_subs.host_os	= cpu_to_be32(0x4); /* linux */
	hs_subs_req->hs_subs.disc	= cpu_to_be32(HS_SUBS_RCV_EVNT);
	hs_subs_req->hs_subs.id.evnt	= cpu_to_be32(HS_EVNT_CLASS_ALL);

	hs_subs_req->hs_cb_id		= cpu_to_be32(0x1);

	hs_subs_req->hs_handle_ptr	= cpu_to_be32(0x1);
	hs_subs_req->hs_handle_data	= cpu_to_be32(0x0);

	return sizeof(*hs_subs_req);
}

static int hs_rpc_register_subs_res(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	uint32_t result;

	result = be32_to_cpu(*((uint32_t *)buffer));
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	return 0;
}

static int hs_cb_func(struct msm_rpc_client *client, void *buffer, int in_size)
{
	int rc = -1;

	struct rpc_request_hdr *hdr = buffer;

	hdr->type = be32_to_cpu(hdr->type);
	hdr->xid = be32_to_cpu(hdr->xid);
	hdr->rpc_vers = be32_to_cpu(hdr->rpc_vers);
	hdr->prog = be32_to_cpu(hdr->prog);
	hdr->vers = be32_to_cpu(hdr->vers);
	hdr->procedure = be32_to_cpu(hdr->procedure);

	process_hs_rpc_request(hdr->procedure,
			    (void *) (hdr + 1));

	msm_rpc_start_accepted_reply(client, hdr->xid,
				     RPC_ACCEPTSTAT_SUCCESS);
	rc = msm_rpc_send_accepted_reply(client, 0);
	if (rc) {
		pr_err("%s: sending reply failed: %d\n", __func__, rc);
		return rc;
	}

	return 0;
}

static int __init hs_rpc_cb_init(void)
{
	int rc = 0, i, num_vers;

	num_vers = ARRAY_SIZE(rpc_vers);

	for (i = 0; i < num_vers; i++) {
		rpc_client = msm_rpc_register_client("hs",
			HS_RPC_PROG, rpc_vers[i], 0, hs_cb_func);

		if (IS_ERR(rpc_client))
			pr_debug("%s: RPC Client version %d failed, fallback\n",
				 __func__, rpc_vers[i]);
		else
			break;
	}

	if (IS_ERR(rpc_client)) {
		pr_err("%s: Incompatible RPC version error %ld\n",
			 __func__, PTR_ERR(rpc_client));
		return PTR_ERR(rpc_client);
	}

	rc = msm_rpc_client_req(rpc_client, HS_SUBSCRIBE_SRVC_PROC,
				hs_rpc_register_subs_arg, NULL,
				hs_rpc_register_subs_res, NULL, -1);
	if (rc) {
		pr_err("%s: RPC client request failed for subscribe services\n",
						__func__);
		goto err_client_req;
	}

	rc = msm_rpc_client_req(rpc_client, HS_PROCESS_CMD_PROC,
			hs_rpc_pwr_cmd_arg, NULL,
			hs_rpc_pwr_cmd_res, NULL, -1);
	if (rc)
		pr_err("%s: RPC client request failed for pwr key"
			" delay cmd, using normal mode\n", __func__);
	return 0;
err_client_req:
	msm_rpc_unregister_client(rpc_client);
	return rc;
}

static int __devinit hs_rpc_init(void)
{
	int rc;

	rc = hs_rpc_cb_init();
	if (rc) {
		pr_err("%s: failed to initialize rpc client, try server...\n",
						__func__);

		rc = msm_rpc_create_server(&hs_rpc_server);
		if (rc) {
			pr_err("%s: failed to create rpc server\n", __func__);
			return rc;
		}
	}

	return rc;
}

static void __devexit hs_rpc_deinit(void)
{
	if (rpc_client)
		msm_rpc_unregister_client(rpc_client);
}

static ssize_t msm_headset_print_name(struct switch_dev *sdev, char *buf)
{
	switch (switch_get_state(&hs->sdev)) {
	case NO_DEVICE:
		return sprintf(buf, "No Device\n");
	case MSM_HEADSET:
		return sprintf(buf, "Headset\n");
	}
	return -EINVAL;
}

static int __devinit hs_probe(struct platform_device *pdev)
{
	int rc = 0;
	struct input_dev *ipdev;

	hs = kzalloc(sizeof(struct msm_handset), GFP_KERNEL);
	if (!hs)
		return -ENOMEM;

	hs->sdev.name	= "h2w";
	hs->sdev.print_name = msm_headset_print_name;

	rc = switch_dev_register(&hs->sdev);
	if (rc)
		goto err_switch_dev_register;

	ipdev = input_allocate_device();
	if (!ipdev) {
		rc = -ENOMEM;
		goto err_alloc_input_dev;
	}
	input_set_drvdata(ipdev, hs);

	hs->ipdev = ipdev;

	if (pdev->dev.platform_data)
		hs->hs_pdata = pdev->dev.platform_data;

	if (hs->hs_pdata->hs_name)
		ipdev->name = hs->hs_pdata->hs_name;
	else
		ipdev->name	= DRIVER_NAME;

	ipdev->id.vendor	= 0x0001;
	ipdev->id.product	= 1;
	ipdev->id.version	= 1;

	input_set_capability(ipdev, EV_KEY, KEY_MEDIA);
	input_set_capability(ipdev, EV_KEY, KEY_VOLUMEUP);
	input_set_capability(ipdev, EV_KEY, KEY_VOLUMEDOWN);
	input_set_capability(ipdev, EV_SW, SW_HEADPHONE_INSERT);
	input_set_capability(ipdev, EV_SW, SW_MICROPHONE_INSERT);
	input_set_capability(ipdev, EV_KEY, KEY_POWER);
	input_set_capability(ipdev, EV_KEY, KEY_END);

	input_set_capability(ipdev, EV_KEY, KEY_0);
	input_set_capability(ipdev, EV_KEY, KEY_1_0xF2);
	input_set_capability(ipdev, EV_KEY, KEY_2);
	input_set_capability(ipdev, EV_KEY, KEY_3_0xF4);
	input_set_capability(ipdev, EV_KEY, KEY_4);
	input_set_capability(ipdev, EV_KEY, KEY_5);
	input_set_capability(ipdev, EV_KEY, KEY_6);
	input_set_capability(ipdev, EV_KEY, KEY_7);
	input_set_capability(ipdev, EV_KEY, KEY_8);
	input_set_capability(ipdev, EV_KEY, KEY_9);

	input_set_capability(ipdev, EV_KEY, KEY_STAR);
	input_set_capability(ipdev, EV_KEY, KEY_POUND);
	input_set_capability(ipdev, EV_KEY, KEY_CALL);
	input_set_capability(ipdev, EV_KEY, KEY_END);
	input_set_capability(ipdev, EV_KEY, KEY_SPACE);
	input_set_capability(ipdev, EV_KEY, KEY_DOT);
	
	input_set_capability(ipdev, EV_KEY, KEY_A);
	input_set_capability(ipdev, EV_KEY, KEY_B);
	input_set_capability(ipdev, EV_KEY, KEY_C);
	input_set_capability(ipdev, EV_KEY, KEY_D);
	input_set_capability(ipdev, EV_KEY, KEY_E);
	input_set_capability(ipdev, EV_KEY, KEY_F);
	input_set_capability(ipdev, EV_KEY, KEY_G);
	input_set_capability(ipdev, EV_KEY, KEY_H);
	input_set_capability(ipdev, EV_KEY, KEY_I);
	input_set_capability(ipdev, EV_KEY, KEY_J);
	input_set_capability(ipdev, EV_KEY, KEY_K);
	input_set_capability(ipdev, EV_KEY, KEY_L);
	input_set_capability(ipdev, EV_KEY, KEY_M);
	input_set_capability(ipdev, EV_KEY, KEY_N);
	input_set_capability(ipdev, EV_KEY, KEY_O);
	input_set_capability(ipdev, EV_KEY, KEY_P);
	input_set_capability(ipdev, EV_KEY, KEY_Q);
	input_set_capability(ipdev, EV_KEY, KEY_R);
	input_set_capability(ipdev, EV_KEY, KEY_S);
	input_set_capability(ipdev, EV_KEY, KEY_T);
	input_set_capability(ipdev, EV_KEY, KEY_U);
	input_set_capability(ipdev, EV_KEY, KEY_V);
	input_set_capability(ipdev, EV_KEY, KEY_W);
	input_set_capability(ipdev, EV_KEY, KEY_X);
	input_set_capability(ipdev, EV_KEY, KEY_Y);
	input_set_capability(ipdev, EV_KEY, KEY_Z);

	input_set_capability(ipdev, EV_KEY, KEY_MENU);
	input_set_capability(ipdev, EV_KEY, KEY_HOME);
	input_set_capability(ipdev, EV_KEY, KEY_SEARCH);
	input_set_capability(ipdev, EV_KEY, KEY_BACK);

	rc = input_register_device(ipdev);
	if (rc) {
		dev_err(&ipdev->dev,
				"hs_probe: input_register_device rc=%d\n", rc);
		goto err_reg_input_dev;
	}

	platform_set_drvdata(pdev, hs);

	rc = hs_rpc_init();
	if (rc) {
		dev_err(&ipdev->dev, "rpc init failure\n");
		goto err_hs_rpc_init;
	}

	return 0;

err_hs_rpc_init:
	input_unregister_device(ipdev);
	ipdev = NULL;
err_reg_input_dev:
	input_free_device(ipdev);
err_alloc_input_dev:
	switch_dev_unregister(&hs->sdev);
err_switch_dev_register:
	kfree(hs);
	return rc;
}

static int __devexit hs_remove(struct platform_device *pdev)
{
	struct msm_handset *hs = platform_get_drvdata(pdev);

	input_unregister_device(hs->ipdev);
	switch_dev_unregister(&hs->sdev);
	kfree(hs);
	hs_rpc_deinit();
	return 0;
}

static struct platform_driver hs_driver = {
	.probe		= hs_probe,
	.remove		= __devexit_p(hs_remove),
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init hs_init(void)
{
	return platform_driver_register(&hs_driver);
}
late_initcall(hs_init);

static void __exit hs_exit(void)
{
	platform_driver_unregister(&hs_driver);
}
module_exit(hs_exit);

MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:msm-handset");
