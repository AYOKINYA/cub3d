#include "mlx.h"
#include <math.h>
#include <stdio.h>

int      create_trgb(int t, int r, int g, int b)
{
   return(t << 24 |  r << 16 | g << 8 | b);
}

// typedef struct  s_data {
//     void        *img;
//     char        *data;
//     int         bpp;
//     int         size_line;
//     int         endian;
// }               t_data;

typedef struct   s_img

{

   void      *img_ptr;

   int         *data; //Here you got an int * even though mlx_get_data_addr returns

                  //a char *, i'll talk more about this in the .c file.

//Here are the 3 "useless" variables. You can find more informations about these in the man page.

   int         size_line;

   int         bpp;

   int         endian;

}            t_img;



/*

 Here is my main struct containing every variables needed by the MLX.

 - mlx_ptr stores the return value of mlx_init

 - win stores the return value of mlx_new_window

 - img will store everything we need for the image part, the struct is described above.

 */

typedef struct   s_mlx

{

   void      *mlx_ptr;

   void      *win;

   t_img      img;

}            t_mlx;


// void            my_mlx_pixel_put(t_img *data, int x, int y, int color)
// {
//     char    *dst;
//    
//    dst = data->data + (y * data->size_line + x * (data->bpp / 8));
//     *(unsigned int*)dst = color;
// }

