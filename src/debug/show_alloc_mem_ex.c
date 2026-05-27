#include "ft_malloc.h"

# include "stdio.h"

static void hex_dump(void *start, size_t size)
{
    (void)start;
    (void)size;
}

static void show_page(const page_t *page)
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
                hex_dump(start, chunk->size);
            }
            chunk = chunk->next;
        }
        page = page->next;
    }
}

void show_alloc_mem_ex()
{
    printf("--- Hexadecimal Dump ---\n\n");

    printf("TINY  : %p\n", (void *)g_allocator.tiny);
    show_page(g_allocator.tiny);

    printf("\nSMALL : %p\n", (void *)g_allocator.small);
    show_page(g_allocator.small);

    printf("\nLARGE : %p\n", (void *)g_allocator.large);
    show_page(g_allocator.large);
}
