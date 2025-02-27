#include "filter.h"
#include <string.h>

data_series_t* series_init(size_t initial_capacity) {
    // 限制初始容量不超过最大值
    if (initial_capacity > MAX_SERIES_SIZE) {
        initial_capacity = MAX_SERIES_SIZE;
    }

    data_series_t* series = malloc(sizeof(data_series_t));
    if (!series) return NULL;

    series->data = malloc(sizeof(void*) * initial_capacity);
    if (!series->data) {
        free(series);
        return NULL;
    }

    series->size = 0;
    series->capacity = initial_capacity;
    return series;
}

int series_add(data_series_t* series, void* data) {
    if (!series || !data) return -1;

    // 如果达到最大容量，移除最旧的数据
    if (series->size >= MAX_SERIES_SIZE) {
        // 移动所有数据向前一位
        memmove(series->data, series->data + 1, (MAX_SERIES_SIZE - 1) * sizeof(void*));
        series->size = MAX_SERIES_SIZE - 1;
    } else if (series->size >= series->capacity) {
        // 如果需要扩容，但不超过最大限制
        size_t new_capacity = series->capacity * 2;
        if (new_capacity > MAX_SERIES_SIZE) {
            new_capacity = MAX_SERIES_SIZE;
        }
        
        void** new_data = realloc(series->data, sizeof(void*) * new_capacity);
        if (!new_data) return -1;

        series->data = new_data;
        series->capacity = new_capacity;
    }

    series->data[series->size++] = data;
    return 0;
}

data_series_t* series_filter(data_series_t* series, filter_func filter) {
    if (!series || !filter) return NULL;

    data_series_t* result = series_init(series->size);
    if (!result) return NULL;

    for (size_t i = 0; i < series->size; i++) {
        if (filter(series->data[i])) {
            if (series_add(result, series->data[i]) != 0) {
                series_destroy(result);
                return NULL;
            }
        }
    }

    return result;
}

void series_destroy(data_series_t* series) {
    if (!series) return;
    free(series->data);
    free(series);
} 