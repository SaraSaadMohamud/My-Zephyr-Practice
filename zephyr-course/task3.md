# Task 3 — Zephyr Kconfig Configuration

## Overview

This task demonstrates how to use advanced **Zephyr Kconfig** features to create a structured and user-friendly configuration system for an LED subsystem.

The configuration provides:

* An LED subsystem enable/disable option.
* Multiple predefined LED blink speeds using `choice`.
* A hidden derived value for the selected blink interval.
* Advanced LED settings.
* Numeric value validation using `range`.
* Conditional configuration using `if`.
* Nested menus using `menu`.
* Expert LED settings for debugging and custom blink patterns.

---

## Learning Objectives

By completing this task, you will learn how to:

1. Create a feature using `menuconfig`.
2. Create mutually exclusive options using `choice`.
3. Generate a derived configuration value from a `choice`.
4. Use hidden Kconfig symbols.
5. Validate numeric values using `range`.
6. Apply dependencies using `if`.
7. Group related options using `menu`.
8. Use Kconfig values inside C/C++ code.
9. Configure the application through `menuconfig`.

---

# 1. LED Subsystem

The main configuration entry is created using `menuconfig`:

```kconfig
menuconfig LED_SUBSYSTEM
    bool "LED Subsystem"
    default y
```

`menuconfig` creates:

```text
CONFIG_LED_SUBSYSTEM
```

and provides both:

* A Boolean enable/disable option.
* A submenu for LED-related settings.

In `menuconfig`:

```text
[*] LED Subsystem  --->
```

When disabled:

```text
[ ] LED Subsystem  --->
```

The LED configuration options inside the subsystem become unavailable.

---

# 2. LED Blink Speed Using `choice`

The blink speed is configured using Kconfig's `choice` construct:

```kconfig
choice
    prompt "LED blink sleep time (1s (medium))"
    default LED_SLEEP_1000_MS

config LED_SLEEP_250_MS
    bool "Fast (250ms)"

config LED_SLEEP_1000_MS
    bool "Medium (1000ms)"

config LED_SLEEP_2000_MS
    bool "Slow (2000ms)"

endchoice
```

The user can select exactly one option:

```text
( ) Fast (250ms)
(*) Medium (1000ms)
( ) Slow (2000ms)
```

The default option is:

```kconfig
default LED_SLEEP_1000_MS
```

Therefore, the default blink interval is **1000 ms**.

---

# 3. Hidden Derived Value

The application needs a numeric value instead of the selected Boolean symbol.

A hidden configuration symbol is therefore used:

```kconfig
config LED_BLINK_SLEEP_TIME_MS
    int
    default 250 if LED_SLEEP_250_MS
    default 1000 if LED_SLEEP_1000_MS
    default 2000 if LED_SLEEP_2000_MS
    default 0
```

This symbol has no prompt, so it does not appear as a separate configuration option.

Its value is derived from the selected blink speed.

### Mapping

| Selected option | Derived value |
| --------------- | ------------: |
| Fast            |        250 ms |
| Medium          |       1000 ms |
| Slow            |       2000 ms |

For example, selecting:

```text
Medium (1000ms)
```

results in:

```text
CONFIG_LED_BLINK_SLEEP_TIME_MS=1000
```

---

# 4. Using the Configuration in C++

The application can access the generated configuration value using the automatically added `CONFIG_` prefix:

```cpp
CONFIG_LED_BLINK_SLEEP_TIME_MS
```

For example:

```cpp
k_msleep(CONFIG_LED_BLINK_SLEEP_TIME_MS);
```

The application does not need to know which option the user selected.

Kconfig handles the conversion from the selected choice to the final numeric value.

---

# 5. Advanced LED Settings

The advanced configuration is implemented using another `menuconfig`:

```kconfig
menuconfig ADVANCED_LED_SETTINGS
    bool "Advanced LED Settings"
```

This creates:

```text
CONFIG_ADVANCED_LED_SETTINGS
```

and provides a submenu:

```text
[ ] Advanced LED Settings  --->
```

The advanced settings are disabled by default.

---

# 6. Conditional Configuration Using `if`

The advanced configuration options are placed inside:

```kconfig
if ADVANCED_LED_SETTINGS

    ...

endif
```

This means the symbols inside the block are only available when:

```text
CONFIG_ADVANCED_LED_SETTINGS=y
```

Example:

```kconfig
if ADVANCED_LED_SETTINGS

config LED_BRIGHTNESS
    int "(100) LED brightness (0-100)"
    range 0 100
    default 100

config LED_FADE_DURATION_MS
    int "(500) LED fade duration (ms)"
    range 0 5000
    default 500

endif
```

The `if` block is useful when multiple configuration symbols share the same dependency.

---

# 7. LED Brightness

The LED brightness is configured using:

```kconfig
config LED_BRIGHTNESS
    int "(100) LED brightness (0-100)"
    range 0 100
    default 100
```

### Configuration

```text
Default: 100
Minimum: 0
Maximum: 100
```

The `range` keyword prevents invalid values outside the allowed range.

For example:

```text
75
```

is valid.

While:

```text
150
```

is outside the allowed range.

---

# 8. LED Fade Duration

The fade duration is configured using:

