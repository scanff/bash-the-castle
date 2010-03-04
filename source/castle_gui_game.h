#ifndef CASTLE_GUI_GAME_H_INCLUDED
#define CASTLE_GUI_GAME_H_INCLUDED

#include "castle_gui.h"

enum {
    GB_ROCK_BIG = 0,
    GB_ROCK_SMALL,
    GB_ROCK_BOMB,
    GB_SAVE,
    GB_MAX
};

class castle_game;
class gui_button;
class castle_gui_game : public castle_gui
{
    public:

    gui_button*    btns_game[GB_MAX];

    castle_gui_game(castle_game*);
    ~castle_gui_game();

    int     x;
    int     y;

    unsigned long save_time;
    int     save_state;

    int hit_test(int);
    void render();


    void draw_cursor(int,int);
};


#endif
