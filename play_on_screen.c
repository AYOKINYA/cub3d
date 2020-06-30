/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_on_screen.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:28 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 15:30:02 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	click_red_cross(t_vars *vars, int click)
{
	if (click == 1)
		mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
	return (1);
}

static int	load_wall_texture(t_vars *vars, t_config *read)
{
	if (!(vars->tex[0].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->north_path, &vars->tex[0].img_width, &vars->tex[0].img_height)))
		return (0);
	vars->tex[0].data = (int *)mlx_get_data_addr(vars->tex[0].img_ptr,\
		&vars->tex[0].bpp, &vars->tex[0].size_line, &vars->tex[0].endian);
	if (!(vars->tex[1].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->south_path, &vars->tex[1].img_width, &vars->tex[1].img_height)))
		return (0);
	vars->tex[1].data = (int *)mlx_get_data_addr(vars->tex[1].img_ptr,\
		&vars->tex[1].bpp, &vars->tex[1].size_line, &vars->tex[1].endian);
	if (!(vars->tex[2].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->east_path, &vars->tex[2].img_width, &vars->tex[2].img_height)))
		return (0);
	vars->tex[2].data = (int *)mlx_get_data_addr(vars->tex[2].img_ptr,\
		&vars->tex[2].bpp, &vars->tex[2].size_line, &vars->tex[2].endian);
	if (!(vars->tex[3].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->west_path, &vars->tex[3].img_width, &vars->tex[3].img_height)))
		return (0);
	vars->tex[3].data = (int *)mlx_get_data_addr(vars->tex[3].img_ptr,\
		&vars->tex[3].bpp, &vars->tex[3].size_line, &vars->tex[3].endian);
	return (1);
}

static int	load_img(t_vars *vars, t_config *read)
{
	if (!(vars->tex = (t_texture *)malloc(sizeof(t_texture) * 5)))
		return (0);
	if (!load_wall_texture(vars, read))
	{
		free(vars->tex);
		vars->tex = 0;
		return (0);
	}
	if (!(vars->tex[4].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->sprite_path, &vars->tex[4].img_width, &vars->tex[4].img_height)))
	{
		free(vars->tex);
		vars->tex = 0;
		return (0);
	}
	vars->tex[4].data = (int *)mlx_get_data_addr(vars->tex[4].img_ptr,\
		&vars->tex[4].bpp, &vars->tex[4].size_line, &vars->tex[4].endian);
	return (1);
}

static int	raycasting(t_vars *vars, int save_flag)
{
	if (!render(vars))
		return (0);
	if (save_flag)
	{
		if (!save_bmp(vars))
			return (0);
		return (2);
	}
	return (1);
}

int			play_on_screen(t_vars *vars, t_config *read, int save_flag)
{
	int ret;

	vars->mlx = mlx_init();
	vars->img.img_ptr = mlx_new_image(vars->mlx,\
							vars->window_width, vars->window_height);
	vars->img.data = (int *)mlx_get_data_addr(vars->img.img_ptr,\
					&vars->img.bpp, &vars->img.size_line, &vars->img.endian);
	if (!load_img(vars, read))
		return (0);
	if ((ret = raycasting(vars, save_flag)) == 0)
		return (0);
	if (ret == 2)
		return (1);
	vars->win = mlx_new_window(vars->mlx,\
							vars->window_width, vars->window_height, "Cub3d");
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
	mlx_hook(vars->win, 2, 1L << 0, &move_keys, vars);
	mlx_hook(vars->win, 17, 1L << 0, &click_red_cross, vars);
	mlx_loop(vars->mlx);
	return (1);
}
