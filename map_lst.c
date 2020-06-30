/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:23 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 13:43:23 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	map_lstadd_back(t_map **lst, t_map *new)
{
	t_map	*temp;

	if (lst == 0 || new == 0)
		return ;
	if (*lst == 0)
		*lst = new;
	else
	{
		temp = map_lstlast(*lst);
		temp->next = new;
	}
}

void	map_lstclear(t_map **lst, void (*del)(void *))
{
	t_map	*before;

	if (lst == 0 || *lst == 0 || del == 0)
		return ;
	while (*lst != 0)
	{
		del((*lst)->row);
		before = *lst;
		*lst = before->next;
		free(before);
	}
	*lst = 0;
}

t_map	*map_lstlast(t_map *lst)
{
	if (lst == 0)
		return (0);
	while (lst->next != 0)
	{
		lst = lst->next;
	}
	return (lst);
}

int		map_lstsize(t_map *lst)
{
	int		count;
	t_map	*temp;

	if (lst == 0)
		return (0);
	temp = lst;
	count = 0;
	while (temp != 0)
	{
		temp = temp->next;
		++count;
	}
	return (count);
}
