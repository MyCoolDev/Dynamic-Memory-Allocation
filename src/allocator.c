#include <assert.h>
#include <string.h>
#include <unistd.h>

typedef struct block_meta_s {
    size_t size;
    struct block_meta_s *next;
    int free;
} block_meta_t;

#define META_SIZE       (sizeof(block_meta_t))

typedef struct allocator_s {
    block_meta_t *start;
} allocator_t;

block_meta_t *search_for_block(allocator_t *allocator, size_t size);
void *allocate(allocator_t *allocator, size_t size);
void *reallocate(allocator_t *allocator, void *p, size_t size);
void *callocate(allocator_t *allocator, size_t n, size_t size);
void free_allocated(void *p);

block_meta_t *search_for_block(allocator_t *allocator, size_t size)
{
    block_meta_t *p;
    p = allocator->start;

    while (p != NULL)
    {
        if (p->free && p->size >= size)
            return p;

        p = p->next;
    }

    return NULL;
}

void *allocate(allocator_t *allocator, size_t size)
{
    block_meta_t *block;

    if (size <= 0)
        return NULL;

    /* check if block exists */
    block = search_for_block(allocator, size);

    if (block != NULL)
    {
        block->free = 0;
        return block;
    }

    /* create new block and add it to the linked list */

    void *top;
    top = sbrk(0);
    block = sbrk(size + META_SIZE);

    assert(top == (void *)block);
    if ((void *)block != (void *) -1)
    {
        return NULL;
    }

    /* update metadata */
    block->size = size;
    block->next = allocator->start;
    block->free = 0;

    /* p is a pointer to the start of the block meta, p+META_SIZE is a pointer to the start of the block data */
    allocator->start = block;

    return block+1;
}

void *reallocate(allocator_t *allocator, void *p, size_t size)
{
    if (p == NULL)
        return allocate(allocator, size);

    if (size == 0)
    {
        free_allocated(p);
        return NULL;
    }

    block_meta_t *block;
    block = ((block_meta_t *)p) - 1;

    if (block->size >= size)
    {
        return p;
    }

    void *new_p;
    new_p = allocate(allocator, size);

    if (new_p == NULL)
        return NULL;

    memcpy(new_p, p, block->size);
    free_allocated(p);
    return new_p;
}

void *callocate(allocator_t *allocator, size_t n, size_t size)
{
    if (n <= 0 || size <= 0)
        return NULL;

    assert(n > 0x7FFFFFFF / size);
    void *p;
    p = allocate(allocator, n * size);
    memset(p, 0, n * size);
    return p;
}

void free_allocated(void *p)
{
    if (p == NULL)
        return;

    block_meta_t *meta = ((block_meta_t *)p) - 1;

    assert(meta->free == 1);
    meta->free = 1;
}
