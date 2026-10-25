#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "../drivers/led_sensor/led_sensor.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define SENSOR_NODE DT_NODELABEL(our_driver0)

int main()
{
    const struct device *driver = DEVICE_DT_GET(SENSOR_NODE);

    if (!device_is_ready(driver)) {
        LOG_ERR("LED sensor driver is not ready");
        return 0;
    }

    while(1){
        int ret = led_sensor_set_enabled(driver, false);

    if (ret < 0) {
        LOG_ERR("Failed to disable LED sensor: %d", ret);
        return 0;
    }

    ret = sensor_sample_fetch(driver);

    LOG_INF("Fetch while disabled: %d", ret);

    ret = led_sensor_set_enabled(driver, true);

    if (ret < 0) {
        LOG_ERR("Failed to enable LED sensor: %d", ret);
        return 0;
    }

    ret = sensor_sample_fetch(driver);

    if (ret < 0) {
        LOG_ERR("sensor_sample_fetch failed: %d", ret);
        return 0;
    }

    LOG_INF("Fetch while enabled succeeded");

    k_msleep(1000);

    struct sensor_value val = {};

    ret = sensor_channel_get(
        driver,
        SENSOR_CHAN_AMBIENT_TEMP,
        &val
    );

    if (ret < 0) {
        LOG_ERR("sensor_channel_get failed: %d", ret);
        return 0;
    }

    LOG_INF("sensor_channel_get succeeded");
    k_msleep(1000);
    }
    

    return 0;
}