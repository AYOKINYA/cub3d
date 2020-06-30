/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:41:50 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:51:39 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_ceilling(t_vars *vars, int x)
{
	int ceil;

	ceil = 0;
	while (ceil < vars->drawstart)
	{
		vars->img.data[ceil * vars->window_width + x] = vars->ceilling_color;
		++ceil;
	}
}

static void	draw_floor(t_vars *vars, int x)
{
	int y;

	y = vars->drawend;
	while (y < vars->window_height)
	{
		vars->img.data[y * vars->window_width + x] = vars->floor_color;
		++y;
	}
}

void		draw_all(t_vars *vars, int x)
{
	draw_ceilling(vars, x);
	draw_walls(vars, x);
	draw_floor(vars, x);
}
