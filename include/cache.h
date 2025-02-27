#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>

typedef struct cache_entry {
    char* key;
    void* value;
    size_t value_size;
    struct cache_entry* next;
} cache_entry_t;

typedef struct {
    cache_entry_t** buckets;
    size_t size;
} cache_t;

// 初始化缓存
cache_t* cache_init(size_t size);

// 存储数据
int cache_put(cache_t* cache, const char* key, void* value, size_t value_size);

// 获取数据
void* cache_get(cache_t* cache, const char* key, size_t* value_size);

// 销毁缓存
void cache_destroy(cache_t* cache);

#endif 