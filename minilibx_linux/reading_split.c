/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/12 16:19:04 by jkang             #+#    #+#             */
/*   Updated: 2020/03/12 16:19:05 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#include "mlx.h"
#include "mlx_int.h"
#include <math.h>

#define window_width 640
#define window_height 480
#define texWidth 64
#define texHeight 64
#define mapWidth 24
#define mapHeight 24

typedef struct	s_data
{

	void		*img_ptr;
	int			*data;
	int			size_line;
	int			bpp;
	int			endian;

}				t_data;

typedef struct  s_vars {
	void        *mlx;
	void        *win;
	int			color;
	char		*string;
	double		cameraX;
	double		sideDistX;
	double		sideDistY;
	double		deltaDistX;
	double		deltaDistY;
	int			stepX;
	int			stepY;
	double		perpWallDist;
	int			side;
	int			lineHegiht;
	double		WallX;
	int			drawStart;
	int			drawEnd;
	double		posX;
	double		posY;
	double		raydirX;
	double		olddirX;
	double		dirX;
	double		raydirY;
	double		olddirY;
	double		dirY;
	double		rot_speed;
	double		moving_speed;
	double		oldplaneX;
	double		planeX;
	double		planeY;
	int			mapX;
	int			mapY;
	t_data		img;
	int			key;
	double		texPos;
	double		step;
	int			texX;
	int			texY;
	int			**texture;
}               t_vars;

typedef struct	s_config
{

	int		resolution_flag;
	int		resolution_width;
	int		resolution_height;
	int		north_texture;
	char	*north_path;
	int		south_texture;
	char	*south_path;
	int		west_texture;
	char	*west_path;
	int		east_texture;
	char	*east_path;
	int		sprite_texture;
	char	*sprite_path;
	int		floor_flag;
	int		floor_r;
	int		floor_g;
	int		floor_b;
	int		ceilling_flag;
	int		ceilling_r;
	int		ceilling_g;
	int		ceilling_b;
	int		map_flag;
	int		map_start_flag;
	int		map_end_flag;
	int		**map;
}				t_config;

typedef struct	s_list
{
	int *row;
	int	col_size;
	struct s_list *next;
}				t_list;

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*before;

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

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == 0)
		return (0);
	while(lst->next != 0)
	{
		lst = lst->next;
	}
	return (lst);
}

void    ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*temp;

	if (lst == 0 || new == 0)
		return ;
	if (*lst == 0)
		*lst = new;
	else
	{
		temp = ft_lstlast(*lst);
		temp->next = new;
	}
}

size_t ft_strlen(const char *s)
{
	size_t count;

	count = 0;
	while (s[count] != '\0')
		++count;
	return (count);
}

char    *ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t i;
	size_t j;

	i = 0;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (i <= len && haystack[i] != '\0')
	{
		if (haystack[i] == needle[0])
		{
			j = 0;
			while (needle[j] != '\0' && i + j < len)
			{
				if (needle[j] != haystack[i + j])
					break ;
				++j;
			}
			if (needle[j] == '\0')
				return ((char *)&haystack[i]);
		}
		++i;
	}
	return (0);
}

static int	ft_is_space(const char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

static int	ft_is_numeric(const char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int res;
	int i;
	int sign;

	res = 0;
	i = 0;
	sign = 1;
	while (str[i] != '\0' && ft_is_space(str[i]))
		++i;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = sign * -1;
		++i;
	}
	while(str[i] != '\0' && ft_is_numeric(str[i]))
	{
		res = res * 10 + (str[i] - 48);
		++i;
	}
	return(sign * res);
}

static int		ft_row_count(const char *str, char sep)
{
	int row_count;
	int i;

	row_count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != sep)
		{
			while (str[i] != '\0' && (str[i] != sep))
				++i;
			++row_count;
		}
		++i;
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
			while (*s != '\0' && (*s != c) && *s != '\n')
				++s;
			map[index] = (char *)malloc(sizeof(char) * (s - from + 1));
			if (map[index] == 0)
			{
				ft_free(map, index);
				return (0);
			}
			ft_strcpy(map[index++], from, s);
		}
		++s;
	}
	map[index] = 0;
	return (map);
}

char			**ft_split(char const *s, char c)
{
	char	**map;

	if (s == 0)
		return (0);
	map = (char **)malloc(sizeof(char *) * (ft_row_count(s, c) + 1));
	if (map == 0)
		return (0);
	return (ft_split_body(s, c, map));
}

char     *ft_strjoin(char *s1, char const *s2)
{
	char	*joined_str;
	size_t		i;
	size_t		j;

	if (s1 == 0 || s2 == 0)
		return (0);
	joined_str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (joined_str == 0)
		return (0);
	i = -1;
	while (++i < ft_strlen((s1)))
		joined_str[i] = s1[i];
	j = -1;
	while (++j < ft_strlen(s2))
		joined_str[i++] = s2[j];
	joined_str[i] = '\0';
	free(s1);
	return (joined_str);
}

char     *ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		++s;
	}
	if (c == '\0')
		return ((char *)s);
	return (0);
}

char     *ft_strdup(const char *s)
{
	int     len;
	int     i;
	char    *temp;

	len = 0;
	while (s[len] != '\0')
		++len;
	temp = (char *)malloc(sizeof(char) * (len + 1));
	if (temp == 0)
		return (0);
	i = 0;
	while (i < len)
	{
		temp[i] = s[i];
		++i;
	}
	temp[i] = '\0';
	return (temp);
}

void    *ft_memmove(void *dst, const void *src, size_t len)
{
    size_t          n;
    unsigned char   *dest;
	unsigned char   *source;

    dest = (unsigned char *)dst;
	source = (unsigned char *)src;
    n = len;
    if (src <= dst)
    {
        while (n--)
            (*(dest + n)= *(source + n));
    }
    else
    {
        while (len--)
            *(dest++) = *(source++);
    }
    return (dst);
}

int				ft_free_and_ret(char **s_res, int ret)
{
	if (*s_res)
	{
		free(*s_res);
		*s_res = 0;
	}
	return (ret);
}

int				ft_has_new_line(char *buf, char **s_res, char **line)
{
	int		i;
	int		BUFFER_SIZE = 10;
	char	tmp[BUFFER_SIZE + 1];
	
	if (ft_strchr(buf, '\n') != 0)
	{
		i = 0;
		while (buf[i] != '\n')
		{
			tmp[i] = buf[i];
			++i;
		}
		tmp[i] = '\0';
		if(!(*line = ft_strjoin(*line, tmp)))
			return (ft_free_and_ret(s_res, -1));
		free(*s_res);
		*s_res = 0;
		if(!(*s_res = ft_strdup(&buf[i + 1])))
			return (ft_free_and_ret(s_res, -1));
		return (1);
	}
	return (0);
}

int             ft_read_s_res(char **s_res, char **line)
{
	int		i;
	int		BUFFER_SIZE = 10;
	char	tmp[BUFFER_SIZE + 1];

	if ((ft_strchr(*s_res, '\n')) == 0)
	{
		if (!(*line = ft_strjoin(*line, *s_res)))
			return (ft_free_and_ret(s_res, -1));
		free(*s_res);
		*s_res = 0;
	}
	else  
	{
		i = 0;
		while ((*s_res)[i] != '\n' && (*s_res)[i] != '\0')
		{
			tmp[i] = (*s_res)[i];
			++i;
		}
		tmp[i] = '\0';
		if (!(*line = ft_strjoin(*line, tmp)))
			return (ft_free_and_ret(s_res, -1));
		ft_memmove(*s_res, *s_res + i + 1, ft_strlen(*s_res) - i);
		return (1);   
	}
	return (0);
}

int             get_next_line(int fd, char **line)
{
	int				BUFFER_SIZE = 10;
	char			buf[BUFFER_SIZE + 1];
	static char		*s_res;
	size_t			ret;

	if (fd < 0 || line == 0 || (read(fd, buf, 0) < 0) || BUFFER_SIZE < 1) 
		return (-1);    
	if(!(*line = ft_strdup("")))
		return (ft_free_and_ret(&s_res, -1));
	if (s_res)
	{
		if (ft_read_s_res(&s_res, line) == 1)
			return (1);
	}
	while ((ret = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[ret] = '\0';
		if (ft_has_new_line(buf, &s_res, line) == 1)
			return (1);
		if(!(*line = ft_strjoin(*line, buf)))
			return (ft_free_and_ret(&s_res, -1));
	}
	return (ret);
}

static int		ft_is_set(const char c, char const *set)
{
	while (*set != '\0')
	{
		if (c == *set)
			return (1);
		++set;
	}
	return (0);
}

char			*ft_strtrim(char const *s1, char const *set)
{
	char    *trimmed_str;
	size_t  s1_len;

	s1_len = ft_strlen(s1);
	trimmed_str = (char *)malloc(sizeof(char) * (s1_len + 1));
	if (trimmed_str == 0)
		return (0);
	ft_memmove(trimmed_str, s1, s1_len + 1);
	while(ft_is_set(trimmed_str[0], set))
		ft_memmove(trimmed_str, trimmed_str + 1, ft_strlen(trimmed_str));
	while(ft_is_set(trimmed_str[ft_strlen(trimmed_str) - 1], set))
		trimmed_str[ft_strlen(trimmed_str) - 1] = '\0';
	return (trimmed_str);
}


int ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*u_s1;
	unsigned char	*u_s2;

	i = 0;
	u_s1 = (unsigned char*)s1;
	u_s2 = (unsigned char*)s2;
	if (n == 0)
		return (0);
	while (u_s1[i] != '\0' && u_s2[i] != '\0' && i < n)
	{
		if (u_s1[i] != u_s2[i])
			return ((u_s1[i] - u_s2[i]));
		else
			++i;
	}
	if (i == n)
		return (0);
	return (u_s1[i] - u_s2[i]);
}

int	get_resolution(t_config *reading, char *line)
{
	char	**info;
	int		i;

	if (ft_strchr(line, 'R'))
	{
		reading->resolution_flag = 1;
		if (!(info = ft_split(ft_strchr(line, 'R'), ' ')))
			return (-1);
		i = 0;
		while (info[i] != 0)
			++i;
		if (i != 3 || ft_strlen(info[0]) != 1)
			return (0);
		reading->resolution_width = ft_atoi(info[1]);
		reading->resolution_height = ft_atoi(info[2]);
		if (reading->resolution_width <= 0 || reading->resolution_height <= 0)
			return (0);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
	}
	return (1);
}

int	validate_color(int r, int g, int b)
{
	if (r < 0 || r > 255)
		return (0);
	else if (g < 0 || g > 255)
		return (0);
	else if (b < 0 || b > 255)
		return (0);
	else
		return (1);
}

int	get_floor_color(t_config *reading, char *line)
{
	char	**info;
	char	**split_str;
	int		i;

	if (ft_strchr(line, 'F'))
	{
		reading->floor_flag = 1;
		if (!(info = ft_split(ft_strchr(line, 'F'), ',')))
			return (0);
		i = 0;
		while (info[i] != 0)
			++i;
		if (!(split_str = ft_split(ft_strchr(info[0], 'F'), ' ')))
			return (0);
		if (i != 4 || ft_strncmp(split_str[0], "F", 1) != 0)
		{
			i = -1;
			while (split_str[++i] != 0)
				free(split_str[i]);
			free(split_str);
			i = -1;
			while (info[++i] != 0)
				free(info[i]);
			free(info);
			return (0);
		}
		reading->floor_r = ft_atoi(split_str[1]);
		reading->floor_g = ft_atoi(info[1]);
		reading->floor_b = ft_atoi(info[2]);
		i = -1;
		while (split_str[++i] != 0)
			free(split_str[i]);
		free(split_str);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
		if (!validate_color(reading->floor_r, reading->floor_g, reading->floor_b))
			return (0);
	}
	return (1);
}

int	get_ceilling_color(t_config *reading, char *line)
{
	char	**info;
	char	**split_str;
	int		i;

	if (ft_strchr(line, 'C'))
	{
		reading->ceilling_flag = 1;
		if (!(info = ft_split(ft_strchr(line, 'C'), ',')))
			return (0);
		i = 0;
		while (info[i] != 0)
			++i;
		if (!(split_str = ft_split(ft_strchr(info[0], 'C'), ' ')))
			return (0);
		if (i != 4 || ft_strncmp(split_str[0], "C", 1) != 0)
		{
			i = -1;
			while (split_str[++i] != 0)
				free(split_str[i]);
			free(split_str);
			i = -1;
			while (info[++i] != 0)
				free(info[i]);
			free(info);
			return (0);
		}
		reading->ceilling_r = ft_atoi(split_str[1]);
		reading->ceilling_g = ft_atoi(info[1]);
		reading->ceilling_b = ft_atoi(info[2]);
		i = -1;
		while (split_str[++i] != 0)
			free(split_str[i]);
		free(split_str);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
		if (!validate_color(reading->ceilling_r, reading->ceilling_g, reading->ceilling_b))
			return (0);
	}
	return (1);
}

int		get_north_texture_path(t_config *reading, char *line)
{
	char	**info;
	int		i;

	if (ft_strnstr(line, "NO", ft_strlen(line)))
	{
		reading->north_texture = 1;
		if (!(info = ft_split(ft_strchr(line, 'N'), ' ')))
			return (0);
		i = 0;
		while (info[i] != 0)
			++i;
		if (i != 2 || ft_strlen(info[0]) != 2)
			return (0);
		reading->north_path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1));
		ft_memmove(reading->north_path, info[1], ft_strlen(info[1]) + 1);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
	}
	return (1);
}

int		get_south_texture_path(t_config *reading, char *line)
{
	char	**info;
	int		i;

	if (ft_strnstr(line, "SO", ft_strlen(line)))
	{
		reading->south_texture = 1;
		if (!(info = ft_split(ft_strchr(line, 'S'), ' ')))
			return (0);
		i = 0;
		while (info[i] != 0)
			++i;
		if (i != 2 || ft_strlen(info[0]) != 2)
			return (0);
		reading->south_path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1));
		ft_memmove(reading->south_path, info[1], ft_strlen(info[1]) + 1);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
	}
	return (1);
}

int		get_west_texture_path(t_config *reading, char *line)
{
	char	**info;
	int		i;

	if (ft_strnstr(line, "WE", ft_strlen(line)))
	{
		reading->west_texture = 1;
		if (!(info = ft_split(ft_strchr(line, 'W'), ' ')))
			return (0);
		i = 0;
		while (info[i] != 0)
			++i;
		if (i != 2 || ft_strlen(info[0]) != 2)
			return (0);
		reading->west_path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1));
		ft_memmove(reading->west_path, info[1], ft_strlen(info[1]) + 1);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
	}
	return (1);
}

int		get_east_texture_path(t_config *reading, char *line)
{
	char	**info;
	int		i;

	if (ft_strnstr(line, "EA", ft_strlen(line)))
	{
		reading->east_texture = 1;
		if (!(info = ft_split(ft_strchr(line, 'E'), ' ')))
			return (0);
		i = 0;
		while (info[i] != 0)
			++i;
		if (i != 2 || ft_strlen(info[0]) != 2)
			return (0);
		reading->east_path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1));
		ft_memmove(reading->east_path, info[1], ft_strlen(info[1]) + 1);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
	}
	return (1);
}

int		get_sprite_texture_path(t_config *reading, char *line)
{
	char	**info;
	int		i;

	if (ft_strnstr(line, "S ", ft_strlen(line)))
	{
		reading->sprite_texture = 1;
		if (!(info = ft_split(ft_strchr(line, 'S'), ' ')))
			return (0);
		i = 0;
		while (info[i] != 0)
			++i;
		if (i != 2 || ft_strlen(info[0]) != 1)
			return (0);
		reading->sprite_path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1));
		ft_memmove(reading->sprite_path, info[1], ft_strlen(info[1]) + 1);
		i = -1;
		while (info[++i] != 0)
			free(info[i]);
		free(info);
	}
	return (1);
}

int		get_texture(t_config *reading, char *line)
{
	if (!get_north_texture_path(reading, line))
		return (0);
	if (!get_south_texture_path(reading, line))
		return (0);
	if (!get_west_texture_path(reading, line))
		return (0);
	if (!get_east_texture_path(reading, line))
		return (0);
	if (!get_sprite_texture_path(reading, line))
		return (0);
	return (1);
}


int	map_is_the_last_element(t_config *reading)
{
	int check;

	check = reading->resolution_flag * reading->north_texture * reading->south_texture * reading->west_texture * reading->east_texture;
	if (check == 0)
		return (0);
	check = check * reading->sprite_texture * reading->floor_flag * reading->ceilling_flag;
	return (check);
}

int	map_start(t_config *reading, char *line, t_list *map)
{
	int index;
	int j;

	index = -1;
	while (line[++index] != 0)
	{
		if (line[index] != ' ' && line[index] != '1')
			return (0);
	}
	if (!map_is_the_last_element(reading))
		return (0);
	reading->map_start_flag = 1;
	map->row = (int *)malloc(sizeof(int) * index);
	j = 0;
	while (j < index)
	{
		if (ft_is_space(line[j]))
			map->row[j] = 1;
		else
			map->row[j] = line[j] - '0';
		++j;
	}
	map->col_size = index;
	return (1);
}

int	ft_is_element(char element)
{
	int ret;

	ret = 0;
	if (element == ' ')
		ret = 1;
	if (element >= '0' && element <= '2')
		ret = 2;
	if (element == 'N' || element == 'S' || element == 'W' || element == 'E')
		ret = 3;
	return (ret);
}

int check_map_element(char *line)
{
	int		i;
	char	*trimmed_line;

	i = 0;
	while (line[i] != 0)
	{
		if (!ft_is_element(line[i]))
			return (0);
		trimmed_line = ft_strtrim(line, " \t");
		if (trimmed_line[0] != '1')
			return (0);
		if (trimmed_line[ft_strlen(trimmed_line) - 1] != '1')
			return(0);
		free(trimmed_line);
		++i;
	}
	return (1);
}

int	check_last_row(t_config *reading, char *line)
{
	int index;

	index = 0;
	while (line[index] != 0)
	{
		if (line[index] != ' ' && line[index] != '1')
			return (0);
		++index;
	}
	reading->map_end_flag = 1;
	return (1);
}

int	get_map(t_config *reading, char *line, t_list *map)
{	
	t_list *new;
	int col_size;
	int j;

	if (reading->map_end_flag == 1)
		return (0);
	if (!check_map_element(line))
		return (0);
	col_size = ft_strlen(line);
	if(!(new = (t_list *)malloc(sizeof(t_list))))
		return (0);
	new->row = (int *)malloc(sizeof(int) * col_size);
	j = 0;
	while (j < col_size)
	{
		if (line[j] == ' ')
			new->row[j] = 1;
		else
			new->row[j] = line[j] - '0';
		++j;
	}
	new->col_size = col_size;
	new->next = 0;
	ft_lstadd_back(&map, new);
	check_last_row(reading, line);
	// int j = 0;
	// while (j < map->col_size)
	// {
	// 	printf("%d", map->row[j]);
	// 	++j;
	// }
	// printf("main \n");
	return (1);
}

int final_check_elements(t_config *reading)
{
	int check;
	
	check = reading->resolution_flag * reading->north_texture * reading->south_texture * reading->west_texture * reading->east_texture;
	if (check != 1)
		return (0);
	check = check * reading->sprite_texture * reading->floor_flag * reading->ceilling_flag;
	if (check != 1)
		return (0);
	check = check * reading->map_start_flag * reading->map_end_flag;
	if (check != 1)
		return (0);
	return (check);
}

int	ft_lstsize(t_list *lst)
{
	int		count;
	t_list	*temp;
	
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

int main(void)
{
    char    *line = 0;
	size_t  ret;
	t_config	*reading;
    int     fd;
	t_list *map;
	
	if(!(map = (t_list *)malloc(sizeof(t_list))))
		return (0);
	map->row = 0;
	map->col_size = 0;
	map->next = 0;
	
	reading = (t_config *)malloc(sizeof(t_config));
    fd = open("pre.cub", O_RDONLY);
    while ((ret = get_next_line(fd, &line)) > 0)
	{
		printf("%s\n", line);
		get_resolution(reading, line);
		get_floor_color(reading, line);
		get_ceilling_color(reading, line);
		get_texture(reading, line);
		if (map_start(reading, line, map))
		{
			free(line);
			break;
		}
		free(line);
	}
	
	printf("map_start %d\n", reading->map_start_flag);
	if (reading->map_start_flag == 1)
	{
		while ((ret = get_next_line(fd, &line)) > 0)
		{
			printf("%s\n", line);
			get_map(reading, line, map);
			free(line);
		}
	}
	else
		printf("error");

	if (!final_check_elements(reading))
		printf("error");
	free(line);
	close(fd);
	printf("new\n");
	while (map != 0)
	{
		int j = 0;
		while (j < map->col_size)
		{
			printf("%d", map->row[j]);
			++j;
		}
		printf("mine \n");
		map = map->next;
	}
	free(reading->north_path);
	free(reading->south_path);
	free(reading->east_path);
	free(reading->west_path);
	free(reading->sprite_path);
	free(reading);
	ft_lstclear(&map, free);
	return (0);
}