/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sprite_texture.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:16 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:43:16 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_sprite_texture_body(char **path, char element, char *line)
{
	char	**info;
	int		i;

	if (!(info = ft_split(ft_strchr(line, element), ' ')))
		return (0);
	i = 0;
	while (info[i] != 0)
		++i;
	if (i != 2 || ft_strlen(info[0]) != 1)
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

int			get_sprite_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "S ", ft_strlen(line)))
	{
		if (read->sprite_texture == 1)
			return (0);
		if (!get_sprite_texture_body(&read->sprite_path, 'S', line))
			return (0);
		read->sprite_texture = 1;
	}
	return (1);
}
