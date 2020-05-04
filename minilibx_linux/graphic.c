#include "mlx.h"
#include "mlx_int.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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
	t_data		img;
	t_texture	*tex;
	int			key;
	double		texPos;
	double		step;
	int			texX;
	int			texY;
	int			**texture;
}               t_vars;

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

int worldMap[mapWidth][mapHeight]=
  {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  };

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

double ZBuffer[window_width];

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

void	draw_ceiling(t_vars *vars, int x)
{
	int ceil;
	int color;

	ceil = 0;
	while (ceil < vars->drawStart)
	{
		color = 0x0087CEEB;
		vars->img.data[ceil * window_width + x] = color;
		++ceil;
	}
}

void	draw_walls(t_vars *vars, int x)
{
	int y;
	int color;
	
	y = vars->drawStart;
	color = 0;
	while (y < vars->drawEnd)
	{
		vars->texY = (int)(vars->texPos) & (texHeight - 1);
		vars->texPos += vars->step;
		if (worldMap[vars->mapX][vars->mapY] == 2)
		{
			//paint pixel if it isn't black, black is the invisible color
			color = vars->tex[4].data[texWidth * vars->texY + vars->texX];
			if ((color & 0x00FFFFFF) != 0)
				vars->img.data[y * window_width + x] = color; 
		}
		else
		{
			if (vars->side == 1 && vars->raydirY >= 0)
				color = vars->tex[0].data[texWidth * vars->texY + vars->texX];
			else if (vars->side == 1 && vars->raydirY < 0)
				color = vars->tex[1].data[texWidth * vars->texY + vars->texX];
			else if (vars->side == 0 && vars->raydirX >= 0)
				color = vars->tex[2].data[texWidth * vars->texY + vars->texX];
			else
				color = vars->tex[3].data[texWidth * vars->texY + vars->texX];
		}
		vars->img.data[y * window_width + x] = color;
		++y;
	}
}

void	draw_floor(t_vars *vars, int x)
{
	int y;
	int color;
	
	y = vars->drawEnd;
	while (y < window_height)
	{
		color = 0x0031B1CB;
		vars->img.data[y * window_width + x] = color;
		++y;
	}
}

void	transform_sprites_coord(t_vars *vars, t_sprite_calc *calc)
{
	int 		w = window_width;

	calc->invDet = 1.0 / (vars->planeX * vars->dirY - vars->dirX * vars->planeY);
	calc->transformX = calc->invDet * (vars->dirY * calc->spriteX - vars->dirX * calc->spriteY);
	calc->transformY = calc->invDet * (-1 * vars->planeY * calc->spriteX + vars->planeX * calc->spriteY); //this is actually the depth inside the screen, that what Z is in 3D
	calc->spritescreenX = (int)((w / 2) * (1 + calc->transformX / calc->transformY));
}

