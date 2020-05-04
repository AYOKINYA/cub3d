/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 20:12:37 by jkang             #+#    #+#             */
/*   Updated: 2020/03/11 23:17:07 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <stdlib.h>

static int		numstring(char const *s1, char c)
{
	int	comp;
	int	cles;

	comp = 0;
	cles = 0;
	if (*s1 == '\0')
		return (0);
	while (*s1 != '\0')
	{
		if (*s1 == c)
			cles = 0;
		else if (cles == 0)
		{
			cles = 1;
			comp++;
		}
		s1++;
	}
	return (comp);
}

static int		numchar(char const *s2, char c, int i)
{
	int	lenght;

	lenght = 0;
	while (s2[i] != c && s2[i] != '\0')
	{
		lenght++;
		i++;
	}
	return (lenght);
}

static char		**freee(char const **dst, int j)
{
	while (j > 0)
	{
		j--;
		free((void *)dst[j]);
	}
	free(dst);
	return (NULL);
}

static char		**affect(char const *s, char **dst, char c, int l)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (s[i] != '\0' && j < l)
	{
		k = 0;
		while (s[i] == c)
			i++;
		dst[j] = (char *)malloc(sizeof(char) * numchar(s, c, i) + 1);
		if (dst[j] == NULL)
			return (freee((char const **)dst, j));
		while (s[i] != '\0' && s[i] != c && s[i] != '\n')
			dst[j][k++] = s[i++];
		dst[j][k] = '\0';
		j++;
	}
	dst[j] = 0;
	return (dst);
}

char			**ft_split(char const *s, char c)
{
	char	**dst;
	int		l;

	if (s == NULL)
		return (NULL);
	l = numstring(s, c);
	dst = (char **)malloc(sizeof(char *) * (l + 1));
	if (dst == NULL)
		return (NULL);
	return (affect(s, dst, c, l));
}
int main()
{
	char **temp;
	char *a = "F 200,   200, 0       \n";
	int i;

	temp = ft_split(a, ',');
	i = 0;
	while (temp[i] != 0)
	{
		printf("%s\n", temp[i]);
		++i;
	}
    printf("%d\n", i);
	i = 0;
	while (temp[i] != 0)
	{
		free(temp[i]);
		++i;
	}
	free(temp);
	return (0);
}
