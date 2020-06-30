/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/12 16:19:04 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:46:59 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	read_and_play(t_vars *vars, t_config *read, char *argv[],\
															int save_flag)
{
	if (!read_config(read, argv[1]))
		return (free_all(vars, read));
	if (!config_to_vars(vars, read))
		return (free_all(vars, read));
	if (!play_on_screen(vars, read, save_flag))
		return (free_all(vars, read));
	return (1);
}

static int	return_error(void)
{
	write(1, "Error\n", 6);
	return (0);
}

static void	init_read_flag(t_config *read)
{
	read->resolution_flag = 0;
	read->north_texture = 0;
	read->south_texture = 0;
	read->west_texture = 0;
	read->east_texture = 0;
	read->sprite_texture = 0;
	read->floor_flag = 0;
	read->map_start_flag = 0;
	read->map_end_flag = 0;
	read->surrounding_flag = 0;
	read->spawn_flag = 0;
}

static int	argc_check(int *save_flag, int argc, char *argv[])
{
	if (argc < 2)
		return (0);
	else if (argc == 2)
		*save_flag = 0;
	else if (argc == 3 && ft_strcmp(argv[2], "--save") == 0)
		*save_flag = 1;
	else
		return (0);
	return (1);
}

int			main(int argc, char *argv[])
{
	t_vars		*vars;
	t_config	*read;
	int			save_flag;

	save_flag = 0;
	if (!(argc_check(&save_flag, argc, argv)))
		return (return_error());
	if (!(vars = (t_vars*)malloc(sizeof(t_vars))))
		return_error();
	if (!(read = (t_config *)malloc(sizeof(t_config))))
	{
		free(vars);
		return (return_error());
	}
	init_read_flag(read);
	if (!read_and_play(vars, read, argv, save_flag))
		return (return_error());
	free_all(vars, read);
	return (0);
}