void	calculate_sprites_height_width(t_sprite_calc *calc)
{
	int			w = window_width;
	int			h = window_height;

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

void	draw_sprites(t_sprite_calc *calc, t_vars *vars)
{
	int			w = window_width;
	int			h = window_height;

	for (int stripe = calc->draw_x_start; stripe < calc->draw_x_end; stripe++)
      {
        calc->stripe_tex_x = (int)(256 * (stripe - (calc->sprite_width / -2 + calc->spritescreenX)) * texWidth / calc->sprite_width) / 256;

        if (calc->transformY > 0 && stripe > 0 && stripe < w && calc->transformY < ZBuffer[stripe])
		{
			for (int y = calc->draw_y_start; y < calc->draw_y_end; y++) //for every pixel of the current stripe
			{
				int d = (y) * 256 - h * 128 + calc->sprite_height * 128; //256 and 128 factors to avoid floats
				calc->stripe_tex_y = ((d * texHeight) / calc->sprite_height) / 256;
				int color = vars->tex[4].data[texWidth * calc->stripe_tex_y + calc->stripe_tex_x]; //get current color from the texture
				if ((color & 0x00FFFFFF) != 0)
					vars->img.data[y * window_width + stripe] = color; //paint pixel if it isn't black, black is the invisible color
			}
		}
      }
}

void	add_sprites(t_vars *vars)
{
	t_sprite	*sprites;
	int			*spriteOrder;
	double		*spriteDistance;
	t_sprite_calc *calc;

	sprites = (t_sprite *)malloc(sizeof(t_sprite) * 8);
	spriteOrder = (int *)malloc(sizeof(int) * 8);
	spriteDistance = (double *)malloc(sizeof(double) * 8);
	calc = (t_sprite_calc *)malloc(sizeof(t_sprite_calc));

	sprites[0].x = 21.5;
	sprites[0].y = 1.5;
	sprites[1].x = 15.5;
	sprites[1].y = 1.5;
	sprites[2].x = 16.0;
	sprites[2].y = 1.8;
	sprites[3].x = 16.2;
	sprites[3].y = 1.2;
	sprites[4].x = 3.5;
	sprites[4].y = 2.5;
	sprites[5].x = 9.5;
	sprites[5].y = 15.5;
	sprites[6].x = 10.0;
	sprites[6].y = 15.1;
	sprites[7].x = 10.5;
	sprites[7].y = 15.8;
	
	//SPRITE CASTING
    //sort sprites from far to close
    for(int i = 0; i < 8; i++)
    {
      spriteOrder[i] = i;
      spriteDistance[i] = ((vars->posX - sprites[i].x) * (vars->posX - sprites[i].x) + (vars->posY - sprites[i].y) * (vars->posY - sprites[i].y));
    }
    sort_sprites(spriteOrder, spriteDistance, 8);

    //after sorting the sprites, do the projection and draw them
    for(int i = 0; i < 8; i++)
    {
		calc->spriteX = sprites[spriteOrder[i]].x - vars->posX;
		calc->spriteY = sprites[spriteOrder[i]].y - vars->posY;
		transform_sprites_coord(vars, calc);
		calculate_sprites_height_width(calc);
		draw_sprites(calc, vars);
    }
	free(sprites);
	free(spriteOrder);
	free(spriteDistance);
	free(calc);
}

void	draw_all(t_vars *vars, int x)
{
	draw_ceiling(vars, x);
	draw_walls(vars, x);
	draw_floor(vars, x);
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
		if (worldMap[vars->mapX][vars->mapY] > 0 && worldMap[vars->mapX][vars->mapY] != 2)
			hit = 1;
	}
}

void	get_height_to_draw(t_vars *vars)
{
	int h;

	h = window_height;
	if (vars->side == 0)
		vars->perpWallDist = (vars->mapX - vars->posX + (1 - vars->stepX) / 2) / vars->raydirX;
	else
		vars->perpWallDist = (vars->mapY - vars->posY + (1 - vars->stepY) / 2) / vars->raydirY;
	vars->lineHeight = (int)(h / vars->perpWallDist);
	vars->drawStart = vars->lineHeight / - 2 + h / 2;
	if (vars->drawStart < 0)
		vars->drawStart = 0;
	vars->drawEnd = vars->lineHeight / 2 + h / 2;
	if (vars->drawEnd >= h)
		vars->drawEnd = h - 1;
}

void	get_texture_loc(t_vars *vars)
{
	if (vars->side == 0) 
		vars->WallX = vars->posY + vars->perpWallDist * vars->raydirY;
	else
		vars->WallX = vars->posX + vars->perpWallDist * vars->raydirX;
	vars->WallX -= floor((vars->WallX));

	vars->texX = (int)(vars->WallX * (double)(texWidth));
	if (vars->side == 0 && vars->raydirX > 0)
		vars->texX = texWidth - vars->texX - 1;
	if (vars->side == 1 && vars->raydirY < 0)
		vars->texX = texWidth - vars->texX - 1;

	vars->step = 1.0 * texHeight / vars->lineHeight;
	vars->texPos = 0.0; //(vars->drawStart - h / 2 + vars->lineHeight / 2) * vars->step;
}

