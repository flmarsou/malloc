ifeq (${HOSTTYPE},)
HOSTTYPE	?= ${shell uname -m}_${shell uname -s}
endif

NAME		:=	libft_malloc_${HOSTTYPE}.so
LINK		:=	libft_malloc.so

# ============================================================================ #
#     Files                                                                    #
# ============================================================================ #

SRC_CORE	:=	\
				src/core/ft_malloc.c  \
				src/core/ft_free.c    \
				src/core/ft_realloc.c \

SRC_MEMORY	:=	\
				src/memory/pages.c  \
				src/memory/chunks.c \

SRC_DEBUG	:=	\
				src/debug/show_alloc_mem.c    \
				src/debug/show_alloc_mem_ex.c \

# ============================================================================ #
#     Variables                                                                #
# ============================================================================ #

SRCS		:=	${SRC_CORE} ${SRC_MEMORY} ${SRC_DEBUG}
OBJS		:=	${SRCS:.c=.o}

CC			:=	cc
CFLAGS		:=	-Wall -Werror -Wextra -Iinclude -O2 -g3 -fPIC

# ============================================================================ #
#     Rules                                                                    #
# ============================================================================ #

all:		${NAME}

${NAME}:	${OBJS}
		${CC} ${CFLAGS} -shared -o ${NAME} ${OBJS} -lpthread
		ln -sf ${NAME} ${LINK}

test:		${NAME}
		cc -o tester test/main.c -L. -lft_malloc -Iinclude
		LD_LIBRARY_PATH=. ./tester

%.o:		%.c
		${CC} ${CFLAGS} -c $< -o $@

clean:
		rm -rf ${OBJS}

fclean:		clean
		rm -rf ${NAME} ${LINK}

re:		fclean all

.PHONY:	all clean fclean re
