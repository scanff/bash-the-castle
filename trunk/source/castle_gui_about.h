#ifndef CASTLE_GUI_ABOUT_H_INCLUDED
#define CASTLE_GUI_ABOUT_H_INCLUDED

#include "castle_gui.h"

class castle_game;
class gui_button;
class castle_gui_about : public castle_gui
{
    public:

    float             ypos;

    castle_gui_about(castle_game*);
    ~castle_gui_about();

    int hit_test(int);
    void render();
};


#endif // CASTLE_GUI_H_INCLUDED
