# TARGET #

TARGET := 3DS
LIBRARY := 0

ifeq ($(TARGET),3DS)
    ifeq ($(strip $(DEVKITPRO)),)
        $(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>devkitPro")
    endif

    ifeq ($(strip $(DEVKITARM)),)
        $(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
    endif
endif

# PORTLIBS #

export PORTLIBS_PATH	:=	$(DEVKITPRO)/portlibs
PORTLIBS	:=	$(PORTLIBS_PATH)/armv6k $(PORTLIBS_PATH)/3ds

# COMMON CONFIGURATION #

NAME := 3DS eBook Reader

BUILD_DIR := build
OUTPUT_DIR := output
INCLUDE_DIRS := include
SOURCE_DIRS := source

EXTRA_OUTPUT_FILES :=

LIBRARY_DIRS := $(DEVKITPRO)/libctru $(PORTLIBS)

LIBRARIES := sftd sfil freetype jpeg turbojpeg png png16 sf2d ctru m z

BUILD_FLAGS := 
RUN_FLAGS :=

# 3DS CONFIGURATION #

TITLE := $(NAME)
DESCRIPTION := Read eBooks on your 3DS!
AUTHOR := reworks & AlbertoSONIC
PRODUCT_CODE := CTR-H-EBOOK
UNIQUE_ID := 0xEB00K

SYSTEM_MODE := 64MB
SYSTEM_MODE_EXT := Legacy

ICON_FLAGS :=

ROMFS_DIR := romfs
BANNER_AUDIO := audio.wav
BANNER_IMAGE := banner.cgfx
ICON := icon.png

# INTERNAL #

include buildtools/make_base