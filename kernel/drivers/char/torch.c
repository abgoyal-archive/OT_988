#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <mach/pmic.h>



//static struct timer_list timer_led;
static struct task_struct * task_test;

static struct platform_device *flash_led_dev;

extern unsigned int is_video_flash_on;//yubin add for bug 298738

/* Sysfs method to input value of flash_led*/
static ssize_t write_flash_led(struct device *dev,  struct device_attribute *attr, const char *buffer, size_t count)
{
       uint16_t x;
	
	sscanf(buffer, "%d", &x);

	if(x>=200)
		{
		x = 200;
	}
	if(is_video_flash_on)//yubin add for bug 298738
		return count;//yubin add for bug 298738

	pmic_flash_led_set_current(x);
	return count;
}

/* Attach the sysfs write method */
DEVICE_ATTR(led, 0777, NULL, write_flash_led);

/* Attribute Descriptor */
static struct attribute *flash_led_attrs[] = {
  &dev_attr_led.attr,
  NULL

};

/* Attribute group */
static struct attribute_group flash_led_attr_group = {
  .attrs = flash_led_attrs,
  	
};

static int __init flash_led_init(void)
{
	
  /* Register a platform device */
  flash_led_dev = platform_device_register_simple("flash_led", -1, NULL, 0);

  if (IS_ERR(flash_led_dev)) {
  	
    PTR_ERR(flash_led_dev);
    printk("flash_led_init: error\n");

  }

  

  /* Create a sysfs node to wirte flash_led */
  sysfs_create_group(&flash_led_dev->dev.kobj, &flash_led_attr_group);
  return 0;

}

static void flash_led_cleanup(void)
{

  /* Cleanup sysfs node */
  sysfs_remove_group(&flash_led_dev->dev.kobj, &flash_led_attr_group);

  /* Unregister driver */
  platform_device_unregister(flash_led_dev);

  return;

}

module_init(flash_led_init);

module_exit(flash_led_cleanup);

