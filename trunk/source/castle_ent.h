#ifndef CASTLE_ENT_H_INCLUDED
#define CASTLE_ENT_H_INCLUDED
// group types
enum {
  TYPE_BLOCK = 0,
  TYPE_CHAR,
  TYPE_ROCK

};
// ent types
enum {

    CHAR_KING = 0,
    CHAR_QUEEN,
    CHAR_PRINCESS,
    CHAR_PRINCE,
    CHAR_SOLDIER,
    CHAR_KNIGHT,
    CHAR_JESTER,
    CHAR_END,

    WOOD_V,
    WOOD_H,
    ROCK_V,
    STUMP_V,
    ROOF_1,
    WOOD_3,
    BARREL_1
};

// state
enum {
    ALIVE,
    HIT,
    DEAD
};

class castle_game;
class castle_ent
{
    public:

    SDL_Surface*    image_data;

    b2Body*         body;
    b2World*        world;
    castle_game*    cgame;
    int             status;

    int             type;
    int             sub_type; // type of block or char
    int             r_x; // save real x pos
    int             r_y; // save real y pos
    float           angle;
    float           w_x; // world x
    float           w_y; // world y

    castle_ent*     hit; // ent this ent has collided with

    castle_ent() : body(0) {};
    ~castle_ent()
    {
        if (body)
        {
            world->DestroyBody(body);
            body = 0;
        }
    };

    virtual void update() {};

};


#endif // CASTLE_ENT_H_INCLUDED
