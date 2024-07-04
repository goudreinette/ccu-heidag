#pragma once

#include "bn_fixed.h"
#include "bn_fixed_point.h"
#include "bn_log.h"
#include "bn_camera_ptr.h"
#include "bn_display.h"
#include "bn_math.h"
#include "bn_regular_bg_map_item.h"
#include "bn_regular_bg_map_cell_info.h"
#include "bn_sprite_text_generator.h"

#include "../../common/include/common_info.h"
#include "../../common/include/common_variable_8x16_sprite_font.h"


/**
 * Math
 */
bn::fixed lerp(bn::fixed a, bn::fixed b, bn::fixed t)
{
    return a + t * (b - a);
}

int mod(int a, int b) 
{
    return ((a % b) + b) % b;
}


bn::fixed distance(bn::fixed_point a, bn::fixed_point b) 
{
    return abs(a.x() - b.x()) + abs(a.y() - b.y());
}



/**
 * Camera
 */
bool camera_moving_to_point = false;
int cam_follow_margin = 3;

void camera_follow_smooth(bn::camera_ptr cam, bn::fixed_point position) 
{
    if (bn::abs(cam.x() - position.x()) > bn::display::width() / cam_follow_margin || 
        bn::abs(cam.y() - position.y()) > bn::display::height() / cam_follow_margin) {
        camera_moving_to_point = true;
    }

    if (camera_moving_to_point) {
        cam.set_x(lerp(position.x(), cam.x(), 0.95));
        cam.set_y(lerp(position.y(), cam.y(), 0.95));

        if (bn::abs(cam.x() - position.x()) < 1  &&  bn::abs(cam.y() - position.y()) < 1) {
            camera_moving_to_point = false;
        }
    }
}


/**
 * Map utils
 * Map (0,0) is top-left but sprite (0,0) is center-center
 */
const bn::point get_map_point_at_position(bn::fixed_point pos, bn::regular_bg_map_item map_item) 
{
    bn::size dimensions = map_item.dimensions();
    int map_x = mod(((pos.x() + bn::display::width() / 2 + 16) / 8).floor_integer(), dimensions.width());
    int map_y = mod(((pos.y() + bn::display::height() / 2 + 30 + 96) / 8).floor_integer(), dimensions.height());
    return bn::point(map_x, map_y);
}


int get_map_tile_index_at_position(bn::fixed_point pos, bn::regular_bg_map_item map_item) 
{
    const bn::point p = get_map_point_at_position(pos, map_item);
    bn::regular_bg_map_cell map_cell = map_item.cell(p);
    int tile_index = bn::regular_bg_map_cell_info(map_cell).tile_index();

    // BN_LOG(bn::format<60>("map tile: ({}, {}): {}", p.x(), p.y(), tile_index));
    // BN_LOG(bn::format<60>("player pos: {}, {}", pos.x(), pos.y()));

    return  tile_index;
}


class MapInfoPrinter 
{
    bn::optional<bn::sprite_text_generator> text_generator;
    bn::vector<bn::sprite_ptr, 64> info_text_sprites;

    public: 
    
    MapInfoPrinter(bn::sprite_font font) {
        text_generator = bn::sprite_text_generator(font);
        text_generator->set_center_alignment();
    }

    void print_map_tiles_at_position(bn::regular_bg_map_item map_item, bn::fixed_point position) {
        int tile_left = get_map_tile_index_at_position(position + bn::fixed_point(-8,0), map_item);
        int tile_center = get_map_tile_index_at_position(position, map_item);
        int tile_right = get_map_tile_index_at_position(position + bn::fixed_point(8,0), map_item);

        bn::string<200> info_text =  bn::format<60>("{} | {} | {}", tile_left, tile_center, tile_right);
        info_text_sprites.clear();
        text_generator->generate(0, 65, info_text, info_text_sprites);
    }
};
