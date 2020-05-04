/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   home_final.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/12 16:19:04 by jkang             #+#    #+#             */
/*   Updated: 2020/04/13 14:06:13 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

#include "mlx.h"
#include "mlx_int.h"
#include <math.h>

typedef struct	s_data
{

	void		*img_ptr;
	int			*data;
	int			size_line;
	int			bpp;
	int			endian;

}				t_data;

typedef struct	s_texture
{

	void		*img_ptr;
	int			*data;
	int			size_line;
	int			bpp;
	int			endian;
	int     	img_width;
    int     	img_height;

}				t_texture;

typedef struct	s_list
{
	int *row;
	int	col_size;
	struct s_list *next;
}				t_list;

typedef struct s_sprite
{
  double x;
  double y;

}		t_sprite;

typedef struct s_sprite_calc
{
  double spriteX;
  double spriteY;
  double invDet;
  double transformX;
  double transformY;
  int spritescreenX;
  int sprite_height;
  int draw_y_start;
  int draw_y_end;
  int sprite_width;
  int draw_x_start;
  int draw_x_end;
  int stripe_tex_x;
  int stripe_tex_y;
}		t_sprite_calc;

typedef struct  s_vars {
	int			window_width;
	int			window_height;
	int			floor_color;
	int			ceilling_color;
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
	int			lineHeight;
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
	int			**map;
	t_data		img;
	t_texture	*tex;
	int			key;
	double		texPos;
	double		step;
	int			texX;
	int			texY;
	t_sprite	*sprites;
	int 		num_sprite;
	int			*zbuffer;
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
	t_list *map;
	int		map_row_size;
	int		map_start_flag;
	int		map_end_flag;
	int		surrounding_flag;
	int		spawn_flag;
	int		spawn_dir;
}				t_config;

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
	while (lst->next != 0)
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
	while (i < len && haystack[i] != '\0')
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
	if (c == ' ' || c == '\f' || c == '\n' ||\
		c == '\r' || c == '\t' || c == '\v')
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

char     *ft_strjoin(char *s1, char const *s2)
{
	char	*joined_str;
	size_t		i;
	size_t		j;

	if (s1 == 0 || s2 == 0)
		return (0);
	joined_str = (char *)malloc(sizeof(char) *\
					(ft_strlen(s1) + ft_strlen(s2) + 1));
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


int ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*u_s1;
	unsigned char	*u_s2;

	i = 0;
	u_s1 = (unsigned char*)s1;
	u_s2 = (unsigned char*)s2;
	while (u_s1[i] != '\0' && u_s2[i] != '\0')
	{
		if (u_s1[i] != u_s2[i])
			return ((u_s1[i] - u_s2[i]));
		else
			++i;
	}
	return (u_s1[i] - u_s2[i]);
}

int	free_int_array(int **arr)
{
	int i;

	if (arr == 0)
		return (0);
	i = 0;
	while (arr[i] != 0)
	{
		free(arr[i]);
		++i;
	}
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
	{
		free(arr[i]);
		++i;
	}
	free(arr);
}

int free_all_texture(t_config *read)
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

int get_resolution_body(t_config *read, char *line)
{
	char	**info;
	int		i;

	if (!(info = ft_split(ft_strchr(line, 'R'), ' ')))
		return (0);
	i = 0;
	while (info[i] != 0)
	{
		printf("%s , %d\n ", info[i], i);
		++i;
	}
	if (i != 3 || ft_strlen(info[0]) != 1)
	{
		free_char_array(info);
		return (0);
	}
	read->resolution_width = ft_atoi(info[1]);
	read->resolution_height = ft_atoi(info[2]);
	if (read->resolution_width <= 0 || read->resolution_height <= 0)
	{
		free_char_array(info);
		return (0);
	}
	free_char_array(info);
	return (1);
}

int	get_resolution(t_config *read, char *line)
{
	if (ft_strchr(line, 'R'))
	{
		if (read->resolution_flag == 1)
			return (0);
		if (!get_resolution_body(read, line))
			return (0);
		read->resolution_flag = 1;
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

int	get_floor_color_body(t_config *read, char *line)
{
	char	**info;
	char	**split_str;
	int		i;

	if (!(info = ft_split(ft_strchr(line, 'F'), ',')))
			return (0);
	i = 0;
	while (info[i] != 0)
	{
		printf("%s , %d\n ", info[i], i);
		++i;
	}
	if (!(split_str = ft_split(ft_strchr(info[0], 'F'), ' ')))
		return (0);
	if (i != 3 || ft_strcmp(split_str[0], "F") != 0)
	{
		printf("fail %d\n", i);
		free_char_array(split_str);
		free_char_array(info);
		return (0);
	}
	read->floor_r = ft_atoi(split_str[1]);
	read->floor_g = ft_atoi(info[1]);
	read->floor_b = ft_atoi(info[2]);
	free_char_array(split_str);
	free_char_array(info);
	return (1);
}

int	get_floor_color(t_config *read, char *line)
{
	if (ft_strchr(line, 'F'))
	{
		if (read->floor_flag == 1)
			return (0);
		if (!get_floor_color_body(read, line))
			return (0);
		if (!validate_color(read->floor_r, read->floor_g, read->floor_b))
			return (0);
		read->floor_flag = 1;
	}
	return (1);
}

int	get_ceilling_color_body(t_config *read, char *line)
{
	char	**info;
	char	**split_str;
	int		i;

	if (!(info = ft_split(ft_strchr(line, 'C'), ',')))
			return (0);
	i = 0;
	while (info[i] != 0)
	{
		printf("%s , %d\n ", info[i], i);
		++i;
	}
	if (!(split_str = ft_split(ft_strchr(info[0], 'C'), ' ')))
		return (0);
	if (i != 3 || ft_strcmp(split_str[0], "C") != 0)
	{
		free_char_array(split_str);
		free_char_array(info);
		return (0);
	}
	read->ceilling_r = ft_atoi(split_str[1]);
	read->ceilling_g = ft_atoi(info[1]);
	read->ceilling_b = ft_atoi(info[2]);
	free_char_array(split_str);
	free_char_array(info);
	return (1);
}

int	get_ceilling_color(t_config *read, char *line)
{
	if (ft_strchr(line, 'C'))
	{
		if (read->ceilling_flag == 1)
			return (0);
		if (!get_ceilling_color_body(read, line))
			return (0);
		if (!validate_color(read->ceilling_r, \
				read->ceilling_g, read->ceilling_b))
			return (0);
		read->ceilling_flag = 1;
	}
	return (1);
}


int		get_texture_body(char **path, char element, char *line)
{
	char	**info;
	int		i;

	if (!(info = ft_split(ft_strchr(line, element), ' ')))
		return (0);
	i = 0;
	while (info[i] != 0)
		++i;
	if (i != 2 || ft_strlen(info[0]) != 2)
	{
		free_char_array(info);
		return (0);
	}
	if (!(*path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1))))
	{
		free_char_array(info);
		return (0);
	}
	ft_memmove(*path, info[1], ft_strlen(info[1]) + 1);
	free_char_array(info);
	return (1);
}

int		get_north_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "NO", ft_strlen(line)))
	{
		if (read->north_texture == 1)
			return (0);
		if (!get_texture_body(&read->north_path, 'N', line))
			return (0);
		read->north_texture = 1;
	}
	return (1);
}

int		get_south_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "SO", ft_strlen(line)))
	{
		if (read->south_texture == 1)
			return (0);
		if (!get_texture_body(&read->south_path, 'S', line))
			return (0);
		read->south_texture = 1;
	}
	return (1);
}

int		get_west_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "WE", ft_strlen(line)))
	{
		if (read->west_texture == 1)
			return (0);
		if (!get_texture_body(&read->west_path, 'W', line))
			return (0);
		read->west_texture = 1;
	}
	return (1);
}

int		get_east_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "EA", ft_strlen(line)))
	{
		if (read->east_texture == 1)
			return (0);
		if (!get_texture_body(&read->east_path, 'E', line))
			return (0);
		read->east_texture = 1;
	}
	return (1);
}

int		get_sprite_texture_body(char **path, char element, char *line)
{
	char	**info;
	int		i;

	if (!(info = ft_split(ft_strchr(line, element), ' ')))
		return (0);
	i = 0;
	while (info[i] != 0)
		++i;
	if (i != 2 || ft_strlen(info[0]) != 1)
	{
		free_char_array(info);
		return (0);
	}
	if (!(*path = (char *)malloc(sizeof(char) * (ft_strlen(info[1]) + 1))))
	{
		free_char_array(info);
		return (0);
	}
	ft_memmove(*path, info[1], ft_strlen(info[1]) + 1);
	free_char_array(info);
	return (1);
}

int		get_sprite_texture_path(t_config *read, char *line)
{
	if (ft_strnstr(line, "S ", ft_strlen(line)))
	{
		if (read->sprite_texture == 1)
			return (0);
		if (!get_sprite_texture_body(&read->sprite_path, 'S', line))
			return (0);
		read->sprite_texture = 1;
	}
	return (1);
}

int		get_texture(t_config *read, char *line)
{
	if (!get_north_texture_path(read, line))
		return (0);
	if (!get_south_texture_path(read, line))
		return (0);
	if (!get_west_texture_path(read, line))
		return (0);
	if (!get_east_texture_path(read, line))
		return (0);
	if (!get_sprite_texture_path(read, line))
		return (0);
	return (1);
}

int		get_color(t_config *read, char *line)
{
	if (!get_floor_color(read, line))
		return (0);
	if (!get_ceilling_color(read, line))
		return (0);
	return (1);
}

int	map_is_the_last_element(t_config *read)
{
	int check;

	check = read->resolution_flag * read->floor_flag * read->ceilling_flag;
	check = check * read->sprite_texture * read->north_texture *\
			read->south_texture * read->west_texture * read->east_texture;
	if (check == 1)
		return (check);
	else
		return (0);
}

int	map_start(t_config *read, char *line, t_list *map)
{
	int index;
	int j;

	index = -1;
	while (line[++index] != 0)
	{
		if (line[index] != ' ' && line[index] != '1')
			return (0);
	}
	if (!map_is_the_last_element(read))
		return (-1);
	if (!(map->row = (int *)malloc(sizeof(int) * index)))
		return (-1);
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
	read->map_start_flag = 1;
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
		if (!(trimmed_line = ft_strtrim(line, " ")))
			return (0);
		if (trimmed_line[0] != '1')
		{
			free(trimmed_line);
			return (0);
		}
		if (trimmed_line[ft_strlen(trimmed_line) - 1] != '1')
		{
			free(trimmed_line);
			return (0);
		}
		free(trimmed_line);
		++i;
	}
	return (1);
}

int	check_last_row(t_config *read, char *line)
{
	int index;

	index = 0;
	while (line[index] != 0)
	{
		if (line[index] != ' ' && line[index] != '1')
			return (0);
		++index;
	}
	read->map_end_flag = 1;
	return (1);
}

int		create_new_row(char *line, t_list *map)
{
	t_list 	*new;
	int		col_size;
	int		j;

	if(!(new = (t_list *)malloc(sizeof(t_list))))
		return (0);
	col_size = ft_strlen(line);
	if (!(new->row = (int *)malloc(sizeof(int) * col_size)))
		return (0);
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
	return (1);
}

int feed_spawn(t_config *read, char spawn)
{
	if (read->spawn_flag == 1)
		return (0);
	read->spawn_flag = 1;
	read->spawn_dir = spawn - '0';
	return (1);
}

int		check_n_s_spawn(t_config *read, char element)
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

int		check_w_e_spawn(t_config *read, char element)
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

int		check_spawn_element(t_config *read, char *line)
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

int		get_map(t_config *read, char *line, t_list *map)
{	
	if (read->map_end_flag == 1)
		return (0);
	if (!check_map_element(line))
		return (0);
	if (!create_new_row(line, map))
		return (0);
	if (!check_spawn_element(read, line))
		return (0);
	check_last_row(read, line);
	return (1);
}

int final_check_elements(t_config *read)
{
	int check;
	
	check = read->resolution_flag * read->north_texture *\
				read->south_texture * read->west_texture * read->east_texture;
	if (check != 1)
		return (0);
	check = check * read->sprite_texture * read->floor_flag * read->ceilling_flag;
	if (check != 1)
		return (0);
	check = check * read->map_start_flag * read->map_end_flag *\
						read->surrounding_flag * read->spawn_flag;
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

int		check_up_body(int **test, int row, int col)
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

int		check_up_wall(int **test, int row_max, int col_max)
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

int		check_down_body(int **test, int row, int col, int row_max)
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


int		check_down_wall(int **test, int row_max, int col_max)
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

int		malloc_test_map(int **test, int row_max, int col_max)
{
	int i;
	int j;

	i = 0;
	while (i < row_max)
	{
		if (!(test[i] = (int *)malloc(sizeof(int) * col_max)))
		{
			j = 0;
			while (j < i)
			{
				free(test[j]);
				++j;
			}
			return (0);
		}
		++i;
	}
	return (1);
}

void		init_test_map(int **test, int row_max, int col_max)
{
	int i;
	int j;

	i = 0;
	while (i < row_max)
	{
		j = 0;
		while (j < col_max)
		{
			test[i][j] = 'X' - '0'; 
			++j;
		}
		++i;
	}
}

int		**creat_map_to_validate(t_config *read, t_list *map, int col_max)
{
	int row_max;
	int **test;
	int i;
	int j;

	row_max = read->map_row_size;
	if (!(test = (int **)malloc(sizeof(int *) * row_max)))
		return (0);
	if (!malloc_test_map(test, row_max, col_max))
		return (0);
	init_test_map(test, row_max, col_max);
	printf("test\n");
	i = -1;
	while (++i < read->map_row_size)
	{
		j = -1;
		while (++j < map->col_size)
			test[i][j] = map->row[j];
		map = map->next;
	}
	i = 0;
	while (i < read->map_row_size)
	{
		j = 0;
		while (j < col_max)
		{
			printf("%d", test[i][j]);
			++j;
		}
		printf("\n");
		++i;
	}
	return (test);
}

int check_surrounding(t_config *read, t_list *map)
{
	t_list	*temp;
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
	if(!(test = creat_map_to_validate(read, map, col_max)))
		return (free_int_array(test));
	if (!check_up_wall(test, row_max, col_max))
		return (free_int_array(test));
	if (!check_down_wall(test, row_max, col_max))
		return (free_int_array(test));
	read->surrounding_flag = 1;
	free_int_array(test);
	printf("done\n");
	return (1);
}

int free_all(t_vars *vars, t_config *read)
{
	free_all_texture(read);
	ft_lstclear(&read->map, free);
	free(read);
	free_int_array(vars->map);
	free(vars);
	return (0);
}

int read_before_map(t_config *read, char *line)
{
	if (!get_resolution(read, line))
		return (0);
	if (!get_color(read, line))
		return (0);
	if (!get_texture(read, line))
		return (0);
	return (1);
}

int read_till_map_starts(int *fd, t_list *map, t_config *read)
{
	size_t  	ret;
	char    	*line;
	
	line = 0;
	while ((ret = get_next_line(*fd, &line)) > 0)
	{
		printf("%s\n", line);
		if (!read_before_map(read, line))
		{
			free(line);
			return (0);
		}
		if (map_start(read, line, map) == 1)
		{
			free(line);
			break ;
		}
		else if (map_start(read, line, map) == -1)
		{
			free(line);
			return (0);
		}
		free(line);
	}
	return (1);
}

int read_till_map_ends(int *fd, t_list *map, t_config *read)
{
	size_t  	ret;
	char    	*line;
	
	line = 0;
	if (read->map_start_flag == 1)
	{
		while ((ret = get_next_line(*fd, &line)) > 0)
		{
			printf("%s\n", line);
			if (!get_map(read, line, map))
			{
				free(line);
				return (0);
			}
			free(line);
		}
	}
	else
	{
		write(1, "Error1\n", 6);
		free(line);
		return (0);
	}
	free(line);
	return (1);
}

int read_map(t_list *map, t_config *read, char *argv)
{
	int			fd;
	
	fd = open(argv, O_RDONLY);
	if (fd == -1)
	{
		close(fd);
		return (0);
	}
	if (!read_till_map_starts(&fd, map, read))
		return (0);
	if (!read_till_map_ends(&fd, map, read))
		return (0);
	close(fd);
	return (1);
}

int read_config_body(t_list *map, t_config *read, char *argv)
{
	if (!read_map(map, read, argv))
		return (0);
	read->map_row_size = ft_lstsize(map);
	if (!check_surrounding(read, map))
		return (0);
	if (!final_check_elements(read))
		return (0);
	return (1);
}

int is_cub_file(char *argv)
{
	size_t len;

	len = ft_strlen(argv);
	if (len < 5)
		return (0);
	else if (argv[len - 1] != 'b' || argv[len - 2] != 'u' ||\
				argv[len - 3] != 'c' || argv[len - 4] != '.')
		return (0);
	return (1);
}

int	read_config(t_vars *vars, t_config *read, char *argv)
{
	if (!is_cub_file(argv))
		return (0);
	if (!(read->map = (t_list *)malloc(sizeof(t_list))))
	{
		free(vars);
		return (0);
	}
	read->map->row = 0;
	read->map->col_size = 0;
	read->map->next = 0;
	if (!read_config_body(read->map, read, argv))
	{
		ft_lstclear(&read->map, free);
		return (0);
	}
	return (1);
}

void 	feed_resolution(t_vars *vars, t_config *read)
{
	vars->window_width = 800;
	vars->window_height = 640;

	if (vars->window_width > read->resolution_width)
		vars->window_width = read->resolution_width;
	if (vars->window_height > read->resolution_height)
		vars->window_height = read->resolution_height;
}

int      create_trgb(int t, int r, int g, int b)
{
   return (t << 24 |  r << 16 | g << 8 | b);
}

void	feed_color(t_vars *vars, t_config *read)
{
	int r;
	int g;
	int b;

	r = read->floor_r;
	g = read->floor_g;
	b = read->floor_b;
	vars->floor_color = create_trgb(0, r, g, b);
	r = read->ceilling_r;
	g = read->ceilling_g;
	b = read->ceilling_b;
	vars->ceilling_color = create_trgb(0, r, g, b);
}

int		is_spawn_spot(int pos)
{
	if (pos == 'N' - '0')
		return ('N' - '0');
	else if (pos == 'S' - '0')
		return ('S' - '0');
	else if (pos == 'W' - '0')
		return ('W' - '0');
	else if (pos == 'E' - '0')
		return ('E' - '0');
	else
		return (0);
}

void	set_spawn_dir(int x, int y, t_vars *vars, char spawn_dir)
{
	double radian;
	
	printf("x %d y %d", x, y);
	vars->posX = x;
	vars->posY = y + 0.5;
	vars->dirX = -1;
	vars->dirY = 0;
	vars->planeX = 0;
	vars->planeY = 0.66;
	if (spawn_dir == 'N' - '0')
		radian = 0 * 3.141592 / 180;
	else if (spawn_dir == 'S' - '0')
		radian = 180 * 3.141592 / 180;
	else if (spawn_dir == 'W' - '0')
		radian = 90 * 3.141592 / 180;
	else if (spawn_dir == 'E' - '0')
		radian = 270 * 3.141592 / 180;
	vars->olddirX = vars->dirX;
	vars->dirX = vars->dirX * cos(radian) - vars->dirY * sin(radian);
	vars->dirY = vars->olddirX * sin(radian) + vars->dirY * cos(radian);
	vars->oldplaneX = vars->oldplaneX;
	vars->planeX = vars->planeX * cos(radian) - vars->planeY * sin(radian);
	vars->planeY = vars->oldplaneX * sin(radian) + vars->planeY * cos(radian);
}

int	get_num_sprite(t_config *read)
{
	int i;
	int j;
	int	ret;
	t_list *temp;
	
	ret = 0;
	temp = read->map;
	i = 0;
	while (i < read->map_row_size)
	{
		j = 0;
		while (j < temp->col_size)
		{
			if (temp->row[j] == 2)
				++ret;
			++j;
		}
		++i;
		temp = temp->next;
	}
	return (ret);
}

int	feed_sprite_body(t_vars *vars, t_config *read, t_list *temp)
{
	int i;
	int j;
	int idx;
	
	idx = 0;
	i = 0;
	while (i < read->map_row_size)
	{
		j = 0;
		while (j < temp->col_size && idx != vars->num_sprite)
		{
			if (temp->row[j] == 2)
			{
				vars->sprites[idx].x = i;
				vars->sprites[idx].y = j + 0.5;
				++idx;
			}
			++j;
		}
		++i;
		temp = temp->next;
	}
	return (1);
}

int		feed_sprite(t_vars *vars, t_config *read)
{
	t_list *temp;

	vars->num_sprite = get_num_sprite(read);
	if (vars->num_sprite == 0)
		return (1);
	vars->sprites = (t_sprite *)malloc(sizeof(t_sprite) * vars->num_sprite);
	if (vars->sprites == 0)
		return (0);
	temp = read->map;
	if (!feed_sprite_body(vars, read, temp))
		return (0);
	if (!(vars->zbuffer = (int *)malloc(sizeof(int) * vars->window_width)))
	{
		free(vars->sprites);
		return (0);
	}
	return (1);
}

void		feed_position(t_vars *vars, t_config *read)
{
	int i;
	int j;
	t_list *temp;

	temp = read->map;
	printf("read config\n");
	i = 0;
	while (i < read->map_row_size)
	{
		j = 0;
		while (j < temp->col_size)
		{
			printf("%d",temp->row[j]);
			if (is_spawn_spot(temp->row[j]))
				break ;
			++j;
		}
		printf("\n");
		if (is_spawn_spot(temp->row[j]))
			break ;
		++i;
		temp = temp->next;
	}
	set_spawn_dir(i, j, vars, temp->row[j]);
}

int		feed_map(t_vars *vars, t_config *read)
{
	int		i;
	int		j;
	t_list *temp;

	printf("feed_map\n");
	if (!(vars->map = (int **)(malloc(sizeof(int *) * read->map_row_size))))
		return (0);
	temp = read->map;
	i = 0;
	while (i < read->map_row_size)
	{
		if (!(vars->map[i] = (int *)(malloc(sizeof(int *) * temp->col_size))))
		{
			free_int_array(vars->map);
			return (0);
		}
		j = 0;
		while (j < temp->col_size)
		{
			vars->map[i][j] = temp->row[j];
			printf("%d", vars->map[i][j]);
			++j;
		}
		++i;
		temp = temp->next;
		printf("\n");
	}
	ft_lstclear(&read->map, free);
	return (1);
}

int		config_to_vars(t_vars *vars, t_config *read)
{
	feed_resolution(vars, read);
	feed_position(vars, read);
	if (!feed_sprite(vars, read))
		return (0);
	feed_color(vars, read);
	if (!feed_map(vars, read))
	{
		free(vars->sprites);
		free(vars->zbuffer);
		return (0);
	}
	return (1);
}

void	ft_bzero(void *s, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		*((unsigned char *)s) = 0;
		++s;
		++i;
	}
}

void sort_sprites(int *sprite_order, double *sprite_distance, int numsprites)
{
	int i;
	int	j;
	int temp_order;
	double temp_distance;

	i = -1;
	while (i++ < numsprites - 1)
    {
        j = -1;
		while (j++ < numsprites - 1)
        {
            if (sprite_distance[j] < sprite_distance[j + 1])
            {
                temp_distance = sprite_distance[j];
                sprite_distance[j] = sprite_distance[j + 1];
                sprite_distance[j + 1] = temp_distance;
				temp_order = sprite_order[j];
                sprite_order[j] = sprite_order[j + 1];
                sprite_order[j + 1] = temp_order;
            }
        }
    }
}

void	transform_sprites_coord(t_vars *vars, t_sprite_calc *calc)
{
	int 		w = vars->window_width;

	calc->invDet = 1.0 / \
				(vars->planeX * vars->dirY - vars->dirX * vars->planeY);
	calc->transformX = calc->invDet *\
			(vars->dirY * calc->spriteX - vars->dirX * calc->spriteY);
	calc->transformY = calc->invDet *\
			(-1 * vars->planeY * calc->spriteX + vars->planeX * calc->spriteY);
	calc->spritescreenX = (int)((w / 2) *\
						(1 + calc->transformX / calc->transformY));
}

void	calculate_sprites_height_width(t_vars *vars, t_sprite_calc *calc)
{
	int			w = vars->window_width;
	int			h = vars->window_height;

	calc->sprite_height = fabs((int)(h / calc->transformY));
	calc->draw_y_start = calc->sprite_height / -2 + h / 2;
	if (calc->draw_y_start < 0)
		calc->draw_y_start = 0;
	calc->draw_y_end = calc->sprite_height / 2 + h / 2;
	if (calc->draw_y_end >= h)
		calc->draw_y_end = h - 1;
	calc->sprite_width = fabs((int)(h / calc->transformY));
	calc->draw_x_start = calc->sprite_width / -2 + calc->spritescreenX;
	if (calc->draw_x_start < 0)
		calc->draw_x_start = 0;
	calc->draw_x_end = calc->sprite_width / 2 + calc->spritescreenX;
	if (calc->draw_x_end >= w)
		calc->draw_x_end = w - 1;
}

void	draw_sprites_body(t_sprite_calc *calc, t_vars *vars, int stripe)
{
	int y;
	int d;
	int color;

	y = calc->draw_y_start;
	d = 0;
	color = 0;
	while (y < calc->draw_y_end)
	{
		d = (y) * 256 - vars->window_height * 128 + calc->sprite_height * 128;
		calc->stripe_tex_y = ((d * vars->tex[4].img_height) / calc->sprite_height) / 256;
		color = vars->tex[4].data[vars->tex[4].img_width * calc->stripe_tex_y + calc->stripe_tex_x];
		if ((color & 0x00FFFFFF) != 0)
			vars->img.data[y * vars->window_width + stripe] = color;
		++y;
	}
}

void	draw_sprites(t_sprite_calc *calc, t_vars *vars)
{
	int	stripe;

	stripe = calc->draw_x_start; 
	while (stripe < calc->draw_x_end)
      {
        calc->stripe_tex_x = (int)(256 * (stripe -\
				(calc->sprite_width / -2 + calc->spritescreenX)) *\
						vars->tex[4].img_width / calc->sprite_width) / 256;
        if (calc->transformY > 0 && stripe > 0 \
				&& stripe < vars->window_width &&\
						calc->transformY < vars->zbuffer[stripe])
			draw_sprites_body(calc, vars, stripe);
		++stripe;
      }
}

void	add_sprite_body(t_vars *vars, int *spriteOrder,\
							double *spriteDistance, t_sprite_calc *calc)
{
	int i;

	i = 0;
    while (i < vars->num_sprite)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((vars->posX - vars->sprites[i].x) *\
	  (vars->posX - vars->sprites[i].x) + (vars->posY - vars->sprites[i].y)\
	  									* (vars->posY - vars->sprites[i].y));
	  ++i;
    }
    sort_sprites(spriteOrder, spriteDistance, vars->num_sprite);
	i = 0;
    while (i < vars->num_sprite)
    {
		calc->spriteX = vars->sprites[spriteOrder[i]].x - vars->posX;
		calc->spriteY = vars->sprites[spriteOrder[i]].y - vars->posY;
		transform_sprites_coord(vars, calc);
		calculate_sprites_height_width(vars, calc);
		draw_sprites(calc, vars);
		++i;
    }
}

int		add_sprites(t_vars *vars)
{
	int				*spriteOrder;
	double			*spriteDistance;
	t_sprite_calc	*calc;

	if (vars->num_sprite == 0)
		return (1);
	if (!(spriteOrder = (int *)malloc(sizeof(int) * vars->num_sprite)))
		return (0);
	if (!(spriteDistance = (double *)malloc(sizeof(double) * vars->num_sprite)))
	{
		free(spriteOrder);
		return (0);
	}
	if (!(calc = (t_sprite_calc *)malloc(sizeof(t_sprite_calc))))
	{
		free(spriteOrder);
		free(spriteDistance);
		return (0);
	}
	add_sprite_body(vars, spriteOrder, spriteDistance, calc);
	free(spriteOrder);
	free(spriteDistance);
	free(calc);
	return (1);
}

void	go_to_intersection(t_vars *vars)
{
	if (vars->raydirX < 0)
	{
		vars->stepX = -1;
		vars->sideDistX = (vars->posX - vars->mapX) * vars->deltaDistX;
	}
	else
	{
		vars->stepX = 1;
		vars->sideDistX = (vars->mapX + 1.0 - vars->posX) * vars->deltaDistX;
	}
	if (vars->raydirY < 0)
	{
		vars->stepY = -1;
		vars->sideDistY = (vars->posY - vars->mapY) * vars->deltaDistY;
	}
	else
	{
		vars->stepY = 1;
		vars->sideDistY = (vars->mapY + 1.0 - vars->posY) * vars->deltaDistY;
	}
}


void	check_wall_hit(t_vars *vars)
{
	int hit;

	hit = 0;
	while (hit == 0)
	{
		if (vars->sideDistX < vars->sideDistY)
		{
			vars->sideDistX += vars->deltaDistX;
			vars->mapX += vars->stepX;
			vars->side = 0;
		}
		else
		{
			vars->sideDistY += vars->deltaDistY;
			vars->mapY += vars->stepY;
			vars->side = 1;
		}
		if ((vars->map)[vars->mapX][vars->mapY] == 1)
			hit = 1;
	}
}

void	get_height_to_draw(t_vars *vars)
{
	int h;

	h = vars->window_height;
	if (vars->side == 0)
		vars->perpWallDist =\
			(vars->mapX - vars->posX + (1 - vars->stepX) / 2) / vars->raydirX;
	else
		vars->perpWallDist =\
			(vars->mapY - vars->posY + (1 - vars->stepY) / 2) / vars->raydirY;
	vars->lineHeight = (int)(h / vars->perpWallDist);
	vars->drawStart = vars->lineHeight / - 2 + h / 2;
	vars->drawEnd = vars->lineHeight / 2 + h / 2;

}

void	get_texture_loc(t_vars *vars, int tex_width, int tex_height)
{
	if (vars->side == 0) 
		vars->WallX = vars->posY + vars->perpWallDist * vars->raydirY;
	else
		vars->WallX = vars->posX + vars->perpWallDist * vars->raydirX;
	vars->WallX -= floor((vars->WallX));
	vars->texX = (int)(vars->WallX * (double)(tex_width));
	if (vars->side == 0 && vars->raydirX > 0)
		vars->texX = tex_width - vars->texX - 1;
	if (vars->side == 1 && vars->raydirY < 0)
		vars->texX = tex_width - vars->texX - 1;
	vars->step = 1.0 * tex_height / vars->lineHeight;
	vars->texPos = 0.0;
}

void	draw_ceilling(t_vars *vars, int x)
{
	int ceil;

	ceil = 0;
	while (ceil < vars->drawStart)
	{
		vars->img.data[ceil * vars->window_width + x] = vars->ceilling_color;
		++ceil;
	}
}

void	draw_tex_on_walls(t_vars *vars, int x, int index)
{
	int y;
	int color;
	int tex_width;
	int tex_height;
	
	y = vars->drawStart;
	color = 0;
	tex_width = vars->tex[index].img_width;
	tex_height = vars->tex[index].img_height;
	get_texture_loc(vars, tex_width, tex_height);
	while (y < vars->drawEnd)
	{
		vars->texY = (int)(vars->texPos) & (tex_height - 1);
		vars->texPos += vars->step;
		if ((int)vars->texY >= 0 && (int)vars->texY <= tex_height - 1 &&\
			 y >= 0 && y <= vars->window_height)
		{
			color = vars->tex[index].data[tex_width * vars->texY + vars->texX];
			vars->img.data[y * vars->window_width + x] = color;
		}
		++y;
	}
}

void	draw_walls(t_vars *vars, int x)
{
	if (vars->side == 1 && vars->raydirY >= 0)
		draw_tex_on_walls(vars, x, 0);
	else if (vars->side == 1 && vars->raydirY < 0)
		draw_tex_on_walls(vars, x, 1);
	else if (vars->side == 0 && vars->raydirX >= 0)
		draw_tex_on_walls(vars, x, 2);
	else if (vars->side == 0 && vars->raydirX < 0)
		draw_tex_on_walls(vars, x, 3);
}

void	draw_floor(t_vars *vars, int x)
{
	int y;
	
	y = vars->drawEnd;
	while (y < vars->window_height)
	{
		vars->img.data[y * vars->window_width + x] = vars->floor_color;
		++y;
	}
}

void	draw_all(t_vars *vars, int x)
{
	draw_ceilling(vars, x);
	draw_walls(vars, x);
	draw_floor(vars, x);
}

int render(t_vars *vars)
{
	int x;

	x = 0;
	while (x < vars->window_width)
	{
		vars->cameraX = 2 * x / (double)vars->window_width - 1;
		vars->raydirX = vars->dirX + vars->planeX * vars->cameraX;
		vars->raydirY = vars->dirY + vars->planeY * vars->cameraX;
		vars->mapX = (int)(vars->posX);
		vars->mapY = (int)(vars->posY);
		vars->deltaDistX = fabs(1 / vars->raydirX);
		vars->deltaDistY = fabs(1 / vars->raydirY);
		go_to_intersection(vars);
		check_wall_hit(vars);
		get_height_to_draw(vars);
		draw_all(vars, x);
		vars->zbuffer[x] = vars->perpWallDist;
		++x;
	}
	if (!add_sprites(vars))
		return (0);
	return (1);
}

void	go_forward(t_vars *vars)
{
	if ((vars->map)[(int)(vars->posX + vars->dirX * vars->moving_speed)][(int)(vars->posY)] != 1)
     	vars->posX += vars->dirX * vars->moving_speed;
	if ((vars->map)[(int)(vars->posX)][(int)(vars->posY + vars->dirY * vars->moving_speed)] != 1)
    	vars->posY += (vars->dirY) * vars->moving_speed;
}


void	go_backward(t_vars *vars)
{
	if((vars->map)[(int)(vars->posX - vars->dirX * vars->moving_speed)][(int)(vars->posY)] != 1)
    	vars->posX -= vars->dirX * vars->moving_speed;
	if((vars->map)[(int)(vars->posX)][(int)(vars->posY - vars->dirY * vars->moving_speed)] != 1)
    	vars->posY -= vars->dirY * vars->moving_speed;
}

void	go_right(t_vars *vars)
{
	if ((vars->map)[(int)(vars->posX + vars->planeX * vars->moving_speed)][(int)(vars->posY)] != 1)
     	vars->posX += (vars->planeX) * vars->moving_speed;
	if ((vars->map)[(int)(vars->posX)][(int)(vars->posY + vars->planeY * vars->moving_speed)] != 1)
    	vars->posY += (vars->planeY) * vars->moving_speed;
}

void	go_left(t_vars *vars)
{
	if (vars->map[(int)(vars->posX - vars->planeX * vars->moving_speed)][(int)(vars->posY)] != 1)
     	vars->posX -= (vars->planeX) * vars->moving_speed;
	if (vars->map[(int)(vars->posX)][(int)(vars->posY - vars->planeY * vars->moving_speed)] != 1)
    	vars->posY -= (vars->planeY) * vars->moving_speed;
}

void	rotate_right(t_vars *vars)
{
	vars->olddirX = vars->dirX;
	vars->dirX = vars->dirX * cos(-vars->rot_speed) - vars->dirY * sin(-vars->rot_speed);
	vars->dirY = vars->olddirX * sin(-vars->rot_speed) + vars->dirY * cos(-vars->rot_speed);
	vars->oldplaneX = vars->planeX;
	vars->planeX = vars->planeX * cos(-vars->rot_speed) - vars->planeY * sin(-vars->rot_speed);
	vars->planeY = vars->oldplaneX * sin(-vars->rot_speed) + vars->planeY * cos(-vars->rot_speed);
}

void	rotate_left(t_vars *vars)
{
	vars->olddirX = vars->dirX;
	vars->dirX = vars->dirX * cos(vars->rot_speed) - vars->dirY * sin(vars->rot_speed);
	vars->dirY = vars->olddirX * sin(vars->rot_speed) + vars->dirY * cos(vars->rot_speed);
	vars->oldplaneX = vars->planeX;
	vars->planeX = vars->planeX * cos(vars->rot_speed) - vars->planeY * sin(vars->rot_speed);
	vars->planeY = vars->oldplaneX * sin(vars->rot_speed) + vars->planeY * cos(vars->rot_speed);
}

int	free_all_loads(t_vars *vars)
{
	free(vars->tex);
	free(vars->sprites);
	free(vars->zbuffer);
	return (0);
}

void free_and_exit(t_vars *vars, t_config *read)
{
	free_all_loads(vars);
	free_all(vars, read);
	exit(0);
}

int		move_keys(int keycode, t_vars *vars)
{
	vars->moving_speed = 0.3;
	vars->rot_speed = 0.2;
	if (keycode == 119)
		go_forward(vars);
	if (keycode == 115)
		go_backward(vars);
  	if (keycode == 100)
		go_right(vars);
	if (keycode == 97)
		go_left(vars);
	if (keycode == 65363)
    	rotate_right(vars);
	if (keycode == 65361)
    	rotate_left(vars);
	if (keycode == 65307)
		exit(0);
	ft_bzero(vars->img.data, sizeof(int) * vars->window_width * vars->window_height);
	if (!render(vars))
		return (0);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
	return (1);
}

int	click_red_cross(t_vars *vars, int click, t_config *read)
{
	if (click == 1)
		mlx_destroy_window(vars->mlx, vars->win);
	free_and_exit(vars, read);
	return (1);
}

int load_wall_texture(t_vars *vars, t_config *read)
{
	if (!(vars->tex[0].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->north_path, &vars->tex[0].img_width, &vars->tex[0].img_height)))
		return (0);
 	vars->tex[0].data = (int *)mlx_get_data_addr(vars->tex[0].img_ptr,\
	 	&vars->tex[0].bpp, &vars->tex[0].size_line, &vars->tex[0].endian);
	if (!(vars->tex[1].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->south_path, &vars->tex[1].img_width, &vars->tex[1].img_height)))
		return (0);
 	vars->tex[1].data = (int *)mlx_get_data_addr(vars->tex[1].img_ptr,\
	 	&vars->tex[1].bpp, &vars->tex[1].size_line, &vars->tex[1].endian);
	if (!(vars->tex[2].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->east_path, &vars->tex[2].img_width, &vars->tex[2].img_height)))
		return (0);
 	vars->tex[2].data = (int *)mlx_get_data_addr(vars->tex[2].img_ptr,\
	 	&vars->tex[2].bpp, &vars->tex[2].size_line, &vars->tex[2].endian);
	if (!(vars->tex[3].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->west_path, &vars->tex[3].img_width, &vars->tex[3].img_height)))
		return (0);
 	vars->tex[3].data = (int *)mlx_get_data_addr(vars->tex[3].img_ptr,\
	 	&vars->tex[3].bpp, &vars->tex[3].size_line, &vars->tex[3].endian);
	if (!(vars->tex[4].img_ptr = mlx_xpm_file_to_image(vars->mlx,\
		read->sprite_path, &vars->tex[4].img_width, &vars->tex[4].img_height)))
		return (0);
	return (1);
}

int	load_img(t_vars *vars, t_config *read)
{
	vars->tex = (t_texture *)malloc(sizeof(t_texture) * 5);
	
	if (!load_wall_texture(vars, read))
		return (0);
 	vars->tex[4].data = (int *)mlx_get_data_addr(vars->tex[4].img_ptr,\
	 	&vars->tex[4].bpp, &vars->tex[4].size_line, &vars->tex[4].endian);
	return (1);
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*dest;

	dest = (unsigned char *)b;
	while (len--)
		*(dest++) = (unsigned char)c;
	return (b);
}

int		extract_color(t_vars *vars, int x, int y)
{
	int	rgb;
	int	color;

	color = vars->img.data[x + (vars->window_height - 1 - y) *\
												vars->window_width];
	rgb = (color & 0xFF0000) | (color & 0x00FF00) | (color & 0x0000FF);
	return (rgb);
}

int		fill_bmp_color(int fd, t_vars *vars, int pad)
{
	unsigned char	zero[3];
	int					x;
	int					y;
	int					color;

	ft_memset(zero, 0, 3);
	y = 0;
	while (y < vars->window_height)
	{
		x = 0;
		while (x < vars->window_width)
		{
			color = extract_color(vars, x, y);
			if (write(fd, &color, 3) < 0)
				return (0);
			if (pad > 0 && write(fd, &zero, pad) < 0)
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

void	int_to_char(unsigned char *num, int value)
{
	num[0] = (unsigned char)(value);
	num[1] = (unsigned char)(value >> 8);
	num[2] = (unsigned char)(value >> 16);
	num[3] = (unsigned char)(value >> 24);
}

int		write_bmp_header(int fd, int file_size, t_vars *vars)
{
	unsigned char bmp_file_header[14];
	unsigned char bmp_info_header[40];
	
	ft_memset(bmp_file_header, 0, 14);
	bmp_file_header[0] = (unsigned char)('B');
	bmp_file_header[1] = (unsigned char)('M');
	int_to_char(bmp_file_header + 2, file_size);
	bmp_file_header[10] = (unsigned char)(54);
	if (write(fd, bmp_file_header, 14) < 0)
		return (0);
	ft_memset(bmp_info_header, 0, 40);
	bmp_info_header[0] = (unsigned char)(40);
	int_to_char(bmp_info_header + 4, vars->window_width);
	int_to_char(bmp_info_header + 8, vars->window_height);
	bmp_info_header[13] = (unsigned char)(1);
	bmp_info_header[14] = (unsigned char)(24);
	if (write(fd, bmp_info_header, 40) < 0)
		return (0);
	return (1);
}

int	save_bmp(t_vars *vars)
{
	int fd;
	int file_size;
	int pad;

	fd = 0;
	pad = (4 - (vars->window_width * 3) % 4) % 4;
	if ((fd = open("save.bmp", O_WRONLY | O_CREAT
									| O_TRUNC | O_APPEND)) < 0)
		return (0);
	file_size = 54 + ((vars->window_width + pad) * vars->window_height);
	if (!write_bmp_header(fd, file_size, vars))
		return (0);
	if (!fill_bmp_color(fd, vars, pad))
		return (0);
	close(fd);
	return (1);
}


int	play_on_screen(t_vars *vars, t_config *read, int save_flag)
{
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx,\
							vars->window_width, vars->window_height, "Cub3d");
	vars->img.img_ptr = mlx_new_image(vars->mlx,\
							vars->window_width, vars->window_height);
 	vars->img.data = (int *)mlx_get_data_addr(vars->img.img_ptr,\
	 						&vars->img.bpp, &vars->img.size_line, &vars->img.endian);
	if (!load_img(vars, read))
	{
		free(vars->tex);
		return (0);
	}
	if (!render(vars))
		return (0);
	if (save_flag)
	{
		if (!save_bmp(vars))
			return (0);
	}
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
	mlx_hook(vars->win, 2, 1L<<0, &move_keys, vars);
	mlx_hook(vars->win, 17, 1L<<0, &click_red_cross, vars);
	mlx_loop(vars->mlx);
	free_all_loads(vars);
	return (1);
}

int argc_check(int *save_flag, int argc, char *argv[])
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

int read_and_play(t_vars *vars, t_config *read, char *argv[], int save_flag)
{
	if (!read_config(vars, read, argv[1]))
		return (free_all(vars, read));
	if (!config_to_vars(vars, read))
		return (free_all(vars, read));
	if (!play_on_screen(vars, read, save_flag))
	{
		free_all_loads(vars);
		return (free_all(vars, read));
	}
	return (1);
}

int	return_error(void)
{
	write(1, "Error\n", 6);
	return (0);
}

void	init_read_flag(t_config *read)
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

int main(int argc, char *argv[])
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