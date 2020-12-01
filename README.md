# RA8876
My RA8876 Driver, mostly optimized for the 10.1" BuyDisplay.com TFT.

So far I've been testing it on a ATxmega384C3, which ran smooth and fast via the 8-bit interface. The library is mostly set for 16-bit colors. Required I/Os can be easily changed in the RA8876.h file, thus adopting it to a different controller shouldn't be hard.

RA8876.h, RA8876.c and RA8876_reg.h are required for the library to work.

Check out the attached main.c to see how it should be used. The functions of the library are not very high level and will be added later in a seperate library. Some examples of such higher-level drivers (loading a image with RLE-compression via BTE, or moving such an image from one memory position to an other) are in the main.c file.

![Pic of what it has so far](https://github.com/gfcwfzkm/RA8876/blob/main/IMG_20201119_115328.jpg)
