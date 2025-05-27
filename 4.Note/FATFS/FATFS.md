# FAFS学习笔记
## FATFS简介
FATFS是一个为嵌入式系统设计的通用FAT文件系统，它支持FAT12、FAT16和FAT32文件系统，并且可以在多种微控制器和操作系统上运行。FATFS具有以下特点：
- 跨平台：FATFS可以在多种微控制器和操作系统上运行，包括ARM、AVR、PIC、MSP430、8051、Z80、x86、Windows、Linux、Mac OS X等。
- 简单易用：FATFS的API非常简单，易于使用，可以快速集成到嵌入式系统中。
- 高效：FATFS的文件操作非常高效，可以快速读写文件，支持大文件操作。
- 可靠：FATFS支持文件系统的一致性检查和修复，可以确保文件系统的可靠性和稳定性。

## FATFS的移植和使用
### FATFS的移植
FATFS的安装和使用非常简单，只需要将FATFS的源代码文件添加到项目中，并且在diskor.c文件中实现FATFS的底层接口即可。接口主要包括：
- disk_initialize：初始化磁盘
- disk_status：获取磁盘状态
- disk_read：读取磁盘数据
- disk_write：写入磁盘数据
- disk_ioctl：执行磁盘控制命令
#### FATFS在LVGL中的移植
- 首先需要在lv_conf.h文件中配置FATFS的相关参数。
```c
#define LV_USE_FS_FATFS  1
#if LV_USE_FS_FATFS
    #define LV_FS_FATFS_LETTER '0'     /* 设置你的操作盘符号 */
    #define LV_FS_FATFS_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif
```
- 然后在lv_fs_fatfs.c文件中的fs_init(void)函数实现挂载文件系统。
- 使用时，文件路径需要加上盘符，如"0:/test.txt"。

### FATFS的API使用
- f_mount：挂载文件系统
```c
FATFS fs;
FRESULT res = f_mount(&fs, "0:", 1);
if (res != FR_OK) {
    // 处理错误
}
```
- f_mkdir：格式化文件系统
```c
BYTE work[FF_MAX_SS];
FRESULT res = f_mkfs("0:",0,work,sizeof(work));
if (res != FR_OK) {
    // 处理错误
}
```
- f_unmount：卸载文件系统
```c
FRESULT res = f_unmount("0:");
if (res != FR_OK) {
    // 处理错误
}
```
- f_open：打开文件

```c
FIL file;
FRESULT res = f_open(&file, "0:test.txt", FA_OPEN_ALWAYS | FA_WRITE);
if (res != FR_OK) {
    // 处理错误
}
```
打开文件的类型包括：
- FA_READ：只读
- FA_WRITE：只写
- FA_OPEN_EXISTING：打开已存在的文件，若文件不存在则返回错误
- FA_CREATE_NEW：创建新文件，若文件已存在则返回错误
- FA_CREATE_ALWAYS：创建新文件，如果文件已存在则覆盖
- FA_OPEN_ALWAYS：打开文件，如果文件不存在则创建	
- FA_OPEN_APPEND：打开文件，文件内容会被追加到文件末尾，即使文件已经存在，所有写入操作都会发生在文件的末尾，而不是覆盖文件的现有内容。

```c
BYTE buffer[] = "Hello, FATFS!";
UINT bytes_written;
FRESULT res = f_write(&file, buffer, sizeof(buffer), &bytes_written);
if (res != FR_OK) {
    // 处理错误
}
```
- f_read：读取文件
```c
BYTE buffer[256];
UINT bytes_read;
FRESULT res = f_read(&file, buffer, sizeof(buffer), &bytes_read);
if (res != FR_OK) {
    // 处理错误
}
 /* UINT bytes_read;
 读取的时候bytes_read表示文件内的剩余内容
 如果文件中剩余 100 字节，bytes_read 将会是 100。
 如果文件中没有剩余内容，bytes_read 将会是 0。 */
```
- f_close：关闭文件
```c
FRESULT res = f_close(&file);
if (res != FR_OK) {
    // 处理错误
}
```
- f_mount：卸载文件系统
```c
FRESULT res = f_mount(NULL, "0:", 1);
if (res != FR_OK) {
    // 处理错误
}
```
## 排错指南
- 若发生FR_NO_FILESYSTEM错误，则表示文件系统未格式化，需要先格式化文件系统，如果格式化后还没有有文件系统，检查挂载的外设的读写接口是否正常。
- 如果文件系统的接口已经测试正常，但是在后续运行时无法读写，检查驱动设置，看看是否在程序的哪个地方让存储设备的读写接口失效。
- 测试的时候读一个地址的图片能正常显示，后续读图片的时候如果没有图片出现则表示文件系统被卸载掉了
- 如果读图片出现no data表示存储设备的读写接口失效，需要检查驱动设置。
    [![pE3A16e.png](https://s21.ax1x.com/2025/02/26/pE3A16e.png)](https://imgse.com/i/pE3A16e)

## 总结
FATFS是一个简单易用、高效可靠的文件系统，适用于嵌入式系统。通过实现FATFS的底层接口，可以快速集成FATFS到项目中，实现文件操作和文件系统管理。在使用FATFS时需要注意文件名大小写敏感、文件路径长度限制和文件系统一致性检查等问题。