#include "globals.h"
#include "castle_game.h"
#include "castle_character.h"
#include "castle_block.h"
#include "castle_level.h"

castle_level::castle_level(castle_game* _g) : cgame(_g)
{
};

castle_level::~castle_level()
{
};

void castle_level::load_level(char* name)
{
    number_chars = 0;

    FILE* f = 0;
    f = fopen(make_path(name),"rb");
    if (!f) return;

    int res = 1;
    int x,y,t,st;
    float a = 0;

    castle_block* cblocks;
    castle_character* cc;

    // background
    int bg,  h;
    fread(&bg,sizeof(int),1,f);
    cgame->current_bg = bg;
#ifdef _WII_
    cgame->current_bg =swap4(bg);
#endif
    while(res != 0)
    {
        res = fread(&t,sizeof(int),1,f);
        if (res <= 0) break;
        res = fread(&st,sizeof(int),1,f);
        if (res <= 0) break;
        res = fread(&x,sizeof(int),1,f);
        if (res <= 0) break;
        res = fread(&y,sizeof(int),1,f);
        if (res <= 0) break;
        res = fread(&h,sizeof(int),1,f);
        if (res <= 0) break;

#ifdef _WII_
        x=swap4(x);
        y=swap4(y);
        st=swap4(st);
        t=swap4(t);
        h=swap4(h); /// hmmmmmmmm!!!!!

#endif
        memcpy(&a,&h,sizeof(float));

        switch(t)
        {
            case TYPE_BLOCK:
                cblocks = new castle_block(cgame,MAX_SCREEN_X+x,y,st,a);
            break;

            case TYPE_CHAR:
                cc = new castle_character(cgame,MAX_SCREEN_X+x,y,st);
                number_chars++;
            break;

        };
    }

    fclose(f);
};
// FORMAT ALWAYS BIG ENDIAN
void castle_level::save_level(char* fn)
{
    char fullname[255] = {0};

    sprintf(fullname,"levels/%s.btc",fn);

    FILE* f = 0;
    f = fopen(make_path(fullname),"wb");
    if (!f) return;

    int x,y,t,st,bg;
    float a = 0;
    bg = cgame->current_bg;
#ifdef _WII_
    bg = swap4(bg);
#endif
    // background
    fwrite(&bg,sizeof(int),1,f);

    loopi(cgame->ents.size())
    {
        if (cgame->ents[i]->type == TYPE_BLOCK ||
            cgame->ents[i]->type == TYPE_CHAR)
        {
            x = cgame->ents[i]->r_x;
            y = cgame->ents[i]->r_y;
            t = cgame->ents[i]->type;
            st = cgame->ents[i]->sub_type;
            a = cgame->ents[i]->angle;

#ifdef _WII_
            x = swap4(x);
            y = swap4(y);
            t = swap4(t);
            st = swap4(st);
            int ia;
            memcpy(&ia,&a,sizeof(int));
            ia = swap4(ia);
#endif

            // type
            fwrite(&t,sizeof(int),1,f);
            // sub type
            fwrite(&st,sizeof(int),1,f);
            // x
            fwrite(&x,sizeof(int),1,f);
            // y
            fwrite(&y,sizeof(int),1,f);
            // angle
#ifdef _WII_
            fwrite(&ia,sizeof(int),1,f);
#else
            fwrite(&a,sizeof(float),1,f);
#endif
        }
    }

    fclose(f);
};
