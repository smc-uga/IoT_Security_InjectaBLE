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



UPDATE 1:
- The Master & Slave devices are able to communicate
- We need to ensure the correct conditions for the attack (BLE versions 5.2 and prior on both Master & Slave)
    > This might just mean we need to turn off certain configurable features rather than 'downgrade' versions
    > For example, on the slave device we want subrating and sleep clock accuracy updating off (already off by default)
      and maybe need to turn off LE security features like encryption (which I believe is on at the moment.)
    > Also, we need to configure the channel hopping algorithm (either #1 or #2)
    > For the Master... I'm not sure what 'bleak' exposes as configurable. So I don't know what this device is capable of.
- Attempt the attack
    > We need to prove the connection is established between the Master & Slave
    > Prove from the attacking device we can track Master & Slave connection packets & inject ourselves between the connection

UPDATE 2:
- The Master & Slave are both using compatible Bluetooth Versions.
    > Master is (I believe), at the most, using Bluetooth Core Specification v5.2 features.
    > Slave is 100% using Bluetooth Core Specification pre-v5.0 features.
    > PHY 1 is used and no encryption
- Slave device is configured to use Channel Hopping Algorithm #1.
- Our Attack Dongle is correctly tracking the Master & Slave connection.
    > It is following the channel hopping algorithm

    > Issues:

        > The injection doesn't work for unknown reasons.

        > The attack only works on newConnections, not on existingConnections (which was one of the paper's claims).

        > Haven't had time to setup attack mitigation on Slave device yet.

Finally, setup double packet detection in the Slave's link layer.

Update 3:

  Master  - works

  Slave   - works

  Attack  - works
