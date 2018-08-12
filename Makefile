all: hello64

launch: hello64
	qemu-system-x86_64 -enable-kvm -fda hello64 -nographic -serial stdio \
		-monitor none

hello64: ${EXTRA_OBJECT}

check: test/test.o
	EXTRA_OBJECT=test/test.o $(MAKE) hello64

clean:
	$(RM) hello64 test/test.o
