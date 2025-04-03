IoT Security Project:
> Focusing on BLE version 5.2 and prior

Master Directory:
- Contains BLE python code for establishing a connection with a slave device
- Runs on Linux

Slave Directory:
- Contains the nRF52840 code for advertising & connecting to a master device (i.e. Linux machine)
- Runs on nRF52840dk
- Side Note: Requires Zephyr RTOS Environment Setup (see slave/README.md)
- Eventually, adding the intrusion detection system

Attacker Directory:
- Contains the Nordic dongle code for the BLE attack
- InjectaBLE Repo: https://github.com/RCayre/injectable-firmware

