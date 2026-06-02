#include "ft_malloc.h"

# include <stdio.h>
# include "ft_color.h"

static void show_page(const page_t *page, size_t *total, size_t *total_raw)
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

            *total += chunk->size;
            *total_raw += chunk->size + sizeof(chunk_t);

            if (!chunk->free)
                printf("      %sChunk [%03zu]%s -> %s%s%p%s - %s%s%p%s : %s%zu bytes%s\n", CYAN, chunk_index, RESET, GREEN, BOLD, start, RESET, GREEN, BOLD, end, RESET, YELLOW, chunk->size, RESET);
            else
                printf("      %sChunk [%03zu]%s -> %s%s%p%s - %s%s%p%s : %s%zu freed bytes%s\n", CYAN, chunk_index, RESET, GREEN, BOLD, start, RESET, GREEN, BOLD, end, RESET, GRAY, chunk->size, RESET);

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

void show_alloc_mem()
{
    pthread_mutex_lock(&g_malloc_mutex);

    size_t total = 0;
    size_t total_raw = sizeof(page_t);

    printf("%sTiny Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.tiny, RESET);
    show_page(g_allocator.tiny, &total, &total_raw);

    printf("%sSmall Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.small, RESET);
    show_page(g_allocator.small, &total, &total_raw);

    printf("%sLarge Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.large, RESET);
    show_page(g_allocator.large, &total, &total_raw);

    printf("%sTotal    %s -> %s%zu bytes%s\n", CYAN, RESET, GREEN, total, RESET);
    printf("%sTotal Raw%s -> %s%zu bytes%s\n", CYAN, RESET, GREEN, total_raw, RESET);

    pthread_mutex_unlock(&g_malloc_mutex);
}
