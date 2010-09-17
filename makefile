#=====================================================================
#(C) Unpublished Work of ADMtek Incorporated.  All Rights Reserved.
#
# boot loader make file
#=====================================================================
# LITTLE_ENDIAN OR BIG_ENDIAN
BYTE_ORDER = LITTLE_ENDIAN


#==========================FLASH CONFIGURATION===========================
# NOR_FLASH OR NAND_FLASH
FLASH_TYPE = NAND_FLASH

# 0x400000 OR 0x200000
BOARD_NOR_FLASH_SIZE = 0x200000



#============================ Make ==============================
# Don't define any builtin rules and variables.
MAKEFLAGS := $(MAKEFLAGS)R

# Delete default suffixes
.SUFFIXES:

# Delete default rules
.DEFAULT:

.DEFAULT:
	$(error $(i)no rules for target "$@")

# Tell GNU make 3.79 not to run in parallel
.NOTPARALLEL:

#============================ Compilers ==============================
CROSS_PREFIX ?= mipsel-linux-
AS	= $(CROSS_PREFIX)as
CC	= $(CROSS_PREFIX)gcc
LD	= $(CROSS_PREFIX)ld
AR	= $(CROSS_PREFIX)ar
OBJCOPY = $(CROSS_PREFIX)objcopy
OBJDUMP = $(CROSS_PREFIX)objdump


#============================ Tools ==================================
RM	= rm -f
MV	= mv
CP	= cp -a


#=======================  Endian dependance  =========================
ifeq ("$(BYTE_ORDER)", "LITTLE_ENDIAN")
CCBYTE_ORDER = little-endian
ENDIAN_FG = -EL
LIB_PATH = -L. -L./lib/el
EDIR = el
else
CCBYTE_ORDER = big-endian
ENDIAN_FG = -EB
LIB_PATH = -L. -L./lib/eb
EDIR = eb
endif


#=======================  Compiler Flags  ============================
CC_FLAG 		= $(ENDIAN_FG) -Wcomment -Os
CPU_FLAG		= -mips32
#START_FLAGF		= -DFLASH_START

ifeq ("$(FLASH_TYPE)", "NAND_FLASH")
INCLUDE_DIR = -I. -I./include
else
INCLUDE_DIR =     -I./include
endif


#=======================  Directorys  ================================
ifeq ("$(FLASH_TYPE)", "NAND_FLASH")
OBJ_DIR = ./build/$(EDIR)/$(FLASH_TYPE)
BIN_DIR = ./bin/$(EDIR)/$(FLASH_TYPE)
SRC_DIR = ./src
else
OBJ_DIR = ./build/$(EDIR)/$(FLASH_TYPE)
BIN_DIR = ./bin/$(EDIR)/$(FLASH_TYPE)
SRC_DIR = ./src
endif


ifeq ("$(FLASH_TYPE)", "NAND_FLASH")
#==================== NandFlash Linker Flags  ===========================
LD_FLAG = -X -N
LIBS = -lz -lc -lnosys

BOOT_NAME     = nand_bootinit
BOOT_NAME_RAM = nand_bootinit_ram
BOOT_OBJS     = $(OBJ_DIR)/nand_bootinit.o
BOOT_OBJS_RAM = $(OBJ_DIR)/nand_bootinit_ram.o

EXEC_NAME = nand_bootmain
EXEC_OBJS = $(OBJ_DIR)/nand_ldrinit.o $(OBJ_DIR)/bloader.o $(OBJ_DIR)/linuxld.o\
		$(OBJ_DIR)/xmodem.o $(OBJ_DIR)/nand.o $(OBJ_DIR)/nand_ecc.o\
		$(OBJ_DIR)/cachelib.o\
		$(OBJ_DIR)/irqlib.o $(OBJ_DIR)/timer.o\
		$(OBJ_DIR)/vector.o $(OBJ_DIR)/except.o $(OBJ_DIR)/utils.o\
		$(OBJ_DIR)/if_5120.o $(OBJ_DIR)/memlib.o\
		$(OBJ_DIR)/uartdrv.o $(OBJ_DIR)/tftp.o $(OBJ_DIR)/eth.o\
		$(OBJ_DIR)/skbuff.o $(OBJ_DIR)/arp.o $(OBJ_DIR)/ip.o\
		$(OBJ_DIR)/udp.o $(OBJ_DIR)/param.o $(OBJ_DIR)/nf.o

