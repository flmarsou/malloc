#include "ft_malloc.h"

int main(void)
{
    char *asd = ft_malloc(128);

    asd[0] = 0xff;

    for (int i = 32; i < 120; ++i)
        asd[i] = i;

    for (int i = 1; i < 23; ++i)
        asd[i] = i;

    show_alloc_mem_ex();

    return (0);
}
