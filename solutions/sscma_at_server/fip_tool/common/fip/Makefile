
.PHONY: memory-map

TOP_DIR=$(shell pwd)
export BUILD_PATH=${TOP_DIR}/build

CVI_BOARD_MEMMAP_H_PATH := ${TOP_DIR}/build/output/cvi_board_memmap.h
CVI_BOARD_MEMMAP_CONF_PATH := ${TOP_DIR}/build/output/cvi_board_memmap.conf
CVI_BOARD_MEMMAP_LD_PATH := ${TOP_DIR}/build/output/cvi_board_memmap.ld

BOARD_MMAP_PATH := build/scripts/memmap.py
MMAP_CONV_PY := build/scripts/mmap_conv.py


${CVI_BOARD_MEMMAP_H_PATH}: ${BOARD_MMAP_PATH} ${MMAP_CONV_PY}
	$(call print_target)
	mkdir -p $(dir $@)
	@${MMAP_CONV_PY} --type h $< $@

${CVI_BOARD_MEMMAP_CONF_PATH}: ${BOARD_MMAP_PATH} ${MMAP_CONV_PY}
	$(call print_target)
	@mkdir -p $(dir $@)
	@${MMAP_CONV_PY} --type conf $< $@

${CVI_BOARD_MEMMAP_LD_PATH}: ${BOARD_MMAP_PATH} ${MMAP_CONV_PY}
	$(call print_target)
	@mkdir -p $(dir $@)
	@${MMAP_CONV_PY} --type ld $< $@

ifeq ($(wildcard ${BOARD_MMAP_PATH}),)
memory-map:
else
memory-map: ${CVI_BOARD_MEMMAP_H_PATH} ${CVI_BOARD_MEMMAP_CONF_PATH} ${CVI_BOARD_MEMMAP_LD_PATH}
	mkdir -p ${TOP_DIR}/build/output/$(CHIP_ARCH)
	cp ${CVI_BOARD_MEMMAP_LD_PATH} ${TOP_DIR}/build/output/${CHIP_ARCH}
endif

FSBL_OUTPUT_PATH = fsbl/build
fsbl%: export CHIP_ARCH=CV181X
fsbl%: export CVIARCH=CV181X
fsbl%: export BOOT_CPU=riscv
fsbl%: export LOADER_2ND_PATH=${TOP_DIR}/os.bin
fsbl%: export RTOS_DUMP_PRINT_ENABLE=y
fsbl%: export RTOS_DUMP_PRINT_SZ_IDX=17
fsbl%: export RTOS_FAST_IMAGE_TYPE=0
fsbl%: export RTOS_ENABLE_FREERTOS=y
fsbl%: export ARCH=riscv
fsbl%: export PROJECT_FULLNAME=cv1812cp_milkv_duo256m_sd
fsbl-build: memory-map
	$(call print_target)
	${Q}mkdir -p fsbl/build
	${Q}ln -snrf -t fsbl/build ${CVI_BOARD_MEMMAP_H_PATH}
	${Q}$(MAKE) BOOT_CPU=riscv ARCH=riscv CROSS_COMPILE=riscv64-unknown-elf- -C fsbl O=build \
		LOADER_2ND_PATH=${LOADER_2ND_PATH} MONITOR_PATH=${TOP_DIR}/sbi.bin
	${Q}cp fsbl/build/fip.bin build/

fsbl-clean:
	$(call print_target)
	${Q}$(MAKE) CROSS_COMPILE=riscv64-unknown-linux-musl- -C fsbl clean O=build
