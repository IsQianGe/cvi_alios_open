$SRCTREE/boards/tools/secureboot/fiptool.py -v genfip \
    './fip.bin' \
    --MONITOR_RUNADDR="0x0000000080000000" \
    --CHIP_CONF='build/cv1812cp_milkv_duo256m_sd/chip_conf.bin' \
    --NOR_INFO='FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF' \
    --NAND_INFO='00000000'\
    --BL2='build/cv1812cp_milkv_duo256m_sd/bl2.bin' \
    --DDR_PARAM='test/cv181x/ddr_param.bin' \
    --MONITOR='./sbi.bin' \
    --LOADER_2ND='./os.bin' \
    --compress='lzma'

cp ./fip.bin $OUTPUT_PATH
