#include "ft_malloc.h"

# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>

void ft_free(void *ptr)
{
    if (!ptr)
        return ;

    chunk_t *chunk = find_allocated_chunk(ptr);
    if (!chunk)
    {
        printf("ft_free(): invalid pointer\n");
        abort();
    }

    if (chunk->free)
        return ;

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
}
