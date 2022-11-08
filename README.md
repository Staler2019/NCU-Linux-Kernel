# Add System Call

> `Kernel 3.10.104`

## Write Up

Please see https://hackmd.io/mkpUU-2JRSGZH3Blhp0LQQ

## VM Set Up

> `Ubuntu 16.04.7 LTS` on `Virtual Box`

1. Install Necessary Packages

   ```.sh
   sudo apt update && sudo apt install wget build-essential libncurses-dev libssl-dev libelf-dev bison flex -y
   ```

2. Download `Kernel 3.10.104` & Extract it

   ```.sh
   wget https://mirrors.edge.kernel.org/pub/linux/kernel/v3.0/linux-3.10.104.tar.gz
   ```

   ```.sh
   tar -xvf linux-3.10.104.tar.gz -C ~/Desktop
   ```

   > `-C` extract to which folder

3. Set Up Kernel Config

   ```.sh
   sudo make menuconfig       # eg. load config-`uname -r` and exit with save
   ```

   - You can copy current configuration from device and load it

     ```.sh
     sudo cp /boot/config-`uname -r` .config
     ```

4. Compile Kernel & Install

   ```.sh
   sudo make -j5 && sudo make modules_install -j5 && sudo make install -j5
   ```

5. Update GRUB Setting

   ```.sh
   sudo update-grub
   ```

   > Because the kernel version from `4.15.0-112` to `3.10.104`, we do the following things before `update-grub`
   >
   > ```.sh
   > sudo nano /etc/default/grub
   > ```
   >
   > 1. Set `GRUB_TIMEOUT=-1`
   > 2. Comment out all items starting with `GRUB_HIDDEN_TIMEOUT_`

6. Reboot the Machine

   ```.sh
   reboot
   ```

   - Select `kernel-3.10.104` in ubuntu advance option when booting

## Create Your Own System Call

> :warning: Please create a folder in the kernel folder, eg. custom_syscall :warning:

1. Write Custom System Call and Add Them into Makefile

   ```MAKEFILE
   # file: custom_syscall/Makefile
   #
   # custom syscall file & *.c=*.o
   obj-y       := helloworld.o get_phy_addr.o get_segment.o
   ```

   ```MAKEFILE
   # file: Makefile
   #
   core-y      += kernel/ mm/ fs/ ipc/ security/ crypto/ block/
   # custom_syscall # add below after the above line
   core-y      += custom_syscall/
   ```

2. Add Custom System Call in the Button of `include/linux/syscalls.h`

   ```.h
   /* file: include/linux/syscalls.h
   */
   // custom // add before `#endif`
   asmlinkage int helloworld(void); // 8787
   asmlinkage unsigned long sys_get_phy_addr(unsigned long vaddr); // 8788
   asmlinkage unsigned long sys_get_segment(unsigned long vaddr, void *out); // 8789
   #endif
   ```

3. Add Custom System Call in the Button of `arch/x86/syscalls/syscall_32.tbl`

   ```.tbl
   # file: arch/x86/syscalls/syscall_32.tbl
   #
   # custom
   8787 i386   helloworld      sys_helloworld
   8788 i386   get_phy_addr    sys_get_phy_addr
   8789 i386   get_segment     sys_get_segment
   ```

4. Re-compile & Re-install Kernel

## Compile and Use Your System Calls

### Pre-require

If your are on amd64 installation, please

```.sh
sudo apt install libc6-dev-i386 -y
```

> [StackExchange Link](https://askubuntu.com/questions/470796/fatal-error-sys-cdefs-h-no-such-file-or-directory)

### Use Your System Call

1. Use your system call and get return value by using function `syscall(syscall_index, arg...)`
2. Compile your .c/.cpp files using

   ```.sh
   gcc -m32 project1.c -o project1 -lpthread -Wall -fPIE
   ```

3. To see `printk()` message, use `dmesg` command. Eg.

   ```.sh
   ./test_helloworld && dmesg | tail
   ```
