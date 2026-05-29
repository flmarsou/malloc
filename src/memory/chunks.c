#include "ft_malloc.h"

chunk_t *find_free_chunk(const page_t *page, size_t size)
{
    chunk_t *chunk = page->chunks;

    while (chunk)
    {
        if (chunk->free && chunk->size >= size)
            return (chunk);
        chunk = chunk->next;
    }

    return (NULL);
}

void split_chunk(chunk_t *chunk, size_t size)
{
    if (chunk->size < size + sizeof(chunk_t) + 1)
    {
        chunk->free = false;
        return ;
    }

    chunk_t *leftover_chunk = (chunk_t *)((char *)chunk + sizeof(chunk_t) + size);
    leftover_chunk->size = chunk->size - size - sizeof(chunk_t);
    leftover_chunk->free = true;
    leftover_chunk->page = chunk->page;
    leftover_chunk->next = chunk->next;
    leftover_chunk->prev = chunk;

    if (chunk->next)
        chunk->next->prev = leftover_chunk;
    chunk->next = leftover_chunk;
    chunk->size = size;
}

chunk_t *find_allocated_chunk(void *ptr)
{
    page_t  *page;
    chunk_t *chunk;

    page_t **pages[] = {&g_allocator.tiny, &g_allocator.small, &g_allocator.large};

    for (int i = 0; i < 3; ++i)
    {
        page = *pages[i];
        while (page)
        {
            chunk = page->chunks;
            while (chunk)
            {
                if ((char *)chunk + sizeof(chunk_t) == ptr)
                    return (chunk);
                chunk = chunk->next;
            }

            page = page->next;
        }
    }

    return (NULL);
}

void merge_chunk(chunk_t *chunk)
{
    if (chunk->next && chunk->next->free)
    {
        chunk->size += sizeof(chunk_t) + chunk->next->size;
        chunk->next  = chunk->next->next;
        if (chunk->next)
            chunk->next->prev = chunk;
    }

    if (chunk->prev && chunk->prev->free)
    {
        chunk->prev->size += sizeof(chunk_t) + chunk->size;
        chunk->prev->next  = chunk->next;
        if (chunk->next)
            chunk->next->prev = chunk->prev;
    }
}
