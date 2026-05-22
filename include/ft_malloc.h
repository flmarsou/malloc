#pragma once

# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

# define TINY_CHUNK_SIZE  128
# define SMALL_CHUNK_SIZE 1024

# define TINY_PAGE_SIZE   (TINY_CHUNK_SIZE * 100)
# define SMALL_PAGE_SIZE  (SMALL_CHUNK_SIZE * 100)

# define ALIGN_PAGE(size) (((size) + (g_pagesize - 1)) & ~(g_pagesize - 1))

typedef struct chunk
{
    size_t        size;
    bool          free;
    struct chunk *next;
    struct chunk *prev;
} chunk_t;

typedef struct page
{
    size_t        size;
    struct chunk *chunks;
    struct page  *next;
    struct page  *prev;
} page_t;

typedef struct
{
    page_t *tiny;
    page_t *small;
    page_t *large;
} allocator_t;

extern size_t      g_pagesize;  // Cached result of sysconf(_SC_PAGESIZE)
extern allocator_t g_allocator; // Global allocator state

void    *ft_malloc(size_t size);

page_t **get_page_head(size_t size);
size_t   get_page_size(size_t size);
page_t  *create_page(size_t size);