void render(t_vars *vars)
{
	int x;
	int w = window_width;

	x = 0;
	while (x < window_width)
	{
		vars->cameraX = 2 * x / (double)w - 1;
		vars->raydirX = vars->dirX + vars->planeX * vars->cameraX;
		vars->raydirY = vars->dirY + vars->planeY * vars->cameraX;
		vars->mapX = (int)(vars->posX);
		vars->mapY = (int)(vars->posY);
		vars->deltaDistX = fabs(1 / vars->raydirX);
		vars->deltaDistY = fabs(1 / vars->raydirY);
		go_to_intersection(vars);
		check_wall_hit(vars);
		get_height_to_draw(vars);
		get_texture_loc(vars);
		draw_all(vars, x);
		ZBuffer[x] = vars->perpWallDist;
		++x;
	}
	add_sprites(vars);
}

void	go_forward(t_vars *vars)
{
	if (worldMap[(int)(vars->posX + vars->dirX)][(int)(vars->posY)] != 1)
     	vars->posX += vars->dirX * vars->moving_speed;
	if (worldMap[(int)(vars->posX)][(int)(vars->posY + vars->dirY)] != 1)
    	vars->posY += (vars->dirY) * vars->moving_speed;
}


void	go_backward(t_vars *vars)
{
	if(worldMap[(int)(vars->posX - vars->dirX)][(int)(vars->posY)] != 1)
    	vars->posX -= vars->dirX * vars->moving_speed;
	if(worldMap[(int)(vars->posX)][(int)(vars->posY - vars->dirY)] != 1)
    	vars->posY -= vars->dirY * vars->moving_speed;
}

void	go_right(t_vars *vars)
{
	if (worldMap[(int)(vars->posX + vars->planeX)][(int)(vars->posY)] != 1)
     	vars->posX += (vars->planeX) * vars->moving_speed;
	if (worldMap[(int)(vars->posX)][(int)(vars->posY + vars->planeY)] != 1)
    	vars->posY += (vars->planeY) * vars->moving_speed;
}

