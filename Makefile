OBJCOPY?=objcopy
ASFLAGS+=-g3
CFLAGS+=-g3

OBJ16 := boot_to_protected gdt enable_line_20 print_bios_16
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

OBJ16 := $(addprefix src16-real/, $(addsuffix .o, $(OBJ16)))
OBJ32 := $(addprefix src32-protected/, $(addsuffix .o, $(OBJ32)))

ASFLAGS+=-I include
CPPFLAGS+=-I include
src16-real/%.o: ASFLAGS+=-m16 -O0
src16-real/%.o: CFLAGS+=-m16 -Os
src32-protected/%.o: ASFLAGS+=-m32 -O0
src32-protected/%.o: CFLAGS+=-m32 -Os

LDFLAGS+=-m elf_i386
LDFLAGS+=-T hello64.ld

hello64.elf: $(OBJ16) $(OBJ32) ${EXTRA_OBJECT}
	$(LD) $(LDFLAGS) -o $@ $^

hello64: hello64.elf
	$(OBJCOPY) -O binary $^ $@

check: test/test.o
	EXTRA_OBJECT=test/test.o $(MAKE) hello64

clean:
	$(RM) hello64 hello64.bin hello64.elf $(OBJ16) $(OBJ32) test/test.o

dump: hello64
	hexdump hello64

S?=16

open_elf: hello64.elf
	objdump -d -Maddr$S,data$S,$M hello64.elf

open_bin: hello64
	objdump -D -b binary -m i386 -Maddr$S,data$S,$M hello64
