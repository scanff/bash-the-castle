#include "globals.h"
#include "castle_game.h"
#include "gui/gui_button.h"
#include "castle_gui_main.h"


castle_gui_main::castle_gui_main(castle_game* g)
{
    cgame = g;
    loopi(B_MAX) btns[i] = 0;

    guibg = cgame->tx->texture_lookup("imgs/gui/title.png");
    if (!guibg) exit(0);

    title = cgame->tx->texture_lookup("imgs/gui/logo.png");
    if (!title) exit(0);

    btns[B_NEWGAME] = new gui_button(cgame,110,260,NULL,0);
    btns[B_NEWGAME]->tx = cgame->tx;
    btns[B_NEWGAME]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[B_NEWGAME]->set_text("New Game");
    btns[B_NEWGAME]->font_sz = 1;
    btns[B_NEWGAME]->pad_y = 8;
    btns[B_NEWGAME]->pad_x = 10;
    btns[B_NEWGAME]->text_color = 0xDDB90F;
    btns[B_NEWGAME]->text_color_over = 0xffffff;

    btns[B_LOADGAME] = new gui_button(cgame,250,260,NULL,0);
    btns[B_LOADGAME]->tx = cgame->tx;
    btns[B_LOADGAME]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[B_LOADGAME]->set_text("Load Game");
    btns[B_LOADGAME]->font_sz = 1;
    btns[B_LOADGAME]->pad_y = 8;
    btns[B_LOADGAME]->pad_x = 10;
    btns[B_LOADGAME]->text_color = 0xDDB90F;
    btns[B_LOADGAME]->text_color_over = 0xffffff;


    btns[B_LEVELEDIT] = new gui_button(cgame,390,260,NULL,0);
    btns[B_LEVELEDIT]->tx = cgame->tx;
    btns[B_LEVELEDIT]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[B_LEVELEDIT]->set_text("Level Editor");
    btns[B_LEVELEDIT]->font_sz = 1;
    btns[B_LEVELEDIT]->pad_y = 8;
    btns[B_LEVELEDIT]->pad_x = 10;
    btns[B_LEVELEDIT]->text_color = 0xDDB90F;
    btns[B_LEVELEDIT]->text_color_over = 0xffffff;


    btns[B_ABOUT] = new gui_button(cgame,110,320,NULL,0);
    btns[B_ABOUT]->tx = cgame->tx;
    btns[B_ABOUT]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[B_ABOUT]->set_text("About");
    btns[B_ABOUT]->font_sz = 2;
    btns[B_ABOUT]->pad_y = 8;
    btns[B_ABOUT]->pad_x = 10;
    btns[B_ABOUT]->text_color = 0xDDB90F;
    btns[B_ABOUT]->text_color_over = 0xffffff;

    btns[B_OPTIONS] = new gui_button(cgame,250,320,NULL,0);
    btns[B_OPTIONS]->tx = cgame->tx;
    btns[B_OPTIONS]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[B_OPTIONS]->set_text("Options");
    btns[B_OPTIONS]->font_sz = 2;
    btns[B_OPTIONS]->pad_y = 8;
    btns[B_OPTIONS]->pad_x = 10;
    btns[B_OPTIONS]->text_color = 0xDDB90F;
    btns[B_OPTIONS]->text_color_over = 0xffffff;

    btns[B_QUIT] = new gui_button(cgame,390,320,NULL,0);
    btns[B_QUIT]->tx = cgame->tx;
    btns[B_QUIT]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[B_QUIT]->set_text("Quit");
    btns[B_QUIT]->font_sz = 2;
    btns[B_QUIT]->pad_y = 8;
    btns[B_QUIT]->pad_x = 10;
    btns[B_QUIT]->text_color = 0xDDB90F;
    btns[B_QUIT]->text_color_over = 0xffffff;

};



castle_gui_main::~castle_gui_main()
{
   loopi(B_MAX)
   {
       delete btns[i];
       btns[i] = 0;
   }
};

int castle_gui_main::hit_test(int s)
{
    if(lasttype==event.type && lastbut==event.button.button) return s; // why?? get event twice without it
    lasttype = event.type;
    lastbut = event.button.button;

    if(btns[B_NEWGAME]->hit_test(&event,0)==B_CLICK)
        return GS_NEWGAME;

    if(btns[B_ABOUT]->hit_test(&event,0)==B_CLICK)
    {
        return GS_ABOUT;
    }

    if(btns[B_LEVELEDIT]->hit_test(&event,0)==B_CLICK)
    {
        cgame->clear_ents();
        return GS_LEVELEDIT;
    }

    if(btns[B_LOADGAME]->hit_test(&event,0)==B_CLICK)
        return GS_LOADGAME;

    if(btns[B_OPTIONS]->hit_test(&event,0)==B_CLICK)
        return GS_OPTIONS;

    if(btns[B_QUIT]->hit_test(&event,0)==B_CLICK)
        return g_running = false;

    return s; // nothing
}


void castle_gui_main::render()
{

    draw_rect(cgame->screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0); // clear backbuffer

    // title / bg
    SDL_BlitSurface(guibg,0,cgame->screen,0);

    // logo
    SDL_Rect d = { 50,110,0,0};
    SDL_BlitSurface(title,0,cgame->screen,&d);


    // buttons
    loopi(B_MAX)
    {
        btns[i]->obj_state = B_OUT; //reset
        btns[i]->hit_test(&event,0);
        btns[i]->draw();
    }

};


