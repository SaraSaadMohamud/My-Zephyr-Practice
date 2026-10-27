#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/util.h>

#include "led_sensor.h"

/*
 * Get the LED sensor device from the Device Tree.
 */
#define SENSOR_NODE DT_NODELABEL(our_driver0)

static const struct device *sensor_dev = DEVICE_DT_GET(SENSOR_NODE);


/*
 * Fetch sensor data.
 *
 * Expected behavior:
 * - Calls sensor_sample_fetch().
 * - Turns the LED ON through the custom driver.
 */
static int cmd_sensor_fetch(const struct shell *sh,
                            size_t argc,
                            char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(sensor_dev)) {
        shell_error(sh, "Sensor device is not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(sensor_dev);

    if (ret < 0) {
        shell_error(sh, "Sensor fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Sensor sample fetched successfully");
    shell_print(sh, "LED is ON");

    return 0;
}


/*
 * Read the sensor channel.
 *
 * Expected behavior:
 * - Calls sensor_channel_get().
 * - Turns the LED OFF through the custom driver.
 * - Prints the returned sensor value.
 */
static int cmd_sensor_read(const struct shell *sh,
                           size_t argc,
                           char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(sensor_dev)) {
        shell_error(sh, "Sensor device is not ready");
        return -ENODEV;
    }

    struct sensor_value val = {0};

    int ret = sensor_channel_get(
        sensor_dev,
        SENSOR_CHAN_AMBIENT_TEMP,
        &val
    );

    if (ret < 0) {
        shell_error(sh, "Sensor read failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Sensor value: %d.%06d",
                val.val1, val.val2);

    shell_print(sh, "LED is OFF");

    return 0;
}


/*
 * Display sensor device information.
 *
 * Prints:
 * - Device name.
 * - Device readiness.
 */
static int cmd_sensor_info(const struct shell *sh,
                           size_t argc,
                           char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_print(sh, "Device name: %s", sensor_dev->name);

    shell_print(sh, "Device ready: %s",
                device_is_ready(sensor_dev) ? "YES" : "NO");

    return 0;
}


/*
 * Define the sensor subcommands.
 */
SHELL_STATIC_SUBCMD_SET_CREATE(
    sensor_subcmds,

    SHELL_CMD(fetch,
              NULL,
              "Fetch a sensor sample",
              cmd_sensor_fetch),

    SHELL_CMD(read,
              NULL,
              "Read the sensor channel",
              cmd_sensor_read),

    SHELL_CMD(info,
              NULL,
              "Show sensor device information",
              cmd_sensor_info),

    SHELL_SUBCMD_SET_END
);


/*
 * Register the root command: sensor
 */
SHELL_CMD_REGISTER(
    sensor,
    &sensor_subcmds,
    "LED sensor commands",
    NULL
);