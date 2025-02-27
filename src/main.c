#include <stdio.h>
#include <unistd.h>
#include "cache.h"
#include "filter.h"
#include "display.h"

// 示例过滤函数：过滤大于50的整数
static int filter_greater_than_50(const void* data) {
    return *(int*)data > 50;
}

int main() {
    // 初始化缓存
    cache_t* cache = cache_init(100);
    if (!cache) {
        printf("Failed to initialize cache\n");
        return 1;
    }

    // 初始化图表 (值范围0-100)
    chart_data_t* chart = chart_init(0, 100);
    if (!chart) {
        printf("Failed to initialize chart\n");
        cache_destroy(cache);
        return 1;
    }

    // 初始化数据序列
    data_series_t* series = series_init(100);
    if (!series) {
        printf("Failed to initialize data series\n");
        chart_destroy(chart);
        cache_destroy(cache);
        return 1;
    }

    printf("自动生成数据中... (按Ctrl+C退出)\n");
    
    while (1) {
        // 自动更新图表数据
        chart_auto_update(chart);
        
        if (chart->size > 0) {
            // 获取最新数据
            int latest_value = chart->data[chart->size - 1];
            
            // 添加到缓存
            char key[32];
            snprintf(key, sizeof(key), "value_%zu", series->size);
            cache_put(cache, key, &latest_value, sizeof(int));

            // 添加到数据序列
            series_add(series, &latest_value);

            // 过滤并显示大于50的数
            data_series_t* filtered = series_filter(series, filter_greater_than_50);
            if (filtered) {
                printf("Filtered numbers (> 50): ");
                for (size_t i = 0; i < filtered->size; i++) {
                    int filtered_value = *(int*)filtered->data[i];
                    printf("%d ", filtered_value);
                    // 记录过滤后的数据
                    chart_log_filtered_data(chart, filtered_value, 1);
                }
                printf("\n");
                series_destroy(filtered);
            }
        }

        sleep(1);  // 暂停1秒
    }

    // 清理资源
    series_destroy(series);
    chart_destroy(chart);
    cache_destroy(cache);
    return 0;
} 