```kconfig
config LED_FADE_DURATION_MS
    int "(500) LED fade duration (ms)"
    range 0 5000
    default 500
```

### Configuration

```text
Default: 500 ms
Minimum: 0 ms
Maximum: 5000 ms
```

This allows the application to configure how long an LED fade operation should take.

---

# 9. Expert Settings Menu

Expert options are grouped inside a submenu:

```kconfig
menu "Expert settings"

config LED_DEBUG
    bool "Enable LED debugging"

config LED_CUSTOM_BLINK_PATTERN
    bool "Custom blink pattern"

endmenu
```

The resulting menu is:

```text
Expert settings  --->
```

Inside it:

```text
[ ] Enable LED debugging
[ ] Custom blink pattern
```

The `menu` construct is purely organizational. It does not create a configuration symbol itself.

---

# 10. `menu` vs `menuconfig`

These two constructs have different purposes.

### `menu`

```kconfig
menu "Expert settings"
    ...
endmenu
```

Used to group configuration options visually.

```text
Expert settings  --->
```

It does **not** create an enable/disable symbol.

### `menuconfig`

```kconfig
menuconfig LED_SUBSYSTEM
    bool "LED Subsystem"
```

Creates a Boolean configuration symbol and a submenu:

```text
[*] LED Subsystem  --->
```

Therefore:

```text
menu       → Folder
menuconfig → Folder + On/Off switch
```

---

# 11. Final Kconfig Structure

The complete configuration structure is:

```kconfig
menuconfig LED_SUBSYSTEM
    bool "LED Subsystem"
    default y

if LED_SUBSYSTEM

choice
    prompt "LED blink sleep time (1s (medium))"
    default LED_SLEEP_1000_MS

config LED_SLEEP_250_MS
    bool "Fast (250ms)"

config LED_SLEEP_1000_MS
    bool "Medium (1000ms)"

config LED_SLEEP_2000_MS
    bool "Slow (2000ms)"

endchoice

config LED_BLINK_SLEEP_TIME_MS
    int
    default 250 if LED_SLEEP_250_MS
    default 1000 if LED_SLEEP_1000_MS
    default 2000 if LED_SLEEP_2000_MS
    default 0

menuconfig ADVANCED_LED_SETTINGS
    bool "Advanced LED Settings"

if ADVANCED_LED_SETTINGS

config LED_BRIGHTNESS
    int "(100) LED brightness (0-100)"
    range 0 100
    default 100

config LED_FADE_DURATION_MS
    int "(500) LED fade duration (ms)"
    range 0 5000
    default 500

menu "Expert settings"

config LED_DEBUG
    bool "Enable LED debugging"

config LED_CUSTOM_BLINK_PATTERN
    bool "Custom blink pattern"

endmenu

endif # ADVANCED_LED_SETTINGS

endif # LED_SUBSYSTEM

source "Kconfig.zephyr"
```

---

# 12. Expected `menuconfig` Hierarchy

The final structure is:

```text
[*] LED Subsystem  --->
    LED blink sleep time (1s (medium))  --->
    [ ] Advanced LED Settings  --->
```

Inside **Advanced LED Settings**:

```text
[*] Advanced LED Settings  --->
    (100) LED brightness (0-100)
    (500) LED fade duration (ms)
    Expert settings  --->
```

Inside **Expert settings**:

```text
[ ] Enable LED debugging
[ ] Custom blink pattern
```

---

# 13. Building and Testing

Activate the Zephyr virtual environment:

```bash
source ~/zephyrproject/.venv/bin/activate
```

Build the application:

```bash
west build
```

Open the interactive Kconfig interface:

```bash
west build -t menuconfig
```

Search for configuration symbols using:

```text
/
```

Save the configuration using:

```text
S
```

Exit using:

```text
Q
```

---

# 14. Configuration Files

The project uses:

```text
Kconfig
```

to define configuration options.

The project configuration file:

```text
prj.conf
```

can be used for directly configurable symbols.

The hidden derived symbol:

```text
CONFIG_LED_BLINK_SLEEP_TIME_MS
```

should **not** be manually assigned in `prj.conf` because its value is derived automatically from the `choice`.

---

# 15. Key Concepts

| Kconfig Feature | Purpose                                  |
| --------------- | ---------------------------------------- |
| `menuconfig`    | Feature switch + submenu                 |
| `choice`        | Select one option from alternatives      |
| `config`        | Define a configuration symbol            |
| `int`           | Numeric configuration value              |
| `bool`          | Boolean configuration value              |
| `range`         | Validate numeric values                  |
| `default`       | Set the default value                    |
| `if`            | Apply a dependency to a group of symbols |
| `menu`          | Group options visually                   |
| Hidden symbol   | Internal/derived configuration value     |

---

# Summary

This task demonstrates how Kconfig can provide a clean configuration interface while keeping application code simple.

The user selects a high-level option:

```text
Fast / Medium / Slow
```

Kconfig converts that selection into a numeric value:

```text
250 / 1000 / 2000 ms
```

The application then uses:

```cpp
CONFIG_LED_BLINK_SLEEP_TIME_MS
```

without needing to handle the individual configuration choices itself.

This approach provides **structured configuration, validation, dependencies, reusable settings, and a user-friendly `menuconfig` interface**.
