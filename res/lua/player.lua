require "input"

PLAYER_RIGHT = 4;
PLAYER_LEFT = 3;
PLAYER_DOWN = 0;
PLAYER_UP = 5;
PLAYER_RIGHT_DOWN = 2;
PLAYER_RIGHT_UP = 7;
PLAYER_LEFT_DOWN = 1;
PLAYER_LEFT_UP = 6;

MAP_PLOT_TILE = 7;

PLAYER_SELECTION_X = 0;
PLAYER_SELECTION_Y = 0;

player_current_energy = 100;
PLAYER_DEPLETE_AMOUNT = 5;

function get_direction()
    return player_get_direction();
end

function get_direction_string()
    value = "DOWN";
    direction = player_get_direction();
    --fuck
    
    if(direction == PLAYER_RIGHT) then
        value = "RIGHT";
    elseif(direction == PLAYER_LEFT) then
        value = "LEFT";
    elseif(direction == PLAYER_DOWN) then
        value = "DOWN";
    elseif(direction == PLAYER_UP) then
        value = "UP";
    elseif(direction == PLAYER_RIGHT_DOWN) then
        value = "RIGHT DOWN";
    elseif(direction == PLAYER_RIGHT_UP) then
        value = "RIGHT UP";
    elseif(direction == PLAYER_LEFT_DOWN) then
        value = "LEFT DOWN";
    elseif(direction == PLAYER_LEFT_UP) then
        value = "LEFT UP";
    end
    
    return value;
end

function check_input(tilemap)
  if(input_is_button_down(1 --[[square]]) == 1) then
    if(player_current_energy < 1) then
        return; --no energy
    end

    selected_tile = tilemap_get_tile(tilemap, PLAYER_SELECTION_X, PLAYER_SELECTION_Y);
    if(selected_tile ~= MAP_PLOT_TILE) then
        tilemap_set_tile(tilemap, PLAYER_SELECTION_X, PLAYER_SELECTION_Y, MAP_PLOT_TILE);
        player_current_energy = player_current_energy - PLAYER_DEPLETE_AMOUNT;
    end
  end
end

function player_fill_energy()
    if(player_current_energy > 100) then return end

    player_current_energy = 100;
end

function draw_selection()
    player_x = player_get_x();
    player_y = player_get_y();
    
    x0 = math.floor((player_x + 7) / 32) * 32;
    y0 = math.floor((player_y + 12) / 32) * 32;
    --x0 = x0;
    --y0 = y0;
    
    --[[
    if(direction == PLAYER_RIGHT) then
        x0 = x0 + 32;
    elseif(direction == PLAYER_LEFT) then
        x0 = x0 - 32;
    elseif(direction == PLAYER_DOWN) then
        y0 = y0 + 32;
    elseif(direction == PLAYER_UP) then
        y0 = y0 - 32;
    elseif(direction == PLAYER_RIGHT_DOWN) then
        x0 = x0 + 32;
        y0 = y0 + 32;
    elseif(direction == PLAYER_RIGHT_UP) then
        x0 = x0 + 32;
        y0 = y0 - 32;
    elseif(direction == PLAYER_LEFT_DOWN) then
        x0 = x0 - 32;
        y0 = y0 + 32;
    elseif(direction == PLAYER_LEFT_UP) then
        x0 = x0 - 32;
        y0 = y0 - 32;
    end
    ]]
    
    y1 = y0 + 32;
    x1 = x0 + 32;
    
    PLAYER_SELECTION_X = x0;
    PLAYER_SELECTION_Y = y0;
    
    if(x0 < 0 or y0 < 0) then
        return;
    else
        draw_rect_camera(x0, y0, x1, y1, RGBA(0, 0, 0, 255));
    end
end