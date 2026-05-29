#include "ft_malloc.h"

# include <unistd.h>

size_t      g_pagesize;
allocator_t g_allocator;

void *ft_malloc(size_t size)
{
    if (size == 0)
        return (NULL);

    if (g_pagesize == 0)
        g_pagesize = sysconf(_SC_PAGESIZE);

    page_t **head = get_page_head(size);
    page_t  *page  = NULL;
    chunk_t *chunk = NULL;

    if (size > SMALL_CHUNK_SIZE)
    {
        // Large: always create a new page
        page = create_page(get_page_size(size));
        if (!page)
            return (NULL);

        append_page(head, page);
        chunk = page->chunks;
    }
    else
    {
        // Tiny/Small: search existing page
        page = *head;
        while (page)
        {
            chunk = find_free_chunk(page, size);
            if (chunk)
                break ;
            page = page->next;
        }

        // Tiny/Small: create a new page
        if (!chunk)
        {
            page = create_page(get_page_size(size));
            if (!page)
                return (NULL);

            append_page(head, page);
            chunk = page->chunks;
        }
    }

    split_chunk(chunk, size);
    chunk->free = false;

    return ((char *)chunk + sizeof(chunk_t));
}
