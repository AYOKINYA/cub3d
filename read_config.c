/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:36 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:43:37 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	final_check_elements(t_config *read)
{
	int check;

	check = read->resolution_flag * read->north_texture *\
				read->south_texture * read->west_texture * read->east_texture;
	if (check != 1)
		return (0);
	check = check * read->sprite_texture * read->floor_flag *\
												read->ceilling_flag;
	if (check != 1)
		return (0);
	check = check * read->map_start_flag * read->map_end_flag *\
						read->surrounding_flag * read->spawn_flag;
	if (check != 1)
		return (0);
	return (check);
}

static int	read_config_body(t_map *map, t_config *read, char *argv)
{
	if (!read_map(map, read, argv))
		return (0);
	read->map_row_size = map_lstsize(map);
	if (!check_surrounding(read, map))
		return (0);
	if (!final_check_elements(read))
		return (0);
	return (1);
}

static int	is_cub_file(char *argv)
{
	size_t len;

	len = ft_strlen(argv);
	if (len < 5)
		return (0);
	else if (argv[len - 1] != 'b' || argv[len - 2] != 'u' ||\
				argv[len - 3] != 'c' || argv[len - 4] != '.')
		return (0);
	return (1);
}

int			read_config(t_config *read, char *argv)
{
	if (!is_cub_file(argv))
		return (0);
	if (!(read->map = (t_map *)malloc(sizeof(t_map))))
		return (0);
	read->map->row = 0;
	read->map->col_size = 0;
	read->map->next = 0;
	if (!read_config_body(read->map, read, argv))
	{
		map_lstclear(&read->map, free);
		return (0);
	}
	return (1);
}
