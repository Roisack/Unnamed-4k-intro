rm *.o
rm 4k_intro*

echo "gcc -m32 -Os -fomit-frame-pointer -c main.c shader.c surface.c gekko_math.c"
gcc -m32 -Os -fomit-frame-pointer -c main.c shader.c surface.c gekko_math.c
echo "ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 main.o shader.o surface.o gekko_math.o /usr/lib32/libSDL.so /usr/lib32/libGL.so -o 4k_intro"
ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 main.o shader.o surface.o gekko_math.o /usr/lib32/libSDL.so /usr/lib32/libGL.so /usr/lib32/libGLU.so -o 4k_intro
#ORIGINAL="$(du -b main.o |cut -c -5)"
#echo "Original size $ORIGINAL"

#SIZE1="$(du -b 4k_intro |cut -c -5)"
echo "strip -s -R .comment -R .gnu.version 4k_intro"
strip -s -R .comment -R .gnu.version 4k_intro
#SIZE2="$(du -b 4k_intro |cut -c -5)"
#SUM=$(($SIZE1 - $SIZE2))
#echo "$SUM bytes less"
#echo "sstrip 4k_intro"
sstrip 4k_intro
#SIZE3="$(du -b 4k_intro |cut -c -5)"
#SUM=$(($SIZE1 - $SIZE3))
#echo "$SUM bytes less"
echo "gzip -cn9 4k_intro >> 4k_intro_final"
#7z a -tGZip -mx=9 4k_intro.gz 4k_intro
cp unpack.header 4k_intro_final
gzip -cn9 4k_intro >> 4k_intro_final
#SIZE4="$(du -b 4k_intro_final |cut -c -4)"
#SUM=$(($SIZE1 - $SIZE4))
#echo "$SUM bytes less"
chmod +x 4k_intro_final

#echo ""
#echo "Original size: $ORIGINAL"
#echo "Final size: $SIZE4"
