/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_bmp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkang <jkang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 13:43:49 by jkang             #+#    #+#             */
/*   Updated: 2020/06/29 15:52:48 by jkang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	extract_color(t_vars *vars, int x, int y)
{
	int	rgb;
	int	color;

	color = vars->img.data[x + (vars->window_height - 1 - y) *\
												vars->window_width];
	rgb = (color & 0xFF0000) | (color & 0x00FF00) | (color & 0x0000FF);
	return (rgb);
}

static int	fill_bmp_color(int fd, t_vars *vars, int pad)
{
	unsigned char	zero[3];
	int				x;
	int				y;
	int				color;

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

static void	int_to_char(unsigned char *num, int value)
{
	num[0] = (unsigned char)(value);
	num[1] = (unsigned char)(value >> 8);
	num[2] = (unsigned char)(value >> 16);
	num[3] = (unsigned char)(value >> 24);
}

static int	write_bmp_header(int fd, int file_size, t_vars *vars)
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

int			save_bmp(t_vars *vars)
{
	int fd;
	int file_size;
	int pad;

	fd = 0;
	pad = (4 - (vars->window_width * 3) % 4) % 4;
	if ((fd = open("save.bmp", O_CREAT | O_TRUNC | O_RDWR, 0644)) < 0)
		return (0);
	file_size = 54 + ((vars->window_width + pad) * vars->window_height);
	if (!write_bmp_header(fd, file_size, vars))
		return (0);
	if (!fill_bmp_color(fd, vars, pad))
		return (0);
	close(fd);
	exit(0);
	return (1);
}
