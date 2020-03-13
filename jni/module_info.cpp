#include <pthread.h>
#include <signal.h>
#include <hardware/hardware.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int err;
  hw_module_t* module;

  if (argc < 2)
  {
    printf("not enough arguments\n");
    return -1;
  }

  if (argc > 3)
  {
    printf("too many arguments\n");
    return -1;
  }
  
  if (argc == 3)
  {
    err = hw_get_module_by_class(argv[1], argv[2], (hw_module_t const**)&module);
  }
  else
  {
    err = hw_get_module(argv[1], (hw_module_t const**)&module);
  }
  
  if (!err)
  {
    printf("SUCESS! Getting Module '%s' was successful.\n", module->name);
    printf("Displaying Module Information:\n");
    printf("tag: %i\n", module->tag);
    printf("module_api_version: %i\n", module->module_api_version);
    printf("hal_api_version: %i\n", module->hal_api_version);
    printf("id: %s\n", module->id);
    printf("name: %s\n", module->name);
    printf("author: %s\n", module->author);
  }
  else
  {
    printf("ERROR: %i ", err);
    if (err == -2)
    {
      printf("module not found");
    }
    printf("\n");
    
  }

  return 0;
}
