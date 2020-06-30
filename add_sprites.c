/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_sprites.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/28 22:47:40 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:45:48 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	add_sprites_body(t_vars *vars, int *spriteorder,\
							double *spritedistance, t_sprite_calc *calc)
{
	int i;

	i = 0;
	while (i < vars->num_sprite)
	{
		spriteorder[i] = i;
		spritedistance[i] = ((vars->posx - vars->sprites[i].x) *\
		(vars->posx - vars->sprites[i].x) + (vars->posy - vars->sprites[i].y)\
										* (vars->posy - vars->sprites[i].y));
		++i;
	}
	sort_sprites(spriteorder, spritedistance, vars->num_sprite);
	i = 0;
	while (i < vars->num_sprite)
	{
		calc->spritex = vars->sprites[spriteorder[i]].x - vars->posx;
		calc->spritey = vars->sprites[spriteorder[i]].y - vars->posy;
		transform_sprites_coord(vars, calc);
		calculate_sprites_height_width(vars, calc);
		draw_sprites(calc, vars);
		++i;
	}
}

int			add_sprites(t_vars *vars)
{
	int				*spriteorder;
	double			*spritedistance;
	t_sprite_calc	*calc;

	if (vars->num_sprite == 0)
		return (1);
	if (!(spriteorder = (int *)malloc(sizeof(int) * vars->num_sprite)))
		return (0);
	if (!(spritedistance = (double *)malloc(sizeof(double) * vars->num_sprite)))
	{
		free(spriteorder);
		return (0);
	}
	if (!(calc = (t_sprite_calc *)malloc(sizeof(t_sprite_calc))))
	{
		free(spriteorder);
		free(spritedistance);
		return (0);
	}
	add_sprites_body(vars, spriteorder, spritedistance, calc);
	free(spriteorder);
	free(spritedistance);
	free(calc);
	return (1);
}
