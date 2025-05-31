# CmbackTrace(KEIL)
## CmbackTrace的移植
- 将cm_backtrace整个文件夹都添加到KEIL的工程中。
- 在工程的Include路径中添加cm_backtrace文件夹的路径。
- 在cmb_cfg.h中根据自己的需求配置
[![pE3NDiV.jpg](https://s21.ax1x.com/2025/02/27/pE3NDiV.jpg)](https://imgse.com/i/pE3NDiV)
- 在工程中添加cmb_fault.S文件，并将芯片厂商提供的HARDFAULT_Handler注释掉即可。
## CmbackTrace的使用
- 在main函数中添加宏定义代码：
```c
#define APPNAME                       "CmbackTrace"
#define HARDWARE_VERSION               "V1.0.0"
#define SOFTWARE_VERSION               "V0.1.0"
```
- 在main函数中添加初始化代码：
```c
cm_backtrace_init(APPNAME, HARDWARE_VERSION, SOFTWARE_VERSION);
```
- 在KEIL中的output窗口中需要将Name of Executable设为宏定义的APPNAME。
[![pE3N5i6.png](https://s21.ax1x.com/2025/02/27/pE3N5i6.png)](https://imgse.com/i/pE3N5i6)
- 之后如果发生HardFault异常，系统会自动进入CmbackTrace的异常处理函数，并打印出异常信息。
[![pE3NgsJ.jpg](https://s21.ax1x.com/2025/02/27/pE3NgsJ.jpg)](https://imgse.com/i/pE3NgsJ)
- 将CmbackTrace/Tool中的add2line.exe工具放到工程中的Object文件夹中，在文件路径运行cmd命令
[![pE3NHQe.jpg](https://s21.ax1x.com/2025/02/27/pE3NHQe.jpg)](https://imgse.com/i/pE3NHQe)
- 输入异常命令中的addr2lin命令，可以看到发生错误的代码文件和行数。
[![pE3NOeA.png](https://s21.ax1x.com/2025/02/27/pE3NOeA.png)](https://imgse.com/i/pE3NOeA)