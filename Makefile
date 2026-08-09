XBE_TITLE = Eins
GEN_XISO = $(XBE_TITLE).iso
OUTPUT_DIR ?= bin

NXDK_DIR ?= $(CURDIR)/../nxdk

NXDK_SDL = y
NXDK_SDL_TTF = y
NXDK_NET = y

SRCS = $(wildcard $(CURDIR)/src/*.c)

CFLAGS += -Wall -Wextra -Wno-unused-parameter -I$(CURDIR)/src \
          -I$(NXDK_DIR)/lib/sdl/include \
          -I$(NXDK_DIR)/lib/sdl_ttf

CXXFLAGS += $(CFLAGS)

all: copy_assets $(TARGET)

copy_assets:
	@mkdir -p $(OUTPUT_DIR)
	@cp -r $(CURDIR)/assets $(OUTPUT_DIR)/assets

.PHONY: copy_assets

# Include main nxdk build system
include $(NXDK_DIR)/Makefile
