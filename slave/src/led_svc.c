/*
 * Copyright (c) 2019 Marcio Montenegro <mtuxpe@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(led_svc);

#include "led_svc.h"

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static bool
    led_state; /* Tracking state here supports GPIO expander-based LEDs. */
static bool led_ok;

void led_update(int on_off) {
  if (!led_ok) {
    return;
  }

  led_state = ((on_off == 1) ? true : false);
  LOG_INF("Turn %s LED", led_state ? "on" : "off");
  gpio_pin_set(led.port, led.pin, led_state);
}

int led_init(void) {
  int ret;

  led_ok = gpio_is_ready_dt(&led);
  if (!led_ok) {
    LOG_ERR("Error: LED on GPIO %s pin %d is not ready", led.port->name,
            led.pin);
    return -ENODEV;
  }

  ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
  if (ret < 0) {
    LOG_ERR("Error %d: failed to configure GPIO %s pin %d", ret, led.port->name,
            led.pin);
  }

  return ret;
}
