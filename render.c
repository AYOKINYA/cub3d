/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:44 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:48:40 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	go_to_intersection(t_vars *vars)
{
	if (vars->raydirx < 0)
	{
		vars->stepx = -1;
		vars->sidedistx = (vars->posx - vars->mapx) * vars->deltadistx;
	}
	else
	{
		vars->stepx = 1;
		vars->sidedistx = (vars->mapx + 1.0 - vars->posx) * vars->deltadistx;
	}
	if (vars->raydiry < 0)
	{
		vars->stepy = -1;
		vars->sidedisty = (vars->posy - vars->mapy) * vars->deltadisty;
	}
	else
	{
		vars->stepy = 1;
		vars->sidedisty = (vars->mapy + 1.0 - vars->posy) * vars->deltadisty;
	}
}

static void	check_wall_hit(t_vars *vars)
{
	int hit;

	hit = 0;
	while (hit == 0)
	{
		if (vars->sidedistx < vars->sidedisty)
		{
			vars->sidedistx += vars->deltadistx;
			vars->mapx += vars->stepx;
			vars->side = 0;
		}
		else
		{
			vars->sidedisty += vars->deltadisty;
			vars->mapy += vars->stepy;
			vars->side = 1;
		}
		if ((vars->map)[vars->mapx][vars->mapy] == 1)
			hit = 1;
	}
}

static void	get_height_to_draw(t_vars *vars)
{
	int h;

	h = vars->window_height;
	if (vars->side == 0)
		vars->perpwalldist =\
			(vars->mapx - vars->posx + (1 - vars->stepx) / 2) / vars->raydirx;
	else
		vars->perpwalldist =\
			(vars->mapy - vars->posy + (1 - vars->stepy) / 2) / vars->raydiry;
	vars->lineheight = (int)(h / vars->perpwalldist);
	vars->drawstart = vars->lineheight / -2 + h / 2;
	vars->drawend = vars->lineheight / 2 + h / 2;
}

int			render(t_vars *vars)
{
	int x;

	x = 0;
	while (x < vars->window_width)
	{
		vars->camerax = 2 * x / (double)vars->window_width - 1;
		vars->raydirx = vars->dirx + vars->planex * vars->camerax;
		vars->raydiry = vars->diry + vars->planey * vars->camerax;
		vars->mapx = (int)(vars->posx);
		vars->mapy = (int)(vars->posy);
		vars->deltadistx = fabs(1 / vars->raydirx);
		vars->deltadisty = fabs(1 / vars->raydiry);
		go_to_intersection(vars);
		check_wall_hit(vars);
		get_height_to_draw(vars);
		draw_all(vars, x);
		vars->zbuffer[x] = vars->perpwalldist;
		++x;
	}
	if (!add_sprites(vars))
		return (0);
	return (1);
}
