LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cocos2d
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libcocos2d.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
						$(LOCAL_PATH)/plugin \
						$(LOCAL_PATH)/include \
						$(LOCAL_PATH)/platform \
						$(LOCAL_PATH)/platform/third_party/android/curl \
						$(LOCAL_PATH)/platform/third_party/android/iconv \
						$(LOCAL_PATH)/platform/third_party/android/libpng \
						$(LOCAL_PATH)/platform/third_party/android/libxml2 \
						$(LOCAL_PATH)/platform/third_party/android/libjpeg

include $(PREBUILT_SHARED_LIBRARY)