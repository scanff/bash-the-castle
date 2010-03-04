#ifndef CASTLE_ROCKS_H_INCLUDED
#define CASTLE_ROCKS_H_INCLUDED

enum {
    PROJECT_BIG_ROCK = 0,
    PROJECT_SMALL_ROCK,
    PROJECT_BOMB
};

class castle_rocks : public castle_ent {

    public:

    castle_game*    wld;
    int             rock_type;

    ~castle_rocks();
    castle_rocks(castle_game* w, int x, int y,int rt);

    void update();
};


#endif // CASTLE_ROCKS_H_INCLUDED
