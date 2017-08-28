# 
# Copyright 2006 The Android Open Source Project
#
# Android Asset Packaging Tool
#

# This tool is prebuilt if we're doing an app-only build.
ifeq ($(TARGET_BUILD_APPS),)

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES += expat
LOCAL_C_INCLUDES += libpng
LOCAL_C_INCLUDES += zlib
LOCAL_CFLAGS += -Wno-format-y2k -DHAVE_OFF64_T -DHAVE_ENDIAN_H -DHAVE_PTHREADS -DHAVE_MEMMOVE
LOCAL_CFLAGS += -DNDEBUG -DHAVE_EXPAT_CONFIG_H -O3

LOCAL_MODULE := libexpat
LOCAL_ARM_MODE := arm
LOCAL_SRC_FILES :=\
	./expat/xmlparse.c \
	./expat/xmlrole.c	\
	./expat/xmltok_ns.c	\
	./expat/xmltok.c	\
	./expat/xmltok_impl.c
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES += expat
LOCAL_C_INCLUDES += libpng
LOCAL_C_INCLUDES += zlib
LOCAL_CFLAGS += -Wno-format-y2k -DHAVE_OFF64_T -DHAVE_ENDIAN_H -DHAVE_PTHREADS -DHAVE_MEMMOVE
LOCAL_CFLAGS += -O3
LOCAL_CFLAGS += -DPNG_ARM_NEON_OPT=2

LOCAL_MODULE := libpng
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
LOCAL_SRC_FILES :=\
	./libpng/png.c \
	./libpng/pngerror.c \
	./libpng/pngget.c \
	./libpng/pngmem.c \
	./libpng/pngpread.c \
	./libpng/pngread.c \
	./libpng/pngrio.c \
	./libpng/pngrtran.c \
	./libpng/pngrutil.c \
	./libpng/pngset.c \
	./libpng/pngtest.c \
	./libpng/pngtrans.c \
	./libpng/pngwio.c \
	./libpng/pngwrite.c \
	./libpng/pngwtran.c \
	./libpng/pngwutil.c \
	./libpng/arm/arm_init.c\
	./libpng/arm/filter_neon.S \
	./libpng/arm/filter_neon_intrinsics.c
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES += expat
LOCAL_C_INCLUDES += libpng
LOCAL_C_INCLUDES += zlib
LOCAL_CFLAGS += -Wno-format-y2k -DHAVE_OFF64_T -DHAVE_ENDIAN_H -DHAVE_PTHREADS -DHAVE_POSIX_FILEMAP
LOCAL_CFLAGS += -O3

LOCAL_SRC_FILES := \
	AaptAssets.cpp \
	AaptConfig.cpp \
	AaptUtil.cpp \
	AaptXml.cpp \
	ApkBuilder.cpp \
	Command.cpp \
	CrunchCache.cpp \
	FileFinder.cpp \
	Main.cpp \
	Package.cpp \
	StringPool.cpp \
	XMLNode.cpp \
	WorkQueue.cpp \
	ResourceFilter.cpp \
	ResourceTable.cpp \
	Images.cpp \
	Resource.cpp \
	ResourceIdCache.cpp \
	SourcePos.cpp \
	ZipEntry.cpp \
	ZipFile.cpp \
	pseudolocalize.cpp \
	./utils-cpp/Asset.cpp \
	./utils-cpp/AssetDir.cpp \
	./utils-cpp/AssetManager.cpp \
	./utils-cpp/BlobCache.cpp \
	./utils-cpp/BufferedTextOutput.cpp \
	./utils-cpp/CallStack.cpp \
	./utils-cpp/Debug.cpp \
	./utils-cpp/FileMap.cpp \
	./utils-cpp/LinearTransform.cpp \
	./utils-cpp/LocaleData.cpp \
	./utils-cpp/ObbFile.cpp \
	./utils-cpp/PropertyMap.cpp \
	./utils-cpp/RefBase.cpp \
	./utils-cpp/ResourceTypes.cpp \
	./utils-cpp/SharedBuffer.cpp \
	./utils-cpp/Static.cpp \
	./utils-cpp/StopWatch.cpp \
	./utils-cpp/StreamingZipInflater.cpp \
	./utils-cpp/String8.cpp \
	./utils-cpp/String16.cpp \
	./utils-cpp/SystemClock.cpp \
	./utils-cpp/Threads.cpp \
	./utils-cpp/Timers.cpp \
	./utils-cpp/Tokenizer.cpp \
	./utils-cpp/TypeWrappers.cpp \
	./utils-cpp/Unicode.cpp \
	./utils-cpp/VectorImpl.cpp \
	./utils-cpp/ZipFileRO.cpp \
	./utils-cpp/ZipUtils.cpp \
	./utils-cpp/misc.cpp \
	./cutils-c/trace-host.c \
	./cutils-c/sched_policy.c \
	./android-base/file.cpp \
	./ziparchive/zip_archive.cc

LOCAL_STATIC_LIBRARIES := \
	libexpat \
	libpng

LOCAL_LDFLAGS += -pthread

# Statically link libz for MinGW (Win SDK under Linux),
# and dynamically link for all others.
ifneq ($(strip $(USE_MINGW)),)
  LOCAL_STATIC_LIBRARIES += libz
else
  LOCAL_LDLIBS += -lz
endif

LOCAL_LDLIBS += -llog


LOCAL_MODULE := aapt
LOCAL_ARM_MODE := arm

include $(BUILD_EXECUTABLE)

endif # TARGET_BUILD_APPS
