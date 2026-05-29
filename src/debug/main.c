#include "ft_malloc.h"

int main(void)
{
    void *ptr1 = ft_malloc(100);
    ft_free(ptr1);
    void *ptr2 = ft_malloc(1000);
    ft_free(ptr2);
    void *ptr3 = ft_malloc(10000);
    ft_free(ptr3);

    void *test = ft_malloc(1000);
    void *toto = ft_realloc(test, 10000);
    ft_free(toto);

    show_alloc_mem();

    return (0);
}
