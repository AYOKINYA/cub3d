/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_to_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:41:36 by jkang             #+#    #+#             */
/*   Updated: 2020/06/30 14:07:06 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	feed_resolution(t_vars *vars, t_config *read)
{
	vars->window_width = 1920;
	vars->window_height = 1080;
	if (vars->window_width > read->resolution_width)
		vars->window_width = read->resolution_width;
	if (vars->window_height > read->resolution_height)
		vars->window_height = read->resolution_height;
}

static int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

static void	feed_color(t_vars *vars, t_config *read)
{
	int r;
	int g;
	int b;

	r = read->floor_r;
	g = read->floor_g;
	b = read->floor_b;
	vars->floor_color = create_trgb(0, r, g, b);
	r = read->ceilling_r;
	g = read->ceilling_g;
	b = read->ceilling_b;
	vars->ceilling_color = create_trgb(0, r, g, b);
}

int			feed_map(t_vars *vars, t_config *read)
{
	int		i;
	int		j;
	t_map	*temp;

	if (!(vars->map = (int **)(malloc(sizeof(int *) * read->map_row_size))))
		return (0);
	temp = read->map;
	i = -1;
	while (++i < read->map_row_size)
	{
		if (!(vars->map[i] = (int *)(malloc(sizeof(int *) * temp->col_size))))
		{
			free_int_array(vars->map);
			return (0);
		}
		j = -1;
		while (++j < temp->col_size)
			vars->map[i][j] = temp->row[j];
		temp = temp->next;
	}
	map_lstclear(&read->map, free);
	return (1);
}

int			config_to_vars(t_vars *vars, t_config *read)
{
	feed_resolution(vars, read);
	feed_position(vars, read);
	if (!feed_sprite(vars, read))
		return (0);
	feed_color(vars, read);
	if (!feed_map(vars, read))
	{
		free(vars->sprites);
		free(vars->zbuffer);
		return (0);
	}
	return (1);
}
