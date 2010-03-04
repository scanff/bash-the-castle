#ifndef CASTLE_GUI_SAVE_H_INCLUDED
#define CASTLE_GUI_SAVE_H_INCLUDED

class gui_keyboard;
class castle_game;
class gui_button;
class castle_gui_save : public castle_gui
{
    public:

    gui_button*    btns_close;
    gui_button*    btns_save;
    gui_keyboard*  gui_keyb;

    castle_gui_save(castle_game*);
    ~castle_gui_save();


    int hit_test(int);
    void render();

};


#endif // CASTLE_GUI_H_INCLUDED
