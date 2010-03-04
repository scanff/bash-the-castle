#include "globals.h"
#include "castle_game.h"
#include "gui/gui_button.h"
#include "castle_gui_options.h"


char* options_text[] = {    "Play Sounds :",
                            "Play Music  :",
                            "Volume :",
                            0
                    };


castle_gui_options::castle_gui_options(castle_game* g)
{
    cgame = g;
    btns_close = 0;

    guibg = cgame->tx->texture_lookup("imgs/gui/title.png");
    if (!guibg) exit(0);


    btns_close = new gui_button(cgame,275,340,NULL,0);
    btns_close->tx = cgame->tx;
    btns_close->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_close->set_text("Close");
    btns_close->font_sz = 1;
    btns_close->pad_y = 8;
    btns_close->pad_x = 10;
    btns_close->text_color = 0xDDB90F;
    btns_close->text_color_over = 0xffffff;

    // OPTION BUTTONS
    loopi(OP_MAX)
    {
        btns_options[i] = 0;

        btns_options[i] = new gui_button(cgame,300,110 + (i*60),NULL,0);
        btns_options[i]->tx = cgame->tx;
        btns_options[i]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
        btns_options[i]->font_sz = 1;
        btns_options[i]->pad_y = 8;
        btns_options[i]->pad_x = 10;
        btns_options[i]->text_color = 0xDDB90F;
        btns_options[i]->text_color_over = 0xffffff;

    }

    // override loop
    btns_options[OP_VOLUP]->set_images("imgs/kb/keyboard_key.png","imgs/kb/keyboard_key_over.png",0,0);
    btns_options[OP_VOLUP]->set_text("+");
    btns_options[OP_VOLDOWN]->set_images("imgs/kb/keyboard_key.png","imgs/kb/keyboard_key_over.png",0,0);
    btns_options[OP_VOLUP]->s_x = btns_options[OP_VOLDOWN]->s_x + 50;
    btns_options[OP_VOLDOWN]->s_y = btns_options[OP_VOLUP]->s_y;
    btns_options[OP_VOLDOWN]->set_text("-");


};

castle_gui_options::~castle_gui_options()
{
    loopi(OP_MAX)
    {
        if (btns_options[i])
        {
            delete btns_options[i];
            btns_options[i] = 0;
        }
    }

    delete btns_close;
    btns_close = 0;
};

int castle_gui_options::hit_test(int s)
{
    if(lasttype==event.type && lastbut==event.button.button) return s; // why?? get event twice without it
    lasttype = event.type;
    lastbut = event.button.button;

    if(btns_close->hit_test(&event,0)==B_CLICK)
        return GS_TITLE;

    if(btns_options[OP_PLAYSOUND]->hit_test(&event,0)==B_CLICK)
        cgame->snd->can_play_sounds = !cgame->snd->can_play_sounds;

    if(btns_options[OP_PLAYMUSIC]->hit_test(&event,0)==B_CLICK)
    {
        cgame->snd->can_play_music = !cgame->snd->can_play_music;
        if (!cgame->snd->can_play_music) cgame->snd->stop_music();
        else cgame->snd->play_music(cgame->snd->music_fn,true);
    }

    if(btns_options[OP_VOLUP]->hit_test(&event,0)==B_CLICK)
        cgame->snd->volume_up();

    if(btns_options[OP_VOLDOWN]->hit_test(&event,0)==B_CLICK)
        cgame->snd->volume_down();


    return s; // nothing
}

void castle_gui_options::render()
{

    draw_rect(cgame->screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0); // clear backbuffer

    // title / bg
    SDL_BlitSurface(guibg,0,cgame->screen,0);


    // buttons
    btns_close->obj_state = B_OUT; //reset
    btns_close->hit_test(&event,0);
    btns_close->draw();


    if (cgame->snd->can_play_sounds)
        btns_options[OP_PLAYSOUND]->set_text("ON");
    else btns_options[OP_PLAYSOUND]->set_text("OFF");

    if (cgame->snd->can_play_music)
        btns_options[OP_PLAYMUSIC]->set_text("ON");
    else btns_options[OP_PLAYMUSIC]->set_text("OFF");

    loopi(OP_MAX)
    {
        btns_options[i]->obj_state = B_OUT; //reset
        btns_options[i]->hit_test(&event,0);
        btns_options[i]->draw();
    }

    // text
    cgame->fnts->change_color(117,33,13);

    int n = 0;
    while(options_text[n])
    {
        cgame->fnts->text(cgame->screen,options_text[n],290,115+(n*60),400,1);
        n++;
    }

};


