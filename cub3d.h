/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:44:20 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:41:25 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

# include "mlx.h"
# include "./libft/libft.h"
# include <math.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 13
# endif

typedef struct		s_data
{
	void			*img_ptr;
	int				*data;
	int				size_line;
	int				bpp;
	int				endian;

}					t_data;

typedef struct		s_map
{
	int				*row;
	int				col_size;
	struct s_map	*next;

}					t_map;

typedef struct		s_texture
{

	void			*img_ptr;
	int				*data;
	int				size_line;
	int				bpp;
	int				endian;
	int				img_width;
	int				img_height;

}					t_texture;

typedef struct		s_sprite
{
	double			x;
	double			y;

}					t_sprite;

typedef struct		s_sprite_calc
{
	double			spritex;
	double			spritey;
	double			invdet;
	double			transformx;
	double			transformy;
	int				spritescreenx;
	int				sprite_height;
	int				draw_y_start;
	int				draw_y_end;
	int				sprite_width;
	int				draw_x_start;
	int				draw_x_end;
	int				stripe_tex_x;
	int				stripe_tex_y;
}					t_sprite_calc;

typedef	struct		s_vars
{
	int				window_width;
	int				window_height;
	int				floor_color;
	int				ceilling_color;
	void			*mlx;
	void			*win;
	int				color;
	char			*string;
	double			camerax;
	double			sidedistx;
	double			sidedisty;
	double			deltadistx;
	double			deltadisty;
	int				stepx;
	int				stepy;
	double			perpwalldist;
	int				side;
	int				lineheight;
	double			wallx;
	int				drawstart;
	int				drawend;
	double			posx;
	double			posy;
	double			raydirx;
	double			olddirx;
	double			dirx;
	double			raydiry;
	double			olddiry;
	double			diry;
	double			rot_speed;
	double			moving_speed;
	double			oldplanex;
	double			planex;
	double			planey;
	int				mapx;
	int				mapy;
	int				**map;
	t_data			img;
	t_texture		*tex;
	int				key;
	double			texpos;
	double			step;
	int				texx;
	int				texy;
	t_sprite		*sprites;
	int				num_sprite;
	int				*zbuffer;
}					t_vars;

typedef struct		s_config
{
	int				resolution_flag;
	int				resolution_width;
	int				resolution_height;
	int				north_texture;
	char			*north_path;
	int				south_texture;
	char			*south_path;
	int				west_texture;
	char			*west_path;
	int				east_texture;
	char			*east_path;
	int				sprite_texture;
	char			*sprite_path;
	int				floor_flag;
	int				floor_r;
	int				floor_g;
	int				floor_b;
	int				ceilling_flag;
	int				ceilling_r;
	int				ceilling_g;
	int				ceilling_b;
	t_map			*map;
	int				map_row_size;
	int				map_start_flag;
	int				map_end_flag;
	int				surrounding_flag;
	int				spawn_flag;
	int				spawn_dir;
}					t_config;

void				map_lstadd_back(t_map **lst, t_map *new);
void				map_lstclear(t_map **lst, void (*del)(void *));
t_map				*map_lstlast(t_map *lst);
int					map_lstsize(t_map *lst);

int					free_int_array(int **arr);
int					free_test_map(int **test, int row_size);
void				free_char_array(char **arr);
int					free_all_texture(t_config *read);
int					free_all(t_vars *vars, t_config *read);
int					get_resolution(t_config *read, char *line);
int					get_floor_color(t_config *read, char *line);
int					get_ceilling_color(t_config *read, char *line);
int					get_north_texture_path(t_config *read, char *line);
int					get_south_texture_path(t_config *read, char *line);
int					get_west_texture_path(t_config *read, char *line);
int					get_east_texture_path(t_config *read, char *line);
int					get_sprite_texture_path(t_config *read, char *line);
int					read_before_map(t_config *read, char *line);
int					check_spawn_element(t_config *read, char *line);

int					**creat_map_to_validate(t_config *read, t_map *map,\
															int col_max);
int					check_surrounding(t_config *read, t_map *map);

int					get_next_line(int fd, char **line);
int					read_map(t_map *map, t_config *read, char *argv);
int					get_map(t_config *read, char *line, t_map *map);
int					read_config(t_config *read, char *argv);

void				feed_position(t_vars *vars, t_config *read);
int					feed_sprite(t_vars *vars, t_config *read);
int					feed_map(t_vars *vars, t_config *read);

void				draw_walls(t_vars *vars, int x);
void				draw_all(t_vars *vars, int x);

void				draw_sprites(t_sprite_calc *calc, t_vars *vars);
void				sort_sprites(int *sprite_order, double *sprite_distance,\
														int numsprites);
void				transform_sprites_coord(t_vars *vars,\
														t_sprite_calc *calc);
void				calculate_sprites_height_width(t_vars *vars,\
														t_sprite_calc *calc);
int					add_sprites(t_vars *vars);

int					render(t_vars *vars);

void				rotate_right(t_vars *vars);
void				rotate_left(t_vars *vars);
int					free_all_loads(t_vars *vars);
int					move_keys(int keycode, t_vars *vars);

int					config_to_vars(t_vars *vars, t_config *read);
int					play_on_screen(t_vars *vars, t_config *read, int save_flag);

int					save_bmp(t_vars *vars);

#endif
