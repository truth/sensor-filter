#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "display.h"

#define SMOOTH_FACTOR 0.3  // 平滑因子
#define INTERPOLATION_POINTS 3  // 插值点数

static int generate_random_value(int min, int max) {
    return min + rand() % (max - min + 1);
}

// 使用指数平滑算法平滑数据
static void smooth_data(chart_data_t* chart) {
    if (!chart || chart->size < 2) return;

    for (size_t i = 1; i < chart->size; i++) {
        chart->smooth_data[i] = SMOOTH_FACTOR * chart->data[i] + 
                               (1 - SMOOTH_FACTOR) * chart->smooth_data[i-1];
    }
}

// 写入数据到CSV文件
static void write_to_csv(FILE* file, int value, int is_filtered) {
    if (!file) return;
    
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    fprintf(file, "%s,%d,%d\n", timestamp, value, is_filtered);
    fflush(file);  // 确保立即写入文件
}

chart_data_t* chart_init(int min_value, int max_value) {
    chart_data_t* chart = malloc(sizeof(chart_data_t));
    if (!chart) return NULL;

    chart->data = malloc(sizeof(int) * DISPLAY_WIDTH);
    chart->smooth_data = malloc(sizeof(double) * DISPLAY_WIDTH);
    if (!chart->data || !chart->smooth_data) {
        free(chart->data);
        free(chart->smooth_data);
        free(chart);
        return NULL;
    }

    // 创建或打开原始数据CSV文件
    chart->raw_csv_file = fopen(RAW_CSV_FILENAME, "a");
    if (chart->raw_csv_file) {
        fseek(chart->raw_csv_file, 0, SEEK_END);
        if (ftell(chart->raw_csv_file) == 0) {
            fprintf(chart->raw_csv_file, "Timestamp,Value,IsFiltered\n");
        }
    } else {
        printf(COLOR_RED "Warning: Could not open raw data CSV file\n" COLOR_RESET);
    }

    // 创建或打开过滤后数据CSV文件
    chart->filtered_csv_file = fopen(FILTERED_CSV_FILENAME, "a");
    if (chart->filtered_csv_file) {
        fseek(chart->filtered_csv_file, 0, SEEK_END);
        if (ftell(chart->filtered_csv_file) == 0) {
            fprintf(chart->filtered_csv_file, "Timestamp,Value,IsFiltered\n");
        }
    } else {
        printf(COLOR_RED "Warning: Could not open filtered data CSV file\n" COLOR_RESET);
    }

    chart->size = 0;
    chart->max_value = max_value;
    chart->min_value = min_value;
    chart->last_update = time(NULL);
    chart->value_range[0] = min_value;
    chart->value_range[1] = max_value;

    // 初始化随机数生成器
    srand(time(NULL));
    
    return chart;
}

int chart_add_point(chart_data_t* chart, int value) {
    if (!chart) return -1;

    // 写入原始数据到CSV文件
    write_to_csv(chart->raw_csv_file, value, 0);

    // 如果数据已满，左移所有数据
    if (chart->size >= DISPLAY_WIDTH) {
        memmove(chart->data, chart->data + 1, (DISPLAY_WIDTH - 1) * sizeof(int));
        memmove(chart->smooth_data, chart->smooth_data + 1, (DISPLAY_WIDTH - 1) * sizeof(double));
        chart->size = DISPLAY_WIDTH - 1;
    }

    // 添加新数据点
    chart->data[chart->size] = value;
    if (chart->size == 0) {
        chart->smooth_data[0] = value;
    }
    chart->size++;

    // 更新平滑数据
    smooth_data(chart);

    // 更新最大最小值
    if (value > chart->max_value) chart->max_value = value;
    if (value < chart->min_value) chart->min_value = value;

    return 0;
}

void chart_auto_update(chart_data_t* chart) {
    if (!chart) return;

    time_t current_time = time(NULL);
    if (current_time - chart->last_update >= DATA_UPDATE_INTERVAL) {
        // 生成新的随机数据点
        int new_value = generate_random_value(
            chart->value_range[0], 
            chart->value_range[1]
        );
        
        // 添加新数据点
        chart_add_point(chart, new_value);
        
        // 更新时间戳
        chart->last_update = current_time;
        
        // 显示更新后的图表
        chart_display(chart);
    }
}

void chart_display(chart_data_t* chart) {
    if (!chart || chart->size == 0) return;

    char display[DISPLAY_HEIGHT][DISPLAY_WIDTH + 3];  // +3 for y-axis
    int range = chart->max_value - chart->min_value;
    if (range == 0) range = 1;

    // 初始化显示缓冲区
    for (int i = 0; i < DISPLAY_HEIGHT; i++) {
        memset(display[i], ' ', DISPLAY_WIDTH + 2);
        display[i][DISPLAY_WIDTH + 2] = '\0';
        display[i][0] = '|';  // Y轴
    }

    // 绘制X轴
    memset(display[DISPLAY_HEIGHT-1], '-', DISPLAY_WIDTH + 2);
    display[DISPLAY_HEIGHT-1][0] = '+';  // 原点

    // 绘制平滑曲线
    for (size_t x = 1; x < chart->size; x++) {
        double y1 = chart->smooth_data[x-1];
        double y2 = chart->smooth_data[x];
        
        // 在两点之间插值
        for (int i = 0; i < INTERPOLATION_POINTS; i++) {
            double t = (double)i / INTERPOLATION_POINTS;
            double y = y1 * (1-t) + y2 * t;
            int plot_y = (int)((DISPLAY_HEIGHT - 2) * (y - chart->min_value) / range);
            plot_y = DISPLAY_HEIGHT - 2 - plot_y;
            
            int plot_x = (int)(x - 1 + t);
            if (plot_y >= 0 && plot_y < DISPLAY_HEIGHT-1 && plot_x < DISPLAY_WIDTH) {
                display[plot_y][plot_x + 1] = '*';
            }
        }
    }

    // 打印图表
    printf("\033[2J\033[H");  // 清屏并移动光标到顶部
    printf(COLOR_CYAN "Real-time Data Chart\n" COLOR_RESET);
    printf(COLOR_YELLOW "Range: [%d - %d] " COLOR_GREEN "Current: %d\n" COLOR_RESET, 
           chart->min_value, chart->max_value, chart->data[chart->size - 1]);

    // 打印Y轴刻度
    for (int i = 0; i < DISPLAY_HEIGHT-1; i++) {
        if (i % 2 == 0) {
            int value = chart->max_value - (i * range / (DISPLAY_HEIGHT-2));
            printf(COLOR_BLUE "%3d %s" COLOR_RESET, value, display[i]);
        } else {
            printf("    %s", display[i]);
        }
        printf("\n");
    }

    // 打印X轴
    printf(COLOR_BLUE "    %s" COLOR_RESET "\n", display[DISPLAY_HEIGHT-1]);

    // 打印X轴刻度
    printf(COLOR_BLUE "    ");
    for (int i = 0; i < DISPLAY_WIDTH; i += 10) {
        printf("%-10d", i);
    }
    printf(COLOR_RESET "\n\n");
}

void chart_log_filtered_data(chart_data_t* chart, int value, int is_filtered) {
    if (!chart) return;
    write_to_csv(chart->filtered_csv_file, value, is_filtered);
}

void chart_destroy(chart_data_t* chart) {
    if (!chart) return;
    
    // 关闭CSV文件
    if (chart->raw_csv_file) {
        fclose(chart->raw_csv_file);
    }
    if (chart->filtered_csv_file) {
        fclose(chart->filtered_csv_file);
    }
    
    free(chart->data);
    free(chart->smooth_data);
    free(chart);
}