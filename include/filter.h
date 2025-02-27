#ifndef FILTER_H
#define FILTER_H

#include <stdlib.h>

#define MAX_SERIES_SIZE 50  // 数据序列最大容量

typedef int (*filter_func)(const void* data);

typedef struct {
    void** data;
    size_t size;
    size_t capacity;
} data_series_t;

// 初始化数据序列
data_series_t* series_init(size_t initial_capacity);

// 添加数据
int series_add(data_series_t* series, void* data);

// 使用过滤器过滤数据
data_series_t* series_filter(data_series_t* series, filter_func filter);

// 销毁数据序列
void series_destroy(data_series_t* series);

#endif 