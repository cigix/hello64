OBJCOPY?=objcopy
ASFLAGS+=-g3
CFLAGS+=-g3
CFLAGS+=-fno-pic -fpack-struct

# Files to be put on the first sector of the floppy device
OBJ16_floppy := boot load_next_sector print_bios_16
OBJ32_floppy :=
# Other objects
OBJ16 := simple_gdt to_protected
OBJ32 := reload_segments print_vga_32

##########

all: hello64

launch: hello64
	qemu-system-x86_64 -enable-kvm -fda hello64 -nographic -serial stdio \
		-monitor none

debug: hello64 hello64.elf gdbinit
	qemu-system-x86_64 -fda hello64 -S -s & \
	sleep 1 ; gdb hello64.elf -x gdbinit ; \
	kill $$! &>/dev/null || true

OBJ_floppy := $(addprefix src16-real/, $(addsuffix .o, $(OBJ16_floppy)))
OBJ_floppy += $(addprefix src32-protected/, $(addsuffix .o, $(OBJ32_floppy)))
OBJ16 := $(addprefix src16-real/, $(addsuffix .o, $(OBJ16)))
OBJ32 := $(addprefix src32-protected/, $(addsuffix .o, $(OBJ32)))

CPPFLAGS+=-I include
src16-real/%.o: ASFLAGS+=-m16 -O0
src16-real/%.o: CFLAGS+=-m16 -Os
src32-protected/%.o: ASFLAGS+=-m32 -O0
src32-protected/%.o: CFLAGS+=-m32 -Os

LDFLAGS+=-m elf_i386

floppy.elf: $(OBJ_floppy)
	$(LD) $(LDFLAGS) -r -o $@ $^

data.elf: $(OBJ16) $(OBJ32) ${EXTRA_OBJECT}
	$(LD) $(LDFLAGS) -r -o $@ $^

hello64.elf: floppy.elf data.elf
	$(LD) $(LDFLAGS) -T hello64.ld -o $@ $^

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

hello64: hello64.elf
	$(OBJCOPY) -O binary $^ $@

check: test/test.o
	EXTRA_OBJECT=test/test.o $(MAKE) hello64

clean:
	$(RM) hello64 hello64.bin hello64.elf
	$(RM) floppy.bin data.bin floppy.elf data.elf
	$(RM) $(OBJ_floppy) $(OBJ16) $(OBJ32)
	$(RM) test/test.o

dump: hello64
	hexdump -C hello64

S?=16

open_elf: hello64.elf
	objdump -d -Maddr$S,data$S,$M $^

open_bin: hello64
	objdump -D -b binary -m i386 -Maddr$S,data$S,$M $^
