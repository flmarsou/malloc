#include "ft_malloc.h"

# include <stdio.h>
# include "ft_color.h"

static char *display_time(size_t time)
{
    static char tmp[15];
    static char buf[20];

    snprintf(tmp, sizeof(tmp), "%zu", time);
    snprintf(buf, sizeof(buf), "%.4s-%.2s-%.2s %.2s:%.2s:%.2s",
        tmp,      // YYYY
        tmp + 4,  // MM
        tmp + 6,  // DD
        tmp + 8,  // hh
        tmp + 10, // mm
        tmp + 12  // ss
    );

    return (buf);
}

static void show_page(const page_t *page)
{
    size_t page_index = 0;

    while (page)
    {
        printf("   %sPage [%zu]%s -> %s%s%p%s\n", CYAN, page_index, RESET, GREEN, BOLD, page, RESET);

        chunk_t *chunk = page->chunks;

        size_t chunk_index = 0;

        while (chunk)
        {
            void *start = (void *)(char *)chunk + sizeof(chunk_t);
            void *end   = start + chunk->size;

            if (!chunk->free)
                printf("      %sChunk [%03zu]%s -> %s%s%p%s - %s%s%p%s : %s%s%s\n", CYAN, chunk_index, RESET, GREEN, BOLD, start, RESET, GREEN, BOLD, end, RESET, YELLOW, display_time(chunk->time), RESET);

            chunk = chunk->next;
            ++chunk_index;
        }
        page = page->next;
        ++page_index;

        if (page)
            printf("\n");
    }

    printf("\n");
}

void show_alloc_mem_time(void)
{
    pthread_mutex_lock(&g_malloc_mutex);

    printf("%sTiny Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.tiny, RESET);
    show_page(g_allocator.tiny);

    printf("%sSmall Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.small, RESET);
    show_page(g_allocator.small);

    printf("%sLarge Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.large, RESET);
    show_page(g_allocator.large);

    pthread_mutex_unlock(&g_malloc_mutex);
}
