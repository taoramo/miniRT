NAME = miniRT

LIBFT = ./libft/libft.a

LIBMLX = ./MLX42

MLX = $(LIBMLX)/build/libmlx42.a -Iinclude -lglfw -L"/Users/$(USER)/.brew/opt/glfw/lib/" -L/usr/loca/Cellar/glfw/lib/

LIBS = $(LIBFT) $(MLX)

INCLUDES = -I ./inc/ -I $(LIBMLX)/include/MLX42 -I ./libft/

SRCS = $(addprefix srcs/, main.c ray.c vec3a.c vec3b.c vec3c.c sphere.c)

OBJS = $(SRCS:.c=.o)

CFLAGS = -Wextra -Wall -Werror -Wunreachable-code -Wpedantic -Wtype-limits -o3 $(INCLUDES)

all: libmlx $(NAME)

libmlx:
	cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(OBJS): $(SRCS)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBS) -o $(NAME)

$(LIBFT):
	make -C ./libft/

clean:
	rm -rf $(OBJS)
	make clean -C ./libft/
	rm -rf $(LIBMLX)/build

fclean: clean
	rm -rf $(NAME)
	make fclean -C ./libft/

re: fclean all

.PHONY: all, clean, fclean, re
