int	ft_wall_height(t_ray *ray, int win_height)
{
    int	wall_height;

    wall_height = (int)(win_height / ray->perp_wall_dist);
    return (wall_height);
}

void	ft_texture_coord(t_ray *ray, t_cube *cube)
{
    int	tex_x;
    int	tex_y;

    if (ray->side == 0)
        ray->wall_x = cube->map->posy + ray->perp_wall_dist * ray->dir_y;
    else
        ray->wall_x = cube->map->posx + ray->perp_wall_dist * ray->dir_x;
    ray->wall_x -= floor(ray->wall_x);
    tex_x = (int)(ray->wall_x * (double)(cube->img->size_line));
    if (ray->side == 0 && ray->dir_x > 0)
        tex_x = cube->img->size_line - tex_x - 1;
    if (ray->side == 1 && ray->dir_y < 0)
        tex_x = cube->img->size_line - tex_x - 1;
    ray->step = 1.0 * cube->img->size_line / \
    ft_wall_height(ray, cube->win_height);
    ray->tex_pos = (ray->draw_start - cube->win_height / 2 + \
    ft_wall_height(ray, cube->win_height) / 2) * ray->step;
    tex_y = (int)ray->tex_pos & (cube->img->size_line - 1);
    while (ray->draw_start < ray->draw_end)
    {
        ray->color = cube->img->data[cube->img->size_line * tex_y + tex_x];
        ft_render(ray, cube, ray->draw_start);
        ray->draw_start++;
        ray->tex_pos += ray->step;
        tex_y = (int)ray->tex_pos & (cube->img->size_line - 1);
    }
}

void	ft_render(t_ray *ray, t_cube *cube, int y)
{
    char	*dst;

    dst = cube->img->data + (y * cube->img->size_line + ray->x * \
    (cube->img->bitsxpixel / 8));
    *(unsigned int*)dst = ray->color;
}

void	ft_draw_wall(t_ray *ray, t_cube *cube)
{
    int	wall_height;

    wall_height = ft_wall_height(ray, cube->win_height);
    ray->draw_start = -wall_height / 2 + cube->win_height / 2;
    if (ray->draw_start < 0)
        ray->draw_start = 0;
    ray->draw_end = wall_height / 2 + cube->win_height / 2;
    if (ray->draw_end >= cube->win_height)
        ray->draw_end = cube->win_height - 1;
    ft_texture_coord(ray, cube);
}