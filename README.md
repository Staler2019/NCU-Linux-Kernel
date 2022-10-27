# Add system call to `kernel 3.10.104`

## Write Up

Please see https://hackmd.io/mkpUU-2JRSGZH3Blhp0LQQ

## VM Set Up

1. Make `Ubuntu 16.04.7 LTS` on `Virtual Box`
2. Install Necessary Packages
    ```.sh
    sudo apt update
    sudo apt install wget build-essential libncurses-dev libssl-dev libelf-dev bison flex -y
    ```
3. Download `Kernel 3.10.104`
    ```.sh
    wget https://mirrors.edge.kernel.org/pub/linux/kernel/v3.0/linux-3.10.104.tar.gz
    tar -xvf linux-3.10.104.tar.gz -C ~/Desktop
    ```.sh
4. Copy System Kernel Config 
    1. Check `uname -r` First
    2. Run the command and change `uname -r` with actual string
        ```.sh
        sudo cp /boot/config-`uname –r` .
        ```
5. Set Up Kernel Config
    ```.sh
    sudo make menuconfig                # eg. load config-`uname -r` and exit with save
    ```
6. Compile Kernel & Install
    ```.sh
    sudo make -j5
    sudo make modules_install -j5 && sudo make install -j5

    sudo nano /etc/default/grub         # GRUB_TIMEOUT=-1 # kernel 4.15.0-112 -> 3.10.104
    sudo update-grub
    reboot                              # and select your `kernel-3.10.104` in ubuntu advance option when booting
    ```

## Create Your Own System Call

> Please create a folder first, eg. custom_syscall

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
    core-y		+= kernel/ mm/ fs/ ipc/ security/ crypto/ block/
    # custom_syscall # add below after the above line
    core-y 		+= custom_syscall/
    ```
2. Add Custom System Call in the button of `include/linux/syscalls.h`
    ```.h
    // custom // add before `#endif`
    asmlinkage int helloworld(void); // 8787
    asmlinkage unsigned long sys_get_phy_addr(unsigned long vaddr); // 8788
    asmlinkage unsigned long sys_get_segment(unsigned long vaddr, void *out); // 8789
    #endif
    ```
3. Add Custom System Call in the button of `arch/x86/syscalls/syscall_32.tbl`
    ```.tbl
    # custom
    8787 i386   helloworld      sys_helloworld
    8788 i386   get_phy_addr    sys_get_phy_addr
    8789 i386   get_segment     sys_get_segment
    ```
4. Compile and Test Your Code by calling and getting the value of `syscall(syscall_index, arg...)`
