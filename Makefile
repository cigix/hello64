OBJCOPY?=objcopy
ASFLAGS+=-g3
CFLAGS+=-g3
CFLAGS+=-fno-pic -fpack-struct -fno-builtin
CFLAGS+=-Wall -Wextra

# Files to be put on the first sector of the floppy device
stage0_16 := boot load_stage1 enable_line_20 cpuid
stage0_32 :=
stage0_64 :=
# Files to be put on the remaining sectors of the floppy device and loaded after
stage1_16 := putc_bios putc_vga puts puti print_test putc_serial io
stage1_32 :=
stage1_64 :=

fold_name_16 := src16-real
fold_name_32 := src32-protected
fold_name_64 := src64-long

##########

common_src := $(notdir $(wildcard src-common/*.c))

all: hello64

launch: hello64
	qemu-system-x86_64 -enable-kvm -fda hello64 -nographic -serial stdio \
		-monitor none

launch_nokvm: hello64
	qemu-system-x86_64 -fda hello64 -nographic -serial stdio -monitor none

debug: hello64 hello64.elf gdbinit
	qemu-system-x86_64 -fda hello64 -serial stdio -monitor none -S -s & \
	sleep 1 ; gdb hello64.elf -x gdbinit ; \
	kill $$! &>/dev/null || true

stage0_OBJ := $(addprefix $(fold_name_16)/, $(addsuffix .o, $(stage0_16)))
stage0_OBJ += $(addprefix $(fold_name_32)/, $(addsuffix .o, $(stage0_32)))
stage0_OBJ += $(addprefix $(fold_name_64)/, $(addsuffix .o, $(stage0_64)))

stage1_OBJ := $(addprefix $(fold_name_16)/, $(addsuffix .o, $(stage1_16)))
stage1_OBJ += $(addprefix $(fold_name_32)/, $(addsuffix .o, $(stage1_32)))
stage1_OBJ += $(addprefix $(fold_name_64)/, $(addsuffix .o, $(stage1_64)))

CPPFLAGS+=-I include
$(fold_name_16)/%.o: ASFLAGS+=-m16 -O0
$(fold_name_16)/%.o: CFLAGS+=-m16 -Os
$(fold_name_32)/%.o: ASFLAGS+=-m32 -O0
$(fold_name_32)/%.o: CFLAGS+=-m32 -Os

LDFLAGS+=-m elf_i386

$(addprefix $(fold_name_16)/, $(common_src)):
	ln -sf $(addprefix ../src-common/, $(common_src)) $(fold_name_16)/

$(addprefix $(fold_name_32)/, $(common_src)):
	ln -s $(addprefix ../src-common/, $(common_src)) $(fold_name_32)/

$(addprefix $(fold_name_64)/, $(common_src)):
	ln -s $(addprefix ../src-common/, $(common_src)) $(fold_name_64)/

stage0.elf: $(stage0_OBJ)
	$(LD) $(LDFLAGS) -r -o $@ $^

stage1.elf: $(stage1_OBJ) ${EXTRA_OBJECT}
	$(LD) $(LDFLAGS) -r -o $@ $^

hello64.elf: stage0.elf stage1.elf
	$(LD) $(LDFLAGS) -T hello64.ld -o $@ $^

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

hello64: hello64.elf
	$(OBJCOPY) -O binary $^ $@

check: test/test.o
	EXTRA_OBJECT=test/test.o $(MAKE) hello64

clean:
	$(RM) hello64 hello64.bin hello64.elf
	$(RM) stage0.bin stage1.bin stage0.elf stage1.elf
	$(RM) $(stage0_OBJ) $(stage1_OBJ)
	$(RM) test/test.o
	$(RM) $(addprefix $(fold_name_16)/, $(common_src))
	$(RM) $(addprefix $(fold_name_32)/, $(common_src))
	$(RM) $(addprefix $(fold_name_64)/, $(common_src))

dump: hello64
	hexdump -C hello64

S?=16

open_elf: hello64.elf
	objdump -d -Maddr$S,data$S,$M $^

open_bin: hello64
	objdump -D -b binary -m i386 -Maddr$S,data$S,$M $^
