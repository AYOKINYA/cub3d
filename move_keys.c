/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_keys.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:25 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:47:19 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	go_forward(t_vars *vars)
{
	if ((vars->map)[(int)(vars->posx + vars->dirx * vars->moving_speed)]\
													[(int)(vars->posy)] != 1)
		vars->posx += vars->dirx * vars->moving_speed;
	if ((vars->map)[(int)(vars->posx)]\
					[(int)(vars->posy + vars->diry * vars->moving_speed)] != 1)
		vars->posy += (vars->diry) * vars->moving_speed;
}

static void	go_backward(t_vars *vars)
{
	if ((vars->map)[(int)(vars->posx - vars->dirx * vars->moving_speed)]\
												[(int)(vars->posy)] != 1)
		vars->posx -= vars->dirx * vars->moving_speed;
	if ((vars->map)[(int)(vars->posx)]\
				[(int)(vars->posy - vars->diry * vars->moving_speed)] != 1)
		vars->posy -= vars->diry * vars->moving_speed;
}

static void	go_right(t_vars *vars)
{
	if ((vars->map)[(int)(vars->posx + vars->planex * vars->moving_speed)]\
												[(int)(vars->posy)] != 1)
		vars->posx += (vars->planex) * vars->moving_speed;
	if ((vars->map)[(int)(vars->posx)]\
				[(int)(vars->posy + vars->planey * vars->moving_speed)] != 1)
		vars->posy += (vars->planey) * vars->moving_speed;
}

static void	go_left(t_vars *vars)
{
	if (vars->map[(int)(vars->posx - vars->planex * vars->moving_speed)]\
												[(int)(vars->posy)] != 1)
		vars->posx -= (vars->planex) * vars->moving_speed;
	if (vars->map[(int)(vars->posx)]\
				[(int)(vars->posy - vars->planey * vars->moving_speed)] != 1)
		vars->posy -= (vars->planey) * vars->moving_speed;
}

int			move_keys(int keycode, t_vars *vars)
{
	vars->moving_speed = 0.3;
	vars->rot_speed = 0.2;
	if (keycode == 13)
		go_forward(vars);
	if (keycode == 1)
		go_backward(vars);
	if (keycode == 2)
		go_right(vars);
	if (keycode == 0)
		go_left(vars);
	if (keycode == 124)
		rotate_right(vars);
	if (keycode == 123)
		rotate_left(vars);
	if (keycode == 53)
		exit(0);
	ft_bzero(vars->img.data, sizeof(int) * vars->window_width *\
											vars->window_height);
	if (!render(vars))
		return (0);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
	return (1);
}
