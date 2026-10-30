BUILD=build
CC=gcc
AS=gcc
CFLAGS=-m32 -std=gnu99 -ffreestanding -O2 -Wall -Wextra -fno-stack-protector -fno-pie -fno-pic
LDFLAGS=-m32 -T linker.ld -nostdlib -no-pie

.PHONY: all run clean check
all: $(BUILD)/NovaOS.iso

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: src/boot.S | $(BUILD)
	$(AS) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.o: src/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.elf: $(BUILD)/boot.o $(BUILD)/kernel.o linker.ld
	$(CC) $(LDFLAGS) -o $@ $(BUILD)/boot.o $(BUILD)/kernel.o -lgcc

$(BUILD)/NovaOS.iso: $(BUILD)/kernel.elf iso/boot/grub/grub.cfg
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(BUILD)/kernel.elf $(BUILD)/iso/boot/kernel.elf
	cp iso/boot/grub/grub.cfg $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(BUILD)/iso

check: $(BUILD)/kernel.elf
	grub-file --is-x86-multiboot2 $(BUILD)/kernel.elf

run: all
	qemu-system-i386 -cdrom $(BUILD)/NovaOS.iso -m 128M

clean:
	rm -rf $(BUILD)
