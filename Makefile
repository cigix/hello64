OBJCOPY?=objcopy
CFLAGS+=-fno-pic -fpack-struct -fno-builtin -mgeneral-regs-only
CFLAGS+=-Wall -Wextra

# Files to be put on the first sector of the floppy device
stage0_16 := boot load_stage1 putc_bios read_floppy
stage0_32 :=
stage0_64 :=
# Files to be put on the remaining sectors of the floppy device
# They will be loaded at 0x7E00, right after the first stage
stage1_16 := enable_line_20 load_stage2 to_protected unreal
stage1_32 := reload_segments_32 setup_idt64 setup_paging64 simple_gdt32 to_long
stage1_64 := paging idt64 putc_serial reload_segments_64 simple_gdt64
# Files to be put on the remaining sectors of the floppy device
# They will be loaded at 0x100000, in the upper memory
stage2_16 :=
stage2_32 :=
stage2_64 := call_win weak_win

fold_name_16 := src16-real
fold_name_32 := src32-protected
fold_name_64 := src64-long

%/idt32.s %/idt64.s: CFLAGS+=-mno-red-zone
%/setup_idt64.s: CFLAGS+=-Wno-pointer-to-int-cast
%/setup_paging64.s: CFLAGS+=-Wno-int-to-pointer-cast

##########

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
stage0_OBJ += $(addprefix $(fold_name_32)/, $(addsuffix _32.o, $(stage0_32)))
stage0_OBJ += $(addprefix $(fold_name_64)/, $(addsuffix .o, $(stage0_64)))

stage1_OBJ := $(addprefix $(fold_name_16)/, $(addsuffix .o, $(stage1_16)))
stage1_OBJ += $(addprefix $(fold_name_32)/, $(addsuffix _32.o, $(stage1_32)))
stage1_OBJ += $(addprefix $(fold_name_64)/, $(addsuffix .o, $(stage1_64)))

stage2_OBJ := $(addprefix $(fold_name_16)/, $(addsuffix .o, $(stage2_16)))
stage2_OBJ += $(addprefix $(fold_name_32)/, $(addsuffix _32.o, $(stage2_32)))
stage2_OBJ += $(addprefix $(fold_name_64)/, $(addsuffix .o, $(stage2_64)))

CPPFLAGS+=-I include
ASFLAGS+=--64 -g
$(fold_name_16)/%.s: CFLAGS+=-m16 -Os -g0 -fno-dwarf2-cfi-asm
$(fold_name_32)/%.s: CFLAGS+=-m32 -Os -g0 -fno-dwarf2-cfi-asm
$(fold_name_64)/%.s: CFLAGS+=-m64 -Os -g3

# Disable implicit rules .c->.o and .S->.o
%.o: %.c

%.o: %.S

%.s: %.c
	@echo -e "  CC\t$@"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -S -o $@ $^

%.s: %.S
	@echo -e "  CPP\t$@"
	@$(CC) $(CPPFLAGS) -E -o $@ $^

%_32.s: %.s
	@echo -e "  FIX\t$@"
	@sed '1i\	.code32' $^ > $@

%.o: %.s
	@echo -e "  AS\t$@"
	@$(AS) $(ASFLAGS) -o $@ $^

stage0.elf: $(stage0_OBJ)
	@echo -e "  LD\t$@"
	@$(LD) $(LDFLAGS) -r -o $@ $^

stage1.elf: $(stage1_OBJ)
	@echo -e "  LD\t$@"
	@$(LD) $(LDFLAGS) -r -o $@ $^

stage2.elf: ${EXTRA_OBJECT} $(stage2_OBJ)
	@echo -e "  LD\t$@"
	@$(LD) $(LDFLAGS) -r -o $@ $^

hello64.elf: stage0.elf stage1.elf stage2.elf
	@echo -e "  LD\t$@"
	@$(LD) $(LDFLAGS) -T hello64.ld

%.bin: %.elf
	@echo -e "  BIN\t$@"
	@$(OBJCOPY) -O binary $^ $@

hello64: hello64.elf
	@echo -e "  BIN\t$@"
	@$(OBJCOPY) -O binary $^ $@

check:
	$(MAKE) -f /dev/null test/test.o
	EXTRA_OBJECT=test/test.o $(MAKE) hello64

clean:
	$(RM) hello64 hello64.bin hello64.elf
	$(RM) stage0.bin stage1.bin stage0.elf stage1.elf stage2.bin stage2.elf
	$(RM) $(stage0_OBJ) $(stage1_OBJ) $(stage2_OBJ)
	$(RM) test/test.o

dump: hello64
	hexdump -C hello64

S?=64

open_elf: hello64.elf
	objdump -d -Maddr$S,data$S,$M $D $^

open_bin: hello64
	objdump -D -b binary -m i386 -Maddr$S,data$S,$M $^
