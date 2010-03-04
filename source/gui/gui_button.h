#ifndef _GUI_BUTTON_H_
#define _GUI_BUTTON_H_

#include "gui_object.h"
//simple button class
class gui_button : public gui_object
{
    public:

    void* user_data;

    gui_button(castle_game* _cg,int x,int y,char* t,long tc,void* _ud = 0) :
        user_data(_ud)
    {

        thegame = _cg;
        s_x = x;
        s_y = y;
        pad_x = 30;
        pad_y = 12;
        text_color = tc;
        fnts = _cg->fnts;
        guibuffer = _cg->screen;
        obj_state = B_OUT;
        obj_type = GUI_BUTTON;

        if (t) strcpy(text_l1,t);

    }

    ~gui_button()
    {
    };


    void draw()
    {

        gui_object::draw();

        if (*text_l1)
        {

            if (center_text) {
                int text_len = fnts->get_length_px(text_l1);
                int cx = 0;
                text_len>0 ? cx = (int)((s_w-(text_len))/2): cx=1;
                fnts->text(guibuffer,text_l1,cx+s_x,s_y+pad_y,limit_text);
            }else{
                fnts->text(guibuffer,text_l1,s_x+pad_x,s_y+pad_y,limit_text);
            }

        }

    };



};

#endif
