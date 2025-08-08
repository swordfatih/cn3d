SHADERC := "C:/Users/fatih/Documents/Github/bgfx.cmake/build/cmake/bgfx/shaderc.exe"
INCLUDE_DIR := C:/Users/fatih/Documents/Github/bgfx.cmake/bgfx/src
PLATFORM := windows
PROFILE := s_5_0

SHADER_FOLDERS := $(sort $(dir $(wildcard */*.sc)))
SHADER_NAMES := $(patsubst %/,%,$(SHADER_FOLDERS))

.PHONY: all clean print $(SHADER_NAMES)

all: $(SHADER_NAMES)

$(SHADER_NAMES):
	@$(MAKE) -f $(MAKEFILE_LIST) SHADER_NAME=$@ compile_shader

compile_shader:
	$(SHADERC) -f $(SHADER_NAME)/vs.sc -o $(SHADER_NAME)/vs.bin --type vertex --platform $(PLATFORM) --profile $(PROFILE) --varyingdef $(SHADER_NAME)/def.sc -i $(INCLUDE_DIR)
	$(SHADERC) -f $(SHADER_NAME)/fs.sc -o $(SHADER_NAME)/fs.bin --type fragment --platform $(PLATFORM) --profile $(PROFILE) --varyingdef $(SHADER_NAME)/def.sc -i $(INCLUDE_DIR)

clean:
	@for dir in $(SHADER_NAMES); do \
		rm -f $$dir/*.bin; \
	done

print:
	@echo Shader folders with .sc files: $(SHADER_NAMES)
