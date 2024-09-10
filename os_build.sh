mkdir -p build
echo "Building kernel..."
echo "Building kernel.asm..."
cd build
nasm -f elf32 -o kasm.o ../src/asm/kernel.asm
echo "Building kernel.cpp..."
g++ -m32 -fno-pic -ffreestanding -nostdlib  -c ../src/cpp/kernel_main.cpp -o kernel_main.o
echo "linking..."
ld -m elf_i386 -T "../link.ld" -o kernel.bin kasm.o kernel_main.o
qemu-system-x86_64  -kernel kernel.bin


