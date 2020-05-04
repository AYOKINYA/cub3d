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

typedef struct  s_vars {
	void        *mlx;
	void        *win;
	int			color;
	char		*string;
	double posX;
	double posY;
	double raydirX;
	double olddirX;
	double dirX;
	double raydirY;
	double olddirY;
	double dirY;
	double rot_speed;
	double moving_speed;
	double oldplaneX;
	double planeX;
	double planeY;
	int		mapX;
	int		mapY;
	t_data	img;
	int		key;
	double texPos;
	double step;
	int		texX;
	int		texY;
	int		texNum;
	int		**texture;
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
/*
int worldMap[mapWidth][mapHeight]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};
*/

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

void verLine(t_vars vars, int x, int drawStart, int drawEnd, int color)
{
  int y;

  y = drawStart;

  while (y < drawEnd)
  {
	vars.img.data[y * window_width + x] = color;
	++y;
  }
}

void	dotting(t_vars *vars, int x, int drawStart, int drawEnd, int side)
{
	int y;
	int ceil;
	int color;
	
	y = drawStart;

	while (y < drawEnd)
	{
		vars->texY = (int)(vars->texPos) & (texHeight - 1);
		vars->texPos += vars->step;
		if (worldMap[vars->mapX][vars->mapY] == 2)
			color = 0X00FF6924; //vars->texture)[6][texHeight * vars->texY + vars->texX];
		else
		{
			if (side == 1 && vars->raydirY >= 0)
				color =(vars->texture)[0][texHeight * vars->texY + vars->texX];
			else if (side == 1 && vars->raydirY < 0)
				color = (vars->texture)[1][texHeight * vars->texY + vars->texX];
			else if (side == 0 && vars->raydirX >= 0)
				color = (vars->texture)[2][texHeight * vars->texY + vars->texX];
			else
				color = (vars->texture)[4][texHeight * vars->texY + vars->texX];
		}
		vars->img.data[y * window_width + x] = color;
		++y;
	}
}

void render(t_vars *vars)
{
	int x;
	int w = window_width;
	int h = window_height;

  x = 0;
  while (x < w)
	{
		double cameraX = 2 * x / (double)w - 1;
		vars->raydirX = vars->dirX + vars->planeX * cameraX;
		vars->raydirY = vars->dirY + vars->planeY * cameraX;

		vars->mapX = (int)(vars->posX);
		vars->mapY = (int)(vars->posY);

		double sideDistX;
		double sideDistY;

		double deltaDistX = fabs(1 / vars->raydirX);
		double deltaDistY = fabs(1 / vars->raydirY);
		double perpWallDist;

		int stepX;
		int stepY;

		int hit = 0;
		int side;

		if (vars->raydirX < 0)
		{
		stepX = -1;
		sideDistX = (vars->posX - vars->mapX) * deltaDistX;
		}
		else
		{
		stepX = 1;
		sideDistX = (vars->mapX + 1.0 - vars->posX) * deltaDistX;
		}

		if (vars->raydirY < 0)
		{
		stepY = -1;
		sideDistY = (vars->posY - vars->mapY) * deltaDistY;
		}
		else
		{
		stepY = 1;
		sideDistY = (vars->mapY + 1.0 - vars->posY) * deltaDistY;
		}
		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				vars->mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				vars->mapY += stepY;
				side = 1;
			}
			if (worldMap[vars->mapX][vars->mapY] > 0)
				hit = 1;
		}
		if (side == 0)
			perpWallDist = (vars->mapX - vars->posX + (1 - stepX) / 2) / vars->raydirX;
		else
			perpWallDist = (vars->mapY - vars->posY + (1 - stepY) / 2) / vars->raydirY;

		int lineHeight = (int)(h / perpWallDist);

		int drawStart = lineHeight / - 2 + h / 2;
		if (drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + h / 2;
		if (drawEnd >= h)
			drawEnd = h - 1;

		vars->texNum = worldMap[vars->mapX][vars->mapY] - 1;

		double wallX;
		if (side == 0) 
			wallX = vars->posY + perpWallDist * vars->raydirY;
		else
			wallX = vars->posX + perpWallDist * vars->raydirX;
		wallX -= floor((wallX));

		vars->texX = (int)(wallX * (double)(texWidth));
		if(side == 0 && vars->raydirX > 0) vars->texX = texWidth - vars->texX - 1;
		if(side == 1 && vars->raydirY < 0) vars->texX = texWidth - vars->texX - 1;

		vars->step = 1.0 * texHeight / lineHeight;
		vars->texPos = (drawStart - h / 2 + lineHeight / 2) * vars->step;

		dotting(vars, x, drawStart, drawEnd, side);

		double floorXWall, floorYWall;
		if(side == 0 && vars->raydirX > 0)
		{
			floorXWall = vars->mapX;
			floorYWall = vars->mapY + wallX;
		}
		else if(side == 0 && vars->raydirX < 0)
		{
			floorXWall = vars->mapX + 1.0;
			floorYWall = vars->mapY + wallX;
		}
		else if(side == 1 && vars->raydirY > 0)
		{
			floorXWall = vars->mapX + wallX;
			floorYWall = vars->mapY;
		}
		else
		{
			floorXWall = vars->mapX + wallX;
			floorYWall = vars->mapY + 1.0;
		}

      double distWall, distPlayer, currentDist;

      distWall = perpWallDist;
      distPlayer = 0.0;

      if (drawEnd < 0) drawEnd = h; //becomes < 0 when the integer overflows

      //draw the floor from drawEnd to the bottom of the screen
      for(int y = drawEnd + 1; y < h; y++)
      {
        currentDist = h / (2.0 * y - h); //you could make a small lookup table for this instead

        double weight = (currentDist - distPlayer) / (distWall - distPlayer);

        double currentFloorX = weight * floorXWall + (1.0 - weight) * vars->posX;
        double currentFloorY = weight * floorYWall + (1.0 - weight) * vars->posY;

        int floorTexX, floorTexY;
        floorTexX = (int)(currentFloorX * texWidth) % texWidth;
        floorTexY = (int)(currentFloorY * texHeight) % texHeight;

        //floor
        vars->img.data[y * window_width + x] = (vars->texture[3][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
        //ceiling (symmetrical!)
        vars->img.data[(h - y - 1) * window_width + x] = (vars->texture[6][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
      }
		++x;
	}
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

int		key_pressed(int keycode, t_vars *vars)
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
}

int main(void)
{
	t_vars	*vars;
	int		i;

	vars = (t_vars*)malloc(sizeof(t_vars));
	vars->texture = (int **)malloc(sizeof(int *) * 8);
	i = 0;
	while (i < 8)
	{
		(vars->texture)[i] = (int *)malloc(sizeof(int) * texWidth * texHeight);
		++i;
	}
  for(int x = 0; x < texWidth; x++)
  {
	for(int y = 0; y < texHeight; y++)
	{
		int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
		//int xcolor = x * 256 / texWidth;
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
	}
  }

	vars->posX = 22, vars->posY = 12;  //x and y start position
	vars->dirX = -1, vars->dirY = 0; //initial direction vector
	vars->planeX = 0, vars->planeY = 0.66; //the 2d raycaster version of camera plane
  
 
	vars->mlx = mlx_init();
	vars->win = mlx_new_window(vars->mlx, window_width, window_height, "Cub3d");
	vars->img.img_ptr = mlx_new_image(vars->mlx, window_width, window_height);
 	vars->img.data = (int *)mlx_get_data_addr(vars->img.img_ptr, &vars->img.bpp, &vars->img.size_line, &vars->img.endian);

	render(vars);
	
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
	mlx_hook(vars->win, 2, 1L<<0, &key_pressed, vars);
	mlx_loop(vars->mlx);
	
	i = 0;
    while (i < 8)
	{
		free((vars->texture)[i]);
		++i;
	}
	free((vars->texture));
	free(vars);
  }