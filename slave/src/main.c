#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ble_slave, LOG_LEVEL_INF);
#include <string.h>

// GAP (Generic Access Profile)
// Required to setup and manage Bluetooth connections
#include <zephyr/bluetooth/bluetooth.h>

// #include <zephyr/bluetooth/hci.h>
// #include <zephyr/bluetooth/conn.h>
// #include <zephyr/bluetooth/uuid.h>

// Possibly needed in the future to facilitate data passing
// #include <zephyr/bluetooth/gatt.h>

void bt_enable_cb(int err) {
  if (err == 0) {
    LOG_INF("Bluetooth Enabled");
  } else {
    LOG_ERR("Bluetooth Not Enabled");
  }
}

int init_ble(void) {
  LOG_INF("Initializing BLE");
  bt_ready_cb_t cb = bt_enable_cb;
  return bt_enable(cb);
}

int start_ble_adv(int duration_s) {

  uint8_t uuid[] = {0xFF, 0xBC}; // 16-bit UUID
  const char *device_name = "Sean's Device";

  const struct bt_data ad[] = {
      BT_DATA(BT_DATA_UUID16_SOME, uuid, sizeof(uuid)),
  };
  const struct bt_data sd[] = {
      BT_DATA(BT_DATA_NAME_COMPLETE, device_name, strlen(device_name)),
  };

  // Start Bluetooth Low Energy Advertisement
  int err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd,
                            ARRAY_SIZE(sd));
  if (err) {
    LOG_ERR("Failed to Start Advertising: (err = %d)", err);
    return err;
  }
  LOG_INF("Started Advertising!");
  return 0;
}

int main(void) {
  int err = init_ble();
  if (err) {
    LOG_ERR("Bluetooth Failed to Initialized");
    return err;
  }
  LOG_INF("Ready to Start Bluetooth");

  err = start_ble_adv(30);

  // How do I set the advertisment duration?
  //	Also, it would be solid to be able to re-advertise.
  // How do I connect two devices?
  // What threads are running?
  //	main
  //	ble host? (idk)
  //	ble controller? (idk)
  //
  // How do I tell main to sleep until woken up on trigger?
  //	Probably wakeup when device is found & trying to connect
  //	State Machine:
  //		STATE_SLEEP // wakeup on trigger
  //		STATE_WAKE  // handle device connection stuff

  LOG_INF("Main Sleeping...");
  k_sleep(K_SECONDS(30));
  LOG_INF("Main Exiting...");

  return 0;
}
