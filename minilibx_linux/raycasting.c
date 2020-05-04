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
#include "stdio.h"


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
	double olddirX;
	double dirX;
	double olddirY;
	double dirY;
	int 	rotSpeed;
	double oldplaneX;
	double planeX;
	double planeY;
	t_data	img;
}               t_vars;


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
	vars.img.data[y * 800 + x] = color;
	++y;
  }
}

int             move_keys_window(int keycode, t_vars vars)
{
	printf("key code: %d\n", keycode);
	if (keycode == 65307) // OS에서는 ESC == 53
  		mlx_destroy_window(vars.mlx, vars.win);
	if (keycode == 65362)
	{
		if(worldMap[(int)(vars.posX + vars.dirX)][(int)(vars.posY)] == 0) vars.posX += vars.dirX;
		if(worldMap[(int)(vars.posX)][(int)(vars.posY + vars.dirY)] == 0) vars.posY += vars.dirY;
	}
	//move backwards if no wall behind you
	if (keycode == 65364)
	{
		if(worldMap[(int)(vars.posX - vars.dirX)][(int)(vars.posY)] == 0) vars.posX -= vars.dirX;
		if(worldMap[(int)(vars.posX)][(int)(vars.posY - vars.dirY)] == 0) vars.posY -= vars.dirY;
	}
	//rotate to the right
	if (keycode == 65363)
	{
		//both camera direction and camera plane must be rotated
		vars.olddirX = vars.dirX;
		vars.dirX = vars.dirX * cos(-vars.rotSpeed) - vars.dirY * sin(-vars.rotSpeed);
		vars.dirY = vars.olddirX * sin(-vars.rotSpeed) + vars.dirY * cos(-vars.rotSpeed);
		vars.oldplaneX = vars.planeX;
		vars.planeX = vars.planeX * cos(-vars.rotSpeed) - vars.planeY * sin(-vars.rotSpeed);
		vars.planeY = vars.oldplaneX * sin(-vars.rotSpeed) + vars.planeY * cos(-vars.rotSpeed);
	}
	//rotate to the left
	if (keycode == 65361)
	{
		//both camera direction and camera plane must be rotated
		vars.olddirX = vars.dirX;
		vars.dirX = vars.dirX * cos(vars.rotSpeed) - vars.dirY * sin(vars.rotSpeed);
		vars.dirY = vars.olddirX * sin(vars.rotSpeed) + vars.dirY * cos(vars.rotSpeed);
		vars.oldplaneX = vars.planeX;
		vars.planeX = vars.planeX * cos(vars.rotSpeed) - vars.planeY * sin(vars.rotSpeed);
		vars.planeY = vars.oldplaneX * sin(vars.rotSpeed) + vars.planeY * cos(vars.rotSpeed);
	}
	vars.rotSpeed = 10;
    //move forward if no wall in front of you
	mlx_put_image_to_window(vars.mlx, vars.win, vars.img.img_ptr, 0, 0);
}


int main()
{
  t_vars    vars;

  vars.posX = 22, vars.posY = 12;  //x and y start position
  vars.dirX = -1, vars.dirY = 0; //initial direction vector
  vars.planeX = 0, vars.planeY = 0.66; //the 2d raycaster version of camera plane
  
 
	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx, 800, 600, "Hello world!");

	vars.img.img_ptr = mlx_new_image(vars.mlx, 800, 600);
 	vars.img.data = (int *)mlx_get_data_addr(vars.img.img_ptr, &vars.img.bpp, &vars.img.size_line, &vars.img.endian);

	int x;
	int w = 800;
  	int h = 600;

	x = 0;
	while (x < w)
    {
      //calculate ray position and direction
      double cameraX = 2 * x / (double)w - 1; //x-coordinate in camera space
      double raydirX = vars.dirX + vars.planeX * cameraX;
      double raydirY = vars.dirY + vars.planeY * cameraX;
      //which box of the map we're in
      int mapX = (int)(vars.posX);
      int mapY = (int)(vars.posY);

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
        sideDistX = (vars.posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - vars.posX) * deltaDistX;
      }

      if (raydirY < 0)
      {
        stepY = -1;
        sideDistY = (vars.posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - vars.posY) * deltaDistY;
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
	  	perpWallDist = (mapX - vars.posX + (1 - stepX) / 2) / raydirX;
      else
	  	perpWallDist = (mapY - vars.posY + (1 - stepY) / 2) / raydirY;

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
	  switch(worldMap[mapX][mapY])
      {
        case 1:  color = 0x00FF0000;  break; //red
        case 2:  color = 0x0000FF00;  break; //green
        case 3:  color = 0x000000FF;  break; //blue
        case 4:  color = 0x0000FFFF;  break; //white
        default: color = 0x00FF00FF;  break; //yellow
      }

      //give x and y sides different brightness
      if (side == 1)
	  	color = color / 2;

      //draw the pixels of the stripe as a vertical line
      verLine(vars, x, drawStart, drawEnd, color);
	  ++x;
    }
	mlx_put_image_to_window(vars.mlx, vars.win, vars.img.img_ptr, 0, 0);
	mlx_hook(vars.win, 2, (1L<<0), move_keys_window, &vars);
    mlx_loop(vars.mlx);

  	return (0);
}