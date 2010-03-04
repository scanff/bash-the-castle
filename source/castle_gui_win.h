#ifndef CASTLE_GUI_WIN_H_INCLUDED
#define CASTLE_GUI_WIN_H_INCLUDED

#include "castle_gui.h"

class castle_game;
class gui_button;
class castle_gui_win : public castle_gui
{
    public:

    gui_button*    btns_cnt;
    gui_button*    btns_quit;

    castle_gui_win(castle_game*);
    ~castle_gui_win();


    int hit_test(int);
    void render();

};


#endif // CASTLE_GUI_H_INCLUDED