EXEC_NAME_RAM = nand_bootmain_ram
EXEC_OBJS_RAM = $(OBJ_DIR)/nand_ldrinit.o $(OBJ_DIR)/bloader_ram.o $(OBJ_DIR)/linuxld.o\
		$(OBJ_DIR)/xmodem.o $(OBJ_DIR)/nand.o $(OBJ_DIR)/nand_ecc.o\
		$(OBJ_DIR)/cachelib.o\
		$(OBJ_DIR)/irqlib.o $(OBJ_DIR)/timer.o\
		$(OBJ_DIR)/vector.o $(OBJ_DIR)/except.o $(OBJ_DIR)/utils.o\
		$(OBJ_DIR)/if_5120.o $(OBJ_DIR)/memlib.o\
		$(OBJ_DIR)/uartdrv.o $(OBJ_DIR)/tftp.o $(OBJ_DIR)/eth.o\
		$(OBJ_DIR)/skbuff.o $(OBJ_DIR)/arp.o $(OBJ_DIR)/ip.o\
		$(OBJ_DIR)/udp.o $(OBJ_DIR)/param.o $(OBJ_DIR)/nf.o

ROM_NAME     = nandloader
ROM_NAME_RAM = nandloader_ram
CMM_CREATE   = loadercmm

else
#======================= NorFlash Linker Flags  ===========================
LD_FLAG = -X -N
LIBS = -lz -lc -lnosys

BOOT_NAME =            nor_bootinit
BOOT_OBJS = $(OBJ_DIR)/nor_bootinit.o

EXEC_NAME = nor_bootmain
EXEC_OBJS = $(OBJ_DIR)/nor_ldrinit.o $(OBJ_DIR)/bloader.o $(OBJ_DIR)/linuxld.o \
			$(OBJ_DIR)/xmodem.o $(OBJ_DIR)/timer.o $(OBJ_DIR)/uartdrv.o	\
			$(OBJ_DIR)/flash.o $(OBJ_DIR)/memlib.o $(OBJ_DIR)/irqlib.o \
			$(OBJ_DIR)/cachelib.o $(OBJ_DIR)/vector.o $(OBJ_DIR)/except.o \
			$(OBJ_DIR)/tftp.o $(OBJ_DIR)/arp.o $(OBJ_DIR)/eth.o \
			$(OBJ_DIR)/if_5120.o $(OBJ_DIR)/ip.o $(OBJ_DIR)/skbuff.o \
			$(OBJ_DIR)/udp.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/param.o $(OBJ_DIR)/nf.o

ifeq ($(BOARD_NOR_FLASH_SIZE), 0x400000)
EXEC_OBJS += $(OBJ_DIR)/mx29lv320.o
CC_FLAG += -DFLASH_4M
endif

ifeq ($(BOARD_NOR_FLASH_SIZE), 0x200000)
EXEC_OBJS += $(OBJ_DIR)/mx29lv160.o
CC_FLAG += -DFLASH_2M
endif

CC_FLAG += -DNOR_FLASH
ROM_NAME = norloader

endif



ifeq ("$(FLASH_TYPE)", "NAND_FLASH")
#====================== NandFlash Rules  =============================
.PHONY : all rom_img rom_img_ram

all : rom_img_ram rom_img

rom_img_ram: boot_img_ram main_img_ram
	cat  $(OBJ_DIR)/$(BOOT_NAME_RAM).img $(OBJ_DIR)/$(EXEC_NAME_RAM).img > $(BIN_DIR)/$(ROM_NAME_RAM).img
	$(OBJCOPY) --set-start=0x80800000 -O srec -I binary  $(BIN_DIR)/$(ROM_NAME_RAM).img $(BIN_DIR)/$(ROM_NAME_RAM).srec
#	$(BIN_DIR)/$(CMM_CREATE) $(BIN_DIR)/$(ROM_NAME).img  $(BIN_DIR)/$(ROM_NAME).cmm

