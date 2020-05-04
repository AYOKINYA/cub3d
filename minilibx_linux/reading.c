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

typedef struct	s_map
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
	int		**map;

}				t_map;

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
			while (*s != '\0' && (*s != c))
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

int      ft_strlen(const char *s)
{
	int count;

	count = 0;
	while (s[count] != '\0')
		++count;
	return (count);
}

char     *ft_strjoin(char *s1, char const *s2)
{
	char	*joined_str;
	int		i;
	int		j;

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

int main(void)
{
    char    *line = 0;
	int		line_count = 0;
	size_t  ret;
	int		i;
	t_map	*reading;
    int     fd;
	char	**info;
	
	reading = (t_map*)malloc(sizeof(t_map));
    fd = open("pre.cub", O_RDONLY);
    while ((ret = get_next_line(fd, &line)) > 0)
	{
		printf("a line is read in main : %s\n", line);
		if (ft_strchr(line, 'R'))
		{
			printf("yes it has R\n");
			reading->resolution_flag = 1;
			if (!(info = ft_split(ft_strchr(line, 'R') + 1, ' ')))
				return (-1);
			i = 0;
			while (info[i] != 0)
				++i;
			if (i != 2)
				printf("error!"); 
			printf("first %s\n", info[0]);
			reading->resolution_width = ft_atoi(info[0]);
			if (reading->resolution_width <= 0)
				printf("error!");
			printf("second %s\n", info[1]);
			reading->resolution_height = ft_atoi(info[1]);
			if (reading->resolution_height <= 0)
				printf("error!");
			i = 0;
			while (info[i] != 0)
			{
				free (info[i]);
				++i;
			}
			free(info);
		}
		free(line);
		++line_count;
	}
	printf("line count %d", line_count);
	free(line);
	free(reading);
}