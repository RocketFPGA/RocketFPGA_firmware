
CROSS=/opt/riscv32i
PREFIX=$(CROSS)/bin/riscv32-unknown-elf-
DEV=/dev/serial/by-id/usb-Luis_Pablo_RocketFPGA_2018-3-17-if00

all: rocket_fw.bin

rocket_fw.elf: startup/sections.lds firmware.c startup/start.c libs/*.c
	$(PREFIX)gcc -O3 -fno-builtin -march=rv32i -mabi=ilp32 -Tstartup/sections.lds -Wl,-Map=rocket_fw.map -Wl,-gc-sections -nostdlib  -o rocket_fw.elf startup/start.c startup/vector.S firmware.c libs/rocketfpga.c libs/rocketprotocol.c libs/printf.c $(CROSS)/lib/gcc/riscv32-unknown-elf/8.2.0/libgcc.a 

rocket_fw.bin: rocket_fw.elf
	$(PREFIX)objcopy -O binary rocket_fw.elf rocket_fw.bin

clean:
	rm -f rocket_sections.lds rocket_fw.elf rocket_fw.bin rocket_fw.map

flash: rocket_fw.bin
	python3 /workspaces/RocketFPGA/RocketFPGA/Software/CodeUploader/fpga_upload.py -d $(DEV) -f rocket_fw.bin -o 1048576
