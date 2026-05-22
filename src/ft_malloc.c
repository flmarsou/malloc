#include "ft_malloc.h"

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

    if (!page)
    {
        size_t page_size = get_page_size(size);

        page = create_page(page_size);
        if (!page)
            return (NULL);

        *pages = page;
    }

    return (NULL);
}
