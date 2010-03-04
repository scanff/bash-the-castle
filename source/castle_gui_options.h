#ifndef CASTLE_GUI_OPTIONS_H_INCLUDED
#define CASTLE_GUI_OPTIONS_H_INCLUDED

#include "castle_gui.h"

enum
{
    OP_PLAYSOUND = 0,
    OP_PLAYMUSIC,
    OP_VOLUP,
    OP_VOLDOWN,
    OP_MAX
};

class castle_game;
class gui_button;
class castle_gui_options : public castle_gui
{
    public:

    gui_button*     btns_close;
    gui_button*     btns_options[OP_MAX];

    castle_gui_options(castle_game*);
    ~castle_gui_options();


    int hit_test(int);
    void render();

};


#endif // CASTLE_GUI_OPTIONS_H_INCLUDED
