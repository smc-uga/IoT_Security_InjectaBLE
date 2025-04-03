#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ble_slave, LOG_LEVEL_INF);
#include <string.h>

// GAP (Generic Access Profile)
// Required to setup and manage Bluetooth connections
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/hci.h>

// Possibly needed in the future to facilitate data passing
// #include <zephyr/bluetooth/gatt.h>

/************************************/
/***** Start BLE Advertisement ******/
/************************************/
int start_ble(void) {

  uint8_t uuid[] = {0xFF, 0xBC}; // 16-bit UUID
  const char *scan_name = "Slave";

  const struct bt_data ad[] = {
      BT_DATA(BT_DATA_UUID16_SOME, uuid, sizeof(uuid)),
  };
  const struct bt_data sd[] = {
      BT_DATA(BT_DATA_NAME_COMPLETE, scan_name, strlen(scan_name)),
  };

  // Do BLE Advertisement until Connection is Established, then stop
  int err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd,
                            ARRAY_SIZE(sd));
  if (err) {
    LOG_ERR("Failed to Start Advertising: (err = %d)", err);
    return err;
  }
  LOG_INF("Started Advertising!");
  return 0;
}

/************************************/
/******* CONNECTION callbacks *******/
/************************************/
static void connected(struct bt_conn *conn, uint8_t err) {
  if (err) {
    printk("Connection failed, err 0x%02x %s\n", err, bt_hci_err_to_str(err));
  } else {
    printk("Connected\n");
  }
}

static void disconnected(struct bt_conn *conn, uint8_t reason) {
  printk("Disconnected, reason 0x%02x %s\n", reason, bt_hci_err_to_str(reason));
}

// Setup connection callbacks
BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
};

/************************************/
/******* Main APP Entry Point *******/
/************************************/
int main(void) {

  // BLE init
  int err = bt_enable(NULL);
  if (err) {
    LOG_ERR("Bluetooth Failed to Initialized");
    return err;
  }
  LOG_INF("Bluetooth Initialized");

  // BLE Advertise (stops advertising on connection)
  err = start_ble();
  if (err) {
    LOG_ERR("Failed to Start BLE");
    return err;
  }

  // Main Thread Sleep... no exit
  LOG_INF("Main Sleeping Forever...");
  k_sleep(K_FOREVER);
  return 0;
}
