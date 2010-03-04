#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#ifdef _WII_

#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gccore.h>
#include <gctypes.h>
#include <debug.h>
#include <unistd.h>
#include <fcntl.h>

#define Sleep(x) usleep(x*1000); // in ms.

#endif

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// a lot of SDL deps.
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_Mixer.h>

// defines
#define GAME_VERSION    (1) // bump with new version
#define MAX_KEYS        (SDLK_LAST)
#define SCREEN_WIDTH    (640)
#define SCREEN_HEIGHT   (480)
#define FONT_SIZE       (30)
#define BITDEPTH        (16) // optimal for wii
#define DEGTORAD(angle) (angle * (M_PI / 180.0f))
#define RADTODEG(angle) (angle * (180 / M_PI))
#define SMALL_MEM       (255)
#define X_OFFSET        (5)
#define Y_OFFSET        (5)
#define HALF_SECOND     (500)
#define MESSAGE_1       (100)
#define MESSAGE_2       (101)

// globals
extern Uint8        g_real_keys[MAX_KEYS];
extern Uint8        g_keys_last_state[MAX_KEYS];
extern bool         g_running;
extern SDL_Event    event;

extern char* make_path(char*);
extern void draw_rect(SDL_Surface*,int,int,int,int,unsigned long);
extern unsigned long swap4 (unsigned long);
//stupid bug in SDL
extern int lastbut;
extern int lasttype;

// macros
#define loop(v,m) for(int v = 0; v<int(m); v++)
#define loopi(m) loop(i,m)
#define loopj(m) loop(j,m)

// box 2d does all the physics
#include "../include/box2d.h"
#include "fonts.h" // TTF wrapper
#include "textures.h" // for loading images
#include "sound.h" // sound
#endif


