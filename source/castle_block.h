#ifndef CASTLE_BLOCK_H_INCLUDED
#define CASTLE_BLOCK_H_INCLUDED

#include "castle_ent.h"
class castle_game;
class castle_block : public castle_ent
{
public:

    castle_game*    wld;
    int             x_px;
    int             y_px;
    int             w_px;
    int             h_px;
    float           fric;

    b2PolygonDef    sd;


	castle_block(castle_game*, int, int, int,float);
	~castle_block();

	void update();

};


#endif // CASTLE_BLOCK_H_INCLUDED
