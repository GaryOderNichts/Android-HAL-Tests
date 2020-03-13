# Android-HAL-Tests
This is a project I made when I was bored. It accesses the Android HAL (Hardware Abstraction Layer) to communcate with the Hardware

## Run
To run one of the tests copy it to your phone and open your favourite terminal emulator. 
Then run `su` to get root access and `cd` into the folder with the executable. 
Run `chmod +x ./executable` to make it executable. Then you can use `./executable` to run the executable.
If a module does not exist on your device it may not be using aosp hals and is instead using vendor hals
Note: The executable may not be located under `/sdcard` or permission problems may occur
Tested on: NVIDIA Shield (bluetooth-test, module-info, local-time), Xiaomi Mi 9 (module-info, local-time)

## Build
In the `Android.mk` under `LOCAL_SRC_FILES` choose any source file you want to compile and change `LOCAL_MODULE` to change the module name.
Adjust the path in `build_config.local` to match a path on your system.
The `Android.mk` expects the following folder structure:
```
ANDROID_SRCDIR/
├── source/
│   └── include/
│       ├── cutils/
│       │   └── ... // place aosp cutils headers here
│       ├── hardware/
│       │   └── ... // place aosp libhardware headers here
│       │           // Note: Use the source from the latest android-8.0.0 tag (android-8.0.0_r43)
│       │           //       newer versions won't compile
│       ├── system/
│       │   └── ... // place aosp system headers here
└── libs/
    └── libhardware.so // Build using aosp or dump from your device
```
Then run `./build.sh`.
I compile using `Android-NDK 12.1` newer versions may not work
