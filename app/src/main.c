/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
//#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/shell/shell.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

#define CB_ENB DT_ALIAS(cbenb)
#define CB_MODE DT_ALIAS(cbmode)
#define CB_BUCK DT_ALIAS(cbbuck)
#define CB_BOOST DT_ALIAS(cbboost)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static const struct gpio_dt_spec cb_enb = GPIO_DT_SPEC_GET(CB_ENB, gpios);
static const struct gpio_dt_spec cb_mode = GPIO_DT_SPEC_GET(CB_MODE, gpios);
static const struct gpio_dt_spec cb_buck = GPIO_DT_SPEC_GET(CB_BUCK, gpios);
static const struct gpio_dt_spec cb_boost = GPIO_DT_SPEC_GET(CB_BOOST, gpios);

void cell_off(void){
	gpio_pin_set_dt(&cb_buck, 0);
	gpio_pin_set_dt(&cb_boost, 0);
	gpio_pin_set_dt(&cb_enb, 1);
	gpio_pin_set_dt(&cb_mode, 1);
}

void cell_buck(void){
	gpio_pin_set_dt(&cb_boost, 0);
	gpio_pin_set_dt(&cb_buck, 1);
	gpio_pin_set_dt(&cb_enb, 1);
	gpio_pin_set_dt(&cb_mode, 0);
}

void cell_boost(void){
	gpio_pin_set_dt(&cb_buck, 0);
	gpio_pin_set_dt(&cb_boost, 1);
	gpio_pin_set_dt(&cb_mode, 1);
	gpio_pin_set_dt(&cb_enb, 0);
	k_msleep(500);
	gpio_pin_set_dt(&cb_enb, 1);
	
}

static int cell_cmd_handler(const struct shell *sh,
                            size_t argc, char **argv, void *data)
{
        int mode;

        /* data is a value corresponding to called command syntax */
        mode = (int)data;
        //adc_set_gain(gain);
		switch (mode)
		{
		case 1:
			cell_buck();
			break;
		case 2:
			cell_boost();
			break;
		default:
			cell_off();
			break;
		}

        shell_print(sh, "ADC gain set to: %s\n"
                           "Value send to ADC driver: %d",
                           argv[0],
                           mode);

        return 0;
}

SHELL_SUBCMD_DICT_SET_CREATE(sub_cell, cell_cmd_handler,
        (buck, 1, "cell buck"), (boost, 2, "cell boost"), (off, 3, "cell off")
);
SHELL_CMD_REGISTER(cell, &sub_cell, "Set Cell Balancer", NULL);

int main(void)
{
	uint8_t data = 0;
	int ret;
	printk("Hello World! %s\n", CONFIG_BOARD);

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	if (!gpio_is_ready_dt(&cb_enb)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&cb_enb, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	if (!gpio_is_ready_dt(&cb_mode)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&cb_mode, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	if (!gpio_is_ready_dt(&cb_buck)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&cb_buck, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	if (!gpio_is_ready_dt(&cb_boost)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&cb_boost, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	cell_off();

	while (1) {
		//printk("Blinking %s: %03d\n", CONFIG_BOARD, data++);
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}

		//gpio_pin_toggle_dt(&cb_enb);
		//gpio_pin_toggle_dt(&cb_mode);
		//gpio_pin_toggle_dt(&cb_buck);
		//gpio_pin_toggle_dt(&cb_boost);
		//gpio_pin_set_dt(&cb_enb, 0);
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

