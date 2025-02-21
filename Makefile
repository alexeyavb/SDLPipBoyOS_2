# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2025-02-09 - Several enhancements + project update mode
#   2025-02-09 - first version
# ------------------------------------------------
####
# OS
####
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CCFLAGS += -DLINUX
endif
ifeq ($(UNAME_S),Darwin)
	CCFLAGS += -Wc99-extensions -DOSX
endif
######################################
# target
######################################
TARGET = sdlpipboy
######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og -std=c99

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
	main.c \
	Core/Src/global.c \
	Core/Src/fps.c \
	Core/Src/ttf_utils.c \
	Core/Src/bmp_font.c \
	Core/Src/core_init.c \
	Core/Src/linux_events_thread.c \
	Core/Src/mainmenu.c \
	Core/Src/demo.c \
	Core/Src/bmp_rain1.c \
	Core/Src/bmp_rain2.c \
	Core/Src/plasma.c \
	Core/Src/alpha_line.c \
	Core/Src/matrix_rain.c \
	Core/Src/display_test.c \
	Core/Src/ttf_test.c

# ASM sources
ASM_SOURCES =
# ASM sources
ASMM_SOURCES =

#######################################
# binaries
#######################################
PREFIX =
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
# CP=/usr/local/opt/binutils/bin/objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
# CP = $(PREFIX)objcopy
CP=/usr/local/opt/binutils/bin/objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
# CPU = -mcpu=cortex-m4
CPU =

# fpu
# FPU = -mfpu=fpv4-sp-d16
FPU = 

# float-abi
# FLOAT-ABI = -mfloat-abi=hard
FLOAT-ABI =

# mcu
# MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
MCU =

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DLCP_PI_PC_T113

# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
	-ICore/Inc \
	-I/usr/include \
	-I/usr/local/include \
	-I/usr/include/SDL2 \
	-I/usr/local/include/SDL2 \
	-I/usr/include/GL \
	-I/usr/local/include/GL

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifdef GCC_PATH
CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CCFLAGS) -Wall -fdata-sections -ffunction-sections
else
CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CCFLAGS) -Wall -fdata-sections -ffunction-sections
endif

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-4
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT =

# libraries
LIBS = -ldl -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lGL
LIBDIR = -L/usr/lib -L/usr/local/lib -L/usr/local/opt/
LDFLAGS = $(MCU) $(LIBDIR) $(LIBS)

# default action: build all
all: $(BUILD_DIR)/$(TARGET).out

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wall $< -o $@
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).out: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.out | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.out | $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
