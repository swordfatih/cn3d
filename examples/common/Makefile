# === CONFIGURABLE VARIABLES ===

# Path to shaderc executable
SHADERC := "C:/Users/fatih/Documents/Github/bgfx.cmake/build/cmake/bgfx/shaderc.exe"

# Path to BGFX include folder
INCLUDE_DIR := C:/Users/fatih/Documents/Github/bgfx.cmake/bgfx/src

# Shader folder (relative to Makefile location)
SHADER_DIR := ../cube/runtime/shaders
SHADER_DIR := $(abspath $(SHADER_DIR))

# Shader options
PLATFORM := windows
PROFILE := s_5_0

# Varying.def file
VARYING_DEF := $(SHADER_DIR)/varying.def.sc

# Shader source files (only vs_*.sc and fs_*.sc)
VERTEX_SHADERS := $(wildcard $(SHADER_DIR)/vs_*.sc)
FRAGMENT_SHADERS := $(wildcard $(SHADER_DIR)/fs_*.sc)

# Output binary file list
VERTEX_BINS := $(patsubst %.sc,%.bin,$(VERTEX_SHADERS))
FRAGMENT_BINS := $(patsubst %.sc,%.bin,$(FRAGMENT_SHADERS))
SHADER_BIN := $(VERTEX_BINS) $(FRAGMENT_BINS)

# === TARGETS ===

# Default target
all: $(SHADER_BIN)

# Vertex shader rule
$(SHADER_DIR)/vs_%.bin: $(SHADER_DIR)/vs_%.sc $(VARYING_DEF)
	$(SHADERC) -f $< -o $@ --type vertex --platform $(PLATFORM) --profile $(PROFILE) --varyingdef $(VARYING_DEF) -i $(INCLUDE_DIR)

# Fragment shader rule
$(SHADER_DIR)/fs_%.bin: $(SHADER_DIR)/fs_%.sc $(VARYING_DEF)
	$(SHADERC) -f $< -o $@ --type fragment --platform $(PLATFORM) --profile $(PROFILE) --varyingdef $(VARYING_DEF) -i $(INCLUDE_DIR)

# Clean rule (cross-platform)
clean:
	echo Cleaning shader binaries...
	rm -f $(SHADER_BIN)