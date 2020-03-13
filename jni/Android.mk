LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= module_info.cpp

LOCAL_C_INCLUDES += $(ANDROID_SRCDIR)/source/include

LOCAL_LDFLAGS := -L $(ANDROID_SRCDIR)/libs
LOCAL_LDLIBS := -lhardware

LOCAL_MODULE:= module-info

LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS += -O0

include $(BUILD_EXECUTABLE)
