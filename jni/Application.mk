APP_PLATFORM=android-21
#APP_ABI := armeabi
APP_ABI := armeabi-v7a
#arm64-v8a

APP_OPTIM := release

NDK_TOOLCHAIN_VERSION := 4.9
APP_STL := gnustl_static

# For LLVM
#APP_STL := c++_static

#APP_CFLAGS := -std=c++11 -std=gnu++11
APP_CPPFLAGS := -fexceptions -frtti -std=c++11 -std=gnu++11
