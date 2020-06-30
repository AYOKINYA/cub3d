/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   feed_position.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:42:44 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:46:50 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_spawn_spot(int pos)
{
	if (pos == 'N' - '0')
		return ('N' - '0');
	else if (pos == 'S' - '0')
		return ('S' - '0');
	else if (pos == 'W' - '0')
		return ('W' - '0');
	else if (pos == 'E' - '0')
		return ('E' - '0');
	else
		return (0);
}

static void	set_spawn_dir(int x, int y, t_vars *vars, char spawn_dir)
{
	double radian;

	radian = 0;
	vars->posx = x + 0.5;
	vars->posy = y + 0.5;
	vars->dirx = -1;
	vars->diry = 0;
	vars->planex = 0;
	vars->planey = 0.66;
	if (spawn_dir == 'N' - '0')
		radian = 0 * 3.141592 / 180;
	else if (spawn_dir == 'S' - '0')
		radian = 180 * 3.141592 / 180;
	else if (spawn_dir == 'W' - '0')
		radian = 90 * 3.141592 / 180;
	else if (spawn_dir == 'E' - '0')
		radian = 270 * 3.141592 / 180;
	vars->olddirx = vars->dirx;
	vars->dirx = vars->dirx * cos(radian) - vars->diry * sin(radian);
	vars->diry = vars->olddirx * sin(radian) + vars->diry * cos(radian);
	vars->oldplanex = vars->oldplanex;
	vars->planex = vars->planex * cos(radian) - vars->planey * sin(radian);
	vars->planey = vars->oldplanex * sin(radian) + vars->planey * cos(radian);
}

void		feed_position(t_vars *vars, t_config *read)
{
	int		i;
	int		j;
	t_map	*temp;

	temp = read->map;
	i = 0;
	while (i < read->map_row_size)
	{
		j = 0;
		while (j < temp->col_size)
		{
			if (is_spawn_spot(temp->row[j]))
				break ;
			++j;
		}
		if (is_spawn_spot(temp->row[j]))
			break ;
		++i;
		temp = temp->next;
	}
	set_spawn_dir(i, j, vars, temp->row[j]);
}
