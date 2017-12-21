# Target list
.PHONY: list
list:
	@echo Valid targets:
	@echo kernel : Builds the kernel
	@echo vdisk : Builds the virtual disk VM image
	@echo boot-qemu : All of the above and and boots qemu
	@echo clean : Cleans up all intermediate files
	@echo dist-clean : Cleans up all everything

# Build the kernel
.PHONY: kernel
kernel:
	make -C src kernel
	cp -f src/kernel.elf disk/boot/

# Build the virtual disk for the kernel
vdisk: kernel
# Create the virtual disk image
	dd if=/dev/zero of=./disk.img bs=1M count=8

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
	sudo grub-install --target=i386-pc --root-directory=./disk.img.d --no-floppy --modules="normal part_gpt ext2 multiboot2" /dev/loop0

# Unmount disk
	sudo umount disk.img.d
	rmdir disk.img.d
	sudo losetup -d /dev/loop0

.PHONY:boot-qemu
boot-qemu: vdisk
	qemu-system-i386 -drive 'file=disk.img,format=raw'

# Clean rule
.PHONY: clean
clean:
	make -C src clean

# Cleans all virtual device targets
.PHONY: distclean
distclean: clean
	make -C src dist-clean
	rm -rf *.img
