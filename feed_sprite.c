/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   feed_sprite.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:42:54 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:56:55 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_num_sprite(t_config *read)
{
	int		i;
	int		j;
	int		ret;
	t_map	*temp;

	ret = 0;
	temp = read->map;
	i = 0;
	while (i < read->map_row_size)
	{
		j = 0;
		while (j < temp->col_size)
		{
			if (temp->row[j] == 2)
				++ret;
			++j;
		}
		++i;
		temp = temp->next;
	}
	return (ret);
}

static int	feed_sprite_body(t_vars *vars, t_config *read, t_map *temp)
{
	int i;
	int j;
	int idx;

	idx = 0;
	i = 0;
	while (i < read->map_row_size)
	{
		j = 0;
		while (j < temp->col_size && idx != vars->num_sprite)
		{
			if (temp->row[j] == 2)
			{
				vars->sprites[idx].x = i;
				vars->sprites[idx].y = j + 0.5;
				++idx;
			}
			++j;
		}
		++i;
		temp = temp->next;
	}
	return (1);
}

int			feed_sprite(t_vars *vars, t_config *read)
{
	t_map *temp;

	vars->num_sprite = get_num_sprite(read);
	if (vars->num_sprite == 0)
		return (1);
	vars->sprites = (t_sprite *)malloc(sizeof(t_sprite) * vars->num_sprite);
	if (vars->sprites == 0)
		return (0);
	temp = read->map;
	if (!feed_sprite_body(vars, read, temp))
		return (0);
	if (!(vars->zbuffer = (int *)malloc(sizeof(int) * vars->window_width)))
	{
		free(vars->sprites);
		vars->sprites = 0;
		return (0);
	}
	return (1);
}
