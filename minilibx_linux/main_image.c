#include "mlx.h"
#include <math.h>
#include <stdio.h>

typedef struct	s_img

{
	void		*img_ptr;
	int			*data; 
	int			size_line;
	int			bpp;
	int			endian;

}				t_img;


typedef struct	s_mlx

{
	void		*mlx_ptr;
	void		*win;
	t_img		img;

}				t_mlx;

int		create_trgb(int t, int r, int g, int b)
{
	return(t << 24 |  r << 16 | g << 8 | b);
}

int	main(void)

{

	t_mlx	mlx;

	int		count_w;
	int		count_h;
	int		i;
	int		degree;
	double	radian;
	int x;
	int y;

	count_h = -1;
	mlx.mlx_ptr = mlx_init();
	mlx.win = mlx_new_window(mlx.mlx_ptr, 800, 600, "A simple example");
	mlx.img.img_ptr = mlx_new_image(mlx.mlx_ptr, 800, 600);
	mlx.img.data = (int *)mlx_get_data_addr(mlx.img.img_ptr, &mlx.img.bpp, &mlx.img.size_line, &mlx.img.endian);
	// while (++count_h < 600)
	// {
	// 	count_w = -1;
	// 	while (++count_w < 800)
	// 	{
	// 		if (count_w % 2)
	// 			mlx.img.data[count_h * 800 + count_w] = 0xFFFFFF;
	// 		else
	// 			mlx.img.data[count_h * 800 + count_w] = 0;
	// 	}
	// }
	i = 1;
	while (i < 30)
	{
		degree = 0;
		while (degree < 360)
		{
			radian = (degree) * 3.141592 / 180;
			x = i * cos(radian);
			y = i * sin(radian);
			mlx.img.data[(y + 100) * 800 + (x + 100)] = create_trgb(0, 255 - 255 / 30 * i, 0, 255 / 30 * i);
			//create_trgb(0, 255 - 255 / 30 * i, 0, 255 / 30 * i);
			++degree;
		}
		++i;
	}
	int t;
	int j;
	t = 0;
	i = 0;
	j = 0;
	while (j < 100)
	{
		i = 0;
		while (i < 1000)
		{
			t += 2 * 3.141592 / 1000 * i;
			x = 16 * sin(t) * sin(t) * sin(t) * 3 * 0.01 * j;
			y = (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)) * 3 * 0.01 * j;
			mlx.img.data[(-y + 200) * 800 + (-x + 200)] = 0xFF0000;
			i++;
		}
		++j;
	}
	mlx_put_image_to_window(mlx.mlx_ptr, mlx.win, mlx.img.img_ptr, 0, 0);
	mlx_loop(mlx.mlx_ptr);
	return (0);
}