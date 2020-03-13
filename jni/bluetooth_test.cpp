#include <pthread.h>
#include <signal.h>
#include <hardware/hardware.h>
#include <hardware/bluetooth.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static bluetooth_device_t* bt_device;
const bt_interface_t* bt_interface = NULL;
bool bt_device_on = false;

int loadHal()
{
  int err = 0;
  hw_module_t* module;
  hw_device_t* device;
  
  err = hw_get_module(BT_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
  if (!err)
  {
    printf("Getting Module '%s' was successful.\n", module->name);
    err = module->methods->open(module, BT_HARDWARE_MODULE_ID, &device);
    if (!err)
    {
      printf("Opening device was successful.\n");
      bt_device = (bluetooth_device_t*)device;
      bt_interface = bt_device->get_bluetooth_interface();

    }
    else
    {
      printf("ERROR: cannot open bluetooth device (%i)\n", err);
    }
  }
  else
  {
    printf("ERROR: cannot open bluetooth module (%i)\n", err);
  }

  return err;
}

static void dut_mode_recv(uint16_t opcode, uint8_t *buf, uint8_t len) {
  printf("Dut rec: %s", buf);
}

static void adapter_properties(bt_status_t status, int num_properties, bt_property_t *properties) {
  // try to convert prop to string
  for (size_t i = 0; i < num_properties; i++)
  {
      bt_property_t prop = (bt_property_t)properties[i];
      if (prop.type == BT_PROPERTY_BDNAME)
      {
        const bt_bdname_t* name = (const bt_bdname_t*) prop.val;
        printf("Name: %s\n", name->name);
      }
  }
}

static void adapter_state_changed(bt_state_t state)
{
  bt_device_on = state == BT_STATE_ON ? true : false;
}

static bt_callbacks_t bt_callbacks = 
{
    sizeof(bt_callbacks_t),
    adapter_state_changed, /*adapter_state_changed */
    adapter_properties, /*adapter_properties_cb */
    NULL, /* remote_device_properties_cb */
    NULL, /* device_found_cb */
    NULL, /* discovery_state_changed_cb */
    NULL, /* pin_request_cb  */
    NULL, /* ssp_request_cb  */
    NULL, /*bond_state_changed_cb */
    NULL, /* acl_state_changed_cb */
    NULL, /* thread_evt_cb */
    dut_mode_recv, /*dut_mode_recv_cb */
//  NULL, /*authorize_request_cb */
#if BLE_INCLUDED == TRUE
    NULL /* le_test_mode_cb */
#else
    NULL
#endif
};

void check_return_status(bt_status_t status)
{
    if (status != BT_STATUS_SUCCESS)
    {
        printf("ERROR: HAL request failed (%d)\n", status);
        exit(-1);
    }
    else
    {
        printf("HAL request success\n");
    }
}

void initBTInterface() 
{
  int err;
  printf("Initializing BT Interface...\n");
  err = bt_interface->init(&bt_callbacks);
  if (err)
  {
    printf("ERROR: BT Interface init failed (%i)\n", err);
    exit(err);
  }
}

static bt_property_t* property_new_(void* val, size_t len, bt_property_type_t type) 
{
  bt_property_t* property = (bt_property_t*) malloc(sizeof(bt_property_t));
  property->val = malloc(len + 1);
  if (type == BT_PROPERTY_BDNAME) 
  {
    strlcpy((char*)property->val, (const char*)val, len);
  } 
  else 
  {
    memcpy(property->val, val, len);
  }
  property->type = type;
  property->len = len;
  return property;
}

bt_property_t* property_new_name(const char* name) 
{
  return property_new_((void*)name, sizeof(bt_bdname_t), BT_PROPERTY_BDNAME);
}

bt_property_t* property_new_scan_mode(bt_scan_mode_t scan_mode) 
{
  return property_new_((void*)&scan_mode, sizeof(bt_scan_mode_t), BT_PROPERTY_ADAPTER_SCAN_MODE);
}

int main(int argc, char *argv[])
{
  int err = loadHal();
  if (err)
  {
    exit(err);
  }

  initBTInterface();

  bt_interface->enable(false);
  while (!bt_device_on);
  
  //bt_property_t* prop = property_new_name("NVIDIA Shield");
  //bt_interface->set_adapter_property(prop);
  //prop = property_new_scan_mode(BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
  //bt_interface->set_adapter_property(prop);

  bt_interface->dut_mode_configure(1);
  uint8_t* ptr = (uint8_t*) "1";

  while (1)
  {
    int testtest = bt_interface->dut_mode_send(0x0009, ptr, sizeof(ptr));
    printf("%i\n", testtest);
    sleep(2);
  }

  while(1);
  return 0;
}