void	go_left(t_vars *vars)
{
	if (worldMap[(int)(vars->posX - vars->planeX)][(int)(vars->posY)] != 1)
     	vars->posX -= (vars->planeX) * vars->moving_speed;
	if (worldMap[(int)(vars->posX)][(int)(vars->posY - vars->planeY)] != 1)
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

int		move_keys(int keycode, t_vars *vars)
{
	vars->moving_speed = 0.5;
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
	ft_bzero(vars->img.data, sizeof(int) * window_width * window_height);
	render(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
	return (0);
}

void	free_all_loads(t_vars *vars)
{
	int i;

	i = 0;
    while (i < 8)
	{
		free((vars->texture)[i]);
		++i;
	}
	free((vars->texture));
	ft_bzero(vars->tex[0].data, sizeof(int) * vars->tex[0].img_width * vars->tex[0].img_width);
	ft_bzero(vars->tex[1].data, sizeof(int) * vars->tex[1].img_width * vars->tex[1].img_width);
	ft_bzero(vars->tex[2].data, sizeof(int) * vars->tex[2].img_width * vars->tex[2].img_width);
	ft_bzero(vars->tex[3].data, sizeof(int) * vars->tex[3].img_width * vars->tex[3].img_width);
	ft_bzero(vars->tex[4].data, sizeof(int) * vars->tex[3].img_width * vars->tex[3].img_width);
	free(vars->tex);
	free(vars);
}

void	load_img(t_vars *vars)
{
	vars->tex = (t_texture *)malloc(sizeof(t_texture) * 5);

	vars->tex[0].img_ptr = mlx_xpm_file_to_image(vars->mlx, "pics/bluestone.xpm", &vars->tex[0].img_width, &vars->tex[0].img_width);
 	vars->tex[0].data = (int *)mlx_get_data_addr(vars->tex[0].img_ptr, &vars->tex[0].bpp, &vars->tex[0].size_line, &vars->tex[0].endian);
	vars->tex[1].img_ptr = mlx_xpm_file_to_image(vars->mlx, "pics/eagle.xpm", &vars->tex[1].img_width, &vars->tex[1].img_width);
 	vars->tex[1].data = (int *)mlx_get_data_addr(vars->tex[1].img_ptr, &vars->tex[1].bpp, &vars->tex[1].size_line, &vars->tex[1].endian);
	vars->tex[2].img_ptr = mlx_xpm_file_to_image(vars->mlx, "pics/purplestone.xpm", &vars->tex[2].img_width, &vars->tex[2].img_width);
 	vars->tex[2].data = (int *)mlx_get_data_addr(vars->tex[2].img_ptr, &vars->tex[2].bpp, &vars->tex[2].size_line, &vars->tex[2].endian);
	vars->tex[3].img_ptr = mlx_xpm_file_to_image(vars->mlx, "pics/redbrick.xpm", &vars->tex[3].img_width, &vars->tex[3].img_width);
 	vars->tex[3].data = (int *)mlx_get_data_addr(vars->tex[3].img_ptr, &vars->tex[3].bpp, &vars->tex[3].size_line, &vars->tex[3].endian);
	vars->tex[4].img_ptr = mlx_xpm_file_to_image(vars->mlx, "pics/barrel.xpm", &vars->tex[4].img_width, &vars->tex[4].img_width);
 	vars->tex[4].data = (int *)mlx_get_data_addr(vars->tex[4].img_ptr, &vars->tex[4].bpp, &vars->tex[4].size_line, &vars->tex[4].endian);
}

void	play_on_screen(t_vars *vars)
{

	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, window_width, window_height, "Cub3d");
	vars->img.img_ptr = mlx_new_image(vars->mlx, window_width, window_height);
 	vars->img.data = (int *)mlx_get_data_addr(vars->img.img_ptr, &vars->img.bpp, &vars->img.size_line, &vars->img.endian);
	load_img(vars);
	render(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
	mlx_hook(vars->win, 2, 1L<<0, &move_keys, vars);
	mlx_loop(vars->mlx);
}

void	init_values(t_vars *vars)
{
	double radian;

	vars->posX = 22;
	vars->posY = 12;  //x and y start position
	vars->dirX = -1;
	vars->dirY = 0; //initial direction vector
	vars->planeX = 0;
	vars->planeY = 0.66; //the 2d raycaster version of camera plane

	//NSWE에 따라서 RADIAN 값 90도씩 바꾸기
	radian = 270 * 3.141592 / 180;
	vars->olddirX = vars->dirX;
	vars->dirX = vars->dirX * cos(radian) - vars->dirY * sin(radian);
	vars->dirY = vars->olddirX * sin(radian) + vars->dirY * cos(radian);
	vars->oldplaneX = vars->oldplaneX;
	vars->planeX = vars->planeX * cos(radian) - vars->planeY * sin(radian);
	vars->planeY = vars->oldplaneX * sin(radian) + vars->planeY * cos(radian);
}
/*
void	load_texture(t_vars *vars)
{
	int	i;
	int	x;
	int y;

	vars->texture = (int **)malloc(sizeof(int *) * 8);
	i = 0;
	while (i < 8)
	{
		(vars->texture)[i] = (int *)malloc(sizeof(int) * texWidth * texHeight);
		++i;
	}
	x = 0;
	while (x < texWidth)
	{
		y = 0;
		while ( y < texHeight)
		{
			int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
			int ycolor = y * 256 / texHeight;
			int xycolor = y * 128 / texHeight + x * 128 / texWidth;
			(vars->texture)[0][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y); //flat red texture with black cross
			(vars->texture)[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
			(vars->texture)[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
			(vars->texture)[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
			(vars->texture)[4][texWidth * y + x] = 256 * xorcolor; //xor green
			(vars->texture)[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
			(vars->texture)[6][texWidth * y + x] = 65536 * ycolor; //red gradient
			(vars->texture)[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
			++y;
		}
		++x;
	}
}
*/
int main(void)
{
	t_vars	*vars;

	vars = (t_vars*)malloc(sizeof(t_vars));
	//load_texture(vars);
	init_values(vars);
	play_on_screen(vars);
	free_all_loads(vars);
}