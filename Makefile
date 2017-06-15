#************************************************#
# Configure
#************************************************#
# 0 for differential VCXO clock, 1 for SingleEnded VCXO clock
VCXO_SE_MODE=0

IMAGENAME = UartBoot

# **********************************************
# Tools
# **********************************************
CC		  = mips-linux-gcc
LD		  = mips-linux-ld
OBJCOPY   = mips-linux-objcopy
OBJDUMP   = mips-linux-objdump

REMOVEFILE = rm -f

# **********************************************
# Directories
# **********************************************

ROOT		= ./
SRCDIR		= $(ROOT)/arch/mips32 $(ROOT)/boot/soc_landshark $(ROOT)/loader $(ROOT)/devices/$(DEVICE)
BINDIR		= $(ROOT)/bin
LINKDIR		= $(ROOT)/scripts
VPATH       = $(SRCDIR)

# **********************************************
# Image file names and map file
# **********************************************
IMAGE_BIN = $(IMAGENAME).bin
IMAGE_ELF = $(IMAGENAME).elf
#IMAGE_REC = $(IMAGENAME).rec
#IMAGE_DIS = $(IMAGENAME).dis
MAP       = $(IMAGENAME).map
#IMAGE_FBIN=$(IMAGENAME).fbin

# **********************************************
# Compiler and linker options
# **********************************************
INCLUDE   = -I$(ROOT)/include -I$(ROOT)/arch/include -I$(ROOT)/boot/include \
            -I$(ROOT)/fs/include -I$(ROOT)/loader/include
D_OPTS_BASE = -DUARTBOOT_CODE -DVCXO_SE_MODE=$(VCXO_SE_MODE)
#D_OPTS_CACHE = -DK0SEG_CACHED -DKSEG0_CACHED_NONCOHERENT
#D_OPTS_CACHE = -DK0SEG_CACHED
D_OPTS_CACHE = 
D_OPTS    = -DDEBUG_LEVEL=3 -DDDR_AUTO_DETECT $(D_OPTS_BASE) $(D_OPTS_CACHE)
#D_OPTS    = -DDEBUG_LEVEL=3 $(D_OPTS_BASE) $(D_OPTS_CACHE)
W_OPTS	  = -Wimplicit -Wformat -Werror
F_OPTS	  = -fomit-frame-pointer -fno-pic -nostdinc
CC_OPTS   = -c -O $(INCLUDE) $(D_OPTS) $(F_OPTS) -mno-abicalls $(W_OPTS) -mips32r2
CC_OPTS16 = -c -O $(INCLUDE) $(D_OPTS) $(F_OPTS) -mno-abicalls $(W_OPTS) -mips16
CC_OPTS_A = $(CC_OPTS) -D__ASSEMBLY__

LD_SCRIPT = $(LINKDIR)/serialboot.lds
LD_OPTS   = -T $(LD_SCRIPT) -o $(IMAGE_ELF) -Map $(MAP) -EB -Bstatic

#TFLAGS	  = -0 0x03090707 -1 0x18070404
#BH        = ./tools/bin/bh
#TOOLS	  = bh
#TOOLS_PATH= tools/src/bh

# **********************************************
# Files to be compiled
# **********************************************
SRC_C  = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_S  = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.S))
SRC    = $(SRC_C) $(SRC_S)

OBJ_C  = $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_S  = $(notdir $(patsubst %.S, %.o, $(SRC_S)))
OBJ    = $(OBJ_C) $(OBJ_S)

# **********************************************
# Rules
# **********************************************
#mips-rtems4.7-objcopy -O binary elf_ic.elf elf_ic.bin

.PHONY : all depend clean

all : $(TOOLS) $(IMAGE_REC) $(IMAGE_BIN)

depend :
	$(CC) $(INCLUDE) -M $(SRC) > depend.mk
-include depend.mk

$(TOOLS) : 
	mkdir -p tools/bin; \
	make -C $(TOOLS_PATH);

$(IMAGE_BIN) : $(IMAGE_ELF)
	$(OBJCOPY) -O binary $(IMAGE_ELF) $(IMAGE_BIN)
#	$(BH) $(TFLAGS) $(IMAGE_BIN) $(IMAGE_FBIN)

$(IMAGE_REC) : $(IMAGE_ELF)
#	$(OBJDUMP) -D $(IMAGE_ELF) > $(IMAGE_DIS)
	$(OBJCOPY) -O srec $(IMAGE_ELF) $(IMAGE_REC)

$(IMAGE_ELF) : $(OBJ_C) $(OBJ_S) $(LD_SCRIPT)
	$(LD) $(LD_OPTS) $(OBJ_C) $(OBJ_S)

$(OBJ_S) : %.o : %.S ./Makefile
	$(CC) $(CC_OPTS_A) $< -o $*.o

$(OBJ_C) : %.o : %.c ./Makefile
	$(CC) $(CC_OPTS) $< -o	$*.o

clean :
	rm -rf *.o *.map *.elf *.rec *.bin *.fbin $(DEVICES)/*.o  $(DEVICES)/*.map \
	$(TOOLS_PATH)/*.o $(TOOLS_PATH)/*.d $(TOOLS_PATH)/bh tools/bin/* 
