#include "globals.h"
#include <dirent.h>
#include "castle_game.h"
#include "gui/gui_button.h"
#include "castle_gui_load.h"


castle_gui_load::castle_gui_load(castle_game* g,char* p) : index(0)
{
    cgame = g;
    btns_close = 0;

    memset(path,0,255);
    if (!p) return;

    strcpy(path,p);

    guibg = cgame->tx->texture_lookup("imgs/gui/title.png");
    if (!guibg) exit(0);


    btns_close = new gui_button(cgame,425,315,NULL,0);
    btns_close->tx = cgame->tx;
    btns_close->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_close->set_text("Close");
    btns_close->font_sz = 1;
    btns_close->pad_y = 8;
    btns_close->pad_x = 10;
    btns_close->text_color = 0xDDB90F;
    btns_close->text_color_over = 0xffffff;


    btns_next = new gui_button(cgame,425,255,NULL,0);
    btns_next->tx = cgame->tx;
    btns_next->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_next->set_text("Next");
    btns_next->font_sz = 1;
    btns_next->pad_y = 8;
    btns_next->pad_x = 10;
    btns_next->text_color = 0xDDB90F;
    btns_next->text_color_over = 0xffffff;

    btns_back = new gui_button(cgame,425,200,NULL,0);
    btns_back->tx = cgame->tx;
    btns_back->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns_back->set_text("Back");
    btns_back->font_sz = 1;
    btns_back->pad_y = 8;
    btns_back->pad_x = 10;
    btns_back->text_color = 0xDDB90F;
    btns_back->text_color_over = 0xffffff;

    loopi(MAX_LOAD_BTNS)
    {
        lst_btns[i] = new gui_button(cgame,80,90+(i*28),NULL,0);
        lst_btns[i]->tx = cgame->tx;
        lst_btns[i]->set_images("imgs/gui/list_out.png","imgs/gui/list_over.png",0,0);
        lst_btns[i]->font_sz = 1;
        lst_btns[i]->pad_y = 0;
        lst_btns[i]->pad_x = 10;
        lst_btns[i]->text_color = 0xDDB90F;
        lst_btns[i]->text_color_over = 0xffffff;
    }

    ls_dir();

};

castle_gui_load::~castle_gui_load()
{
    loopi(MAX_LOAD_BTNS)
    {
        delete lst_btns[i];
        lst_btns[i] = 0;
    }

    delete btns_next;
    btns_next = 0;

    delete btns_back;
    btns_back = 0;

    delete btns_close;
    btns_close = 0;

};

int castle_gui_load::hit_test(int s)
{

    bool dbl_event = false;
    if(lasttype==event.type && lastbut==event.button.button) dbl_event = true;
    lasttype = event.type;
    lastbut = event.button.button;

    if(btns_close->hit_test(&event,0)==B_CLICK && !dbl_event)
        return MESSAGE_1;

    if(btns_back->hit_test(&event,0)==B_CLICK && !dbl_event)
    {
        index -= MAX_LOAD_BTNS;
        if (index < 0) index = 0;

        loopi(MAX_LOAD_BTNS) lst_btns[i]->set_text("");

        ls_dir();
    }

    if(btns_next->hit_test(&event,0)==B_CLICK && !dbl_event)
    {

        if (get_ls_number_files() < index+MAX_LOAD_BTNS) return s;

        index += MAX_LOAD_BTNS;
        loopi(MAX_LOAD_BTNS) lst_btns[i]->set_text("");

        ls_dir();

    }

    loopi(MAX_LOAD_BTNS)
    {
        if (lst_btns[i]->hit_test(&event,0) == B_CLICK && !dbl_event)
        {
            selected = lst_btns[i]->text_l1;
            return MESSAGE_2;
        }
    }

    return s; // nothing
}

void castle_gui_load::render()
{

    ls_dir(); // refresh

    // title / bg
    SDL_BlitSurface(guibg,0,cgame->screen,0);

    // buttons
    btns_close->obj_state = B_OUT; //reset
    btns_close->hit_test(&event,0);
    btns_close->draw();

    btns_next->obj_state = B_OUT; //reset
    btns_next->hit_test(&event,0);
    btns_next->draw();

    btns_back->obj_state = B_OUT; //reset
    btns_back->hit_test(&event,0);
    btns_back->draw();

    loopi(MAX_LOAD_BTNS)
    {
        lst_btns[i]->obj_state = B_OUT; //reset
        lst_btns[i]->hit_test(&event,0);
        lst_btns[i]->draw();
    }

};

#ifdef _WII_

int castle_gui_load::get_ls_number_files()
{
    int num_files = 0;
    DIR *pdir;
    struct dirent *pent;
    struct stat statbuf;

    pdir=opendir(path);

    if (!pdir){
        return 0;
    }

    while ((pent=readdir(pdir))!=NULL) {

        stat(pent->d_name,&statbuf);
        if(strcmp(".", pent->d_name) == 0 || strcmp("..", pent->d_name) == 0)
        {
            memset(pent->d_name,0,NAME_MAX+1);
            continue;
        }
        num_files++;

    }

    closedir(pdir);

    return num_files;
};

void castle_gui_load::ls_dir()
{
    int current = 0;
    int count = 0;
    DIR *pdir;
    struct dirent *pent;
    struct stat statbuf;

    pdir=opendir(path);

    if (!pdir){
        return;
    }

    while ((pent=readdir(pdir))!=NULL)
    {

        stat(pent->d_name,&statbuf);
        if(strcmp(".", pent->d_name) == 0 || strcmp("..", pent->d_name) == 0)
        {
            memset(pent->d_name,0,NAME_MAX+1);
            continue;
        }

        if (current >= MAX_LOAD_BTNS) break;

        if (count++ >= index)
            lst_btns[current++]->set_text(pent->d_name);

    }

    closedir(pdir);

    return;
};

#else

int castle_gui_load::get_ls_number_files()
{
    int num_files = 0;

    DIR *dir;
    struct dirent *ent;
    dir = opendir (path);

    if (dir != 0)
    {

      while ((ent = readdir (dir)) != 0)
      {
        if(strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
            continue;

        num_files++;

      }

      closedir (dir);

    }

    return num_files;
};

void castle_gui_load::ls_dir()
{
    int current = 0;
    int count = 0;
    DIR *dir;
    struct dirent *ent;
    dir = opendir (path);

    if (dir != 0)
    {

      while ((ent = readdir (dir)) != 0)
      {
        if(strcmp(".", ent->d_name) == 0 || strcmp("..", ent->d_name) == 0)
            continue;

        if (current >= MAX_LOAD_BTNS) break;

        if (count++ >= index)
            lst_btns[current++]->set_text(ent->d_name);

      }

      closedir (dir);

    } else { return; }

};
#endif
