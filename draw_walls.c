/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:42:38 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:45:19 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	get_texture_loc(t_vars *vars, int tex_width, int tex_height)
{
	if (vars->side == 0)
		vars->wallx = vars->posy + vars->perpwalldist * vars->raydiry;
	else
		vars->wallx = vars->posx + vars->perpwalldist * vars->raydirx;
	vars->wallx -= floor((vars->wallx));
	vars->texx = (int)(vars->wallx * (double)(tex_width));
	if (vars->side == 0 && vars->raydirx > 0)
		vars->texx = tex_width - vars->texx - 1;
	if (vars->side == 1 && vars->raydiry < 0)
		vars->texx = tex_width - vars->texx - 1;
	vars->step = 1.0 * tex_height / vars->lineheight;
	vars->texpos = 0.0;
}

static void	draw_tex_on_walls(t_vars *vars, int x, int index)
{
	int y;
	int color;
	int tex_width;
	int tex_height;

	y = vars->drawstart;
	color = 0;
	tex_width = vars->tex[index].img_width;
	tex_height = vars->tex[index].img_height;
	get_texture_loc(vars, tex_width, tex_height);
	while (y < vars->drawend)
	{
		vars->texy = (int)(vars->texpos) & (tex_height - 1);
		vars->texpos += vars->step;
		if ((int)vars->texy >= 0 && (int)vars->texy <= tex_height - 1 &&\
			y >= 0 && y <= vars->window_height)
		{
			color = vars->tex[index].data[tex_width * vars->texy + vars->texx];
			vars->img.data[y * vars->window_width + x] = color;
		}
		++y;
	}
}

void		draw_walls(t_vars *vars, int x)
{
	if (vars->side == 1 && vars->raydiry >= 0)
		draw_tex_on_walls(vars, x, 0);
	else if (vars->side == 1 && vars->raydiry < 0)
		draw_tex_on_walls(vars, x, 1);
	else if (vars->side == 0 && vars->raydirx >= 0)
		draw_tex_on_walls(vars, x, 2);
	else if (vars->side == 0 && vars->raydirx < 0)
		draw_tex_on_walls(vars, x, 3);
}
