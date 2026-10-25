#ifndef LED_SENSOR_H
#define LED_SENSOR_H

#include <zephyr/device.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enable or disable the LED sensor.
 *
 * @param dev Pointer to the LED sensor device.
 * @param enabled True to enable, false to disable.
 *
 * @return 0 on success, negative error code otherwise.
 */
int led_sensor_set_enabled(const struct device *dev, bool enabled);

#ifdef __cplusplus
}
#endif

#endif /* LED_SENSOR_H */