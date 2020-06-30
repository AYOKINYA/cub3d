/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:42:58 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 15:47:27 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		free_test_map(int **test, int row_size)
{
	if (test == 0)
		return (0);
	while (row_size--)
		free(test[row_size]);
	free(test);
	return (0);
}

int		free_int_array(int **arr)
{
	int i;

	if (arr == 0)
		return (0);
	i = 0;
	while (arr[i] != 0)
		++i;
	while (i > 0 && i--)
		free(arr[i]);
	free(arr);
	return (0);
}

void	free_char_array(char **arr)
{
	int i;

	if (arr == 0)
		return ;
	i = 0;
	while (arr[i] != 0)
		++i;
	while (i > 0 && i--)
		free(arr[i]);
	free(arr);
}

int		free_all_texture(t_config *read)
{
	if (read->north_texture == 1)
		free(read->north_path);
	if (read->south_texture == 1)
		free(read->south_path);
	if (read->west_texture == 1)
		free(read->west_path);
	if (read->east_texture == 1)
		free(read->east_path);
	if (read->sprite_texture == 1)
		free(read->sprite_path);
	return (0);
}

int		free_all(t_vars *vars, t_config *read)
{
	free_all_texture(read);
	free(read);
	free_all_loads(vars);
	free(vars);
	return (0);
}
