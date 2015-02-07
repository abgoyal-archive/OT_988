// #ifdef MODULE	// for test by sinclair.lee
/*
 * Copyright (C) 2007-2008 HTC Corporation.
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
#include <linux/input.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/ft_i2c.h>
#include <linux/earlysuspend.h>
#include <linux/gpio.h>

// #define FTDEBUG

#ifdef FTDEBUG
#undef dev_dbg
#define dev_dbg		dev_info
#endif

#define FT_TS_FUZZ 		0
#define FT_TS_FLAT 		0
#define PROTOCOL_LEN		26
#define FT_MAX_AREA		4096

enum {
	starttch_packet			= 0xF9,
	work_startreg_packet	= 0xFC,
};

// Order : HEAD(0xAAAA), Length(0b00xx_xxxx), finger(0b0000_xxxx), X1(0x0xxx), Y1(0x0xxx), 
enum {
	idx_finger_state = 3,
};

static struct workqueue_struct *ft_wq;

typedef struct i2c_client	bus_device;

static struct ft5202_data {
	int intr_gpio;
	int scl_pin;
	int sda_pin;
	int use_irq;
	/* delete when finish migration */
	int fw_ver;
	int report_rate;
	struct hrtimer timer;
	struct work_struct work;
	bus_device *client;
	struct input_dev *input;
	wait_queue_head_t wait;
	int (*power)(int on);
	struct early_suspend early_suspend;
} ft_data;

#include <mach/pmic.h>

static int ft5202_wake = PM_MPP_9;

#ifdef CONFIG_HAS_EARLYSUSPEND
static void ft_ts_early_suspend(struct early_suspend *h);
static void ft_ts_late_resume(struct early_suspend *h);
#endif

static ssize_t touch_vendor_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	ssize_t ret = 0;

	sprintf(buf, "%s_%#x\n", FT_I2C_NAME, ft_data.fw_ver);
	ret = strlen(buf) + 1;

	return ret;
}

static DEVICE_ATTR(vendor, 0444, touch_vendor_show, NULL);

static struct kobject *android_touch_kobj;

static int touch_sysfs_init(void)
{
	int ret ;

	android_touch_kobj = kobject_create_and_add("android_touch", NULL) ;
	if (android_touch_kobj == NULL) {
		printk(KERN_INFO
		       "touch_sysfs_init: subsystem_register failed\n");
		ret = -ENOMEM;
		goto err;
	}

	ret = sysfs_create_file(android_touch_kobj, &dev_attr_vendor.attr);
	if (ret) {
		printk(KERN_INFO
		       "touch_sysfs_init: sysfs_create_group failed\n");
		goto err4;
	}

	return 0 ;
err4:
	kobject_del(android_touch_kobj);
err:
	return ret ;
}

static int ft5202_detect_int_level(void)
{
	unsigned v;
	v = gpio_get_value(ft_data.intr_gpio);
	/* printk("ft5202_detect_int_level: v = %0x\n", v); */
	return v;
}

#if 0	// for gcc-4.4.3 block unused part
static int __ft5202_poll(bus_device *client)
{
	int status = 0, retry = 10;

	do {
		status = ft5202_detect_int_level();
		dev_dbg(&client->dev, "%s: status = %d\n", __func__, status);
		retry--;
		msleep_interruptible(20);
	} while (status == 1 && retry > 0);

	dev_dbg(&client->dev, "%s: poll interrupt status %s\n",
			__func__, status == 1 ? "high" : "low");
	return (status == 0 ? 0 : -ETIMEDOUT);
}

static int ft5202_poll(bus_device *client)
{
	return __ft5202_poll(client);
}
#endif

static int ft5202_get_data(bus_device *client, uint8_t *cmd, size_t csiz,
			    uint8_t *buf, size_t size, int sleep)
{
	int rc=0;
	unsigned time_out = msecs_to_jiffies(10);

	dev_dbg(&client->dev, "%s: enter.\n", __func__);

	if (buf == NULL)
		return -EINVAL;

	if ((i2c_master_send(client, cmd, csiz)) != csiz) {
		dev_err(&client->dev,
			"%s: i2c_master_send failed\n", __func__);
		return -EINVAL;
	}
	if(size < 1) return 0;

	udelay(10);

	if (sleep == 1) {
		rc = wait_event_timeout(ft_data.wait,
				i2c_master_recv(client, buf, size) == size, time_out);
		if (rc == 0) {
			dev_err(&client->dev,
				"%s: i2c_master_recv failed\n", __func__);
			return -ETIMEDOUT;
		}
	} else {
#if 0
		if(sleep == 0)
			rc = ft5202_poll(client);

		if (rc < 0)
			return -EINVAL;
		else
#endif
		{
			if (i2c_master_recv(client, buf, size) != size)
				return -EINVAL;
		}
	}

	return 0;
}

