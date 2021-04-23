NAME	= libasm.a

SRCS	= ft_strlen.s \
		  ft_strcpy.s \
		  ft_strcmp.s \
		  ft_write.s \
		  ft_strdup.s \
		  ft_read.s \

OBJS	= $(SRCS:.s=.o)

TEST	= test

%.o		: %.s
	nasm -f macho64 $<

all		: $(NAME)

$(NAME)	: $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean	: 
	rm -rf $(OBJS)

fclean	: clean
	rm -rf $(NAME) $(TEST) $(NAME)

re		: fclean all

test	: re
	gcc main.c -o test -L. -lasm