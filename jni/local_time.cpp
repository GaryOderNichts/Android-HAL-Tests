#include <pthread.h>
#include <signal.h>
#include <hardware/hardware.h>
#include <hardware/local_time_hal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int err;
  hw_module_t* module;
  hw_device_t* device;
  
  err = hw_get_module(LOCAL_TIME_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
  if (!err)
  {
    printf("Getting Module '%s' was sucessful.\n", module->name);
    err = module->methods->open(module, LOCAL_TIME_HARDWARE_INTERFACE, &device);
    if (!err)
    {
        printf("SUCCESS: device with tag '%i' found\n", device->tag);
        local_time_hw_device* localtime_device = (local_time_hw_device*) device;
        printf("Current local time is: %lli\n", localtime_device->get_local_time(localtime_device));
    }
    else
    {
        printf("ERROR: %i ", err);
        if (err == -19)
        {
            printf("device not found\n");
        }
        else
        {
            printf("\n");
        }
    }
    
  }
  else
  {
    printf("ERROR: %i ", err);
    if (err == -2)
    {
      printf("module not found\n");
    }
    else
    {
        printf("\n");
    }
  }
  

  return 0;
}
