/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:42:28 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:42:54 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		sort_sprites(int *sprite_order, double *sprite_distance,\
														int numsprites)
{
	int		i;
	int		j;
	int		temp_order;
	double	temp_distance;

	i = -1;
	while (++i < numsprites)
	{
		j = -1;
		while (++j < numsprites - 1)
		{
			if (sprite_distance[j] < sprite_distance[j + 1])
			{
				temp_distance = sprite_distance[j];
				sprite_distance[j] = sprite_distance[j + 1];
				sprite_distance[j + 1] = temp_distance;
				temp_order = sprite_order[j];
				sprite_order[j] = sprite_order[j + 1];
				sprite_order[j + 1] = temp_order;
			}
		}
	}
}

void		transform_sprites_coord(t_vars *vars, t_sprite_calc *calc)
{
	int w;

	w = vars->window_width;
	calc->invdet = 1.0 / \
				(vars->planex * vars->diry - vars->dirx * vars->planey);
	calc->transformx = calc->invdet *\
			(vars->diry * calc->spritex - vars->dirx * calc->spritey);
	calc->transformy = calc->invdet *\
			(-1 * vars->planey * calc->spritex + vars->planex * calc->spritey);
	calc->spritescreenx = (int)((w / 2) *\
						(1 + calc->transformx / calc->transformy));
}

void		calculate_sprites_height_width(t_vars *vars, t_sprite_calc *calc)
{
	int			w;
	int			h;

	w = vars->window_width;
	h = vars->window_height;
	calc->sprite_height = abs((int)(h / calc->transformy));
	calc->draw_y_start = calc->sprite_height / -2 + h / 2;
	if (calc->draw_y_start < 0)
		calc->draw_y_start = 0;
	calc->draw_y_end = calc->sprite_height / 2 + h / 2;
	if (calc->draw_y_end >= h)
		calc->draw_y_end = h - 1;
	calc->sprite_width = abs((int)(h / calc->transformy));
	calc->draw_x_start = calc->sprite_width / -2 + calc->spritescreenx;
	if (calc->draw_x_start < 0)
		calc->draw_x_start = 0;
	calc->draw_x_end = calc->sprite_width / 2 + calc->spritescreenx;
	if (calc->draw_x_end >= w)
		calc->draw_x_end = w - 1;
}

static void	draw_sprites_body(t_sprite_calc *calc, t_vars *vars, int stripe)
{
	int y;
	int d;
	int color;

	y = calc->draw_y_start;
	d = 0;
	color = 0;
	while (y < calc->draw_y_end)
	{
		d = (y) * 256 - vars->window_height * 128 + calc->sprite_height * 128;
		calc->stripe_tex_y =\
				((d * vars->tex[4].img_height) / calc->sprite_height) / 256;
		color = vars->tex[4].data[vars->tex[4].img_width *\
									calc->stripe_tex_y + calc->stripe_tex_x];
		if ((color & 0x00FFFFFF) != 0)
			vars->img.data[y * vars->window_width + stripe] = color;
		++y;
	}
}

void		draw_sprites(t_sprite_calc *calc, t_vars *vars)
{
	int	stripe;

	stripe = calc->draw_x_start;
	while (stripe < calc->draw_x_end)
	{
		calc->stripe_tex_x = (int)(256 * (stripe -\
				(calc->sprite_width / -2 + calc->spritescreenx)) *\
						vars->tex[4].img_width / calc->sprite_width) / 256;
		if (calc->transformy > 0 && stripe > 0 \
				&& stripe < vars->window_width &&\
						calc->transformy < vars->zbuffer[stripe])
			draw_sprites_body(calc, vars, stripe);
		++stripe;
	}
}
