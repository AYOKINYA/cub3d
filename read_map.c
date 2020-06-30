/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:39 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 14:11:52 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	map_is_the_last_element(t_config *read)
{
	int check;

	check = read->resolution_flag * read->floor_flag * read->ceilling_flag;
	check = check * read->sprite_texture * read->north_texture *\
			read->south_texture * read->west_texture * read->east_texture;
	if (check == 1)
		return (check);
	else
		return (0);
}

static int	map_start(t_config *read, char *line, t_map *map)
{
	int index;

	if (*line == '\0')
		return (0);
	index = -1;
	while (line[++index] != 0)
	{
		if (line[index] != ' ' && line[index] != '1')
			return (0);
	}
	if (!map_is_the_last_element(read))
		return (-1);
	if (!(map->row = (int *)malloc(sizeof(int) * index)))
		return (-1);
	map->col_size = index;
	index = -1;
	while (++index < map->col_size)
	{
		if (ft_is_space(line[index]))
			map->row[index] = 1;
		else
			map->row[index] = line[index] - '0';
	}
	read->map_start_flag = 1;
	return (1);
}

static int	read_till_map_starts(int *fd, t_map *map, t_config *read)
{
	int			ret;
	char		*line;

	line = 0;
	while (get_next_line(*fd, &line) > 0)
	{
		if (!read_before_map(read, line))
		{
			free(line);
			return (0);
		}
		if ((ret = map_start(read, line, map)) == 1)
		{
			free(line);
			return (1);
		}
		else if (ret == -1)
		{
			free(line);
			return (0);
		}
		free(line);
	}
	free(line);
	return (1);
}

static int	read_till_map_ends(int *fd, t_map *map, t_config *read)
{
	size_t		ret;
	char		*line;

	line = 0;
	if (read->map_start_flag == 1)
	{
		while ((ret = get_next_line(*fd, &line)) > 0)
		{
			if (!get_map(read, line, map) || *line == '\0')
			{
				free(line);
				return (0);
			}
			free(line);
		}
	}
	else
	{
		write(1, "Error1\n", 6);
		free(line);
		return (0);
	}
	return (1);
}

int			read_map(t_map *map, t_config *read, char *argv)
{
	int	fd;

	fd = open(argv, O_RDONLY);
	if (fd == -1)
	{
		close(fd);
		return (0);
	}
	if (!read_till_map_starts(&fd, map, read))
		return (0);
	if (!read_till_map_ends(&fd, map, read))
		return (0);
	close(fd);
	return (1);
}
