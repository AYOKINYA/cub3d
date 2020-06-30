/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_spawn_elem.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:39:08 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:44:50 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	feed_spawn(t_config *read, char spawn)
{
	if (read->spawn_flag == 1)
		return (0);
	read->spawn_flag = 1;
	read->spawn_dir = spawn - '0';
	return (1);
}

static int	check_n_s_spawn(t_config *read, char element)
{
	if (element - '0' == 'N' - '0')
	{
		if (!feed_spawn(read, 'N'))
			return (0);
	}
	else if (element - '0' == 'S' - '0')
	{
		if (!feed_spawn(read, 'S'))
			return (0);
	}
	return (1);
}

static int	check_w_e_spawn(t_config *read, char element)
{
	if (element - '0' == 'W' - '0')
	{
		if (!feed_spawn(read, 'W'))
			return (0);
	}
	else if (element - '0' == 'E' - '0')
	{
		if (!feed_spawn(read, 'E'))
			return (0);
	}
	return (1);
}

int			check_spawn_element(t_config *read, char *line)
{
	int i;

	i = 0;
	while (line[i] != 0)
	{
		if (!check_n_s_spawn(read, line[i]))
			return (0);
		else if (!check_w_e_spawn(read, line[i]))
			return (0);
		++i;
	}
	return (1);
}
