#include "globals.h"
#include "castle_game.h"
#include "gui/gui_button.h"
#include "castle_gui_game.h"
#include "castle_trebuchet.h"
#include "castle_rocks.h"

castle_gui_game::castle_gui_game(castle_game* g) :
x(20),y(30), save_time(0), save_state(0)
{
    cgame = g;

    loopi(GB_MAX)
    {
        btns_game[i] = new gui_button(cgame,x + 20 + (i * 50 ),y+15,NULL,0);
        btns_game[i]->tx = cgame->tx;
        btns_game[i]->set_images("imgs/gui/rock_big.png","imgs/gui/rock_big.png",0,0);
        btns_game[i]->set_text("");
    }

    btns_game[GB_ROCK_BIG]->set_images("imgs/gui/rock_big.png","imgs/gui/rock_big.png",0,0);
    btns_game[GB_ROCK_SMALL]->set_images("imgs/gui/rock_small.png","imgs/gui/rock_small.png",0,0);
    btns_game[GB_ROCK_BOMB]->set_images("imgs/gui/rock_bomb.png","imgs/gui/rock_bomb.png",0,0);
    btns_game[GB_SAVE]->set_images("imgs/gui/gui_save.png","imgs/gui/gui_save.png",0,0);

};

castle_gui_game::~castle_gui_game()
{
    loopi(GB_MAX)
    {
        delete btns_game[i];
        btns_game[i] = 0;
    }
};


int castle_gui_game::hit_test(int s)
{
    if(lasttype==event.type && lastbut==event.button.button) return 0; // why?? get event twice without it
    lasttype = event.type;
    lastbut = event.button.button;

    if(btns_game[GB_ROCK_BIG]->hit_test(&event,0)==B_CLICK)
    {
        cgame->treb->load_rocks(PROJECT_BIG_ROCK);
        cgame->snd->play_sound("sound/rockload.wav");
        return 1;
    }

    if(btns_game[GB_ROCK_SMALL]->hit_test(&event,0)==B_CLICK)
    {
        cgame->treb->load_rocks(PROJECT_SMALL_ROCK);
        cgame->snd->play_sound("sound/rockload.wav");
        return 1;
    }

    if(btns_game[GB_ROCK_BOMB]->hit_test(&event,0)==B_CLICK)
    {
        cgame->treb->load_rocks(PROJECT_BOMB);
        cgame->snd->play_sound("sound/rockload.wav");
        return 1;
    }

    if(cgame->status == GS_PLAYING && btns_game[GB_SAVE]->hit_test(&event,0)==B_CLICK)
    {
        save_time = SDL_GetTicks();
        save_state = cgame->create_save();
        return 1;
    }

    return 0; // nothing
}

void castle_gui_game::render()
{
    // buttons
    loopi(GB_MAX)
    {
        if (cgame->status != GS_PLAYING && GB_SAVE == i) continue;

        btns_game[i]->obj_state = B_OUT; //reset
        btns_game[i]->hit_test(&event,0);
        btns_game[i]->draw();
    }

    if (save_state)
    {
        if ((SDL_GetTicks() - save_time) < 5000)
        {
            cgame->fnts->change_color(117,33,13);

            if (save_state == 1)
                cgame->fnts->text(cgame->screen,"Game Saved",235,150,400);
            else
                cgame->fnts->text(cgame->screen,"Error Saving Game",235,150,400);

        }
        else
        {
            save_state = 0;
        }
    }


};


