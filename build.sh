DATE=`date "+%Y-%m-%d_%H_%M_%S"`
NAME="MazokuPic"
BUILD_DIR="build"

echo "Building..."
echo "Date $DATE"

mkdir -p $BUILD_DIR
mkdir -p $BUILD_DIR/cd_root

#一旦クリーンアップ
rm $BUILD_DIR/*.cdi
rm $BUILD_DIR/cd_root/*.BIN
rm $BUILD_DIR/*.elf
rm $BUILD_DIR/*.bin
rm $BUILD_DIR/*.BIN

make
make dist

#MakeFileが生成しやがったimgとelfを移動
mv $NAME.elf $BUILD_DIR/$NAME.elf
mv romdisk.img $BUILD_DIR/romdisk.img

#ELFしたり色々
elf2bin $BUILD_DIR/$NAME.elf
scramble $BUILD_DIR/$NAME.bin $BUILD_DIR/cd_root/1ST_READ.BIN

#IPを作成して、cdiファイルを生成
makeip -v -f -l iplogo.png ip.txt build/IP.BIN
makedisc $BUILD_DIR/$DATE.cdi $BUILD_DIR/cd_root $BUILD_DIR/IP.BIN Rumisan