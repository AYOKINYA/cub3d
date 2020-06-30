/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:05 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:43:06 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	ft_is_element(char element)
{
	int ret;

	ret = 0;
	if (element == ' ')
		ret = 1;
	if (element >= '0' && element <= '2')
		ret = 2;
	if (element == 'N' || element == 'S' || element == 'W' || element == 'E')
		ret = 3;
	return (ret);
}

static int	check_map_element(char *line)
{
	int		i;
	char	*trimmed_line;

	i = 0;
	while (line[i] != 0)
	{
		if (!ft_is_element(line[i]))
			return (0);
		if (!(trimmed_line = ft_strtrim(line, " ")))
			return (0);
		if (trimmed_line[0] != '1')
		{
			free(trimmed_line);
			return (0);
		}
		if (trimmed_line[ft_strlen(trimmed_line) - 1] != '1')
		{
			free(trimmed_line);
			return (0);
		}
		free(trimmed_line);
		++i;
	}
	return (1);
}

static int	check_last_row(t_config *read, char *line)
{
	int index;

	index = 0;
	while (line[index] != 0)
	{
		if (line[index] != ' ' && line[index] != '1')
			return (0);
		++index;
	}
	read->map_end_flag = 1;
	return (1);
}

static int	create_rows(char *line, t_map *map)
{
	t_map	*new;
	int		col_size;
	int		j;

	if (!(new = (t_map *)malloc(sizeof(t_map))))
		return (0);
	col_size = ft_strlen(line);
	if (!(new->row = (int *)malloc(sizeof(int) * col_size)))
	{
		free(new);
		return (0);
	}
	j = 0;
	while (j < col_size)
	{
		if (line[j] == ' ')
			new->row[j] = 1;
		else
			new->row[j] = line[j] - '0';
		++j;
	}
	new->col_size = col_size;
	new->next = 0;
	map_lstadd_back(&map, new);
	return (1);
}

int			get_map(t_config *read, char *line, t_map *map)
{
	if (read->map_end_flag == 1)
		return (0);
	if (!check_map_element(line))
		return (0);
	if (!create_rows(line, map))
		return (0);
	if (!check_spawn_element(read, line))
		return (0);
	check_last_row(read, line);
	return (1);
}
