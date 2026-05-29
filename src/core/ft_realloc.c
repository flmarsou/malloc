#include "ft_malloc.h"

# include <string.h>

void *ft_realloc(void *ptr, size_t size)
{
    if (!ptr)
        return (ft_malloc(size));

    if (size == 0)
    {
        ft_free(ptr);
        return (NULL);
    }

    chunk_t *chunk = (chunk_t *)((char *)ptr - sizeof(chunk_t));

    if (chunk->size >= size)
        return (ptr);

    void *new_ptr = ft_malloc(size);
    if (!new_ptr)
        return (NULL);

    memcpy(new_ptr, ptr, chunk->size);
    ft_free(ptr);
    return (new_ptr);
}
