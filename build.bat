rm -rf output/
rm *.3dsx
rm *.cia

make
COPY output\3ds\3DSEBookReader\3DSEBookReader.3dsx 3DSEBookReader.3dsx