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
