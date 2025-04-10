#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ble_slave, LOG_LEVEL_INF);
#include <string.h>

#include <zephyr/bluetooth/bluetooth.h> // BLE: Advertising/Connecting with GAP
#include <zephyr/bluetooth/gatt.h> // GATT: Service & Characteristic (LED on/off)
#include <zephyr/bluetooth/hci.h> // Included for the 'bt_hci_err_to_str' function

#include "led_svc.h"

ssize_t recv(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {

  if (len <= 0) {
    LOG_ERR("GATT SERVER FAILED TO RECEIVE ANY BYTES FROM CLIENT");
    return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
  }

  uint8_t on_off = ((uint8_t *)buf)[0];
  led_update((int)on_off);
  return BT_GATT_ERR(BT_ATT_ERR_SUCCESS);
}

// iot_svc: defines the high level service
// led_char: defines a characteristic within that service
static const struct bt_uuid_16 iot_svc_uuid = BT_UUID_INIT_16(0xFFAA);
static const struct bt_uuid_16 led_char_uuid = BT_UUID_INIT_16(0xFFAB);

// iot_svc: Top Level Service that is exposed to Connected Device
// led_char: Field accessible by connected device (part of exposed service)
BT_GATT_SERVICE_DEFINE(iot_svc,

                       BT_GATT_PRIMARY_SERVICE(&iot_svc_uuid),

                       BT_GATT_CHARACTERISTIC(&led_char_uuid.uuid, 0,
                                              BT_GATT_PERM_WRITE, NULL, recv,
                                              (void *)1));

/************************************/
/***** Start BLE Advertisement ******/
/************************************/
int advertise(void) {

  uint8_t uuid[] = {0xFF, 0xBC}; // 16-bit UUID
  const char *scan_name = "Slave";

  const struct bt_data ad[] = {
      BT_DATA(BT_DATA_UUID16_SOME, uuid, sizeof(uuid)),
  };
  const struct bt_data sd[] = {
      BT_DATA(BT_DATA_NAME_COMPLETE, scan_name, strlen(scan_name)),
  };

  // Do BLE Advertisement until Connection is Established, then stop
  return bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd,
                         ARRAY_SIZE(sd));
}

int start_ble(void) {
  int err = advertise();
  if (err) {
    LOG_ERR("Failed to Start Advertising: (err = %d)", err);
  } else {
    LOG_INF("Started Advertising!");
  }
  return err;
}

/************************************/
/******* CONNECTION callbacks *******/
/************************************/
struct bt_conn *ble_conn;

static void connected(struct bt_conn *conn, uint8_t err) {

  if (err) {
    LOG_ERR("Connection failed, err 0x%02x %s\n", err, bt_hci_err_to_str(err));
  } else {
    LOG_INF("Connected");
    if (!ble_conn) {
      ble_conn = bt_conn_ref(conn);
    }
  }
}

static void disconnected(struct bt_conn *conn, uint8_t reason) {
  if (ble_conn) {
    bt_conn_unref(ble_conn);
    ble_conn = NULL;
  }

  LOG_INF("Disconnected, reason %u %s", reason, bt_hci_err_to_str(reason));
}

// Catch the disconnection (specifically, the newly free'd connection object)
static void restart_adv(void) {
  // Restart advertising
  int err = advertise();
  if (err) {
    LOG_ERR("Failed to restart advertising: err = %d", err);
  } else {
    LOG_INF("Re-advertising after disconnect");
  }
}

// Setup connection callbacks
BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
    .recycled = restart_adv,
};

/************************************/
/******* Main APP Entry Point *******/
/************************************/
int main(void) {

  // Initialize LED
  int err = led_init();
  if (err) {
    LOG_ERR("LED Failed to Initialized");
    return err;
  }

  // BLE init
  err = bt_enable(NULL);
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
