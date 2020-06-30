/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wall_texture.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:19 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:43:19 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_texture_body(char **path, char element, char *line)
{
	char	**info;
	int		i;

	if (!(info = ft_split(ft_strchr(line, element), ' ')))
		return (0);
	i = 0;
	while (info[i] != 0)
		++i;
	if (i != 2 || ft_strlen(info[0]) != 2)
	{
		free_char_array(info);
		return (0);
	}
	if (!(*path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1))))
	{
		free_char_array(info);
		return (0);
	}
	ft_memmove(*path, info[1], ft_strlen(info[1]) + 1);
	free_char_array(info);
	return (1);
}

int			get_north_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "NO", ft_strlen(line)))
	{
		if (read->north_texture == 1)
			return (0);
		if (!get_texture_body(&read->north_path, 'N', line))
			return (0);
		read->north_texture = 1;
	}
	return (1);
}

int			get_south_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "SO", ft_strlen(line)))
	{
		if (read->south_texture == 1)
			return (0);
		if (!get_texture_body(&read->south_path, 'S', line))
			return (0);
		read->south_texture = 1;
	}
	return (1);
}

int			get_west_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "WE", ft_strlen(line)))
	{
		if (read->west_texture == 1)
			return (0);
		if (!get_texture_body(&read->west_path, 'W', line))
			return (0);
		read->west_texture = 1;
	}
	return (1);
}

int			get_east_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "EA", ft_strlen(line)))
	{
		if (read->east_texture == 1)
			return (0);
		if (!get_texture_body(&read->east_path, 'E', line))
			return (0);
		read->east_texture = 1;
	}
	return (1);
}
