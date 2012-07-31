rm *.o
rm 4k_intro*

gcc -m32 -Os -c main.c shader.c surface.c gekko_math.c audio.c
ld -m elf_i386 -dynamic-linker /lib/ld-linux.so.2 main.o shader.o surface.o gekko_math.o audio.o /usr/lib32/libSDL.so /usr/lib32/libGLEW.so /usr/lib32/libasound.so -o 4k_intro_debug

chmod +x 4k_intro_debug

