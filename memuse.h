void *memuse_alloc(size_t size);
void memuse_lock(void *mem, size_t size);
void memuse_unalloc(void *mem);
void memuse_unlock(void *mem, size_t size);
