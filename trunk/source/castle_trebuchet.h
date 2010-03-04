#ifndef CASTLE_TREBUCHET_H_INCLUDED
#define CASTLE_TREBUCHET_H_INCLUDED


enum {
    TREB_PREVIEW = 0,
    TREB_LOADING,
    TREB_READY,
    TREB_LAUNCHING,
    TREB_FIRE,
    TREB_DONE,
    TREB_CANRELOAD,
    TREB_FAIL
};

class castle_rocks;
class castle_trebuchet : public castle_ent
{
    public:

    #define MAX_LOAD (3)

    SDL_Surface*    image_data_frame;
    SDL_Surface*    image_data_arm;
    SDL_Surface*    image_data_cweight;

    castle_game*    cgame;

    b2Body*         body_frame;
    b2Body*         body_arm;
    b2Body*         body_counter;

    b2Joint*        joint_armbody;
    b2Joint*        joint_armcounter;
    b2Joint*        joint_rope[MAX_LOAD];

    b2PolygonDef    sd;
    b2Vec2          rope_pos;
    b2Vec2          counter_pos;
    float           counter_angle;
    int             status;

    int             num_ammo; // total ammo loaded
    int             project_type; // projectile type

    castle_rocks*   current_ammo[MAX_LOAD];

    float x_arm;
    float y_arm;
    float a_arm;

    unsigned long start_time;


    castle_trebuchet(castle_game* cg);
    ~castle_trebuchet();

    void render();
    void load_rocks(int);

    // status ...
    void preview();
    void update(unsigned long);
    void ready();
    void start();
    void launching();
    void fire(unsigned long);
    void done(unsigned long);
    void can_reload();
    void fail();
};


#endif // CASTLE_TREBUCHET_H_INCLUDED
