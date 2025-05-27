# USB库的移植
## USB库文件列表
- USB_driver：USB驱动程序

| 文件名称          | 功能描述               |
|------------------|----------------------|
| usb_core.c/h     | USB 核心文件           |
| usbd_core.c/h    | USB 设备核心库         |
| usbd_int.c/h     | USB 中断处理           |
| usbd_sdr.c/h     | USB 标准请求处理       |
| usbd_std.h       | USB 标准头文件         |

- USB_HID：USB HID类驱动程序


| 文件名称                | 功能描述                             |
|------------------------|------------------------------------|
| hid_iap_class.c/h       | HID IAP 数据处理接口                 |
| hid_iap_desc.c/h        | HID IAP 设备描述                     |
| keyboard_class.c/h     | Keyboard 数据处理接口               |
| keyboard_desc.c/h      | Keyboard 设备描述                   |
| mouse_class.c/h        | Mouse 数据处理接口                  |
| mouse_desc.c/h         | Mouse 设备描述                      |
| msc_class.c/h          | Mass Storage 数据处理接口           |
| msc_bot_scsi.c/h       | Bulk-Only Transfer 和 SCSI 命令处理接口 |
| msc_desc.c/h           | Mass Storage 设备描述               |
| printer_class.c/h      | Printer 数据处理接口                |
| printer_desc.c/h       | Printer 设备描述                    |
| audio_hid_class.c/h    | Audio 和 HID 复合设备数据处理接口   |
| audio_hid_desc.c/h     | Audio 和 HID 复合设备描述           |

## 移植步骤
- 移植USB的所有驱动程序
- 移植USB HID类驱动程序的msc的所有.c和.h文件
- 在msc_diskio.c中实现磁盘读写函数与磁盘的容量描述

## 排错指南
- 如果出现无法识别的USB设置，检查时钟配置，对于at32f4xx系列的芯片，需要在clock_config.h中
  配置PLL时钟使USB能够正常工作。
代码示例如下：

```c
    crm_pll_config(CRM_PLL_SOURCE_HEXT_DIV, CRM_PLL_MULT_30, CRM_PLL_OUTPUT_RANGE_GT72MHZ);
```