int             main(void)
{
    t_mlx mlx;
   int    i;
   int      j;
   int      degree;
   double      radian;
   int    x;
   int      y;
   int      length;
   int color;

    mlx.mlx_ptr = mlx_init();
    mlx.win = mlx_new_window(mlx.mlx_ptr, 800, 600, "Hello world!");

    mlx.img.img_ptr = mlx_new_image(mlx.mlx_ptr, 800, 600);
   mlx.img.data = (int *)mlx_get_data_addr(mlx.img.img_ptr, &mlx.img.bpp, &mlx.img.size_line, &mlx.img.endian);
   /*
   squarre
   my_mlx_pixel_put(&img, 5, 5, 0x00FF0000);
   
   i = 0;
   while (i < 200)
   {
      j = 0;
      while (j < 200)
      {
         my_mlx_pixel_put(&img, i + 100, j + 100, 0x00FF0000);
         ++j;
      }
      ++i;
   }
   */
    
   //circle with gradation (looks like 3d)
   
   i = 1;
   while (i < 30)
   {
      degree = 0;
      while (degree < 360)
      {
         radian = (degree) * 3.141592 / 180;
         x = i * cos(radian);
         y = i * sin(radian);
         /*
         mlx_pixel_put(mlx, mlx_win, x + 100, y + 100, create_trgb(0, 255 - 255 / 30 * i, 0, 255 / 30 * i));
         my_mlx_pixel_put(&img, x + 100, y + 100, create_trgb(0, 255 - 255 / 30 * i, 0, 255 / 30 * i));
         my_mlx_pixel_put(&img, x + 200, y + 200, create_trgb(0, 150 - 150 / 30 * i, 108 - 108 / 30 * i, 0));
         my_mlx_pixel_put(&img, x + 140, y + 200, create_trgb(0, 111 - 111 / 30 * i, 79 - 79 / 30 * i, 40 - 40 / 30 * i));
         my_mlx_pixel_put(&img, x + 260, y + 200, create_trgb(0, 111 - 111 / 30 * i, 79 - 79 / 30 * i, 40 - 40 / 30 * i));
         */
         mlx.img.data[(y + 100) * 800 + (x + 100)] = create_trgb(0, 255 - 255 / 30 * i, 0, 255 / 30 * i);
         ++degree;
      }
      ++i;
   }

   
   // sin 정방향
   
   // i = 0;
   // while(i < 30)
   // {
   //    degree = 0;
   //    while(degree < 360)
   //    {
   //       radian = degree * 3.141592 / 180;
   //       x = degree;
   //       y = -1 * i * cos(radian);
   //       mlx_pixel_put(mlx, mlx_win, x + 100, y + 100, 0x00FF0000);
   //       degree++;
   //    }
   //    i++;
   // }
   

   /*
   i = 1;
   while (i < 50)
   {
      degree = 0;
      while (degree < 360)
      {
         radian = degree * 3.141592 / 180;
         x = i * radian;
         y = i * sinh(radian);
         mlx_pixel_put(mlx, mlx_win, x + 100, y + 100, 0xFFFF00);
         ++degree;
      }
      ++i;
      
   }
   */

   //good girl bad girl triangle
/*
   length = 0;
   while (length < 100)
   {
      i = 0;
      while (i <= length)
      {
         radian = 45 * 3.141592 / 180;
         x = i;
         y = x * tan(radian);
         mlx_pixel_put(mlx, mlx_win, length * 2 - x + 200, 200, 0xFF00FF);
         mlx_pixel_put(mlx, mlx_win, length * 2 - x + 200, -1 * y + 200, 0xFF00FF);
         ++i;
      }
      ++length;
   }
*/
   /*
   i = 0;
   while(i < 100)
   {
      j = 0;
      while(j <= i)
      {
         mlx_pixel_put(mlx, mlx_win, -1 * i + 100, -1 * j + 100, 0xFFFFFF);
         j++;
      }
      i++;
   }
*/
   //heart
   
   // int t;
   // t = 0;
   // i = 0;
   // j = 0;
   // while (j < 100)
   // {
   //    i = 0;
   //    while (i < 1000)
   //    {
   //       t += 2 * 3.141592 / 1000 * i;
   //       x = 16 * sin(t) * sin(t) * sin(t) * 3 * 0.01 * j;
   //       y = (13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)) * 3 * 0.01 * j;
   //       my_mlx_pixel_put(&img, x + 200, -y + 200, 0xFF0000);
   //       mlx_pixel_put(mlx, mlx_win, x + 200, -y + 200, 0xFF0000);
   //       i++;
   //    }
   //    ++j;
   // }
   //monster
   /*
   i = 0; //red
   while(i < 30)
   {
      degree = 180;
      while(degree < 360)
      {
         radian = degree * 3.141592 / 180;
         x = i * cos(radian);
         y = i * sin(radian);
         mlx_pixel_put(mlx, mlx_win, x + 350, y + 210, 0x80FF00);
         degree++;
      }
      i++;
   }
   i = 0;
   while(i < 57)
   {
      j = 0;
      while(j < 20)
      {
         mlx_pixel_put(mlx, mlx_win, i + 322, j + 210, 0x80FF00);
         j++;
      }
      i++;
   }
   //legs
   i = 0;
   while(i < 30)
   {
      j = 0;
      while(j <= i)
      {
         mlx_pixel_put(mlx, mlx_win, i + 322, j + 215, 0x80FF00);
         j++;
      }
      i++;
   }
   i = 0;
   while(i < 30)
   {
      j = 0;
      while(j < 29 - i)
      {
         mlx_pixel_put(mlx, mlx_win, i + 322, j + 215, 0x80FF00);
         j++;
      }
      i++;
   }
   i = 0;
   while(i < 30)
   {
      j = 0;
      while(j <= i)
      {
         mlx_pixel_put(mlx, mlx_win, i + 350, j + 215, 0x80FF00);
         j++;
      }
      i++;
   }
   i = 0;
   while(i < 30)
   {
      j = 0;
      while(j < 29 - i)
      {
         mlx_pixel_put(mlx, mlx_win, i + 350, j + 215, 0x80FF00);
         j++;
      }
      i++;
   }
   //eyes
   i = 0; //white
   while(i < 10)
   {
      degree = 0;
      while(degree < 360)
      {
         radian = degree * 3.141592 / 180;
         x = i * cos(radian);
         y = i * sin(radian);
         mlx_pixel_put(mlx, mlx_win, x + 335, y + 200, 0xFFFFFF);
         degree++;
      }
      i++;
   }
   i = 0; //blue
   while(i < 6)
   {
      degree = 0;
      while(degree < 360)
      {
         radian = degree * 3.141592 / 180;
         x = i * cos(radian);
         y = i * sin(radian);
         mlx_pixel_put(mlx, mlx_win, x + 330, y + 200, 0x0000FF);
         degree++;
      }
      i++;
   }
      i = 0; //white
   while(i < 10)
   {
      degree = 0;
      while(degree < 360)
      {
         radian = degree * 3.141592 / 180;
         x = i * cos(radian);
         y = i * sin(radian);
         mlx_pixel_put(mlx, mlx_win, x + 360, y + 200, 0xFFFFFF);
         degree++;
      }
      i++;
   }
   i = 0; //blue
   while(i < 6)
   {
      degree = 0;
      while(degree < 360)
      {
         radian = degree * 3.141592 / 180;
         x = i * cos(radian);
         y = i * sin(radian);
         mlx_pixel_put(mlx, mlx_win, x + 355, y + 200, 0x0000FF);
         degree++;
      }
      i++;
   }
*/
   mlx_put_image_to_window(mlx.mlx_ptr, mlx.win, mlx.img.img_ptr, 0, 0);
    mlx_loop(mlx.mlx_ptr);
   return (0);
}