#ifndef CASTLE_GUI_LEVEL_H_INCLUDED
#define CASTLE_GUI_LEVEL_H_INCLUDED


enum {
    LB_EXIT = 0,
    LB_SAVE,
    LB_CLEAR,
    LB_UNDO,
    LB_TEST,
    LB_LOAD,
    LB_PLAY,
    LB_PLAY_CLOSE,
    // bg's
    LB_BG1,
    LB_BG2,
    LB_BG3,
    // add object buttons
    LB_KING,
    LB_WOOD1,
    LB_WOOD2,
    LB_WOOD3,
    LB_STONE1,
    LB_STUMP1,
    LB_BARREL1,
    LB_ROOF1,
    LB_QUEEN,
    LB_JESTER,
    LB_SOLDIER,
    LB_PRINCESS,
    LB_MAX

};
class castle_gui_load;
class castle_gui_save;
class gui_button;
class castle_gui_level : public castle_gui
{
    public:

    gui_button*     btns[LB_MAX];

    castle_gui_level(castle_game*);
    ~castle_gui_level();

    castle_ent*     current;
    castle_ent*     highlight;

    castle_gui_save*    save_gui;
    castle_gui_load*    load_gui;

    b2Vec2          pos;
    float           angle;

    bool            save;
    bool            load;
    bool            play_level;

    // save the current pos of the ents before test !
    struct save_ents
    {
        float x;
        float y;
        float a;
    };

    vector<save_ents*> s_ents;

    // drawing for editor
    void render();
    int hit_test(int);


    castle_ent* hit_ent();

    void save_cur_ents(); // save the current position of th ents
    void delete_save_ents();
    void restore_ents();

};


#endif // CASTLE_LEVEL_H_INCLUDED
