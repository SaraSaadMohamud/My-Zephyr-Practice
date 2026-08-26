# 💡 Zephyr RTOS — LED Blink Application

A simple **LED Blink application** developed using **Zephyr RTOS** and tested on the **STM32F4-DISCO** development board.

This application demonstrates the basic Zephyr workflow:

**Build → Flash → Run → Verify**

---

##  Objective

The goal of this task is to create a simple Zephyr application that toggles an LED every **1 second**.

The application demonstrates:

* Zephyr GPIO API
* Devicetree-based hardware configuration
* Zephyr logging
* Building an application with `west`
* Flashing firmware to a physical STM32 board

---

##  Hardware & Software

### Hardware

* **STM32F4-DISCO**
* STM32F407 MCU
* On-board user LED
* ST-LINK debugger/programmer

### Software

* Ubuntu Linux
* Zephyr RTOS **4.2.0**
* Zephyr SDK **0.16.8**
* West **1.5.0**
* STM32CubeProgrammer **2.23.0**
* Python Virtual Environment

---

##  Project Structure

```text
zephyr-course/
└── app/
    ├── CMakeLists.txt
    ├── prj.conf
    └── src/
        └── main.cpp
```

---

##  Application Code

The application uses Zephyr's GPIO Devicetree API to access the board's `led0` alias.

```cpp
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led))
        return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0)
        return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0)
            return 0;

        led_state = !led_state;

        LOG_INF("LED state: %s",
                led_state ? "ON" : "OFF");

        k_msleep(SLEEP_TIME_MS);
    }

    return 0;
}
```

---

## How It Works

### 1. Get the LED from Devicetree

```cpp
#define LED_NODE DT_ALIAS(led0)
```

Zephyr uses the board's Devicetree configuration to identify the LED instead of hard-coding a GPIO pin.

The LED specification is then obtained using:

```cpp
static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(LED_NODE, gpios);
```

### 2. Configure the GPIO

```cpp
gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
```

The LED GPIO is configured as an output.

### 3. Toggle the LED

```cpp
gpio_pin_toggle_dt(&led);
```

This changes the LED state from ON to OFF or from OFF to ON.

### 4. Wait for one second

```cpp
k_msleep(1000);
```

The application waits for **1000 ms = 1 second** between toggles.

### 5. Print the LED state

Zephyr's logging system is used to report the current LED state:

```cpp
LOG_INF("LED state: %s",
        led_state ? "ON" : "OFF");
```

---

## ⚙️ Build the Application

Activate the Python virtual environment and navigate to the Zephyr workspace:

```bash
cd ~/my-zephyr-course
```

Build for the STM32F4-DISCO:

```bash
west build -b stm32f4_disco zephyr-course/app
```

For a clean build:

```bash
west build -p always -b stm32f4_disco zephyr-course/app
```

A successful build generates the Zephyr firmware inside:

```text
build/zephyr/
```

---

## Flash the Firmware

Connect the STM32F4-DISCO board through its ST-LINK interface and run:

```bash
west flash
```

The firmware is programmed into the STM32 internal Flash.

A successful flashing operation ends with a message similar to:

```text
Start operation achieved successfully
```

---

##  Verification

After flashing, the onboard LED should toggle every **1 second**:

```text
ON
↓
1 second
↓
OFF
↓
1 second
↓
ON
↓
...
```

This confirms that:

* The firmware was successfully built.
* The firmware was successfully flashed.
* The GPIO was correctly configured.
* The application is running on the STM32F4-DISCO.

---

##  Native Simulation

The application can also be built for Zephyr's native simulator:

```bash
west build -p always -b native_sim zephyr-course/app
```

Unlike the STM32 target, `native_sim` runs as a Linux executable and does **not** require flashing hardware.

Run it with:

```bash
west build -t run
```

The logging output can be observed directly in the terminal.
![NativeSim](/zephyr-course/app/native_sim.png)
---

##  Task Checklist

* [x] Create LED Blink application
* [x] Use Zephyr GPIO API
* [x] Build for STM32F4-DISCO
* [x] Flash firmware to the hardware
* [x] Verify LED toggles every second
* [x] Add logging for LED state
* [x] Create Git tag `l2-task1`
* [x] Push tag to GitHub

---

##  Git Tag

The completed task is marked with:

```text
l2-task1
```

Create the tag:

```bash
git tag l2-task1
```

Push it to the remote repository:

```bash
git push origin l2-task1
```

---

##  Concepts Practiced

This application provides practical experience with:

* **Zephyr RTOS**
* **West**
* **Devicetree**
* **GPIO drivers**
* **Kernel sleep API**
* **Zephyr Logging**
* **C/C++ application development**
* **Native simulation**
* **Firmware flashing**
* **STM32F4-DISCO**
* **Git tags and versioning**

---

## 🎓 Course

**Zephyr RTOS Practice**

**Module 2 — West, West Topologies & Hello World**

### Task 2 — Build and Run an LED Blink Application

**Target Board:** STM32F4-DISCO
**RTOS:** Zephyr
**Status:** ✅ Completed
