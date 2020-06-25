include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE ?= arm-none-eabi-

CFLAGS =
LIBS =
INC =

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

CFLAGS_CORTEX_M4 += -mthumb -mtune=cortex-m4 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS_CORTEX_M4 += -fsingle-precision-constant
CFLAGS_CORTEX_M4 += -DMICROPY_FLOAT_IMPL=MICROPY_FLOAT_IMPL_FLOAT

CFLAGS += -Wall -Werror -Wdouble-promotion -Wfloat-conversion
CFLAGS += -std=gnu99 -nostdlib $(CFLAGS_CORTEX_M4) $(COPT)

LDFLAGS = -nostdlib -Map=$@.map --cref --gc-sections

CSUPEROPT = -Os # save some code space

# Tune for Debugging or Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os -DNDEBUG
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -ggdb  # Add debug information anyway
endif

SRC_C = \
	main.c \
	uart_core.c \
	lib/utils/printf.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/mp-readline/readline.c \
#	$(BUILD)/_frozen_mpy.c \

SRC_C += lib/libc/string0.c


# Math library
LIBM_SRC_C = $(addprefix lib/libm/,\
	math.c \
	acoshf.c \
	asinfacosf.c \
	asinhf.c \
	atan2f.c \
	atanf.c \
	atanhf.c \
	ef_rem_pio2.c \
	erf_lgamma.c \
	fmodf.c \
	kf_cos.c \
	kf_rem_pio2.c \
	kf_sin.c \
	kf_tan.c \
	log1pf.c \
	nearbyintf.c \
	roundf.c \
	sf_cos.c \
	sf_erf.c \
	sf_frexp.c \
	sf_ldexp.c \
	sf_modf.c \
	sf_sin.c \
	sf_tan.c \
	wf_lgamma.c \
	wf_tgamma.c \
	thumb_vfp_sqrtf.c \
	)
LIBM_SRC_C += lib/libm/thumb_vfp_sqrtf.c
LIBS += "$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)"
LIBM_O = $(addprefix $(BUILD)/, $(LIBM_SRC_C:.c=.o))



# Gecko SDK
GECKOSDK = lib/gecko_sdk_suite/v2.7
INC += -I$(TOP)/$(GECKOSDK)/platform/CMSIS/Include -I$(TOP)/$(GECKOSDK)/platform/emlib/inc
SRC_C += $(addprefix $(GECKOSDK)/platform/emlib/src/,\
	em_system.c \
	em_cmu.c \
	em_gpio.c \
	em_leuart.c \
	em_usart.c \
	)

# EFR32 chip specific parts
CFLAGS  += -DEFR32MG1P232F256GM48
INC     += -I$(TOP)/$(GECKOSDK)/platform/Device/SiliconLabs/EFR32MG1P/Include
SRC_C   +=          $(GECKOSDK)/platform/Device/SiliconLabs/EFR32MG1P/Source/GCC/startup_efr32mg1p.c
SRC_C   +=          $(GECKOSDK)/platform/Device/SiliconLabs/EFR32MG1P/Source/system_efr32mg1p.c
LDFLAGS += -T$(TOP)/$(GECKOSDK)/platform/Device/SiliconLabs/EFR32MG1P/Source/GCC/efr32mg1p.ld


#
CFLAGS += $(INC)
OBJ = $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(LIBM_O)

all: $(BUILD)/firmware.bin

#$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
#	$(ECHO) "MISC freezing bytecode"
#	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary -j .isr_vector -j .text -j .data $^ $(BUILD)/firmware.bin

include $(TOP)/py/mkrules.mk
