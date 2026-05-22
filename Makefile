EXE	:=	ft_malloc

SRC	:=	\
		src/main.c \
		src/ft_malloc.c \
		src/pages.c \

CC		:=	cc
CFLAGS	:=	-Wall -Werror -Wextra -Iinclude -O2 -g3

all:	${EXE}

${EXE}:	${SRC}
		${CC} ${CFLAGS} ${SRC} -o ${EXE}

clean:
		rm -rf ${EXE}

re:		clean all

.PHONY:	all clean re
