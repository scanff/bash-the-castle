#include "globals.h"
#include "castle_game.h"
#include "gui/gui_button.h"
#include "castle_gui_fail.h"


char* fail_text[] = {    "Bash Failed!",
                        0
                    };



castle_gui_fail::castle_gui_fail(castle_game* g)
{
    cgame = g;
    btns_quit = btns_rty = 0;

    guibg = cgame->tx->texture_lookup("imgs/gui/gamedlg.png");
    if (!guibg) exit(0);


    btns_rty = new gui_button(cgame,200,250,NULL,0);
    btns_rty->tx = cgame->tx;
    btns_rty->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_rty->set_text("Retry");
    btns_rty->font_sz = 1;
    btns_rty->pad_y = 8;
    btns_rty->pad_x = 10;
    btns_rty->text_color = 0xDDB90F;
    btns_rty->text_color_over = 0xffffff;

    btns_quit = new gui_button(cgame,325,250,NULL,0);
    btns_quit->tx = cgame->tx;
    btns_quit->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_quit->set_text("Quit");
    btns_quit->font_sz = 1;
    btns_quit->pad_y = 8;
    btns_quit->pad_x = 10;
    btns_quit->text_color = 0xDDB90F;
    btns_quit->text_color_over = 0xffffff;

};

castle_gui_fail::~castle_gui_fail()
{
    delete btns_quit;
    btns_quit = 0;

    delete btns_rty;
    btns_rty = 0;
};


int castle_gui_fail::hit_test(int s)
{
    if(lasttype==event.type && lastbut==event.button.button) return s; // why?? get event twice without it
    lasttype = event.type;
    lastbut = event.button.button;

    if(btns_quit->hit_test(&event,0)==B_CLICK)
        return GS_TITLE;

   if(btns_rty->hit_test(&event,0)==B_CLICK)
        return GS_RETRY;

    return s; // nothing
}

void castle_gui_fail::render()
{

    // info dlg
    SDL_Rect d = {(SCREEN_WIDTH / 2)-(guibg->w/2),100,0,0};
    SDL_BlitSurface(guibg,0,cgame->screen,&d);

    // buttons
    btns_rty->obj_state = B_OUT; //reset
    btns_rty->hit_test(&event,0);
    btns_rty->draw();

    btns_quit->obj_state = B_OUT; //reset
    btns_quit->hit_test(&event,0);
    btns_quit->draw();

    // text
    cgame->fnts->change_color(117,33,13);
    cgame->fnts->text(cgame->screen,fail_text[0],235,150,400);


};


