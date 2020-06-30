/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:02 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:43:03 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	validate_color(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (0);
	else if (g < 0 || g > 255)
		return (0);
	else if (b < 0 || b > 255)
		return (0);
	else
		return (1);
}

static int	get_floor_color_body(t_config *read, char *line)
{
	char	**info;
	char	**split_str;
	int		i;

	if (!(info = ft_split(ft_strchr(line, 'F'), ',')))
		return (0);
	i = 0;
	while (info[i] != 0)
		++i;
	if (!(split_str = ft_split(ft_strchr(info[0], 'F'), ' ')))
		return (0);
	if (i != 3 || ft_strcmp(split_str[0], "F") != 0)
	{
		free_char_array(split_str);
		free_char_array(info);
		return (0);
	}
	read->floor_r = ft_atoi(split_str[1]);
	read->floor_g = ft_atoi(info[1]);
	read->floor_b = ft_atoi(info[2]);
	free_char_array(split_str);
	free_char_array(info);
	return (1);
}

int			get_floor_color(t_config *read, char *line)
{
	if (ft_strchr(line, 'F'))
	{
		if (read->floor_flag == 1)
			return (0);
		if (!get_floor_color_body(read, line))
			return (0);
		if (!validate_color(read->floor_r, read->floor_g, read->floor_b))
			return (0);
		read->floor_flag = 1;
	}
	return (1);
}

static int	get_ceilling_color_body(t_config *read, char *line)
{
	char	**info;
	char	**split_str;
	int		i;

	if (!(info = ft_split(ft_strchr(line, 'C'), ',')))
		return (0);
	i = 0;
	while (info[i] != 0)
		++i;
	if (!(split_str = ft_split(ft_strchr(info[0], 'C'), ' ')))
		return (0);
	if (i != 3 || ft_strcmp(split_str[0], "C") != 0)
	{
		free_char_array(split_str);
		free_char_array(info);
		return (0);
	}
	read->ceilling_r = ft_atoi(split_str[1]);
	read->ceilling_g = ft_atoi(info[1]);
	read->ceilling_b = ft_atoi(info[2]);
	free_char_array(split_str);
	free_char_array(info);
	return (1);
}

int			get_ceilling_color(t_config *read, char *line)
{
	if (ft_strchr(line, 'C'))
	{
		if (read->ceilling_flag == 1)
			return (0);
		if (!get_ceilling_color_body(read, line))
			return (0);
		if (!validate_color(read->ceilling_r, \
				read->ceilling_g, read->ceilling_b))
			return (0);
		read->ceilling_flag = 1;
	}
	return (1);
}
