#!/bin/sh
FIP_PATH=./fip_tool/common

# build sbi.bin
pushd . && cd $FIP_PATH/debugloader/sbi && ./build.sh && popd

# build os.bin
cp $OUTPUT_PATH/yoc.bin $FIP_PATH/debugloader/duoRVOS/rtos.bin
pushd . && cd $FIP_PATH/debugloader/duoRVOS && make clean &&make && popd

# build fib.bin
cp $OUTPUT_PATH/sbi.bin $OUTPUT_PATH/os.bin $FIP_PATH/fip/
pushd . && cd $FIP_PATH/fip && make fsbl-build && popd
