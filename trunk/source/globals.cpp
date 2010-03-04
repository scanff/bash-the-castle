#include "globals.h"

char* make_path(const char*);
char* make_path(char* path_rel)
{
#ifdef _WII_
	static char abs_path[255] = {0};
	sprintf(abs_path,"sd:/apps/btc/%s",path_rel);
	return abs_path;
#else
	return (char*)path_rel;
#endif

}
void draw_rect(SDL_Surface*,int,int,int,int,unsigned long);
void draw_rect(SDL_Surface* s,int x,int y, int w, int h,unsigned long color)
{
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.h = h;
    r.w = w;
    SDL_FillRect(s, &r, color);
};


unsigned long swap4 (unsigned long nLongNumber)
{
   return (((nLongNumber&0x000000FF)<<24)+((nLongNumber&0x0000FF00)<<8)+
   ((nLongNumber&0x00FF0000)>>8)+((nLongNumber&0xFF000000)>>24));
}
