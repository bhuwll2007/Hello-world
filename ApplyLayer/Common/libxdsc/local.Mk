LOCAL_PATH := $(call local-dir)

ifneq ($(call is_module_prbt,libxdsc),yes)

include $(CLEAR_MFILE)

LOCAL_MODULE := libxdsc

LOCAL_SRC_FILES := $(shell find $(LOCAL_PATH)/ -name "*.c" 2>/dev/null)
LOCAL_SRC_FILES := $(foreach file, $(LOCAL_SRC_FILES), $(call relative_path, $(LOCAL_PATH), $(file)))

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)

LOCAL_CFLAGS := -std=c99 -Wall -fmessage-length=0
LOCAL_CPPFLAGS := -std=c++11 -Wall -fmessage-length=0

ifeq ($(strip $(MY_DEBUG)), yes)
LOCAL_CFLAGS += -O0 -g3
LOCAL_CPPFLAGS += -O0 -g3
else
LOCAL_CFLAGS += -O3
LOCAL_CPPFLAGS += -O3
endif
    
include $(SHARED_LIB_MFILE)

LOCAL_DEPEND_MODULES := libxdsc
include $(BUILDING2PREBUILT_MFILE)

else

include $(CLEAR_MFILE)
LOCAL_MODULE := libxdsc
LOCAL_SRC_FILES := libxdsc.so
LOCAL_MODULE_PATH := $(TARGET_OUT_SYSTEM_DIR)/lib
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)
include $(PREBUILT_MFILE)

endif
