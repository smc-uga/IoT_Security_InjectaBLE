#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ble_slave, LOG_LEVEL_INF);
#include <string.h>

#include <zephyr/bluetooth/bluetooth.h> // BLE: Advertising/Connecting with GAP
#include <zephyr/bluetooth/gatt.h> // GATT: Service & Characteristic (LED on/off)
#include <zephyr/bluetooth/hci.h> // Included for the 'bt_hci_err_to_str' function

#include "led_svc.h"

int led = 0;

ssize_t recv(struct bt_conn *conn, const struct bt_gatt_attr *attr,
             const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {

  led_update();

  if (led == 0) {
    LOG_INF("Turn ON led");
    led = 1;
  } else {
    LOG_INF("Turn OFF led");
    led = 0;
  }

  return 0;
}

ssize_t send(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
             uint16_t len, uint16_t offset) {

  LOG_INF("READ");
  led_update();

  if (led == 0) {
    LOG_INF("Turn ON led");
    led = 1;
  } else {
    LOG_INF("Turn OFF led");
    led = 0;
  }

  return 0;
}

// iot_svc: defines the high level service
// led_char: defines a characteristic within that service
static const struct bt_uuid_16 iot_svc_uuid = BT_UUID_INIT_16(0xFFAA);
static const struct bt_uuid_16 led_char_uuid = BT_UUID_INIT_16(0xFFAB);

// iot_svc: Top Level Service that is exposed to Connected Device
// led_char: Field accessible by connected device (part of exposed service)
BT_GATT_SERVICE_DEFINE(
    iot_svc,

    BT_GATT_PRIMARY_SERVICE(&iot_svc_uuid),

    BT_GATT_CHARACTERISTIC(&led_char_uuid.uuid,
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                           BT_GATT_PERM_WRITE | BT_GATT_PERM_READ, send, recv,
                           (void *)1));

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

// Setup connection callbacks
BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected = connected,
    .disconnected = disconnected,
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
