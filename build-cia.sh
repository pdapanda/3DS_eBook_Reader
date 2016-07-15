rm -rf output/
rm *.3dsx
rm *.cia
rm -rf meta/banner.bin
rm -rf meta/data.smdh

make
cp output/3ds/3DSEBookReader/3DSEBookReader.3dsx 3DSEBookReader.3dsx

bannertool makebanner -i meta/banner.png -a meta/banner.wav -o meta/banner.bin
bannertool makesmdh -s "3DS Ebook Reader" -l "Read Ebooks on your 3DS!" -p "reworks" -i meta/icon.png -o meta/data.smdh
arm-none-eabi-strip output/3DSEBookReader.elf
makerom -f cia -o output/3DSEbookReader.cia -elf output/3DSEbookReader.elf -rsf meta/template.rsf -target t -exefslogo -icon meta/data.smdh -banner meta/banner.bin
cp output/3DSEbookReader.cia 3DSEbookReader.cia