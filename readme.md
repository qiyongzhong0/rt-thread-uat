# uAT软件包

## 1.简介

**uAT** 是一个微型的AT设备接口驱动，适用于半双工通信的轻量型AT设备驱动使用。

### 1.1目录结构

`uAT` 软件包目录结构如下所示：

``` 
uat
├───inc                     // 头文件目录
│   └───uat.h           	// API 接口头文件
├───src                     // 源码目录
│   └───uat.c               // 算法源文件
│   license                 // 软件包许可证
│   readme.md               // 软件包使用说明
└───SConscript              // RT-Thread 默认的构建脚本
```

### 1.2许可证

uAT package 遵循 LGPLv2.1 许可，详见 `LICENSE` 文件。

### 1.3依赖

- RT_Thread 4.0
- RS485

## 2.使用

### 2.1接口函数说明

#### uat_inst_t * uat_inst_create(const char *serial, int baudrate, int byte_tmo_ms, int bufsize);
- 功能 ：动态创建uat实例
- 参数 ：serial--串口设备名称
- 参数 ：baudrate--串口波特率
- 参数 ：byte_tmo_ms--字节超时时间，单位ms
- 参数 ：bufsize--收发缓冲区尺寸
- 返回 ：成功返回实例指针，失败返回NULL

#### void uat_inst_destory(uat_inst_t *hinst);
- 功能 ：销毁uat实例
- 参数 ：hinst--uat实例指针
- 返回 ：无

#### char *uat_get_buf(uat_inst_t *hinst);
- 功能 ：获取uat数据缓冲区指针
- 参数 ：hinst--uat实例指针
- 返回 ：缓冲区指针

#### void uat_set_end_sign(uat_inst_t *hinst, const char *end_sign);
- 功能 ：设置响应结束标识
- 参数 ：hinst--uat实例指针
- 参数 ：end_sign--结束标识字符串指针
- 返回 ：无

#### int uat_data_recv(uat_inst_t *hinst, char *buf, int size, int tmo_ms);
- 功能 ：从接口接收数据
- 参数 ：hinst--uat实例指针
- 参数 ：buf--接收数据缓冲区指针
- 参数 ：size--缓冲区尺寸
- 参数 ：tmo_ms--接收超时时间
- 返回 ：>=0--接收到的数据长度，<0--错误

#### int uat_data_send(uat_inst_t *hinst, const char *buf, int size);
- 功能 ：向接口发送数据
- 参数 ：hinst--uat实例指针
- 参数 ：buf--发送缓冲区指针
- 参数 ：size--发送数据长度
- 返回 ：>=0--发送的数据长度，<0--错误

#### const char * uat_search_keyword(const char *buf, int size, const char *kw);
- 功能 ：搜索数据缓冲区中的关键字
- 参数 ：buf--数据缓冲区指针
- 参数 ：size--数据长度
- 参数 ：kw--关键字字符串指针
- 返回 ：搜索到关键字则返回指向关键字所在位置的指针，未搜索到则返回NULL

#### const char * uat_get_line_by_kw(const char *buf, int size, const char *kw);
- 功能 ：获取关键字开始的一行数据
- 参数 ：buf--数据缓冲区指针
- 参数 ：size--数据长度
- 参数 ：kw--关键字字符串指针
- 返回 ：搜索到关键字则返回由关键字开始的行字符串指针，未搜索到则返回NULL

#### int uat_parse_line_args(const char *line, const char *expr, ...);
- 功能 ：解析行参数
- 参数 ：line--行字符串指针
- 参数 ：expr--格式字符串
- 参数 ：...--后跟可变参数
- 返回 ：解析成功的参数数量

#### int uat_execute_at_cmd(uat_inst_t *hinst, int tmo_ms, const char *cmd_expr, ...);
- 功能 ：执行AT指令
- 参数 ：hinst--uat实例指针
- 参数 ：tmo_ms--接收应答超时时间
- 参数 ：expr--格式字符串
- 参数 ：...--后跟可变参数
- 返回 ：>=0--返回应答的数据长度，<0--错误

### 2.3获取组件

- **方式1：**
通过 *Env配置工具* 或 *RT-Thread studio* 开启软件包，根据需要配置各项参数；配置路径为 *RT-Thread online packages -> miscellaneous packages -> quick pid* 


### 2.4配置参数说明

| 参数宏 | 说明 |
| ---- | ---- |
| UAT_USING_TEST    | 使用测试功能

## 3. 联系方式

* 维护：qiyongzhong
* 主页：https://gitee.com/qiyongzhong0/rt-thread-uat
* 邮箱：917768104@qq.com
