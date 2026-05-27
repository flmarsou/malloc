#include "ft_malloc.h"
#include <stdio.h>

size_t      g_pagesize;
allocator_t g_allocator;

void *ft_malloc(size_t size)
{
    if (size == 0)
        return (NULL);

    if (g_pagesize == 0)
        g_pagesize = sysconf(_SC_PAGESIZE);

    page_t **pages = get_page_head(size);
    page_t  *page  = *pages;

    chunk_t *chunk = NULL;

    // Search existing pages
    while (page)
    {
        chunk = find_free_chunk(page, size);
        if (chunk)
            break ;
        page = page->next;
    }

    // No free chunk found, create a new page
    if (!chunk)
    {
        page = create_page(get_page_size(size));
        if (!page)
            return (NULL);

        page_t *head = *pages;
        if (!head)
            *pages = page;
        else
        {
            while (head->next)
                head = head->next;
            head->next = page;
            page->prev = head;
        }

        chunk = page->chunks;
    }

    split_chunk(chunk, size);
    chunk->free = false;

    return ((char *)chunk + sizeof(chunk_t));
}
