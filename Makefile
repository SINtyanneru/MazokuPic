TARGET = MazokuPic.elf
OBJS = MazokuPic.o romdisk.o 
KOS_ROMDISK_DIR = romdisk

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

KOS_LOCAL_CFLAGS = -I$(KOS_BASE)/addons/zlib
	
clean: rm-elf
	-rm -f $(OBJS)
	
rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS) 
	kos-cc src/* -o $(TARGET) $(OBJS) -lpng -lmp3 -loggvorbisplay -lvorbis -logg -lz -lm

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist: $(TARGET)
	-rm -f $(OBJS) romdisk_boot.img
	$(KOS_STRIP) $(TARGET)
