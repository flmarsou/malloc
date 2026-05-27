#include "ft_malloc.h"

# include "stdio.h"

static void show_page(const page_t *page, size_t *total)
{
    while (page)
    {
        chunk_t *chunk = page->chunks;

        while (chunk)
        {
            if (!chunk->free)
            {
                void *start = (void *)(char *)chunk + sizeof(chunk_t);
                void *end   = start + chunk->size;

                printf("%p - %p : %zu bytes\n", start, end, chunk->size);
                *total += chunk->size;
            }
            chunk = chunk->next;
        }
        page = page->next;
        printf("\n");
    }
}

void show_alloc_mem()
{
    size_t total = 0;

    printf("--- Allocated Memory ---\n\n");

    printf("TINY  : %p\n", (void *)g_allocator.tiny);
    show_page(g_allocator.tiny, &total);

    printf("\nSMALL : %p\n", (void *)g_allocator.small);
    show_page(g_allocator.small, &total);

    printf("\nLARGE : %p\n", (void *)g_allocator.large);
    show_page(g_allocator.large, &total);

    printf("\nTotal : %zu bytes\n", total);
}
