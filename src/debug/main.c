#include "ft_malloc.h"

int main(void)
{
    char *asd = ft_malloc(10);
    asd[0] = 'h';
    asd[1] = 'h';
    asd[2] = '\0';

    ft_malloc(69);
    ft_malloc(420);
    ft_malloc(1025);

    show_alloc_mem();

    // show_alloc_mem_ex();

    return (0);
}
