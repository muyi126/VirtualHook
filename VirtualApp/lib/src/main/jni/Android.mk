LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := va++

LOCAL_CFLAGS := -Wno-error=format-security -fpermissive -DLOG_TAG=\"VA++\"
LOCAL_CFLAGS += -fno-rtti -fno-exceptions
APP_ALLOW_MISSING_DEPS=true

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)/Foundation
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)/Jni

LOCAL_SRC_FILES := Jni/Vhook.cpp \
                   Jni/VAJni.cpp \
				   Foundation/IOUniformer.cpp \
				   Foundation/VMPatch.cpp \
				   Foundation/SymbolFinder.cpp \
				   Foundation/Path.cpp \
				   Foundation/SandboxFs.cpp \
				   Substrate/hde64.c \
                   Substrate/SubstrateDebug.cpp \
                   Substrate/SubstrateHook.cpp \
                   Substrate/SubstratePosixMemory.cpp \
                   Jni/SGMFileEncrypt.c \
                   Jni/SGMHelperC.c \


LOCAL_LDLIBS := -llog -latomic
LOCAL_STATIC_LIBRARIES := fb
LOCAL_SHARED_LIBRARIES := xhook
include $(BUILD_SHARED_LIBRARY)
include $(MAIN_LOCAL_PATH)/fb/Android.mk
include $(MAIN_LOCAL_PATH)/xhook/Android.mk
