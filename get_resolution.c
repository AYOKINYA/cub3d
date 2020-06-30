/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_resolution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:11 by jkang             #+#    #+#             */
/*   Updated: 2020/06/30 14:11:08 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	validate_resolution(t_config *read)
{
	int h;

	h = read->resolution_height;
	if (read->resolution_width == 640)
	{
		if (h != 180 && h != 1080 && h != 1440 &&\
							(h < 480 || h > 1000))
			return (0);
	}
	else if ((read->resolution_width * 9 != read->resolution_height * 16)\
	&& (read->resolution_width * 2 != read->resolution_height * 3) &&\
		(read->resolution_width * 3 != read->resolution_height * 4))
		return (0);
	return (1);
}

static int	get_resolution_body(t_config *read, char *line)
{
	char	**info;
	int		i;

	if (!(info = ft_split(ft_strchr(line, 'R'), ' ')))
		return (0);
	i = 0;
	while (info[i] != 0)
		++i;
	if (i != 3 || ft_strlen(info[0]) != 1)
	{
		free_char_array(info);
		return (0);
	}
	read->resolution_width = ft_atoi(info[1]);
	read->resolution_height = ft_atoi(info[2]);
	if (read->resolution_width <= 0 || read->resolution_height <= 0)
	{
		free_char_array(info);
		return (0);
	}
	free_char_array(info);
	return (1);
}

int			get_resolution(t_config *read, char *line)
{
	if (ft_strchr(line, 'R'))
	{
		if (read->resolution_flag == 1)
			return (0);
		if (!get_resolution_body(read, line))
			return (0);
		if (read->resolution_width > 1920)
			read->resolution_width = 1920;
		if (read->resolution_height > 1080)
			read->resolution_height = 1080;
		if (!validate_resolution(read))
			return (0);
		read->resolution_flag = 1;
	}
	return (1);
}
