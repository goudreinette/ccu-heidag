#pragma once

#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"
#include "bn_format.h"
#include "bn_log.h"
#include "bn_sprites_mosaic.h"
#include "bn_sprite_actions.h"


#include "bn_sprite_items_lipje.h"
#include "bn_sprite_items_c.h"
#include "bn_sprite_items_c2.h"
#include "bn_sprite_items_u.h"
#include "bn_music_items.h"


#include "globals.h"
#include "scene.h"



namespace splash 
{
    using namespace bn;


    next_scene run() 
    {
        // CCU logo
        const int duration_per_letter = 45;
        sprite_ptr c1 = sprite_items::c.create_sprite(-64,-128);
        sprite_ptr c2 = sprite_items::c2.create_sprite(0,-128);
        sprite_ptr u = sprite_items::u.create_sprite(64,-128);

        // Enable mosaic
        c1.set_mosaic_enabled(true);
        c2.set_mosaic_enabled(true);
        u.set_mosaic_enabled(true);

        sprite_move_to_action c1_action(c1, duration_per_letter, -64, 0);
        sprite_move_to_action c2_action(c2, duration_per_letter, 0, 0);
        sprite_move_to_action u_action(u, duration_per_letter, 64, 0);

        music_items::splashscreen.play();

        bool pixelating = false;
        fixed pixelation = 0;

        while(true) 
        {
            if (!c1_action.done()) {
                c1_action.update();
            }
            if (c1_action.done() && !c2_action.done()) {
                c2_action.update();
            }
            if (c1_action.done() && c2_action.done() && !u_action.done()) {
                u_action.update();
            }

            // Everything is done
            if (u_action.done()) {
                pixelating = true;
            }

            if (pixelating) {
                sprites_mosaic::set_stretch(pixelation, pixelation);
                pixelation += 0.05;

                if (pixelation >= 1) {
                    music::stop();
                    return next_scene::main_menu;
                }
            }

            core::update(); 
        }
    }
}








