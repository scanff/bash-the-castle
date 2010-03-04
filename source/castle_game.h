#ifndef CASTLE_GAME_H_INCLUDED
#define CASTLE_GAME_H_INCLUDED

#include "castle_ent.h"
#include "castle_collide.h"

#define MAX_SCREEN_X    (SCREEN_WIDTH * 2)
#define SCALE (5)

enum { // Game states
    GS_TITLE = 0, // Title screen
    GS_ABOUT, // About
    GS_LEVELEDIT, // Level Editor
    GS_PLAYING, // Playing
    GS_LOADGAME, // load a save
    GS_WIN, // yeah completed level
    GS_NEXT_LVL, // go to next level
    GS_OPTIONS, // options
    GS_NEWGAME,
    GS_FAIL,
    GS_RETRY,
    GS_GAMEOPTIONS,
    GS_EXIT,
    GS_SAVE,
    GS_NEWGAME_FROM_LOAD,
    GS_LEVELEDIT_TEST,
    GS_LEVELEDIT_TEST_INIT,
    GS_LEVELEDIT_TEST_CLOSE,
    GS_MAX
};

class castle_level;
class castle_trebuchet;
class castle_block;
class castle_gui;
class fonts;
class castle_game : public contact_listener
{
    public:

    SDL_Surface*        screen; // The screen buffer

    castle_level*       level;
    texture_cache*      tx;
    sound_cache*        snd;
    castle_gui*         guis[GS_MAX];
    fonts*              fnts;

    vector<castle_ent*> ents;
    castle_trebuchet*   treb;

    b2World*            world;
    b2Body*             ground;

    SDL_Surface*        background;

    int                 camera_x;
    int                 status;     // Game state

    contact_listener    collision_listener;

    float               ground_w; // ground width

    int                 current_level;
    int                 current_bg;

    bool                simulate;
    unsigned long       win_time;
    bool                can_ply_snd;
    int                 score; // game score
    int                 start_score; // score at start of level, used for saves
    int                 num_rocks; // how may tries you get this level
    int                 total_rocks; // max tries

    SDL_Surface*        bbutton;
    unsigned long       bbuton_time;
    unsigned long       bbuton_time_start;

    SDL_Surface*        cursor;
    SDL_Surface*        selection;

    // some options
    bool                play_music;
    bool                play_sounds;

    int                 save_counter;


    castle_game();
    ~castle_game();

    float screenx_2_real(int x);
    float screeny_2_real(int y);
    float realx_2_screen(float);
    float realy_2_screen(float);

     // world and ground set up
    void make_world(void); // main init

    float get_px_y(b2Vec2*);
    float get_px_x(b2Vec2*);

    void drawB2image(castle_ent* blk,b2Body* body);
    void drawB2Shape(b2Body* body, b2Shape* shape);

    int get_chars_alive(); // how many characters are left

    void load_level(char*,bool);
    void update(unsigned long);
    void reset();
    void clear_ents();
    void reorder_ents();

    // options
    void load_options();
    void save_options();

    // load save
    int create_save();
    int load_save(char*);

    // drawing .. .etc.
    void render();
    void render_overlay();
    void draw_cursor(int,int);


};


#endif // CASTLE_GAME_H_INCLUDED