boot_img_ram: $(BOOT_OBJS_RAM)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext 0x80800000 \
				-Map $(OBJ_DIR)/$(BOOT_NAME_RAM).map -o $(OBJ_DIR)/$(BOOT_NAME_RAM).elf	\
				$(BOOT_OBJS_RAM) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(BOOT_NAME_RAM).elf  $(OBJ_DIR)/$(BOOT_NAME_RAM).bin
	$(OBJCOPY) -I binary -O binary --pad-to 0x1000  $(OBJ_DIR)/$(BOOT_NAME_RAM).bin \
				$(OBJ_DIR)/$(BOOT_NAME_RAM).img

main_img_ram: $(EXEC_OBJS_RAM)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext 0x80801000 \
			-Map $(OBJ_DIR)/$(EXEC_NAME_RAM).map -o $(OBJ_DIR)/$(EXEC_NAME_RAM).elf \
			$(EXEC_OBJS_RAM) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(EXEC_NAME_RAM).elf  $(OBJ_DIR)/$(EXEC_NAME_RAM).bin
	$(CP) $(OBJ_DIR)/$(EXEC_NAME_RAM).bin $(OBJ_DIR)/$(EXEC_NAME_RAM).img



rom_img: boot_img main_img
	cat $(OBJ_DIR)/$(BOOT_NAME).img $(OBJ_DIR)/$(EXEC_NAME).img > $(BIN_DIR)/$(ROM_NAME).img
#	$(BIN_DIR)/$(CMM_CREATE) $(BIN_DIR)/$(ROM_NAME).img  $(BIN_DIR)/$(ROM_NAME).cmm

boot_img: $(BOOT_OBJS)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext  0x80800000 \
				-Map $(OBJ_DIR)/$(BOOT_NAME).map -o $(OBJ_DIR)/$(BOOT_NAME).elf	\
				$(BOOT_OBJS) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(BOOT_NAME).elf  $(OBJ_DIR)/$(BOOT_NAME).bin
	$(OBJCOPY) -I binary -O binary --pad-to 0x1000  $(OBJ_DIR)/$(BOOT_NAME).bin \
				$(OBJ_DIR)/$(BOOT_NAME).img

main_img: $(EXEC_OBJS)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext 0x80801000 \
			-Map $(OBJ_DIR)/$(EXEC_NAME).map -o $(OBJ_DIR)/$(EXEC_NAME).elf \
			$(EXEC_OBJS) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(EXEC_NAME).elf  $(OBJ_DIR)/$(EXEC_NAME).bin
	$(CP) $(OBJ_DIR)/$(EXEC_NAME).bin $(OBJ_DIR)/$(EXEC_NAME).img


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.S
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@


else
#==========================  NorFlash Rules  ==================================
.PHONY : all rom_img

all : rom_img

rom_img: boot_img main_img
	cat  $(OBJ_DIR)/$(BOOT_NAME).img $(OBJ_DIR)/$(EXEC_NAME).img > $(BIN_DIR)/$(ROM_NAME).img

boot_img: $(BOOT_OBJS)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e romreset_except -Ttext 0x80800000 \
				-Map $(OBJ_DIR)/$(BOOT_NAME).map -o $(OBJ_DIR)/$(BOOT_NAME).elf	\
				$(BOOT_OBJS) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(BOOT_NAME).elf  $(OBJ_DIR)/$(BOOT_NAME).bin
	$(OBJCOPY) -I binary -O binary --pad-to 0x1000  $(OBJ_DIR)/$(BOOT_NAME).bin \
				$(OBJ_DIR)/$(BOOT_NAME).img

main_img: $(EXEC_OBJS)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext 0x80801000 \
			-Map $(OBJ_DIR)/$(EXEC_NAME).map -o $(OBJ_DIR)/$(EXEC_NAME).elf \
			$(EXEC_OBJS) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(EXEC_NAME).elf  $(OBJ_DIR)/$(EXEC_NAME).bin
	$(CP) $(OBJ_DIR)/$(EXEC_NAME).bin $(OBJ_DIR)/$(EXEC_NAME).img

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.S
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@

endif




#========================== Clean rules ==============================
.PHONY : cleanall clean
cleanall: clean
clean:
	@$(RM) -v $(OBJ_DIR)/$(BOOT_NAME).* $(EXEC_OBJS) $(OBJ_DIR)/$(EXEC_NAME).* \
			$(BIN_DIR)/$(ROM_NAME).img $(OBJ_DIR)/*.o

