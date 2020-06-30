/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_before_map.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:33 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:43:34 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_texture(t_config *read, char *line)
{
	if (!get_north_texture_path(read, line))
		return (0);
	if (!get_south_texture_path(read, line))
		return (0);
	if (!get_west_texture_path(read, line))
		return (0);
	if (!get_east_texture_path(read, line))
		return (0);
	if (!get_sprite_texture_path(read, line))
		return (0);
	return (1);
}

static int	get_color(t_config *read, char *line)
{
	if (!get_floor_color(read, line))
		return (0);
	if (!get_ceilling_color(read, line))
		return (0);
	return (1);
}

int			read_before_map(t_config *read, char *line)
{
	if (!get_resolution(read, line))
		return (0);
	if (!get_color(read, line))
		return (0);
	if (!get_texture(read, line))
		return (0);
	return (1);
}
