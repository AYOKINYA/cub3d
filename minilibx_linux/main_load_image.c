#include "mlx.h"

int     main(void)
{
    void    *mlx;
    void    *img;
    void    *win;
    char    *relative_path = "pics/bluestone.xpm";
    int     img_width;
    int     img_height;

    mlx = mlx_init();
    win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
    img = mlx_xpm_file_to_image(mlx, relative_path, &img_width, &img_height);
    mlx_put_image_to_window(mlx, win, img, img_width, img_height);
	mlx_loop(mlx);
}