/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_surrounding.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:41:30 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:55:57 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	check_up_body(int **test, int row, int col)
{
	int i;
	int up;

	i = 0;
	up = 0;
	while (i < row)
	{
		if (test[i][col] == 1)
			up = 1;
		++i;
	}
	return (up);
}

static int	check_up_wall(int **test, int row_max, int col_max)
{
	int col;
	int row;

	row = 1;
	while (row < row_max - 1)
	{
		col = 1;
		while (col < col_max - 1)
		{
			if (test[row][col] != 1 && test[row][col] != 'X' - '0')
			{
				if (!check_up_body(test, row, col))
					return (0);
			}
			++col;
		}
		++row;
	}
	return (1);
}

static int	check_down_body(int **test, int row, int col, int row_max)
{
	int i;
	int down;

	down = 0;
	i = row + 1;
	while (i < row_max)
	{
		if (test[i][col] == 1)
			down = 1;
		++i;
	}
	return (down);
}

static int	check_down_wall(int **test, int row_max, int col_max)
{
	int col;
	int row;

	row = 1;
	while (row < row_max - 1)
	{
		col = 1;
		while (col < col_max - 1)
		{
			if (test[row][col] != 1 && test[row][col] != 'X' - '0')
			{
				if (!check_down_body(test, row, col, row_max))
					return (0);
			}
			++col;
		}
		++row;
	}
	return (1);
}

int			check_surrounding(t_config *read, t_map *map)
{
	t_map	*temp;
	int		col_max;
	int		row_max;
	int		**test;

	col_max = 0;
	row_max = read->map_row_size;
	temp = map;
	while (temp != 0)
	{
		if (col_max < temp->col_size)
			col_max = temp->col_size;
		temp = temp->next;
	}
	if (col_max <= 2 || row_max <= 2)
		return (0);
	if (!(test = creat_map_to_validate(read, map, col_max)))
		return (0);
	if (!check_up_wall(test, row_max, col_max))
		return (free_test_map(test, row_max));
	if (!check_down_wall(test, row_max, col_max))
		return (free_test_map(test, row_max));
	read->surrounding_flag = 1;
	free_test_map(test, row_max);
	return (1);
}
