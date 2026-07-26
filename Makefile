PROJECT := orochi
BUILD := build
TARGET := $(BUILD)/$(PROJECT).gb

ifndef GBDK_HOME
$(error GBDK_HOME is not set.)
endif

ifndef HUGEDRIVER
$(error HUGEDRIVER is not set.)
endif

LCC := $(GBDK_HOME)/bin/lcc
GBLIB := $(GBDK_HOME)/lib/gb/gb.lib

LINKOBJS := $(BUILD)/set_data.o \
						$(BUILD)/sfr.o \
						$(BUILD)/delay.o \
						$(BUILD)/set_bk_t.o \
						$(BUILD)/set_xy_t.o \
						$(BUILD)/set_tile.o \
						$(BUILD)/get_addr.o \
						$(BUILD)/fill_rect.o \
						$(BUILD)/fill_rect_bk.o \
						$(BUILD)/cgb_palettes.o \
						$(BUILD)/metasprites.o \
						$(BUILD)/metasprites_hide_spr.o \
						$(BUILD)/set_wi_t.o

ifdef GBDK_DEBUG
LCCFLAGS += -debug -v
endif

LCCFLAGS += -Wm-yc # run on both DMG and CGB

# Include directories/paths
INCLUDES := -Isrc -Ires -I$(BUILD)/generated -I$(HUGEDRIVER)/include

LIBRESPRITE := libresprite
PNG2ASSET := $(GBDK_HOME)/bin/png2asset

include res/png2asset.mk

RES_ASE := $(addprefix res/,$(addsuffix .ase,$(PNG2ASSET_ASSETS)))

RES_GEN := $(addprefix $(BUILD)/generated/,$(PNG2ASSET_ASSETS))

RES_C_GEN := $(addsuffix .c,$(RES_GEN))
RES_H_GEN := $(addsuffix .h,$(RES_GEN))

RES_C_DISK := $(shell find res -name '*.c' 2>/dev/null)
RES_S := $(shell find res -name '*.s' 2>/dev/null)

RES_C := $(sort $(RES_C_DISK) $(RES_C_GEN))
RES_H := $(RES_H_GEN)

SRC_C := $(shell find src -name '*.c' 2>/dev/null)
SRC_S := $(shell find src -name '*.s' 2>/dev/null)

OBJS := $(SRC_C:src/%.c=$(BUILD)/%.o) \
        $(SRC_S:src/%.s=$(BUILD)/%.o) \
        $(RES_C_DISK:res/%.c=$(BUILD)/%.o) \
        $(RES_C_GEN:$(BUILD)/generated/%.c=$(BUILD)/generated/%.o) \
        $(RES_S:res/%.s=$(BUILD)/%.o) \
        $(HUGEDRIVER)/lib/hUGEDriver.o

.PHONY: all run clean

GENERATED := $(RES_C) $(RES_H)

all: $(GENERATED) $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

# Export .ase files to .png format
build/generated/%.png: res/%.ase | $(BUILD)
	@mkdir -p $(dir $@)
	$(LIBRESPRITE) -b $(abspath $<) --save-as $(abspath $@)

# Compile .png files to .c and .h targets
build/generated/%.c build/generated/%.h &: build/generated/%.png res/png2asset.mk
	@mkdir -p $(dir $@)
	$(PNG2ASSET) $< -c build/generated/$*.c $(PNG2ASSET_FLAGS/$*)

$(BUILD)/%.o: src/%.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(LCC) $(LCCFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD)/%.o: res/%.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(LCC) $(LCCFLAGS) $(INCLUDES) -c -o $@ $<

$(BUILD)/generated/%.o: build/generated/%.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(LCC) $(LCCFLAGS) $(INCLUDES) -c -o $@ $<

$(LINKOBJS): | $(BUILD)
	ar x --output=$(BUILD) $(GBLIB) $(notdir $@)

$(TARGET): $(OBJS) $(LINKOBJS)
	$(LCC) $(LCCFLAGS) -o $@ $(OBJS) $(LINKOBJS)

run: $(TARGET)
	gearboy $(TARGET)

clean:
	rm -rf $(BUILD)
