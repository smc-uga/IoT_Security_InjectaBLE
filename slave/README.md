This directory will contain the Slave BLE Device code.

Runs on the nRF52840dk.

BLE Version: v5.2 or earlier

# NOTE TO TEAM:

    Setting up the Zephyr environment is quite tedious so I don't expect y'all to do this.
    All you need to do is read & understand the *src/main.c* file.

------

# Requires: Zephyr RTOS Environment setup

> Instructions Here: https://docs.zephyrproject.org/latest/develop/getting_started/index.html

    Insert in ~/.bashrc: export ZEPHYR_BASE="<path_to_zephyrproject>"
    Example: export ZEPHYR_BASE="$HOME/school/Spring_2025/IoT_Security/research/zephyr_prj"

------

# Goals:
### 1) Setup Device as our Slave in a BLE Connection

  > GAP (Generic Access Profile)

    a. Advertise Presence
    b. Establish Connection with Master

  > GATT (Generic Attribute Profile)

    a. Enable LED (on/off) functionality

### 2) Eventually, attack will target this slave device.

### 3) Setup Link Layer Intrusion Detection System

