#=====================================================================
#(C) Unpublished Work of ADMtek Incorporated.  All Rights Reserved.
#
# boot loader make file
#=====================================================================



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

# Set V=something for verbose building messages
#V = 1
v = $(if $(V),,@)

#============================ Compilers ==============================
CROSS_PREFIX ?= $(HOME)/gnutools/mipsisa32-elf/bin/mipsisa32-elf-
AS	= $(CROSS_PREFIX)as
CC	= $(CROSS_PREFIX)gcc
LD	= $(CROSS_PREFIX)ld
AR	= $(CROSS_PREFIX)ar
OBJCOPY = $(CROSS_PREFIX)objcopy
OBJDUMP = $(CROSS_PREFIX)objdump


#============================ Tools ==================================
RM	= rm    $(if $(V),-v) -f
RM_R	= rm    $(if $(V),-v) -fr
MV	= mv    $(if $(V),-v)
CP	= cp    $(if $(V),-v) -a
MKDIR   = mkdir $(if $(V),-v)
MKDIR_P = mkdir $(if $(V),-v) -p


#=======================  Endian dependance  =========================
ENDIAN_FG = -EL
EDIR      = el
LIB_PATH  = -L./lib/$(EDIR)


#=======================  Compiler Flags  ============================
CC_FLAG 	= $(ENDIAN_FG) -Wall -W -Os
CPU_FLAG	= -mips32
INCLUDE_DIR	= -I./include


#=======================  Directories  ===============================
OBJ_DIR = ./build
BIN_DIR = ./bin
SRC_DIR = ./src

TFTPBOOT = $(HOME)/tftpboot

 OBJ_DIR_STAMP =  $(OBJ_DIR)/.dir
 BIN_DIR_STAMP =  $(BIN_DIR)/.dir
TFTPBOOT_STAMP = $(TFTPBOOT)/.dir


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


#====================== NandFlash Rules  =============================
.PHONY : all rom_img rom_img_ram

all : rom_img_ram rom_img

rom_img_ram: boot_img_ram main_img_ram $(BIN_DIR_STAMP)
	cat  $(OBJ_DIR)/$(BOOT_NAME_RAM).img $(OBJ_DIR)/$(EXEC_NAME_RAM).img > $(BIN_DIR)/$(ROM_NAME_RAM).img
#	$(OBJCOPY) --set-start=0x80800000 -O srec -I binary  $(BIN_DIR)/$(ROM_NAME_RAM).img $(BIN_DIR)/$(ROM_NAME_RAM).srec
#	$(BIN_DIR)/$(CMM_CREATE) $(BIN_DIR)/$(ROM_NAME).img  $(BIN_DIR)/$(ROM_NAME).cmm

boot_img_ram: $(BOOT_OBJS_RAM) $(OBJ_DIR_STAMP)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext 0x80800000 \
				-Map $(OBJ_DIR)/$(BOOT_NAME_RAM).map -o $(OBJ_DIR)/$(BOOT_NAME_RAM).elf	\
				$(BOOT_OBJS_RAM) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(BOOT_NAME_RAM).elf  $(OBJ_DIR)/$(BOOT_NAME_RAM).bin
	$(OBJCOPY) -I binary -O binary --pad-to 0x1000  $(OBJ_DIR)/$(BOOT_NAME_RAM).bin \
				$(OBJ_DIR)/$(BOOT_NAME_RAM).img

main_img_ram: $(EXEC_OBJS_RAM) $(OBJ_DIR_STAMP)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext 0x80801000 \
			-Map $(OBJ_DIR)/$(EXEC_NAME_RAM).map -o $(OBJ_DIR)/$(EXEC_NAME_RAM).elf \
			$(EXEC_OBJS_RAM) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(EXEC_NAME_RAM).elf  $(OBJ_DIR)/$(EXEC_NAME_RAM).bin
	$(CP) $(OBJ_DIR)/$(EXEC_NAME_RAM).bin $(OBJ_DIR)/$(EXEC_NAME_RAM).img



rom_img: boot_img main_img $(BIN_DIR_STAMP)
	cat $(OBJ_DIR)/$(BOOT_NAME).img $(OBJ_DIR)/$(EXEC_NAME).img > $(BIN_DIR)/$(ROM_NAME).img
#	$(BIN_DIR)/$(CMM_CREATE) $(BIN_DIR)/$(ROM_NAME).img  $(BIN_DIR)/$(ROM_NAME).cmm

boot_img: $(BOOT_OBJS) $(OBJ_DIR_STAMP)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _nand_reset -Ttext  0x80800000 \
				-Map $(OBJ_DIR)/$(BOOT_NAME).map -o $(OBJ_DIR)/$(BOOT_NAME).elf	\
				$(BOOT_OBJS) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(BOOT_NAME).elf  $(OBJ_DIR)/$(BOOT_NAME).bin
	$(OBJCOPY) -I binary -O binary --pad-to 0x1000  $(OBJ_DIR)/$(BOOT_NAME).bin \
				$(OBJ_DIR)/$(BOOT_NAME).img

main_img: $(EXEC_OBJS) $(OBJ_DIR_STAMP)
	$(LD) $(ENDIAN_FG) $(LD_FLAG) $(LIB_PATH) -e _ldrinit -Ttext 0x80801000 \
			-Map $(OBJ_DIR)/$(EXEC_NAME).map -o $(OBJ_DIR)/$(EXEC_NAME).elf \
			$(EXEC_OBJS) $(LIBS)
	$(OBJCOPY) -O binary $(OBJ_DIR)/$(EXEC_NAME).elf  $(OBJ_DIR)/$(EXEC_NAME).bin
	$(CP) $(OBJ_DIR)/$(EXEC_NAME).bin $(OBJ_DIR)/$(EXEC_NAME).img


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(OBJ_DIR_STAMP)
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.S $(OBJ_DIR_STAMP)
	$(CC) $(CC_FLAG) $(INCLUDE_DIR) $(CPU_FLAG) $(EXT_DEF) $(EXTRA_DEFINE) -c $< -o $@


#======================== Directory rules ============================
%/.dir :
	@echo "> Creating directory $@"
	$(v)$(MKDIR_P)        $(dir $@)
	$(v)touch                   $@


#========================== Clean rules ==============================
.PHONY : clean
clean:
	@echo "> Cleaning $(OBJ_DIR) $(BIN_DIR)"
	$(v)$(RM_R)       $(OBJ_DIR) $(BIN_DIR)

