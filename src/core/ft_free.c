#include "ft_malloc.h"

# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>

void ft_free(void *ptr)
{
    if (!ptr)
        return ;

    pthread_mutex_lock(&g_malloc_mutex);

    chunk_t *chunk = find_allocated_chunk(ptr);
    if (!chunk)
    {
        printf("ft_free(): invalid pointer\n");
        pthread_mutex_unlock(&g_malloc_mutex);
        abort();
    }

    if (chunk->free)
    {
        pthread_mutex_unlock(&g_malloc_mutex);
        return ;
    }

    chunk->free = true;
    merge_chunk(chunk);

    page_t  *page = chunk->page;

    if (!page->chunks->next && page->chunks->free)
    {
        page_t **head;

        if (page->size == ALIGN_PAGE(TINY_PAGE_SIZE))
            head = &g_allocator.tiny;
        else if (page->size == ALIGN_PAGE(SMALL_PAGE_SIZE))
            head = &g_allocator.small;
        else
            head = &g_allocator.large;

        remove_page(head, page);
        munmap(page, page->size);
    }

    pthread_mutex_unlock(&g_malloc_mutex);
}
