#include <stdio.h>
#include <stdlib.h>

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

static char		**ft_split_body(char const *s, char c, char **map)
{
	const char	*from;
	int			index;

	index = 0;
	while (*s != '\0')
	{
		if (*s != c)
		{
			from = s;
			while (*s != '\0' && *s != c)
				++s;
			map[index] = (char *)malloc(sizeof(char) * (s - from + 2));
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
	return (ft_split_body(s, c, map));
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