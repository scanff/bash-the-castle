#include "globals.h"
#include "castle_game.h"
#include "gui/gui_button.h"
#include "castle_gui_exit.h"


char* exit_text[] = {    "Exit To Menu?",
                        0
                    };



castle_gui_exit::castle_gui_exit(castle_game* g)
{
    cgame = g;
    btns_quit = btns_cancel = 0;

    guibg = cgame->tx->texture_lookup("imgs/gui/gamedlg.png");
    if (!guibg) exit(0);


    btns_cancel = new gui_button(cgame,200,250,NULL,0);
    btns_cancel->tx = cgame->tx;
    btns_cancel->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_cancel->set_text("Cancel");
    btns_cancel->font_sz = 1;
    btns_cancel->pad_y = 8;
    btns_cancel->pad_x = 10;
    btns_cancel->text_color = 0xDDB90F;
    btns_cancel->text_color_over = 0xffffff;

    btns_quit = new gui_button(cgame,325,250,NULL,0);
    btns_quit->tx = cgame->tx;
    btns_quit->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_quit->set_text("Exit");
    btns_quit->font_sz = 1;
    btns_quit->pad_y = 8;
    btns_quit->pad_x = 10;
    btns_quit->text_color = 0xDDB90F;
    btns_quit->text_color_over = 0xffffff;

};

castle_gui_exit::~castle_gui_exit()
{
    delete btns_quit;
    btns_quit = 0;

    delete btns_cancel;
    btns_cancel = 0;
};


int castle_gui_exit::hit_test(int s)
{
    if(lasttype==event.type && lastbut==event.button.button) return s; // why?? get event twice without it
    lasttype = event.type;
    lastbut = event.button.button;

    if(btns_quit->hit_test(&event,0)==B_CLICK)
        return GS_TITLE;

   if(btns_cancel->hit_test(&event,0)==B_CLICK)
        return GS_PLAYING;

    return s; // nothing
}

void castle_gui_exit::render()
{

    // info dlg
    SDL_Rect d = {(SCREEN_WIDTH / 2)-(guibg->w/2),100,0,0};
    SDL_BlitSurface(guibg,0,cgame->screen,&d);

    // buttons
    btns_cancel->obj_state = B_OUT; //reset
    btns_cancel->hit_test(&event,0);
    btns_cancel->draw();

    btns_quit->obj_state = B_OUT; //reset
    btns_quit->hit_test(&event,0);
    btns_quit->draw();

    // text
    cgame->fnts->change_color(117,33,13);
    cgame->fnts->text(cgame->screen,exit_text[0],235,150,400);

};


