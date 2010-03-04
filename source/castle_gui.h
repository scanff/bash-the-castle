#ifndef CASTLE_GUI_H_INCLUDED
#define CASTLE_GUI_H_INCLUDED

class castle_gui
{
    public:

    castle_gui() {};
    virtual ~castle_gui() {};

    castle_game*   cgame;

    SDL_Surface*   title;
    SDL_Surface*   guibg;

    char*          selected;

    virtual void init(){};
    virtual void render(){};
    virtual int hit_test(int){};


};


#endif // CASTLE_GUI_H_INCLUDED
