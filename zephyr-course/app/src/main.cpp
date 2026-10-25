#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define SENSOR_NODE DT_NODELABEL(our_driver0)

int main()
{
    const struct device *driver = DEVICE_DT_GET(SENSOR_NODE);

    if (!device_is_ready(driver)) {
        LOG_ERR("LED sensor driver is not ready");
        return 0;
    }

    LOG_INF("LED sensor driver is ready");

    while (true) {
        /* Turn the LED ON */
        int ret = sensor_sample_fetch(driver);

        if (ret < 0) {
            LOG_ERR("sensor_sample_fetch failed: %d", ret);
            return 0;
        }

        LOG_INF("sensor_sample_fetch: LED should be ON");

        k_msleep(2000);

        /* Turn the LED OFF */
        struct sensor_value val = {};

        ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);

        if (ret < 0) {
            LOG_ERR("sensor_channel_get failed: %d", ret);
            return 0;
        }

        LOG_INF("sensor_channel_get: LED should be OFF");

        k_msleep(2000);
    }

    return 0;
}