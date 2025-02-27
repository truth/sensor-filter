#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdlib.h>
#include <time.h>

#define DISPLAY_HEIGHT 20
#define DISPLAY_WIDTH 60
#define DATA_UPDATE_INTERVAL 1  // 数据更新间隔(秒)
#define RAW_CSV_FILENAME "raw_data.csv"    // 原始数据CSV文件名
#define FILTERED_CSV_FILENAME "filtered_data.csv"  // 过滤后数据CSV文件名

// ANSI颜色代码
#define COLOR_RESET   "\x1b[0m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RED     "\x1b[31m"

typedef struct {
    int* data;
    size_t size;
    int max_value;
    int min_value;
    time_t last_update;    // 上次更新时间
    int value_range[2];    // 随机值范围 [min, max]
    double* smooth_data;   // 平滑后的数据
    FILE* raw_csv_file;    // 原始数据CSV文件指针
    FILE* filtered_csv_file; // 过滤后数据CSV文件指针
} chart_data_t;

// 初始化图表数据
chart_data_t* chart_init(int min_value, int max_value);

// 添加数据点
int chart_add_point(chart_data_t* chart, int value);

// 自动更新数据并显示图表
void chart_auto_update(chart_data_t* chart);

// 显示图表
void chart_display(chart_data_t* chart);

// 销毁图表
void chart_destroy(chart_data_t* chart);

// 新增：记录过滤后的数据
void chart_log_filtered_data(chart_data_t* chart, int value, int is_filtered);

#endif