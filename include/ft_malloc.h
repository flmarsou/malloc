#pragma once

# include <stddef.h>
# include <stdbool.h>

# define TINY_CHUNK_SIZE  128  // Maximum allocation size considered "tiny"
# define SMALL_CHUNK_SIZE 1024 // Maximum allocation size considered "small"

# define TINY_PAGE_SIZE   ((TINY_CHUNK_SIZE + sizeof(chunk_t)) * 121 + sizeof(page_t))  // Maximum size in bytes of tiny pages
# define SMALL_PAGE_SIZE  ((SMALL_CHUNK_SIZE + sizeof(chunk_t)) * 121 + sizeof(page_t)) // Maximum size in bytes of small pages

// Rounds up `size` to the nearest multiple of `g_pagesize`.
# define ALIGN_PAGE(size) (((size) + (g_pagesize - 1)) & ~(g_pagesize - 1))

// ========================================================================== //
//    Data                                                                    //
// ========================================================================== //

typedef struct chunk
{
    size_t        size; // Size of the usable data (excludes header)
    bool          free; // Wether this chunk is available for allocation
    struct page  *page; // Back-pointer to the parent page
    struct chunk *next;
    struct chunk *prev;
} chunk_t;

typedef struct page
{
    size_t        size;   // Size of the page (including header)
    struct chunk *chunks; // Head of the chunk list
    struct page  *next;
    struct page  *prev;
} page_t;

typedef struct
{
    page_t *tiny;  // Head of the tiny page list (<= TINY_CHUNK_SIZE bytes)
    page_t *small; // Head of the small page list (<= SMALL_CHUNK_SIZE bytes)
    page_t *large; // Head of the large page list (anything bigger)
} allocator_t;

extern size_t      g_pagesize;  // Cached result of sysconf(_SC_PAGESIZE)
extern allocator_t g_allocator; // Global allocator state

// ========================================================================== //
//    Functions                                                               //
// ========================================================================== //

//    Malloc                         //
// ================================= //

void    *ft_malloc(size_t size);

page_t **get_page_head(size_t size);
size_t   get_page_size(size_t size);
page_t  *create_page(size_t size);
void     append_page(page_t **head, page_t *page);

chunk_t *find_free_chunk(const page_t *page, size_t size);
void     split_chunk(chunk_t *chunk, size_t size);

//    Free                           //
// ================================= //

void     ft_free(void *ptr);

void     remove_page(page_t **head, page_t *page);

chunk_t *find_allocated_chunk(void *ptr);
void     merge_chunk(chunk_t *chunk);

//    Realloc                        //
// ================================= //

void *ft_realloc(void *ptr, size_t size);

//    Debug                          //
// ================================= //

void show_alloc_mem();
void show_alloc_mem_ex();
