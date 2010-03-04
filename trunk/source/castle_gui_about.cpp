#include "globals.h"
#include "castle_game.h"
#include "gui/gui_button.h"
#include "castle_gui_about.h"


char* about_text[] = {
    "",
    "",
    "A Game By Scanff, made for the Wii console.",
    "",
    "",
    "[Credits]",
    "",
    "Scanff - Programming, some art and some sound",
    "WaxyPumpkin72 - Music and Sound",
    "bg4545 - Main Art and Graphics",
    "",
    "",
    "[Thanks To]",
    "",
    "Tantric - WiiSDL and code I modified for the keyboard",
    "The DevKitPro Team",
    "WiiBrew Community",
    "Box2D engine",
    "",
    "",
    "[Help]",
    "",
    "You are ordered to destory all the castles. Go forth!",
    "",
    "Use the Wiimote and (A) button to navigate the gui.",
    "The (B) button acts as launch and release.",
    "Press (B) once to launch the trebuchet and (B) to release the",
    "projectile."


};



castle_gui_about::castle_gui_about(castle_game* g) : ypos(480)
{
    cgame = g;
    title = cgame->tx->texture_lookup("imgs/gui/logo.png");


};

castle_gui_about::~castle_gui_about()
{
};

int castle_gui_about::hit_test(int s)
{
    if (g_real_keys[SDLK_ESCAPE])
    {
        return GS_TITLE;
    }
    return s; // nothing
}

void castle_gui_about::render()
{

    draw_rect(cgame->screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0); // clear backbuffer

    // logo
    SDL_Rect d = { 50,(int)(ypos),0,0};
    if (title) SDL_BlitSurface(title,0,cgame->screen,&d);


    // text
    cgame->fnts->change_color(217,217,217);
    loopi(27) cgame->fnts->text(cgame->screen,about_text[i],80,(int)(ypos) + (100+(i*30)),600);

    ypos-=0.3f;

    if ((int)(ypos) + (100+(27*30)) < -15) ypos = 480; // reset
};


