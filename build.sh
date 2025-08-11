#!/bin/bash

#
#		殆ChatGPTに書かせたわ、Makefileマジで読めん。
#

set -e

DATE=$(date "+%Y-%m-%d_%H_%M_%S")
NAME="MazokuPic"
BUILD_DIR="build"
KOS_BASE=/opt/toolchains/dc/kos
CFLAGS="-I${KOS_BASE}/addons/zlib -Wall -g"

echo "Building..."
echo "Date $DATE"

mkdir -p $BUILD_DIR
mkdir -p $BUILD_DIR/cd_root

# 一旦クリーンアップ
rm -f $BUILD_DIR/*.cdi
rm -f $BUILD_DIR/cd_root/*.BIN
rm -f $BUILD_DIR/*.elf
rm -f $BUILD_DIR/*.bin
rm -f $BUILD_DIR/*.BIN
rm -f $BUILD_DIR/*.c
rm -f $BUILD_DIR/*.o

# romdisk.imgが$BUILD_DIRにある想定
ROMDISK_IMG="$BUILD_DIR/romdisk.img"

# bin2cでromdisk.imgからromdisk_tmp.c生成（$BUILD_DIR内）
bin2c "$ROMDISK_IMG" "$BUILD_DIR/romdisk_tmp.c" romdisk

# romdisk_tmp.cコンパイル→romdisk.o作成（$BUILD_DIR内）
sh-elf-gcc -O2 -fno-PIC -fno-PIE -fomit-frame-pointer -m4-single -ml -mfsrra -mfsca -ffunction-sections -fdata-sections -matomic-model=soft-imask -ftls-model=local-exec -D__DREAMCAST__ \
  -I${KOS_BASE}/include \
  -I${KOS_BASE}/kernel/arch/dreamcast/include \
  -I${KOS_BASE}/addons/include \
  -I${KOS_BASE}/addons/include/dreamcast \
  -I${KOS_BASE}/../kos-ports/include \
  -D_arch_dreamcast -D_arch_sub_pristine -Wall -g \
  -c "$BUILD_DIR/romdisk_tmp.c" -o "$BUILD_DIR/romdisk_tmp.o"

sh-elf-gcc -o "$BUILD_DIR/romdisk.o" -r "$BUILD_DIR/romdisk_tmp.o" \
  -L${KOS_BASE}/lib/dreamcast -L${KOS_BASE}/addons/lib/dreamcast -L${KOS_BASE}/../kos-ports/lib \
  -Wl,--whole-archive -lromdiskbase

rm -f "$BUILD_DIR/romdisk_tmp.c" "$BUILD_DIR/romdisk_tmp.o"

# MazokuPic.cコンパイル（$BUILD_DIR内にMazokuPic.oを置く）
kos-cc -c MazokuPic.c -o "$BUILD_DIR/MazokuPic.o" $CFLAGS

# リンク
kos-cc -o "$BUILD_DIR/$NAME.elf" "$BUILD_DIR/MazokuPic.o" "$BUILD_DIR/romdisk.o" -lpng -lz -lm

# ELF→BIN変換
elf2bin "$BUILD_DIR/$NAME.elf"

# BINをスクランブルしてCDルートにコピー
scramble "$BUILD_DIR/$NAME.bin" "$BUILD_DIR/cd_root/1ST_READ.BIN"

# IPファイルを作成してCDイメージ生成
makeip -v -f -l iplogo.png ip.txt "$BUILD_DIR/IP.BIN"
makedisc "$BUILD_DIR/$DATE.cdi" "$BUILD_DIR/cd_root" "$BUILD_DIR/IP.BIN" Rumisan

echo "Build complete: $BUILD_DIR/$DATE.cdi"