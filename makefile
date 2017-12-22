# Target list
.PHONY: list
list:
	@echo 'target(deps)       : Description'
	@echo
	@echo 'kernel-r           : Builds the kernel'
	@echo 'kernel-d           : Builds the kernel with debug symbols'
	@echo 'vdisk              : Builds the virtual disk VM image from ./disk/'
	@echo 'vdisk-r (kernel)   : Builds the virtual disk VM image from ./disk/'
	@echo 'vdisk-d (kernel-d) : Buidls the virtual disk VM image from ./disk/'
	@echo 'qemu(vdisk)        : Boots the created VM disk'
	@echo 'qemu-r(vdisk-r)    : Boots the created VM disk'
	@echo 'qemu-d(vdisk-d)    : Boots the created VM disk'
	@echo 'badclean           : Cleans up everything from a bad vdisk build'
	@echo 'clean              : Cleans up all intermediate files'
	@echo 'distclean(clean)   : Cleans up all distributable files'
	@echo

# Build the kernel
.PHONY: kernel-r
kernel-r:
	make -C src kernel-r
	cp -f ./src/kernel.elf ./disk/boot/
.PHONY: kernel-d
kernel-d:
	make -C src kernel-d
	cp -f ./src/kernel.elf ./disk/boot/

# Build the virtual disk image
.PHONY: vdisk
vdisk:
# Create the virtual disk image
	dd if=/dev/zero of=./disk.img bs=1M count=64

# Create GPT
	./make_gpt.sh ./disk.img

# Create loopback device and format
	sudo losetup -P /dev/loop0 ./disk.img
	sudo mkfs.ext2 /dev/loop0p2

# Mount disk image and set ownership
	mkdir disk.img.d
	sudo mount /dev/loop0p2 ./disk.img.d
	sudo chown -R code:code ./disk.img.d

# Copy disk files
	cp -r disk/* disk.img.d/

# Install grub
	sudo grub-install --target=i386-pc --root-directory=./disk.img.d --boot-directory=./disk.img.d/boot --themes= --no-floppy --modules="normal part_gpt ext2 multiboot2" /dev/loop0

# Unmount disk
	sudo umount disk.img.d
	rmdir disk.img.d
	sudo losetup -d /dev/loop0

.PHONY: vdisk-r
vdisk-r: kernel-r vdisk
.PHONY: vdisk-d
vdisk-d: kernel-d vdisk

.PHONY: qemu
qemu: vdisk
	qemu-system-i386 -drive 'file=disk.img,format=raw'

.PHONY: qemu-r
qemu-r: vdisk-r qemu
.PHONY: qemu-d
qemu-d: vdisk-d
	qemu-system-i386 -S -s -drive 'file=disk.img,format=raw'

# Cleans up from a bad/partial disk build
.PHONY:bad-clean
bad-clean:
	sudo umount disk.img.d || true
	sudo losetup -d /dev/loop0 || true
	rmdir disk.img.d || true

# Clean
.PHONY: clean
clean:
	make -C src clean

# Clean all
.PHONY: distclean
distclean: clean
	make -C src distclean
	rm -f *.img
