#ifndef CASTLE_GUI_EXIT_H_INCLUDED
#define CASTLE_GUI_EXIT_H_INCLUDED

#include "castle_gui.h"

class castle_game;
class gui_button;
class castle_gui_exit : public castle_gui
{
    public:

    gui_button*    btns_cancel;
    gui_button*    btns_quit;

    castle_gui_exit(castle_game*);
    ~castle_gui_exit();


    int hit_test(int);
    void render();

    void draw_cursor(int,int);
};


#endif
