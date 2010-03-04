#include "globals.h"
#include "castle_game.h"

SDL_Event       event;
Uint8           g_real_keys[MAX_KEYS];
Uint8           g_keys_last_state[MAX_KEYS];
bool            g_running;
castle_game*    cgame; // the core

void translate_keys()
{

#ifdef _WII_


    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_HOME){
        g_real_keys[SDLK_ESCAPE] = 1;
    }

    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_LEFT){
        g_real_keys[SDLK_LEFT] = 1;
    }
    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_RIGHT){
        g_real_keys[SDLK_RIGHT] = 1;
    }
    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN) {
         g_real_keys[SDLK_DOWN] = 1;
    }
    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP) {
         g_real_keys[SDLK_UP] = 1;
    }

    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_A) {
         g_real_keys[SDLK_a] = 1;
    }

    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_B) {
         g_real_keys[SDLK_b] = 1;
    }

    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_PLUS) {
         g_real_keys[SDLK_PLUS] = 1;

    }

    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS) {
         g_real_keys[SDLK_MINUS] = 1;

    }

    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_1) {
         g_real_keys[SDLK_1] = 1;

    }

    if(WPAD_ButtonsHeld(0) & WPAD_BUTTON_2) {
         g_real_keys[SDLK_2] = 1;

    }

#else

    if ((event.button.state == SDL_PRESSED) &&
        (event.button.button == SDL_BUTTON_LEFT))
        g_real_keys[SDLK_b] = 1;

#endif


}


#ifdef _WII_

void countevs(int chan, const WPADData *data)
{
};


//exit function
void ShutdownCB()
{
	g_running = false;
};

#endif

int main(int argc, char **argv)
{
	int fullscreen = 0;

#ifdef _WII_
    u32 type;
	WPADData *wd_one;

	fullscreen = 1;
	fatInitDefault();

	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
    WPAD_SetVRes(WPAD_CHAN_ALL, SCREEN_WIDTH*2, SCREEN_HEIGHT*2);
	WPAD_SetIdleTimeout(400);

	// Wii Power/Reset buttons
	WPAD_SetPowerButtonCallback((WPADShutdownCallback)ShutdownCB);
	SYS_SetPowerCallback(ShutdownCB);

#else
	Uint8 *		g_nKeyStatus;
#endif

    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER  ) < 0 ) {
        exit(1);
    }

	if ( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, BITDEPTH,
			 (fullscreen ? SDL_FULLSCREEN : 0) | SDL_HWSURFACE | SDL_DOUBLEBUF ) < 0 ) {
		exit(1);
	}

	SDL_ShowCursor(0); // don't show
    SDL_WM_SetCaption( "Bash The Castle", 0 );

    cgame = 0;
	cgame = new castle_game;
	if (!cgame) exit(1); // really bad

    draw_rect(cgame->screen,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0); // clear backbuffer
    SDL_Flip(cgame->screen);

    unsigned long last_time, current_time;
    last_time = current_time = SDL_GetTicks();

    cgame->make_world(); // setup the world.
    cgame->simulate = false; // test

    g_running = true;

    while(g_running)
    {
        current_time = SDL_GetTicks();
        if ((current_time-last_time)< 16) // 60 FPS
        {
            SDL_Delay(2);
            continue;
        }

        last_time = current_time;

        loopi(MAX_KEYS)
        {
            g_keys_last_state[i] = g_real_keys[i];
            g_real_keys[i] = 0;
        }

#ifdef _WII_
		WPAD_ScanPads();
		WPAD_ReadPending(WPAD_CHAN_ALL, countevs);
		WPAD_Probe(WPAD_CHAN_ALL, &type);
		wd_one = WPAD_Data(0);
#else
		SDL_PumpEvents();
        g_nKeyStatus = SDL_GetKeyState(NULL);
        loopi(MAX_KEYS) g_real_keys[i] = g_nKeyStatus[i];
#endif
        translate_keys();

#ifdef _WII_

        if (wd_one->ir.valid)
        {
            event.motion.x = wd_one->ir.x;
            event.motion.y = wd_one->ir.y;

            if(g_real_keys[SDLK_a] && !g_keys_last_state[SDLK_a]){  // (A) Mapped to left mouse
                event.type = SDL_MOUSEBUTTONDOWN;
                event.button.state = SDL_PRESSED;
                event.button.button = SDL_BUTTON_LEFT;
            }else if (g_real_keys[SDLK_a] && g_real_keys[SDLK_a]){
                event.type = SDL_MOUSEBUTTONUP;
                event.button.state = SDL_RELEASED;
                event.button.button = SDL_BUTTON_LEFT;
            }else if(g_real_keys[SDLK_b] && !g_keys_last_state[SDLK_b]){  // (B) Mapped to right mouse
                event.type = SDL_MOUSEBUTTONDOWN;
                event.button.state = SDL_PRESSED;
                event.button.button = SDL_BUTTON_RIGHT;
            }else if (!g_real_keys[SDLK_b] && g_keys_last_state[SDLK_b]) {
                event.type = SDL_MOUSEBUTTONUP;
                event.button.state = SDL_RELEASED;
                event.button.button = SDL_BUTTON_RIGHT;
            }

        }else{

            // place the cursor off screen so it does not act laggy, when infact it's just got negitive co-ords
            event.motion.x = 1000;
            event.motion.y = 1000;
        }
#else
        SDL_PollEvent( &event );
#endif

        cgame->update(SDL_GetTicks());
    }

    // bye, bye
    delete cgame;

    SDL_Quit();

	return 0;
}
