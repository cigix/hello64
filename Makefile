OBJCOPY?=objcopy
ASFLAGS+=-g3

OBJ16 := boot.o

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

hello64.elf: $(OBJ16) ${EXTRA_OBJECT}
	$(LD) -Ttext 0x7C00 -o $@ $^

hello64: hello64.elf
	$(OBJCOPY) -O binary $^ $@

check: test/test.o
	EXTRA_OBJECT=test/test.o $(MAKE) hello64

clean:
	$(RM) hello64 hello64.bin hello64.elf $(OBJ16) test/test.o
