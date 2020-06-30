NAME = cub3d

SRC = map_lst.c \
	  check_spawn_elem.c \
	  check_surrounding.c \
	  create_map_to_validate.c \
	  draw_all.c \
	  draw_sprites.c \
	  draw_walls.c \
	  add_sprites.c \
	  feed_position.c \
	  feed_sprite.c \
	  free.c \
	  get_color.c \
	  get_map.c \
	  get_next_line.c \
	  get_resolution.c \
	  get_sprite_texture.c \
	  get_wall_texture.c \
	  move_keys.c \
	  config_to_vars.c \
	  read_before_map.c \
	  read_config.c \
	  read_map.c \
	  render.c \
	  play_on_screen.c \
	  rotate_and_free.c \
	  cub3d.c \
	  save_bmp.c

OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror

MLXDIR = minilibx_mms_20200219
MLX_LNK = -lmlx -framework OpenGL -framework AppKit 
MLX = libmlx.dylib

FTDIR = ./libft
FT_LIB = $(addprefix $(FTDIR)/,libft.a)
FT_INC = -I $(FTDIR)
FT_LNK = -L $(FTDIR) -lft

%.o : %.c
	$(CC) $(CFLAGS) $(FT_INC) -o $@ -c $<

$(NAME): ${MLX_LIB} $(FT_LIB) $(OBJ)
	$(CC) $(OBJ) $(MLX_LNK) $(FT_LNK) -lm -o $(NAME)


all :  $(NAME)

$(FT_LIB):
	make -C $(FTDIR)

$(MLX_LIB):
	make -C $(MLXDIR)


clean:
	rm -f ${OBJ}
	make -C $(FTDIR) clean
	make -C $(MLXDIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(FTDIR) fclean

re: fclean all

.PHONY : all bonus clean fclean re
