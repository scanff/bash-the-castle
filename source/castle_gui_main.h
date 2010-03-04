#ifndef CASTLE_GUI_MAIN_H_INCLUDED
#define CASTLE_GUI_MAIN_H_INCLUDED


#include "castle_gui.h"

enum {
    B_NEWGAME = 0,
    B_LOADGAME,
    B_LEVELEDIT,
    B_ABOUT,
    B_OPTIONS,
    B_QUIT,
    B_MAX
};

class castle_game;
class gui_button;
class castle_gui_main : public castle_gui
{
    public:

    gui_button*    btns[B_MAX];

    castle_gui_main(castle_game*);
    ~castle_gui_main();


    int hit_test(int);
    void render();

    void draw_cursor(int,int);

};


#endif // CASTLE_GUI_H_INCLUDED
