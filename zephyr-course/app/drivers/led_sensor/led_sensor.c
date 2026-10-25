#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <errno.h>
#include <stdbool.h>

#include "led_sensor.h"

LOG_MODULE_REGISTER(our_led_sensor, LOG_LEVEL_INF);

#define DT_DRV_COMPAT our_led_sensor


/* Configuration for each LED sensor instance */
struct led_sensor_config {
    struct gpio_dt_spec led;
};

struct led_sensor_data {
    bool enabled;
};

/* Turn the LED ON when sensor_sample_fetch() is called */
static int led_sensor_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    const struct led_sensor_config *config = dev->config;
    const struct led_sensor_data *data = dev->data;

    ARG_UNUSED(chan);

    if (!data->enabled) {
        (void)gpio_pin_set_dt(&config->led, 0);
        return -EACCES;
    }
    
    int ret = gpio_pin_set_dt(&config->led, 1);

    if (ret < 0) {
        LOG_ERR("Failed to turn LED ON: %d", ret);
        return ret;
    }

    LOG_INF("LED turned ON");

    return 0;
}


/* Turn the LED OFF when sensor_channel_get() is called */
static int led_sensor_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    const struct led_sensor_config *config = dev->config;

    ARG_UNUSED(chan);
    ARG_UNUSED(val);

    int ret = gpio_pin_set_dt(&config->led, 0);

    if (ret < 0) {
        LOG_ERR("Failed to turn LED OFF: %d", ret);
        return ret;
    }

    LOG_INF("LED turned OFF");

    return 0;
}

int led_sensor_set_enabled(const struct device *dev, bool enabled)
{
    if (dev == NULL || !device_is_ready(dev)) {
        return -ENODEV;
    }

    struct led_sensor_data *data = dev->data;

    data->enabled = enabled;

    LOG_INF("LED sensor enabled: %s",
            enabled ? "true" : "false");

    return 0;
}


/* Register the callbacks required by the Sensor API */
static DEVICE_API(sensor, led_sensor_api) = {
    .sample_fetch = led_sensor_sample_fetch,
    .channel_get = led_sensor_channel_get,
};


/* Initialize the GPIO used by the LED */
static int led_sensor_init(const struct device *dev)
{
    const struct led_sensor_config *config = dev->config;

    if (!gpio_is_ready_dt(&config->led)) {
        LOG_ERR("LED GPIO device is not ready");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&config->led,
                                    GPIO_OUTPUT_INACTIVE);

    if (ret < 0) {
        LOG_ERR("Failed to configure LED GPIO: %d", ret);
        return ret;
    }

    LOG_INF("LED sensor initialized");

    return 0;
}


/* Create one device instance for every enabled matching DT node */
#define LED_SENSOR_DEFINE(inst)                                      \
    static const struct led_sensor_config led_sensor_config_##inst = { \
        .led = GPIO_DT_SPEC_INST_GET(inst, led_gpios),                \
    };                                                                \
    static struct led_sensor_data led_sensor_data_##inst = {          \
        .enabled = true,                                              \
    };                                                                \
    DEVICE_DT_INST_DEFINE(                                            \
        inst,                                                         \
        led_sensor_init,                                              \
        NULL,                                                         \
        &led_sensor_data_##inst,                                      \
        &led_sensor_config_##inst,                                    \
        POST_KERNEL,                                                  \
        90,                                                           \
        &led_sensor_api                                               \
    );

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)