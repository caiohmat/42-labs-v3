.PHONY:	clean fclean re all

NAME	:= monitoring
CFLAGS	:= -Wall -Wextra -Werror
LFLAGS	:= -I/home/coder/.local/include
CC		:= gcc
SRC		:=	main.c \
			get_data.c \
			utils.c \
			monitoring_loop.c \
			curl_setup.c \
			gnlmod/get_next_linemod.c \
			gnlmod/get_next_line_utils.c

SRC_FOLDER 	:= src
OBJ_FOLDER	:= obj
SRCS 		:= $(addprefix ${SRC_FOLDER}/, ${SRC})
OBJS 		:= $(addprefix ${OBJ_FOLDER}/, ${SRCS:.c=.o})

all	: $(NAME)

$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LFLAGS) -L/home/coder/.local/lib -lcurl

$(OBJ_FOLDER)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)

clean:
	rm -rf $(OBJ_FOLDER)

fclean: clean
	rm -f $(NAME)

re	: fclean all