static int ft5202_set_data(bus_device *client, uint8_t *cmd, size_t csiz)
{
	int i;

	dev_dbg(&client->dev, "%s: enter.\n", __func__);

	if (cmd == NULL)
		return -EINVAL;

	for(i=0, cmd[csiz-1]=0; i<(csiz-1); i++)
		cmd[csiz-1] ^= cmd[i];

	if ((i2c_master_send(client, cmd, csiz)) != csiz) {
		dev_err(&client->dev,
			"%s: i2c_master_send failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}

static int ft5202_get_version(bus_device *client)
{
	int rc;
	uint8_t cmd[2] = { work_startreg_packet, 0x40+0x3B };	// add 0x40 for read only
	uint8_t buf_recv[4] = { 0 };

	rc = ft5202_get_data(client, cmd, 2, buf_recv, 2, -1);
	if (rc < 0)
		return rc;

	/* delete after migration */
	ft_data.fw_ver =buf_recv[0];

	printk(KERN_INFO "%s: firmware version: 0x%x\n",
			__func__, ft_data.fw_ver);
	return 0;
}

static int ft5202_get_report_rate(bus_device *client)
{
	int rc;
	uint8_t cmd[2] = { work_startreg_packet, 0x40+0x08 };
	uint8_t buf_recv[4] = { 0 };

	rc = ft5202_get_data(client, cmd, 2, buf_recv, 2, -1);
	if (rc < 0)
		return rc;

	/* delete after migration */
	ft_data.report_rate =buf_recv[0];

	printk(KERN_INFO "%s: report rate: 0x%x\n",
			__func__, ft_data.report_rate);
	return 0;
}

#if 0	// for gcc-4.4.3 block unused part
static int ft5202_set_report_rate(bus_device *client, uint8_t rate)
{
	int rc;
	uint8_t cmd[4] = { work_startreg_packet, 0x08,  };

	cmd[2] = rate;
	rc = ft5202_set_data(client, cmd, 4);

	return rc;
}
#endif

static int ft5202_set_workmode(bus_device *client, uint8_t mode)
{
	int rc;
	uint8_t cmd[4] = { work_startreg_packet, 0x3C,  };

	cmd[2] = mode;
	rc = ft5202_set_data(client, cmd, 4);

	return rc;
}

#ifdef CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5202_POWER_OFF	//	Teddy 2011-09-29
#else
static int ft5202_set_powermode(bus_device *client, uint8_t mode)
{
	int rc;
	uint8_t cmd[4] = { work_startreg_packet, 0x3A,  };

	cmd[2] = mode;
	rc = ft5202_set_data(client, cmd, 4);

	return rc;
}
#endif	//	CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5202_POWER_OFF

static int __set_report_type(bus_device *client)
{
	return 0;
}

static inline int ft5202_parse_xy(bus_device *client, uint8_t *data, uint16_t *x, uint16_t *y)
{
	struct ft_i2c_platform_data *pdata;
	uint16_t val;

	pdata = client->dev.platform_data;

	val = (data[0] & 0x0f);
	val <<= 8;
	val |= data[1];
	if(ft_data.fw_ver == 0x11)
		*x = val;
	else
	*x = pdata->abs_x_max - val;

	val = (data[2] & 0x0f);
	val <<= 8;
	val |= data[3];
	*y = pdata->abs_y_max - val;

	return 0;
}

/*	ft5202_ts_init -- hand shaking with touch panel
 *
 *	1. set to work mode
 *	2. check its' firmware version
 *	3. set up sensitivity, report rate, ...
 */
static int ft5202_ts_init(bus_device *client)
{
	int rc;

	rc = ft5202_get_version(client);
	if (rc < 0)
	{
#ifdef FTDEBUG
		printk(KERN_INFO "ft5202_get_version error.\n");
#endif
		goto hand_shake_failed;
	}
	ft5202_get_report_rate(client);
//	ft5202_set_report_rate(client, 0x3);	// 3 * 10Hz = 30Hz

	rc = ft5202_set_workmode(client, 0x01);
	if (rc < 0)
	{
#ifdef FTDEBUG
		printk(KERN_INFO "ft5202_set_workmode error.\n");
#endif
		goto hand_shake_failed;
	}
	dev_dbg(&client->dev, "%s: firmware checking done.\n", __func__);

	rc = __set_report_type(client);
	if (rc < 0)
		goto hand_shake_failed;
	dev_dbg(&client->dev,
		"%s: channging operating mode done.\n", __func__);

hand_shake_failed:
	return rc;
}

static void ft5202_recovery(bus_device *client)
{
	int rc;

	dev_err(&client->dev, "%s: ft5202 recovery processing\n", __func__);

	/* power off level shift */
	ft_data.power(0);
	msleep_interruptible(5);
	/* power on level shift */
	ft_data.power(1);
	/* re-initial */
	msleep_interruptible(100);
	rc = ft5202_ts_init(client);
	if (ft5202_detect_int_level() == 0)
		queue_work(ft_wq, &ft_data.work);
}

static int ft5202_recv_data(bus_device *client, uint8_t *buf)
{
	int rc, bytes_to_recv = PROTOCOL_LEN;
	uint8_t cmd[2] = {starttch_packet, 0};

	if (buf == NULL)
		return -EINVAL;

	memset(buf, 0, bytes_to_recv);
	rc = ft5202_get_data(client, cmd, 1, buf, bytes_to_recv, 0);

#ifdef FTDEBUG
	{
		int i;
		uint8_t *p, tmp[PROTOCOL_LEN * 3 + 4];
		for(i=0, p=tmp; i<bytes_to_recv; i++)
		{
			p += sprintf(p, "%02x ", buf[i]);
		}
		dev_dbg(&client->dev, "%s: %s\n", __func__, tmp);
	}
#endif

	if (rc != 0) {	// any error?
		ft5202_recovery(client);
		return -EINVAL;
	}

	return rc;
}

static void ft5202_report_data(bus_device *client, uint8_t *buf)
{
	static unsigned report_time;
	unsigned report_time2;
	static uint8_t prev_finger_stat=0;
	int head;

	if(buf[2] != PROTOCOL_LEN)	// this chip, somtimes, reports invalid data so double check is needed
		return;

	head = buf[0];
	head = (head<<8) | buf[1];

	switch (head) {
	case 0xAAAA:
	{
		uint16_t x1, x2, y1, y2;
		uint8_t finger_stat, w1 = 1, w2 = 1;

		ft5202_parse_xy(client, &buf[5], &x1, &y1);
		finger_stat = buf[idx_finger_state] & 0x0F;

		if ((finger_stat > 0) || (prev_finger_stat > 0))
		{
			input_report_key(ft_data.input, BTN_TOUCH, finger_stat>0);
			if (finger_stat > 0)
			{
				input_report_abs(ft_data.input, ABS_X, x1);
				input_report_abs(ft_data.input, ABS_Y, y1);
			}
		}
		if ((finger_stat > 0) && (prev_finger_stat == 0))
		{
			dev_info(&client->dev, "x1 = %d, y1 = %d, "
				"w1 = %d, w2 = %d, finger status = %d ..\n",
				x1, y1, w1, w2, finger_stat);
		}
		dev_dbg(&client->dev, "x1 = %d, y1 = %d, "
			"w1 = %d, w2 = %d, finger status = %d\n",
			x1, y1, w1, w2, finger_stat);
		if ((finger_stat > 1) || (prev_finger_stat > 1))
		{
			input_report_key(ft_data.input, BTN_2, finger_stat>1);
			if (finger_stat > 1)
			{
				ft5202_parse_xy(client, &buf[9], &x2, &y2);
				input_report_abs(ft_data.input, ABS_HAT0X, x2);
				input_report_abs(ft_data.input, ABS_HAT0Y, y2);
				dev_dbg(&client->dev, "x2 = %d, y2 = %d\n", x2, y2);
			}
		}
		
		if(finger_stat > 0)
		{
			input_report_abs(ft_data.input, ABS_MT_TOUCH_MAJOR, FT_MAX_AREA);
			input_report_abs(ft_data.input, ABS_MT_POSITION_X, x1);
			input_report_abs(ft_data.input, ABS_MT_POSITION_Y, y1);
			input_mt_sync(ft_data.input);
			if(finger_stat > 1)
			{
				input_report_abs(ft_data.input, ABS_MT_TOUCH_MAJOR, FT_MAX_AREA);
				input_report_abs(ft_data.input, ABS_MT_POSITION_X, x2);
				input_report_abs(ft_data.input, ABS_MT_POSITION_Y, y2);
				input_mt_sync(ft_data.input);
			}
		}
		else if(prev_finger_stat >= 1 && finger_stat == 0)
		{
			input_report_abs(ft_data.input, ABS_MT_TOUCH_MAJOR, 0);
			input_mt_sync(ft_data.input);
		}

		prev_finger_stat = finger_stat;
		input_sync(ft_data.input);
		break;
	}
	default:
		dev_err(&client->dev,
			"%s: Unknown packet type: %0x\n", __func__, buf[0]);
		ft5202_recovery(client);

		break;
	}

	report_time2 = jiffies;
	dev_dbg(&client->dev,
		"report time = %d\n",
		jiffies_to_msecs(report_time2 - report_time));

	report_time = report_time2;

}

static void ft5202_work_func(struct work_struct *work)
{
	int rc;
	uint8_t buf[PROTOCOL_LEN] = { 0 };
	bus_device *client = ft_data.client;

	dev_dbg(&client->dev, "%s: enter. \n", __func__);

#if 0
	/* this means that we have already serviced it */
	if (ft5202_detect_int_level())
		return;
#endif

	rc = ft5202_recv_data(client, buf);
	if (rc < 0)
		return;

	ft5202_report_data(client, buf);
}

static irqreturn_t ft5202_ts_interrupt(int irq, void *dev_id)
{
	/* the queue_work has spin_lock protection */
	/* disable_irq(irq); */
	queue_work(ft_wq, &ft_data.work);

	return IRQ_HANDLED;
}

static enum hrtimer_restart ft5202_ts_timer_func(struct hrtimer *timer)
{
	queue_work(ft_wq, &ft_data.work);
	hrtimer_start(&ft_data.timer,
		      ktime_set(0, 12500000),
		      HRTIMER_MODE_REL);

	return HRTIMER_NORESTART;
}

static int ft5202_register_interrupt(bus_device *client)
{
	int err = 0;

	if (client->irq) {
		ft_data.use_irq = 1;

		err = request_irq(client->irq, ft5202_ts_interrupt, 2,
				  FT_I2C_NAME, &ft_data);
		if (err < 0) {
			dev_err(&client->dev,
				"%s(%s): Can't allocate irq %d\n",
				__FILE__, __func__, client->irq);
			ft_data.use_irq = 0;
		}
	}

	if (!ft_data.use_irq) {
		hrtimer_init(&ft_data.timer,
			     CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		ft_data.timer.function = ft5202_ts_timer_func;
		hrtimer_start(&ft_data.timer, ktime_set(1, 0),
			      HRTIMER_MODE_REL);
	}

	dev_dbg(&client->dev,
		"ft starts in %s mode.\n",
		ft_data.use_irq == 1 ? "interrupt":"polling");
	return 0;
}

static int ft5202_probe(
	bus_device *client, const struct i2c_device_id *id)
{
	int err = 0;
	struct ft_i2c_platform_data *pdata;

	ft_wq = create_singlethread_workqueue("ft_wq");
	if (!ft_wq) {
		err = -ENOMEM;
		goto fail;
	}

	printk(KERN_INFO "ft5202_probe enter.\n");
	dev_dbg(&client->dev, "ft5202_probe enter.\n");
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
	{
		printk(KERN_ERR "%s i2c SMBUS is not supported\n", __FUNCTION__);
//		return -ENODEV;
	}
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		dev_err(&client->dev,
			"No supported serial(i2c) func what we need?!!\n");
		err = -ENOTSUPP;
		goto fail;
	}

	ft_data.client = client;
	strlcpy(client->name, FT_I2C_NAME, I2C_NAME_SIZE);
	i2c_set_clientdata(client, &ft_data);
	INIT_WORK(&ft_data.work, ft5202_work_func);
	init_waitqueue_head(&ft_data.wait);

	ft_data.input = input_allocate_device();
	if (ft_data.input == NULL) {
		err = -ENOMEM;
		goto fail;
	}

	pdata = client->dev.platform_data;
	if (likely(pdata != NULL)) {
		ft_data.intr_gpio =
			((struct ft_i2c_platform_data *)pdata)->intr_gpio;
		ft_data.power =
			((struct ft_i2c_platform_data *)pdata)->power;
		ft_data.power(1);
		if(pmic_secure_mpp_control_digital_output(ft5202_wake, PM_MPP__DLOGIC__LVL_MSMP, PM_MPP__DLOGIC_OUT__CTRL_LOW))
		 	printk(KERN_ERR "ft5202 wake mpp setting error...\n");
		msleep_interruptible(5);
		pmic_secure_mpp_control_digital_output(ft5202_wake, PM_MPP__DLOGIC__LVL_MSMP, PM_MPP__DLOGIC_OUT__CTRL_HIGH);
		dev_info(&client->dev, "touch panel is powered on. \n");
		msleep_interruptible(500);	/* ft will be ready after about 500 ms */
	} else {
		dev_err(&client->dev, "without platform data??!!\n");
	}

	err = ft5202_ts_init(client);
	if (err < 0) {
		printk(KERN_INFO "looks like it's not FocalTech, so..i'll quit\n");
		err = -ENODEV;
		goto fail;
	}

	if (pdata) {
		while (pdata->version > ft_data.fw_ver) {
			printk(KERN_INFO "ft5202_probe: old tp detected, "
					"panel version = 0x%x\n",
					ft_data.fw_ver);
			pdata++;
		}
	}
	printk(KERN_INFO "ft5202_register_input\n");

	ft_data.input->name = FT_I2C_NAME;
	ft_data.input->id.bustype = BUS_I2C;

	set_bit(EV_SYN, ft_data.input->evbit);
	set_bit(EV_KEY, ft_data.input->evbit);
	set_bit(BTN_TOUCH, ft_data.input->keybit);
	set_bit(BTN_2, ft_data.input->keybit);
	set_bit(EV_ABS, ft_data.input->evbit);
	input_set_capability(ft_data.input, EV_KEY, BTN_TOUCH);
	input_set_capability(ft_data.input, EV_KEY, BTN_2);

	input_set_abs_params(ft_data.input, ABS_X,
			     pdata->abs_x_min,  pdata->abs_x_max,
			     FT_TS_FUZZ, FT_TS_FLAT);
	input_set_abs_params(ft_data.input, ABS_Y,
			     pdata->abs_y_min,  pdata->abs_y_max,
			     FT_TS_FUZZ, FT_TS_FLAT);
	input_set_abs_params(ft_data.input, ABS_HAT0X,
			     pdata->abs_x_min,  pdata->abs_x_max,
			     FT_TS_FUZZ, FT_TS_FLAT);
	input_set_abs_params(ft_data.input, ABS_HAT0Y,
			     pdata->abs_y_min,  pdata->abs_y_max,
			     FT_TS_FUZZ, FT_TS_FLAT);
	input_set_abs_params(ft_data.input, ABS_PRESSURE, 0, 255,
			     FT_TS_FUZZ, FT_TS_FLAT);
	input_set_abs_params(ft_data.input, ABS_TOOL_WIDTH, 1, 8,
			     1, FT_TS_FLAT);

	input_set_abs_params(ft_data.input, ABS_MT_POSITION_X,
				pdata->abs_x_min,  pdata->abs_x_max, FT_TS_FUZZ, FT_TS_FLAT);
	input_set_abs_params(ft_data.input, ABS_MT_POSITION_Y,
				pdata->abs_y_min,  pdata->abs_y_max, FT_TS_FUZZ, FT_TS_FLAT);
	input_set_abs_params(ft_data.input, ABS_MT_TOUCH_MAJOR,
				0, FT_MAX_AREA, FT_TS_FUZZ, FT_TS_FLAT);

	err = input_register_device(ft_data.input);
	if (err < 0) {
		dev_err(&client->dev,
			"%s: input_register_device failed, err = %d\n",
			__func__, err);
		goto fail;
	}

	ft5202_register_interrupt(ft_data.client);

	/* checking the interrupt to avoid missing any interrupt */
	if (ft5202_detect_int_level() == 0)
		ft5202_ts_interrupt(client->irq, NULL);
#ifdef CONFIG_HAS_EARLYSUSPEND
	ft_data.early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 1;
	ft_data.early_suspend.suspend = ft_ts_early_suspend;
	ft_data.early_suspend.resume = ft_ts_late_resume;
	register_early_suspend(&ft_data.early_suspend);
#endif
	touch_sysfs_init();
	return 0;

fail:
	input_free_device(ft_data.input);
	if (ft_wq)
		destroy_workqueue(ft_wq);
	return err;
}

static int ft5202_remove(bus_device *client)
{
	struct ft5202_data *tp = i2c_get_clientdata(client);

	if (ft_wq)
		destroy_workqueue(ft_wq);

	if (ft_data.use_irq)
		free_irq(client->irq, tp);
	else
		hrtimer_cancel(&ft_data.timer);

	dev_dbg(&client->dev, "%s: enter.\n", __func__);

	kobject_del(android_touch_kobj);
	input_unregister_device(tp->input);
	return 0;
}

static int ft5202_suspend(bus_device *client, pm_message_t mesg)
{
	dev_dbg(&client->dev, "%s: enter. irq = %d\n", __func__, client->irq);

#ifdef CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5202_POWER_OFF	// 20110413
	disable_irq(client->irq);
	cancel_work_sync(&ft_data.work);

	pmic_secure_mpp_control_digital_output(ft5202_wake, PM_MPP__DLOGIC__LVL_MSMP, PM_MPP__DLOGIC_OUT__CTRL_LOW);

	/* power off level shift */
	ft_data.power(0);
#else
	if (ft5202_set_powermode(client, 0x03) < 0)	// hibernate mode
		dev_err(&client->dev, "%s: cannot enter suspend mode\n", __func__);
	else
		disable_irq(client->irq);
#endif

	return 0;
}

static int ft5202_resume(bus_device *client)
{
	int rc;

	dev_dbg(&client->dev,
		"%s: enter. irq = %d\n", __func__, client->irq);

#ifdef CONFIG_TOUCHSCREEN_FOCALTECH_I2C_5202_POWER_OFF	// 20110413
//	disable_irq(client->irq);	// block 20110413 suspend already called this

	/* power on level shift */
	ft_data.power(1);
	pmic_secure_mpp_control_digital_output(ft5202_wake, PM_MPP__DLOGIC__LVL_MSMP, PM_MPP__DLOGIC_OUT__CTRL_HIGH);

	/* re-initial */
	msleep_interruptible(500);
	rc = ft5202_ts_init(client);

	enable_irq(client->irq);

	if (ft5202_detect_int_level() == 0)
		ft5202_ts_interrupt(client->irq, NULL);
#else
	rc = pmic_secure_mpp_control_digital_output(ft5202_wake, PM_MPP__DLOGIC__LVL_MSMP, PM_MPP__DLOGIC_OUT__CTRL_LOW);
	if(rc)
		dev_err(&client->dev, "%s: cannot resume from suspend mode\n", __func__);
	
	msleep_interruptible(5);
	pmic_secure_mpp_control_digital_output(ft5202_wake, PM_MPP__DLOGIC__LVL_MSMP, PM_MPP__DLOGIC_OUT__CTRL_HIGH);

	enable_irq(client->irq);
#endif

	return 0;
}


#ifdef CONFIG_HAS_EARLYSUSPEND
static void ft_ts_early_suspend(struct early_suspend *h)
{
	bus_device *client = ft_data.client;

	dev_dbg(&client->dev, "%s enter.\n", __func__);
	ft5202_suspend(client, PMSG_SUSPEND);
}

static void ft_ts_late_resume(struct early_suspend *h)
{
	bus_device *client = ft_data.client;

	dev_dbg(&client->dev, "%s enter.\n", __func__);
	ft5202_resume(client);
}
#endif

/* -------------------------------------------------------------------- */
static const struct i2c_device_id ft5202_ts_id[] = {
	{ FT_I2C_NAME, 0 },
	{ }
};

static struct i2c_driver ft5202_driver = {
	.probe		= ft5202_probe,
	.remove		= ft5202_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= ft5202_suspend,
	.resume		= ft5202_resume,
#endif
	.id_table	= ft5202_ts_id,
	.driver		= {
		.name = FT_I2C_NAME,
	},
};

static int __init ft5202_init(void)
{
	return i2c_add_driver(&ft5202_driver);
}

static void __exit ft5202_exit(void)
{
	i2c_del_driver(&ft5202_driver);
}

#ifdef MODULE
EXPORT_SYMBOL(ft5202_init);
EXPORT_SYMBOL(ft5202_exit);
#endif

module_init(ft5202_init);
module_exit(ft5202_exit);

MODULE_AUTHOR("Sinclair.Lee <sinclair.lee@m7system.com>");
MODULE_DESCRIPTION("FT ft5202 driver");
MODULE_LICENSE("GPL");
// #endif

