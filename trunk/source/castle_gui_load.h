#ifndef CASTLE_GUI_LOAD_H_INCLUDED
#define CASTLE_GUI_LOAD_H_INCLUDED

#include "castle_gui.h"

class castle_game;
class gui_button;
class castle_gui_load : public castle_gui
{
    public:

    #define MAX_LOAD_BTNS   (10)

    gui_button*    lst_btns[MAX_LOAD_BTNS];
    gui_button*    btns_close;
    gui_button*    btns_back;
    gui_button*    btns_next;

    char           path[255];

    int            index;

    castle_gui_load(castle_game*,char* path);
    ~castle_gui_load();

    int get_ls_number_files();
    void ls_dir();


    int hit_test(int);
    void render();


    void draw_cursor(int,int);
};


#endif // CASTLE_GUI_H_INCLUDED
