mkdir -p fw_bin

${SRCTREE}/host-tools/Xuantie-900-gcc-elf-newlib-x86_64-V2.6.1/bin/riscv64-unknown-elf-gcc -nostdlib -c entry.s -o entry.o
${SRCTREE}/host-tools/Xuantie-900-gcc-elf-newlib-x86_64-V2.6.1/bin/riscv64-unknown-elf-gcc -nostdlib -c main.c -o main.o
${SRCTREE}/host-tools/Xuantie-900-gcc-elf-newlib-x86_64-V2.6.1/bin/riscv64-unknown-elf-ld -o fw_bin/sbi.elf -Tlink.ld entry.o main.o
${SRCTREE}/host-tools/Xuantie-900-gcc-elf-newlib-x86_64-V2.6.1/bin/riscv64-unknown-elf-objcopy -Obinary fw_bin/sbi.elf fw_bin/sbi.bin
${SRCTREE}/host-tools/Xuantie-900-gcc-elf-newlib-x86_64-V2.6.1/bin/riscv64-unknown-elf-objdump -D fw_bin/sbi.elf > fw_bin/sbi.asm

cp -arf fw_bin/sbi.bin $OUTPUT_PATH
