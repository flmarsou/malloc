#include "ft_malloc.h"

# include <unistd.h>

size_t          g_pagesize;
allocator_t     g_allocator;
pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

void *ft_malloc(size_t size)
{
    if (size == 0)
        return (NULL);

    pthread_mutex_lock(&g_malloc_mutex);

    if (g_pagesize == 0)
        g_allocator.pagesize = sysconf(_SC_PAGESIZE);

    page_t **head = get_page_head(size);
    page_t  *page  = NULL;
    chunk_t *chunk = NULL;

    if (size > SMALL_CHUNK_SIZE)
    {
        // Large: always create a new page
        page = create_page(get_page_size(size));
        if (!page)
        {
            pthread_mutex_unlock(&g_malloc_mutex);
            return (NULL);
        }

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
            {
                pthread_mutex_unlock(&g_malloc_mutex);
                return (NULL);
            }

            append_page(head, page);
            chunk = page->chunks;
        }
    }

    split_chunk(chunk, size);
    chunk->free = false;

    pthread_mutex_unlock(&g_malloc_mutex);

    return ((char *)chunk + sizeof(chunk_t));
}
