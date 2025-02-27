#include <string.h>
#include <stdio.h>
#include "cache.h"

static unsigned int hash(const char* key, size_t cache_size) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31 + *key) % cache_size;
        key++;
    }
    return hash;
}

cache_t* cache_init(size_t size) {
    cache_t* cache = malloc(sizeof(cache_t));
    if (!cache) return NULL;

    cache->buckets = calloc(size, sizeof(cache_entry_t*));
    if (!cache->buckets) {
        free(cache);
        return NULL;
    }

    cache->size = size;
    return cache;
}

int cache_put(cache_t* cache, const char* key, void* value, size_t value_size) {
    if (!cache || !key || !value) return -1;

    unsigned int index = hash(key, cache->size);
    
    // 检查是否已存在
    cache_entry_t* entry = cache->buckets[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = malloc(value_size);
            if (!entry->value) return -1;
            memcpy(entry->value, value, value_size);
            entry->value_size = value_size;
            return 0;
        }
        entry = entry->next;
    }

    // 创建新条目
    entry = malloc(sizeof(cache_entry_t));
    if (!entry) return -1;

    entry->key = strdup(key);
    entry->value = malloc(value_size);
    if (!entry->value) {
        free(entry->key);
        free(entry);
        return -1;
    }

    memcpy(entry->value, value, value_size);
    entry->value_size = value_size;
    entry->next = cache->buckets[index];
    cache->buckets[index] = entry;

    return 0;
}

void* cache_get(cache_t* cache, const char* key, size_t* value_size) {
    if (!cache || !key) return NULL;

    unsigned int index = hash(key, cache->size);
    cache_entry_t* entry = cache->buckets[index];

    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (value_size) *value_size = entry->value_size;
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

void cache_destroy(cache_t* cache) {
    if (!cache) return;

    for (size_t i = 0; i < cache->size; i++) {
        cache_entry_t* entry = cache->buckets[i];
        while (entry) {
            cache_entry_t* next = entry->next;
            free(entry->key);
            free(entry->value);
            free(entry);
            entry = next;
        }
    }

    free(cache->buckets);
    free(cache);
} 