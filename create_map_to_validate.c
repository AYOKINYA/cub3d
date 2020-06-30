/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_map_to_validate.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:41:40 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:41:41 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	malloc_test_map(int **test, int row_max, int col_max)
{
	int i;
	int j;

	i = 0;
	while (i < row_max)
	{
		if (!(test[i] = (int *)malloc(sizeof(int) * col_max)))
		{
			j = 0;
			while (j < i)
			{
				free(test[j]);
				++j;
			}
			return (0);
		}
		++i;
	}
	return (1);
}

static void	init_test_map(int **test, int row_max, int col_max)
{
	int i;
	int j;

	i = 0;
	while (i < row_max)
	{
		j = 0;
		while (j < col_max)
		{
			test[i][j] = 'X' - '0';
			++j;
		}
		++i;
	}
}

int			**creat_map_to_validate(t_config *read, t_map *map, int col_max)
{
	int row_max;
	int **test;
	int i;
	int j;

	row_max = read->map_row_size;
	if (!(test = (int **)malloc(sizeof(int *) * row_max)))
		return (0);
	if (!malloc_test_map(test, row_max, col_max))
	{
		free(test);
		return (0);
	}
	init_test_map(test, row_max, col_max);
	i = -1;
	while (++i < read->map_row_size)
	{
		j = -1;
		while (++j < map->col_size)
			test[i][j] = map->row[j];
		map = map->next;
	}
	return (test);
}
