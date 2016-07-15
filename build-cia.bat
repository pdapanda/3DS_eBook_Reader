rm -rf output/
rm *.3dsx
rm *.cia
rm -rf meta/banner.bin
rm -rf meta/data.smdh

make
COPY output\3ds\3DSEBookReader\3DSEBookReader.3dsx 3DSEBookReader.3dsx

bannertool makebanner -i meta/banner.png -a meta/banner.wav -o meta/banner.bin
smdhtool --create "3DS Ebook Reader" "Read Ebooks on your 3DS!" "reworks" meta/icon.png  meta/data.smdh
arm-none-eabi-strip output/3DSEBookReader.elf
makerom -f cia -o output/3DSEbookReader.cia -elf output/3DSEbookReader.elf -rsf meta/template.rsf -target t -exefslogo -icon meta/data.smdh -banner meta/banner.bin
COPY output\3DSEbookReader.cia 3DSEbookReader.cia