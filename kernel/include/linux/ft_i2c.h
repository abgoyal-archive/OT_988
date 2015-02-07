#ifndef FT_I2C_H
#define FT_I2C_H

#define FT_I2C_NAME "ft-touch"

struct ft_i2c_platform_data {
	uint16_t version;
	int abs_x_min;
	int abs_x_max;
	int abs_y_min;
	int abs_y_max;
	int intr_gpio;
	int scl_pin;
	int sda_pin;
	int (*power)(int on);
};

#endif

