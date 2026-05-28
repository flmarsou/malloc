#include "ft_malloc.h"

# include <sys/mman.h>

page_t **get_page_head(size_t size)
{
    if (size <= TINY_CHUNK_SIZE)
        return (&g_allocator.tiny);
    else if (size <= SMALL_CHUNK_SIZE)
        return (&g_allocator.small);
    else
        return (&g_allocator.large);
}

size_t get_page_size(size_t size)
{
    if (size <= TINY_CHUNK_SIZE)
        return (ALIGN_PAGE(TINY_PAGE_SIZE));
    else if (size <= SMALL_CHUNK_SIZE)
        return (ALIGN_PAGE(SMALL_PAGE_SIZE));
    else
        return (ALIGN_PAGE(size + sizeof(page_t) + sizeof(chunk_t)));
}

page_t *create_page(size_t size)
{
    page_t *page;

    page = mmap(NULL, size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 0);

    if (page == MAP_FAILED)
        return (NULL);

    chunk_t *chunk = (chunk_t *)((char *)page + sizeof(page_t));

    chunk->size = size - sizeof(page_t) - sizeof(chunk_t);
    chunk->free = true;
    chunk->page = page;
    chunk->next = NULL;
    chunk->prev = NULL;

    page->size   = size;
    page->chunks = chunk;
    page->next   = NULL;
    page->prev   = NULL;

    return (page);
}
