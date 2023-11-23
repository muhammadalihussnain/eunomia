APP=ring_buffer

# this is for using distribution provided bpftool
BPFTOOL="bpftool"

# Change these to wherever you keep
# your mainline and stable kernel
LINUX_KERNEL_DIR_MAINLINE=/code/mainline
LINUX_KERNEL_DIR_STABLE=/code/linux-stable
LINUX_KERNEL_DIR_BPF_NEXT=/code/bpf-next

# change this to set whether you want to use
# mainline or stable
#LINUX_KERNEL_DIR=$(LINUX_KERNEL_DIR_MAINLINE)
#LINUX_KERNEL_DIR=$(LINUX_KERNEL_DIR_STABLE)
LINUX_KERNEL_DIR=$(LINUX_KERNEL_DIR_BPF_NEXT)
BPFTOOL=$(LINUX_KERNEL_DIR)/tools/bpf/bpftool/bpftool

.PHONY: $(APP)
$(APP): skel
	gcc -o ring_buffer ring_buffer.c -lbpf -Wl,-rpath=/usr/lib64/

.PHONY: vmlinux
vmlinux:
	bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h

.PHONY: bpf
bpf: vmlinux
	clang -g -O3 -target bpf -D__TARGET_ARCH_x86 -c ring_buffer.bpf.c -o ring_buffer.bpf.o

.PHONY: skel
skel: bpf
	bpftool gen skeleton ring_buffer.bpf.o name ring_buffer > ring_buffer.skel.h

.PHONY: run
run: $(APP)
	sudo ./$(APP)

.PHONY: clean
clean:
	-rm -rf *.o *.skel.h vmlinux.h $(APP)

.PHONY: gdb
gdb: bpf
	sudo gdb --args bpftool gen skeleton ring_buffer.bpf.o name ring_buffer