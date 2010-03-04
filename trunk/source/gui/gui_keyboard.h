#ifndef GUI_KEYBOARD_H_INCLUDED
#define GUI_KEYBOARD_H_INCLUDED

// Based off Tantric's libgui keyboard code.  Thank you!


#include "gui_textbox.h"

class gui_keyboard
{
    typedef struct _keytype {
        char ch, chShift;
    } Key;

    public:

    Key             keys[4][11];
    gui_button*     key_buttons[4][11];
    gui_button*     key_space;
    gui_button*     key_caps;
    gui_button*     key_shift;
    gui_button*     key_backspace;
    gui_textbox*    text_out;
    SDL_Surface*    dest;

    char*           dest_buffer;

    int             pos_x;
    int             pos_y;

    int             caps;
    int             shift;

    castle_game*    cgame;

    gui_keyboard(int _ix, int _iy,castle_game* _cg) :
        dest(_cg->screen), pos_x(_ix), pos_y(_iy), caps(0), shift(0), cgame(_cg)
    {

      	Key thekeys[4][11] = {
        {
            {'1','!'},
            {'2','@'},
            {'3','#'},
            {'4','$'},
            {'5','%'},
            {'6','^'},
            {'7','&'},
            {'8','*'},
            {'9','('},
            {'0',')'},
            {'\0','\0'}
        },
        {
            {'q','Q'},
            {'w','W'},
            {'e','E'},
            {'r','R'},
            {'t','T'},
            {'y','Y'},
            {'u','U'},
            {'i','I'},
            {'o','O'},
            {'p','P'},
            {'-','_'}
        },
        {
            {'a','A'},
            {'s','S'},
            {'d','D'},
            {'f','F'},
            {'g','G'},
            {'h','H'},
            {'j','J'},
            {'k','K'},
            {'l','L'},
            {':',';'},
            {'\'','"'}
        },

        {
            {'z','Z'},
            {'x','X'},
            {'c','C'},
            {'v','V'},
            {'b','B'},
            {'n','N'},
            {'m','M'},
            {',','<'},
            {'.','>'},
            {'/','?'},
            {'\0','\0'}
        }
        };

        memcpy(keys, thekeys, sizeof(thekeys));

        // create the keys

        char txt[2] = { 0, 0 };

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<11; j++)
            {
                key_buttons[i][j] = 0;

                if(keys[i][j].ch != '\0')
                {

                    txt[0] = keys[i][j].ch;
                    key_buttons[i][j] = new gui_button(cgame,pos_x+(j*42+21*i+40), pos_y+(i*42+80),0,0);
                    if (!key_buttons[i][j]) continue;

                    key_buttons[i][j]->tx = cgame->tx;
                    key_buttons[i][j]->set_images("imgs/kb/keyboard_key.png","imgs/kb/keyboard_key_over.png",0,0);
                    key_buttons[i][j]->set_text(txt);
                    key_buttons[i][j]->pad_y = 5;
                    key_buttons[i][j]->text_color = 0xDDB90F;
                    key_buttons[i][j]->text_color_over = 0xffffff;
                    key_buttons[i][j]->center_text = true;


                }
            }
        }

        key_space = 0;
        key_space = new gui_button(cgame,pos_x+0, pos_y+(4*42+80),0,0);
        key_space->tx = cgame->tx;
        key_space->set_images((char*)"imgs/kb/space_out.png",(char*)"imgs/kb/space_out.png",0,0);
        key_space->pad_y = 0;
        key_space->s_x = (SCREEN_WIDTH/2)-(key_space->s_w/2);
        key_space->text_color = 0xDDB90F;
        key_space->text_color_over = 0xffffff;
        key_space->center_text = true;

        key_backspace = 0;
        key_backspace = new gui_button(cgame,pos_x+(10*42+40), pos_y+(0*42+80),0,0);
        key_backspace->tx = cgame->tx;
        key_backspace->set_images((char*)"imgs/kb/keyboard_mediumkey.png",(char*)"imgs/kb/keyboard_mediumkey_over.png",0,0);
        key_backspace->pad_y = 5;
        key_backspace->set_text("Back");
        key_backspace->text_color = 0xDDB90F;
        key_backspace->text_color_over = 0xffffff;
        key_backspace->center_text = true;

        key_caps = 0;
        key_caps = new gui_button(cgame,pos_x+0, pos_y+(2*42+80),0,0);
        key_caps->tx = cgame->tx;
        key_caps->set_images((char*)"imgs/kb/keyboard_mediumkey.png",(char*)"imgs/kb/keyboard_mediumkey_over.png",0,0);
        key_caps->pad_y = 5;
        key_caps->set_text("Caps");
        key_caps->text_color = 0xDDB90F;
        key_caps->text_color_over = 0xffffff;
        key_caps->center_text = true;

        key_shift = 0;
        key_shift = new gui_button(cgame,pos_x+21, pos_y+(3*42+80),0,0);
        key_shift->tx = cgame->tx;
        key_shift->set_images((char*)"imgs/kb/keyboard_mediumkey.png",(char*)"imgs/kb/keyboard_mediumkey_over.png",0,0);
        key_shift->pad_y = 5;
        key_shift->set_text("Shift");
        key_shift->text_color = 0xDDB90F;
        key_shift->text_color_over = 0xffffff;
        key_shift->center_text = true;

        text_out = 0;
        text_out = new gui_textbox(dest,_cg->fnts,pos_x+(0), pos_y+(30),0,0,false);
        text_out->tx = cgame->tx;
        text_out->set_images((char*)"imgs/kb/blank.png",(char*)"imgs/kb/blank.png",0,0);
        text_out->pad_y = 5;
        text_out->s_x = (SCREEN_WIDTH/2)-(key_space->s_w/2);
        text_out->text_color = 0x000000;
        text_out->text_color_over = 0x000000;

        dest_buffer = text_out->text_l1;
    };

    ~gui_keyboard()
    {

        delete text_out;
        delete key_space;
        delete key_shift;
        delete key_caps;
        delete key_backspace;

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<11; j++)
            {
                if (key_buttons[i][j])
                {
                    delete key_buttons[i][j];
                    key_buttons[i][j] = 0;

                }
            }
        }
    };

    void update_keys()
    {
        char txt[2] = {0};
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<11; j++)
            {
                if (key_buttons[i][j])
                {
                    if (shift || caps) txt[0] = keys[i][j].chShift;
                    else txt[0] = keys[i][j].ch;

                    key_buttons[i][j]->set_text(txt);
                }
            }
        }
    };


    int hit_test()
    {
        bool dbl_event = false;
        if(lasttype==event.type && lastbut==event.button.button) dbl_event = true;
        lasttype = event.type;
        lastbut = event.button.button;

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<11; j++)
            {
                if (key_buttons[i][j])
                {
                    if(key_buttons[i][j]->hit_test(&event,0)==B_CLICK && !dbl_event)
                    {
                        char tb[2] = {0};

                        if(shift || caps)  tb[0] = keys[i][j].chShift;
                        else tb[0] = keys[i][j].ch;

                        strcat(dest_buffer,tb);
                    }
                }
            }
        }
        if (key_space->hit_test(&event,0)==B_CLICK && !dbl_event)
        {
            strcat(dest_buffer," ");
        }

        if (key_backspace->hit_test(&event,0)==B_CLICK && !dbl_event)
        {
            char* ptr = dest_buffer;

            while(*++ptr != 0);

            if (dest_buffer == ptr) *dest_buffer = 0;
            else *(ptr-1) = 0;
        }

        if(key_shift->hit_test(&event,0)==B_CLICK && !dbl_event)
        {
            shift ^= 1;
            update_keys();
        }

        if(key_caps->hit_test(&event,0)==B_CLICK)
        {
            caps ^= 1;
            update_keys();
        }

        text_out->set_text(dest_buffer);

        return 0;
    };

    void draw()
    {

        text_out->draw();
        key_space->draw();
        key_backspace->draw();
        key_caps->draw();
        key_shift->draw();

        for(int i=0; i<4; i++)
            for(int j=0; j<11; j++)
                if (key_buttons[i][j])
                    key_buttons[i][j]->draw();


    };

    void reset()
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<11; j++)
                if (key_buttons[i][j]) key_buttons[i][j]->obj_state = B_OUT; //reset

        key_space->obj_state = B_OUT;
        key_backspace->obj_state = B_OUT;
        key_caps->obj_state = B_OUT;
        key_shift->obj_state = B_OUT;
    };

};

#endif // GUI_KEYBOARD_H_INCLUDED
