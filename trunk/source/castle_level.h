#ifndef CASTLE_LEVEL_H_INCLUDED
#define CASTLE_LEVEL_H_INCLUDED

// move to own files
class castle_level
{
    public:

    castle_game*    cgame;
    int             number_chars;


    castle_level(castle_game*);
    ~castle_level();

    // load and save

    void load_level(char*);
    void save_level(char*);


};


#endif // CASTLE_LEVEL_H_INCLUDED
