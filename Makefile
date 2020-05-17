
CROSS=/opt/riscv32i
PREFIX=$(CROSS)/bin/riscv32-unknown-elf-
DEV=/dev/serial/by-id/usb-Luis_Pablo_RocketFPGA_2018-3-17-if00

rocket_fw.elf: sections.lds firmware.c start.c
	# $(PREFIX)gcc -march=rv32i -mabi=ilp32 -Wl,-Bstatic,-T,sections.lds,--strip-debug -ffreestanding -static-libgcc -lgcc -nostdlib -o rocket_fw.elf  start.S firmware.c $(CROSS)/lib/gcc/riscv32-unknown-elf/8.2.0/libgcc.a
	# $(PREFIX)gcc -march=rv32i -mabi=ilp32 -Tsections.lds -nostdlib  -o rocket_fw.elf start.S firmware.c $(CROSS)/lib/gcc/riscv32-unknown-elf/8.2.0/libgcc.a
	$(PREFIX)gcc -O3 -fno-builtin -march=rv32i -mabi=ilp32 -Tsections.lds -Wl,-Map=image.map -Wl,-gc-sections -nostdlib  -o rocket_fw.elf start.c vector.S firmware.c printf.c $(CROSS)/lib/gcc/riscv32-unknown-elf/8.2.0/libgcc.a
	$(PREFIX)objdump -f -s -d --source rocket_fw.elf > rocket_fw.map

rocket_fw.bin: rocket_fw.elf
	$(PREFIX)objcopy -O binary rocket_fw.elf rocket_fw.bin

clean:
	rm -f rocket_sections.lds rocket_fw.elf rocket_fw.bin rocket_fw.map

all: rocket_fw.bin
	python3 /workspaces/RocketFPGA/RocketFPGA/Software/CodeUploader/fpga_upload.py -d $(DEV) -f rocket_fw.bin -o 1048576
