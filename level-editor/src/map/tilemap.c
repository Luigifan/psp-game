#include "tilemap.h"

tilemap_t* tilemap_create(int width, int height)
{
  tilemap_t* tilemap = malloc(sizeof(tilemap_t));
  int total_tiles = width * height;
  tilemap->tiles = malloc(sizeof(tile_t) * total_tiles);
  tilemap->surrounding_tile_id = 1; //stone
  printf("allocated %d for map\n", sizeof(tile_t) * total_tiles);

  int x, y;
  for(x = 0; x < width; x++)
  {
    for(y = 0; y < height; y++)
    {
      tilemap->tiles[x * height + y].id = 0;
      tilemap->tiles[x * height + y].tile_type = TILE_TYPE_PASSABLE;
    }
  }

  tilemap->width = width;
  tilemap->height = height;

  tilemap->map_name = "Test Map";

  return tilemap;
}

void tilemap_destroy(tilemap_t* map)
{
  assert(map != NULL);
  assert(map->tiles != NULL);
  free(map->tiles);
  free(map);
}

void tilemap_update(tilemap_t* map, const camera_t cam)
{/*TODO*/}

void tilemap_draw(tilemap_t* map, const camera_t cam, sprite_t* tileset)
{
  /*
  Get bounds for drawing
  */
  int min_x, max_x, min_y, max_y, x_iter, y_iter;

  camera_get_index_bounds(cam, map, &min_x, &max_x, &min_y, &max_y);

  for(x_iter = zeroize(min_x); x_iter < min(max_x, map->width); x_iter++)
  {
    for(y_iter = zeroize(min_y); y_iter < min(max_y, map->height); y_iter++)
    {
      int index = x_iter * map->height + y_iter;

      if(index < 0 || index > (map->height * map->width))
      {
        continue;
      } //skip NULL tiles
      else
      {
        tile_t tile = map->tiles[index];
        vector_t sheet_location = tile_get_location_by_id(tile.id);
        sprite_draw_camera_source(tileset, cam, x_iter * 32, y_iter * 32, sheet_location.x, sheet_location.y, 32, 32);
      }
    }
  }
}

int tilemap_is_player_colliding(tilemap_t* map, player_t* player, const camera_t camera)
{
  rectangle_t player_hitbox = camera_player_to_world_rectangle(camera);
    player_hitbox.w = 32; player_hitbox.h = 32;

  rectangle_t theoretical_level_bounds;
  theoretical_level_bounds.x = 32;
  theoretical_level_bounds.y = 32;
  theoretical_level_bounds.w = (map->width - 1) * 32;
  theoretical_level_bounds.h = (map->height - 1) * 32;

  if(rectangle_intersects(&player_hitbox, &theoretical_level_bounds))
    return 0;
  else
    return 1;
}

void camera_get_index_bounds(const camera_t camera, tilemap_t* tilemap, int* min_x, int* max_x, int* min_y, int* max_y)
{
  int half_map_width, half_map_height;
  half_map_width = ((tilemap->width * 32) / 2);
  half_map_height = ((tilemap->height * 32) / 2);

  float corrected_x, corrected_y;
  corrected_x = -camera.x;
  corrected_y = -camera.y;

  (*min_x) = floor(
    (corrected_x) / 32
  );
  (*max_x) = ceil(
    (corrected_x + SCREEN_WIDTH) / 32
  );
  (*min_y) = floor(
    (corrected_y) / 32
  );
  (*max_y) = ceil(
    (corrected_y + SCREEN_HEIGHT) / 32
  );
}

int tilemap_write_to_file(const char* filename, tilemap_t* map)
{
  int total_tiles = map->width * map->height;
  int filesize = (sizeof(char) * 2) + //header (MS)
                  (sizeof(short)) + //VERSION
                  strlen(map->map_name) + //map name
                  (sizeof(int) * 2) + // two ints for width and height
                  (sizeof(short) * total_tiles); // the tiles in this level

  char* buffer = malloc(sizeof(char) * filesize);
  int pointer = 0, i;
  serializer_write_char(buffer, &pointer, HEADER_0);
  serializer_write_char(buffer, &pointer, HEADER_1);

  serializer_write_short(buffer, &pointer, (short)VERSION);

  serializer_write_string(buffer, &pointer, map->map_name);

  serializer_write_int(buffer, &pointer, map->width);
  serializer_write_int(buffer, &pointer, map->height);

  for(i = 0; i < total_tiles; i++)
  {
    serializer_write_short(buffer, &pointer, map->tiles[i].id);
  }

  return serializer_write_to_file(buffer, filesize, filename);
}

int tilemap_verify_header(char* buffer, short version)
{
  char filler[30];

  if(buffer[0] == HEADER_0 && buffer[1] == HEADER_1 && version == VERSION)
    return 1;
  else
  {
    if(buffer[0] != HEADER_0 || buffer[1] != HEADER_1)
    {
      sprintf(filler, "Header mismatch in level file. (got %c%c; expected %c%c)",
        buffer[0], buffer[1],
        HEADER_0, HEADER_1
      );
      #ifdef PSP
      oslFatalError(filler);
      #else
      gFatalError(filler);
      #endif
    }
    else if(version != VERSION)
    {
      sprintf(filler, "Version mismatch in level file. (got %d; expected %d)", version, VERSION);
      #ifdef PSP
      oslFatalError(filler);
      #else
      gFatalError(filler);
      #endif
    }
  }
  return 0;
}

tilemap_t* tilemap_read_from_file(const char* filename)
{
  int file_size = serializer_get_file_size(filename);
  if(file_size > 0)
  {
    char* buffer = malloc(sizeof(char) * file_size);
    int pointer = 0, i;

    serializer_read_from_file(buffer, file_size, filename);

    char HEADER[2] = {0};
    HEADER[0] = serializer_read_char(buffer, &pointer);
    HEADER[1] = serializer_read_char(buffer, &pointer);
    short version = serializer_read_short(buffer, &pointer);

    if(tilemap_verify_header(HEADER, version))
    {
      char* map_name = serializer_read_string(buffer, &pointer);
      int width, height;
      width = serializer_read_int(buffer, &pointer);
      height = serializer_read_int(buffer, &pointer);

      tilemap_t* return_value = tilemap_create(width, height);
      return_value->map_name = map_name;
      int total_tiles = width * height;
      int i = 0;
      for(i = 0; i < total_tiles; i++)
      {
        //read shorts for tiles
        short id = serializer_read_short(buffer, &pointer);
        if(id >= 0)
          return_value->tiles[i].id = id;
      }

      free(buffer);

      return return_value;
    }
    else
      return NULL;
  }
  else
    return NULL; //fnf

  return NULL;
}