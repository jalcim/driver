sudo dmesg -C
cd build
make clean
make
sudo insmod devalloc.ko
cd ..
gcc main/main.c
sudo ./a.out
sudo rmmod devalloc
dmesg
