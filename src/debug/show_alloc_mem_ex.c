#include "ft_malloc.h"

# include <stdio.h>
# include "ft_color.h"

static void set_color(unsigned char value)
{
    if (value == 0)
        printf("%s", GRAY);
    else if (value >= 33 && value < 127)
        printf("%s%s", BOLD, GREEN);
    else if (value == ' ' || value == '\t' || value == '\n' || value == '\r')
        printf("%s", YELLOW);
    else if (value >= 128)
        printf("%s", RED);
    else
        printf("%s", CYAN);
}

static void reset_color()
{
    printf("%s", RESET);
}

static void hex_dump(void *start, size_t size)
{
    unsigned char *ptr = (unsigned char *)start;

    for (size_t offset = 0; offset < size; offset += 16)
    {
        // Offset
        printf("         %04zx | ", offset);

        // Hex Bytes
        for (size_t byte = 0; byte < 16; ++byte)
        {
            unsigned char value = ptr[offset + byte];

            if (offset + byte < size)
            {
                set_color(value);
                printf("%02x ", value);
                reset_color();
            }
            else
                printf("   ");

            if (byte == 7)
                printf(" ");
        }

        // ASCII
        printf("| ");
        for (size_t byte = 0; byte < 16 && offset + byte < size; byte++)
        {
            unsigned char value = ptr[offset + byte];

            set_color(value);
            printf("%c", (value >= 32 && value < 127) ? value : '.');
            reset_color();
        }
        printf("\n");
    }
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
            if (!chunk->free)
            {
                void *start = (void *)(char *)chunk + sizeof(chunk_t);
                void *end   = start + chunk->size;

                printf("      %sChunk [%03zu]%s -> %s%s%p%s - %s%s%p%s : %s%zu bytes%s\n", CYAN, chunk_index, RESET, GREEN, BOLD, start, RESET, GREEN, BOLD, end, RESET, YELLOW, chunk->size, RESET);
                hex_dump(start, chunk->size);
            }
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

void show_alloc_mem_ex(void)
{
    pthread_mutex_lock(&g_malloc_mutex);

    if (g_allocator.tiny)
    {
        printf("%sTiny Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.tiny, RESET);
        show_page(g_allocator.tiny);
    }

    if (g_allocator.small)
    {
        printf("%sSmall Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.small, RESET);
        show_page(g_allocator.small);
    }

    if (g_allocator.large)
    {
        printf("%sLarge Head%s -> %s%s%p%s\n", CYAN, RESET, GREEN, BOLD, (void *)g_allocator.large, RESET);
        show_page(g_allocator.large);
    }

    pthread_mutex_unlock(&g_malloc_mutex);
}
