LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := addon
LOCAL_MODULE_FILENAME := libaddon

COCOS2DX_PATH := $(LOCAL_PATH)/..


LOCAL_SRC_FILES := \
VVTableSuite/VVBasicCell.cpp \
VVTableSuite/VVScrollView.cpp \
VVTableSuite/VVTableView.cpp \
VVTableSuite/VVTableViewCell.cpp \
VVTableSuite/VVTargetedTableView.cpp \
VVTableSuite/VVTextField.cpp \
\
VVUISuite/VVAlertView.cpp \
VVUISuite/VVAlertViewForInput.cpp \
VVUISuite/VVAlertViewForLogin.cpp \
VVUISuite/VVPopupLayer.cpp \
\
VVUtils/VVActionSet.cpp \
VVUtils/VVLocalizedStringManager.cpp \
VVUtils/VVMultiScreenManager.cpp \
VVUtils/VVLog.cpp \
\
CCMenuItemDynamic/CCMenuItemDynamic.cpp \
CCMenuItemDynamic/CCScale9Sprite.cpp \

LOCAL_C_INCLUDES := \
$(COCOS2DX_PATH)/cocos2dx \
$(COCOS2DX_PATH)/cocos2dx/plugin \
$(COCOS2DX_PATH)/cocos2dx/cocoa \
$(COCOS2DX_PATH)/cocos2dx/platform \
$(COCOS2DX_PATH)/cocos2dx/platform/third_party/android/curl \
$(COCOS2DX_PATH)/cocos2dx/include \
$(COCOS2DX_PATH)/CocosDenshion/include \
\
$(COCOS2DX_PATH)/addon \
$(COCOS2DX_PATH)/addon/CCMenuItemDynamic \
$(COCOS2DX_PATH)/addon/VVTableSuite \
$(COCOS2DX_PATH)/addon/VVUISuite \
$(COCOS2DX_PATH)/addon/VVUtils \ 

LOCAL_LDLIBS := -L$(call host-path, $(LOCAL_PATH)/../../libs/armeabi) \
				-L$(call host-path, $(LOCAL_PATH)/../../libs/armeabi-v7a) \
                -lcocos2d -lcocosdenshion \
                -L$(call host-path, $(COCOS2DX_PATH)/cocos2dx/platform/third_party/android/libraries) -lcurl \
                -lGLESv1_CM -llog -lz \
				
include $(BUILD_SHARED_LIBRARY)