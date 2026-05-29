EXE	:=	ft_malloc

SRC_CORE	:=	\
				src/core/ft_malloc.c \
				src/core/ft_free.c \
				src/core/ft_realloc.c \

SRC_MEMORY	:=	\
				src/memory/pages.c \
				src/memory/chunks.c \

SRC_DEBUG	:=	\
				src/debug/main.c \
				src/debug/show_alloc_mem.c \
				src/debug/show_alloc_mem_ex.c \

SRCS		:=	${SRC_CORE} ${SRC_MEMORY} ${SRC_DEBUG}

CC		:=	cc
CFLAGS	:=	-Wall -Werror -Wextra -Iinclude -O2 -g3

all:	${EXE}

${EXE}:	${SRC}
		${CC} ${CFLAGS} ${SRCS} -o ${EXE}

clean:
		rm -rf ${EXE}

re:		clean all

.PHONY:	all clean re
