/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_and_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:59:02 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:48:58 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate_right(t_vars *vars)
{
	vars->olddirx = vars->dirx;
	vars->dirx = vars->dirx * cos(-vars->rot_speed) -\
							vars->diry * sin(-vars->rot_speed);
	vars->diry = vars->olddirx * sin(-vars->rot_speed) +\
							vars->diry * cos(-vars->rot_speed);
	vars->oldplanex = vars->planex;
	vars->planex = vars->planex * cos(-vars->rot_speed) -\
							vars->planey * sin(-vars->rot_speed);
	vars->planey = vars->oldplanex * sin(-vars->rot_speed) +\
							vars->planey * cos(-vars->rot_speed);
}

void	rotate_left(t_vars *vars)
{
	vars->olddirx = vars->dirx;
	vars->dirx = vars->dirx * cos(vars->rot_speed) -\
								vars->diry * sin(vars->rot_speed);
	vars->diry = vars->olddirx * sin(vars->rot_speed) + \
								vars->diry * cos(vars->rot_speed);
	vars->oldplanex = vars->planex;
	vars->planex = vars->planex * cos(vars->rot_speed) -\
								vars->planey * sin(vars->rot_speed);
	vars->planey = vars->oldplanex * sin(vars->rot_speed) +\
								vars->planey * cos(vars->rot_speed);
}

int		free_all_loads(t_vars *vars)
{
	if (vars->tex != 0)
		free(vars->tex);
	if (vars->sprites != 0)
		free(vars->sprites);
	if (vars->zbuffer != 0)
		free(vars->zbuffer);
	return (0);
}
