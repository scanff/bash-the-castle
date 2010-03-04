#ifndef CASTLE_CHARACTER_H_INCLUDED
#define CASTLE_CHARACTER_H_INCLUDED

#include "castle_ent.h"

class castle_game;
class castle_character : public castle_ent
{
public:

    castle_game*    wld;
    int             x_px;
    int             y_px;
    int             w_px;
    int             h_px;
    float           fric;


    b2PolygonDef    sd;

    SDL_Surface*    image_char_alive;
    SDL_Surface*    image_char_dead;

	castle_character(castle_game*, int, int, int);
	~castle_character();

	void update();

};

#endif // CASTLE_CHARACTER_H_INCLUDED
