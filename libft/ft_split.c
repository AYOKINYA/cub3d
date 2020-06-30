/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 20:12:37 by jkang             #+#    #+#             */
/*   Updated: 2020/04/10 16:27:35 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_row_count(const char *str, char sep)
{
	int row_count;
	int i;

	row_count = 0;
	i = 0;
	if (*str == '\0')
		return (0);
	while (*str != '\0')
	{
		if (*str == sep)
			i = 0;
		else if (i == 0)
		{
			i = 1;
			++row_count;
		}
		++str;
	}
	return (row_count);
}

static void		ft_strcpy(char *dest, const char *from, const char *to)
{
	while (from < to)
		*(dest++) = *(from++);
	*dest = 0;
}

static void		ft_free(char **map, int index)
{
	int i;

	i = index;
	while (i > 0)
	{
		--i;
		free(map[i]);
	}
	free(map);
}

static char		**ft_split_body(char const *s, char c, char **map, int rowsize)
{
	const char	*from;
	int			index;

	index = 0;
	while (*s != '\0' && index < rowsize)
	{
		if (*s != c)
		{
			from = s;
			while (*s != '\0' && *s != c)
				++s;
			map[index] = (char *)malloc(sizeof(char) * (s - from + 1));
			if (map[index] == 0)
			{
				ft_free(map, index);
				return (0);
			}
			ft_strcpy(map[index], from, s);
			++index;
		}
		++s;
	}
	map[index] = 0;
	return (map);
}

char			**ft_split(char const *s, char c)
{
	char	**map;
	int		row_size;

	if (s == 0)
		return (0);
	row_size = ft_row_count(s, c);
	map = (char **)malloc(sizeof(char *) * (row_size + 1));
	if (map == 0)
		return (0);
	return (ft_split_body(s, c, map, row_size));
}
