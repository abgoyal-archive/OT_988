#!/bin/bash
#make LINUXDIR=${ANDROID_BUILD_TOP}/kernel CROSS_COMPILE=${ANDROID_EABI_TOOLCHAIN}/arm-eabi- ARCH=arm dhd-cdc-sdmmc-gpl-debug
#make ARCH=arm CROSS_COMPILE=/home/hpyu/Android/ponyo/prebuilt/linux-x86/toolchain/arm-eabi-4.2.1/bin/arm-eabi- LINUXDIR=/home/hpyu/Android/ponyo/out/target/product/msm7630_surf/obj/KERNEL_OBJ dhd-cdc-sdmmc-gpl-debug

make LINUXVER=2.6.35.7 ARCH=arm CROSS_COMPILE=/media/Ubuntu/gitclient/tequila_0721/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi- LINUXDIR=/media/Ubuntu/gitclient/tequila_0721/out/target/product/msm7627_ffa/obj/KERNEL_OBJ dhd-cdc-sdmmc-gpl-debug

#make -j4 ARCH=arm CROSS_COMPILE=/home/hpyu/Android/ponyo/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi- LINUXDIR=/home/hpyu/Android/wifi/kernel-pongo dhd-cdc-sdmmc-gpl-debug

