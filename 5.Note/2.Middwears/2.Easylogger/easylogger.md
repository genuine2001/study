# Easylogger库的移植与使用

## 1. 移植（裸机）
### 1.1 将port与src中的.c文件复制到工程目录下
### 1.2 添加inc的文件路径到工程的include路径中
### 1.3 配置elog_port.c中的输出接口
~~~c
void elog_port_output(const char *log, size_t size) {
    
    /* add your code here */
    printf("%.*s", size, log);
}
~~~
### 1.4 编译即可

## 2. 使用
### 2.1 调用elog_init()函数初始化日志系统
### 2.2 调用elog_set_fmt()函数设置日志格式
~~~c
   elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_TAG | ELOG_FMT_DIR | ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_TAG | ELOG_FMT_DIR | ELOG_FMT_FUNC | ELOG_FMT_LINE);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_TAG | ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
~~~
### 2.3 调用elog_start()函数启动日志系统

