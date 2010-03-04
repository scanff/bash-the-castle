#include "globals.h"
#include "castle_game.h"
#include "castle_gui.h"
#include "gui/gui_button.h"
#include "gui/gui_keyboard.h"
#include "castle_gui_save.h"


castle_gui_save::castle_gui_save(castle_game* g)
{
    cgame = g;
    btns_close = 0;

    guibg = cgame->tx->texture_lookup("imgs/gui/title.png");
    if (!guibg) exit(0);

    btns_save = new gui_button(cgame,340,350,NULL,0);
    btns_save->tx = cgame->tx;
    btns_save->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_save->set_text("Save");
    btns_save->font_sz = 1;
    btns_save->pad_y = 8;
    btns_save->pad_x = 10;
    btns_save->text_color = 0xDDB90F;
    btns_save->text_color_over = 0xffffff;

    btns_close = new gui_button(cgame,470,350,NULL,0);
    btns_close->tx = cgame->tx;
    btns_close->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_close->set_text("Close");
    btns_close->font_sz = 1;
    btns_close->pad_y = 8;
    btns_close->pad_x = 10;
    btns_close->text_color = 0xDDB90F;
    btns_close->text_color_over = 0xffffff;

    gui_keyb = new gui_keyboard(40,50,cgame);

};

castle_gui_save::~castle_gui_save()
{
    delete gui_keyb;
    gui_keyb = 0;

    delete btns_save;
    btns_save = 0;

    delete btns_close;
    btns_close = 0;
};

int castle_gui_save::hit_test(int s)
{
    // close/cancel button
    if(btns_close->hit_test(&event,0)==B_CLICK)
        return MESSAGE_1;

    if(btns_save->hit_test(&event,0)==B_CLICK)
        return MESSAGE_2;


    return s; // nothing
}

void castle_gui_save::render()
{

    //draw_rect(cgame->screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0); // clear backbuffer
    // title / bg
    SDL_BlitSurface(guibg,0,cgame->screen,0);

    // buttons
    btns_close->obj_state = B_OUT; //reset
    btns_close->hit_test(&event,0);
    btns_close->draw();
    btns_save->obj_state = B_OUT; //reset
    btns_save->hit_test(&event,0);
    btns_save->draw();


    gui_keyb->reset();
    // hit test keyboard keys
    gui_keyb->hit_test();
    gui_keyb->draw();



};


