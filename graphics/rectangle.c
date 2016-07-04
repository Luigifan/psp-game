#include "rectangle.h"

void rectangle_draw(const rectangle_t* rect, uint32_t color)
{
    #if PSP
    oslDrawRect(
        rect->x,
        rect->y,
        rect->x + rect->w,
        rect->y + rect->h,
        color
    );
    #endif
}

void rectangle_draw_filled(const rectangle_t* rect, uint32_t color)
{
    #if PSP
    oslDrawFillRect(
        rect->x,
        rect->y,
        rect->x + rect->w,
        rect->y + rect->h,
        color
    );
    #endif
}

void rectangle_draw_camera(const rectangle_t* rect, uint32_t color, const camera_t* camera)
{
    #if PSP
    oslDrawRect(
        rect->x + camera->x,
        rect->y + camera->y,
        rect->x + rect->w + camera->x,
        rect->y + rect->h + camera->y,
        color
    );
    #endif
}

void rectangle_draw_filled_camera(const rectangle_t* rect, uint32_t color, const camera_t* camera)
{
    #if PSP
    oslDrawFillRect(
        rect->x + camera->x,
        rect->y + camera->y,
        rect->x + rect->w + camera->x,
        rect->y + rect->h + camera->y,
        color
    );
    #endif
}

void rectangle_update_bounds(rectangle_t* a)
{
  a->left = a->x;
  a->right = a->x + a->w;

  a->top = a->y;
  a->bottom = a->y + a->h;
}

int rectangle_intersects(rectangle_t* a, rectangle_t* b)
{
  rectangle_update_bounds(a);
  rectangle_update_bounds(b);

  if(
    ((a->left < b->right) & (a->right > b->left))
    &&
    ((a->top < b->bottom) & (a->bottom > b->top))
  )
    return 1;

  return 0;
}

rectangle_t camera_player_to_world_rectangle(const camera_t* camera)
{
  //they can set w/h later tbh
  rectangle_t value;
  value.x = -camera->x + ORIGIN_X - 16;
  value.y = -camera->y + ORIGIN_Y - 16;

  return value;
}

rectangle_t camera_get_viewport(const camera_t* camera)
{
  rectangle_t value;
  value.w = SCREEN_WIDTH;
  value.h = SCREEN_HEIGHT;
  value.x = -camera->x;
  value.y = -camera->y;

  return value;
}