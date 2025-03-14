# 实时数据监控与过滤系统

这是一个基于C语言开发的实时数据监控系统，具有数据缓存、过滤和可视化功能。系统支持实时数据显示、数据过滤、平滑曲线绘制以及数据持久化存储。

## 功能特性

1. **数据缓存管理**
   - 高效的键值对存储
   - 动态内存管理
   - 自动垃圾回收

2. **数据过滤系统**
   - 支持自定义过滤规则
   - 最多保留50条历史数据
   - 自动移除旧数据

3. **实时数据可视化**
   - ASCII艺术风格图表显示
   - 平滑曲线绘制
   - 动态坐标轴
   - 彩色终端输出

4. **数据持久化**
   - CSV格式数据存储
   - 分别记录原始数据和过滤后的数据
   - 自动时间戳记录

## 系统要求

- Linux操作系统
- GCC编译器
- Make工具

## 目录结构 
<pre>
project/
├── include/ # 头文件
│ ├── cache.h # 缓存模块头文件
│ ├── filter.h # 过滤器模块头文件
│ └── display.h # 显示模块头文件
├── src/ # 源文件
│ ├── cache.c # 缓存模块实现
│ ├── filter.c # 过滤器模块实现
│ ├── display.c # 显示模块实现
│ └── main.c # 主程序
├── Makefile # 编译配置文件
└── README.md # 说明文档
</pre>

## 编译和运行

1. 编译程序：

``` bash 
make
``` 

2. 运行程序：

``` bash 
./bin/program
``` 

3. 清理编译文件：

``` bash 
make clean
```

## 使用说明

1. 程序启动后会自动生成随机数据并实时显示
2. 数据显示包括：
   - 实时曲线图
   - 当前值和数据范围
   - 过滤后的数据列表
3. 数据记录在两个CSV文件中：
   - raw_data.csv：原始数据
   - filtered_data.csv：过滤后的数据
4. 按Ctrl+C退出程序

## 数据格式

CSV文件格式：

``` csv
Timestamp,Value,IsFiltered
2024-01-20 10:30:45,42,0
2024-01-20 10:30:46,56,1
```

## 自定义配置

可以通过修改头文件中的宏定义来调整系统参数：

- `MAX_SERIES_SIZE`: 数据序列最大容量（默认50）
- `DISPLAY_HEIGHT`: 显示图表高度
- `DISPLAY_WIDTH`: 显示图表宽度
- `DATA_UPDATE_INTERVAL`: 数据更新间隔（秒）

## 注意事项

1. 确保终端支持ANSI颜色显示
2. 建议在全屏终端中运行以获得最佳显示效果
3. CSV文件会自动创建在程序运行目录下

## 开发者信息

- 开发语言：C
- 编译器：GCC
- 构建工具：Make
- 开发平台：Linux

## 许可证

[根据实际情况填写许可证信息]

