/*
Copyright (c) 2004-2007, Lode Vandevenne

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "mlx.h"
#include "mlx_int.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define mapWidth 24
#define mapHeight 24

#define window_width 800
#define window_height 600

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
	double olddirX;
	double dirX;
	double olddirY;
	double dirY;
	double rot_speed;
	double moving_speed;
	double oldplaneX;
	double planeX;
	double planeY;
	t_data	img;
	int		key;
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

void render(t_vars *vars)
{
  int x;
  int w = window_width;
  int h = window_height;

  x = 0;
  while (x < w)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / (double)w - 1; //x-coordinate in camera space
      double raydirX = vars->dirX + vars->planeX * cameraX;
      double raydirY = vars->dirY + vars->planeY * cameraX;
      //which box of the map we're in
      int mapX = (int)(vars->posX);
      int mapY = (int)(vars->posY);

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

        //length of ray from one x or y-side to next x or y-side
      double deltaDistX = fabs(1 / raydirX);
      double deltaDistY = fabs(1 / raydirY);
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?
      //calculate step and initial sideDist
      if (raydirX < 0)
      {
        stepX = -1;
        sideDistX = (vars->posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - vars->posX) * deltaDistX;
      }

      if (raydirY < 0)
      {
        stepY = -1;
        sideDistY = (vars->posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - vars->posY) * deltaDistY;
      }
      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, OR in x-direction, OR in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0)
          hit = 1;
      }
      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
      if (side == 0)
      	perpWallDist = (mapX - vars->posX + (1 - stepX) / 2) / raydirX;
      else
      	perpWallDist = (mapY - vars->posY + (1 - stepY) / 2) / raydirY;

      //Calculate height of line to draw on screen
      int lineHeight = (int)(h / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = lineHeight / -2 + h / 2;
      if (drawStart < 0)
      	drawStart = 0;
      int drawEnd = lineHeight / 2 + h / 2;
      if (drawEnd >= h)
      	drawEnd = h - 1;

      //choose wall color
      	int color;
    	// switch(worldMap[mapX][mapY])
		// {
		// case 1:  color = 0x00FF0000;  break; //red
		// case 2:  color = 0x0000FF00;  break; //green
		// case 3:  color = 0x000000FF;  break; //blue
		// case 4:  color = 0x00FFFFFF;  break; //white
		// default: color = 0x00FF00FF;  break; //yellow
		// }

      //give x and y sides different brightness
    	if (side == 1 && raydirY >= 0)
      		color = 0x00FF0000;
		else if (side == 1 && raydirY < 0)
			color = 0x0000FF00;
		else if (side == 0 && raydirX >= 0)
			color = 0x000000FF;
		else
			color = 0x009932CC;

      //draw the pixels of the stripe as a vertical line
    	verLine(*vars, x, drawStart, drawEnd, color);
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
	ft_bzero(vars->img.data, sizeof(int) * 800 * 600);
	render(vars);
	mlx_put_image_to_window(vars->mlx, vars->win, vars->img.img_ptr, 0, 0);
}

int main()
{
	t_vars    *vars;

	vars = (t_vars*)malloc(sizeof(t_vars));

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

  	return (0);
}