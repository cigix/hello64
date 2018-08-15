OBJCOPY?=objcopy
ASFLAGS+=-g3
CFLAGS+=-g3

OBJ16 := boot.o print_C_bios.o

##########

all: hello64

launch: hello64
	qemu-system-x86_64 -enable-kvm -fda hello64 -nographic -serial stdio \
		-monitor none

debug: hello64 hello64.elf gdbinit
	qemu-system-x86_64 -enable-kvm -fda hello64 -nographic -serial stdio \
		-monitor none -S -s & \
	gdb hello64.elf -x gdbinit ; \
	kill $$! &>/dev/null || true

OBJ16 := $(addprefix src16-real/, $(OBJ16))

src16-real/%.o: ASFLAGS+=-m16
src16-real/%.o: CFLAGS+=-m16

LDFLAGS+=-m elf_i386
LDFLAGS+=-T hello64.ld

hello64.elf: $(OBJ16) ${EXTRA_OBJECT}
	$(LD) $(LDFLAGS) -o $@ $^

hello64: hello64.elf
	$(OBJCOPY) -O binary $^ $@

check: test/test.o
	EXTRA_OBJECT=test/test.o $(MAKE) hello64

clean:
	$(RM) hello64 hello64.bin hello64.elf $(OBJ16) test/test.o

dump: hello64
	hexdump hello64

open_elf: hello64.elf
	objdump -d -Maddr16,data16,$M hello64.elf

open_bin: hello64
	objdump -D -b binary -m i386 -Maddr16,data16,$M hello64
