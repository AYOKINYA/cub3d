#include "mlx.h"
#include "mlx_int.h"
#include "stdio.h"

typedef struct  s_vars {
    void        *mlx;
    void        *win;
	int			x;
	int			y;
	int			color;
	char		*string;
}               t_vars;

int             close_window(int keycode, t_vars *vars)
{
    printf("key code: %d\n", keycode);
    if (keycode == 65307) // OS에서는 ESC == 53
        mlx_destroy_window(vars->mlx, vars->win);

}

int             mouse_position_window(int loc_x, int loc_y, t_vars *vars)
{
    printf("x: %d\n", loc_x);
    printf("y: %d\n", loc_y);
}

int             write_window(int keycode, t_vars *vars)
{
    vars->x = 100;
	vars->y = 100;
	vars->color = 0xFFFF00;
	vars->string = "HELLO EVERYONE";
    printf("key code: %d\n", keycode);
	mlx_string_put(vars->mlx, vars->win, vars->x, vars->y, vars->color, vars->string);
}



// int             main(void)
// {
//     t_vars      vars;

//     vars.mlx = mlx_init();
//     vars.win = mlx_new_window(vars.mlx, 1920, 1080, "Hello world!");
//     mlx_key_hook(vars.win, close_window, &vars);
	
//     mlx_loop(vars.mlx);
// } 


int             main(void)
{
    t_vars    vars;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 1920, 1080, "Hello world!");
    mlx_hook(vars.win, 6, (1L<<6), mouse_position_window, &vars);
    mlx_loop(vars.mlx);
}