#include "globals.h"
#include "castle_game.h"
#include "castle_block.h"
#include "castle_rocks.h"
#include "castle_character.h"
#include "castle_collide.h"
#include "castle_trebuchet.h"
#include "castle_gui_main.h"
#include "castle_gui_about.h"
#include "castle_gui_level.h"
#include "castle_gui_win.h"
#include "castle_gui_fail.h"
#include "castle_gui_exit.h"
#include "castle_gui_load.h"
#include "castle_gui_options.h"
#include "castle_gui_game.h"
#include "castle_level.h"
#include "fonts.h"


castle_game::castle_game() :
tx(0), snd(0), background(0), treb(0), current_level(1),
current_bg(1), simulate(false), win_time(0),num_rocks(0), total_rocks(6),
bbuton_time(0), bbuton_time_start(0), cursor(0), selection(0), play_music(true),
play_sounds(true), save_counter(0)
{
    screen = SDL_GetVideoSurface();
    if (!screen) exit(1);

    tx = new texture_cache;
    snd = new sound_cache;
    fnts = new fonts();


    castle_gui_main* gui_main = new castle_gui_main(this);
    castle_gui_about* gui_about = new castle_gui_about(this);
    castle_gui_level* gui_lvl = new castle_gui_level(this);
    castle_gui_win* gui_win = new castle_gui_win(this);
    castle_gui_fail* gui_fail = new castle_gui_fail(this);
    castle_gui_exit* gui_exit = new castle_gui_exit(this);
    castle_gui_load* gui_load = new castle_gui_load(this,make_path("saves/"));
    castle_gui_options* gui_opts = new castle_gui_options(this);
    castle_gui_game* gui_game = new castle_gui_game(this);

    // store pointers
    loopi(GS_MAX) guis[i] = 0;

    guis[GS_TITLE] = gui_main;
    guis[GS_ABOUT] = gui_about;
    guis[GS_LEVELEDIT] = gui_lvl;
    guis[GS_WIN] = gui_win;
    guis[GS_FAIL] = gui_fail;
    guis[GS_EXIT] = gui_exit;
    guis[GS_LOADGAME] = gui_load;
    guis[GS_OPTIONS] = gui_opts;
    guis[GS_GAMEOPTIONS] = gui_game;

    level = new castle_level(this);


    camera_x = 0;

    // b button
  //  bbutton = tx->texture_lookup("imgs/gui/bbutton.png");
    //selection dlg

    selection = tx->texture_lookup("imgs/gui/select.png");

    load_options();

    status = GS_TITLE; // Title screen

};

castle_game::~castle_game()
{
    save_options();

    // clear current data
    loopi(ents.size())
    {
        delete ents[i];
        ents[i] = 0;
    }

    if (level) delete level;
    loopi(GS_MAX) if (guis[i]) delete guis[i];
    if(fnts) delete fnts;
    if (treb) delete treb;
    if (snd) delete snd;
    if (tx) delete tx;

};


float castle_game::realx_2_screen(float x)
{
	return (float)x*SCALE;
}

float castle_game::realy_2_screen(float y)
{
    return (float)y*(-SCALE);
}

float castle_game::screenx_2_real(int x) { return x/SCALE; };
float castle_game::screeny_2_real(int y) { return y/(-SCALE); };

 // world and ground set up
void castle_game::make_world(void)
{
    b2AABB worldAABB;
    worldAABB.lowerBound.Set(-2000.0f, -2000.0f);
    worldAABB.upperBound.Set(2000.0f, 2000.0f);

    b2Vec2 gravity(0.0f, -55.0f);
    bool doSleep = false;
    world = new b2World(worldAABB, gravity, doSleep);

    // collision listener ...
    world->SetContactListener(&collision_listener);


    // The ground
    b2BodyDef gbd;
    gbd.linearDamping = 3.9f;
    gbd.angularDamping = 3.9f;
    gbd.position.Set(0.0f, -91.0f);
    gbd.userData = 0;
    ground = world->CreateBody(&gbd);

    b2PolygonDef gsd;
    ground_w = 400.0f;
    gsd.SetAsBox(ground_w, 10.0f);
    ground->CreateShape(&gsd);

};

void castle_game::load_level(char* name,bool have_ext = false)
{
    clear_ents();

    // ----- test, needs to be read from array or level file!!!
    char current[255] = {0};

    // for preset levels ... conditional if user made level
    if (!have_ext) sprintf(current,"levels/%s.btc",name);
    else sprintf(current,"levels/%s",name);

    level->load_level(current);

};

// remove any nulls
void castle_game::reorder_ents()
{
    bool done = false;
    int size = ents.size() - 1;
    int i = 0;

    while(!done)
    {
        size = ents.size() - 1;

        for(i = 0 ; i < ents.size(); i++)
        {
            if (ents[i] == 0)
            {
                ents.erase(ents.begin()+i);
                break;
            }
        }

        if (i>=size) done = true;
    }

};

void castle_game::clear_ents()
{
    // clear current data
    loopi(ents.size())
    {
        delete ents[i];
        ents[i] = 0;
    }
    ents.resize(0);

    if (treb)
    {
        delete treb;
        treb = 0;
    }

};

void castle_game::reset()
{
    char filename[255] = {0};
    sprintf(filename,"level%d",current_level);

    load_level(filename);

    if (!treb)
        treb = new castle_trebuchet(this);

    treb->status = TREB_PREVIEW;//TREB_LOADING;
};


float castle_game::get_px_y(b2Vec2* v)
{ // note: y is negitive
    float v1, v2;

    v[0].y > v[1].y ? v1 = v[0].y : v1 = v[1].y;
    v[2].y > v[3].y ? v2 = v[2].y : v2 = v[3].y;

    return v1 > v2 ? v1 : v2;
};

float castle_game::get_px_x(b2Vec2* v)
{
    float v1, v2;

    v[0].x < v[1].x ? v1 = v[0].x : v1 = v[1].x;
    v[2].x < v[3].x ? v2 = v[2].x : v2 = v[3].x;

    return v1 < v2 ? v1 : v2;
};

void castle_game::drawB2image(castle_ent* blk,b2Body* body)
{

    double rotation = 0.0f;
    SDL_Surface* rot_surface;
    SDL_Rect rot_rect;

    // calculate the image coords based on the bb.
    b2Shape* shape = body->GetShapeList();
    b2XForm xf = body->GetXForm();
    b2Vec2 vs[4];

    if(shape->GetType() == e_polygonShape)
    {

        b2PolygonShape* poly = (b2PolygonShape*)shape;
        const b2OBB& obb = poly->GetOBB();
        b2Vec2 h = obb.extents;

        vs[0].Set(-h.x, -h.y);
        vs[1].Set( h.x, -h.y);
        vs[2].Set( h.x,  h.y);
        vs[3].Set(-h.x,  h.y);

        loopi(4)
        {
            vs[i] = obb.center + b2Mul(obb.R, vs[i]);
            vs[i] = b2Mul(xf, vs[i]);
        }
    }else if(shape->GetType() == e_circleShape){

        b2CircleShape* circle = (b2CircleShape*)shape;
        float r  = circle->GetRadius();

        vs[0].Set(-r, -r);
        vs[1].Set( r, -r);
        vs[2].Set( r,  r);
        vs[3].Set(-r,  r);

        loopi(4)
        {
            vs[i] = b2Mul(xf, vs[i]);
        }

    }



    rot_rect.x = camera_x+realx_2_screen(get_px_x(vs));
    rot_rect.y = realy_2_screen(get_px_y(vs));
    rot_rect.h = 0;
    rot_rect.w = 0;

    if ((rot_rect.x + blk->image_data->w < 0)
        || rot_rect.x > SCREEN_WIDTH) return; // offscreen

     // calculate angle
    rotation = (double)body->GetAngle() / M_PI * 180.0;
    rot_surface = rotozoomSurface(blk->image_data,rotation,1,0);
    if (!rot_surface) return;
    // Draw to screen
    SDL_BlitSurface(rot_surface,0,screen,&rot_rect);
    SDL_FreeSurface(rot_surface);

};

void castle_game::drawB2Shape(b2Body* body, b2Shape* shape)
{

    b2XForm xf = body->GetXForm();

    if(shape->GetType() == e_polygonShape)
    {
        b2PolygonShape* poly = (b2PolygonShape*)shape;
        int32 vertexCount = poly->GetVertexCount();
        const b2Vec2* localVertices = poly->GetVertices();

        b2Assert(vertexCount <= b2_maxPolygonVertices);
        b2Vec2 vertices[b2_maxPolygonVertices];

        int32 i;
        for (i = 0; i < vertexCount; ++i)
            vertices[i] = b2Mul(xf, localVertices[i]);

        for (i = 0; i < vertexCount-1; ++i)
            lineColor(screen,camera_x+realx_2_screen(vertices[i].x), realy_2_screen(vertices[i].y), camera_x+realx_2_screen(vertices[i+1].x), realy_2_screen(vertices[i+1].y), 0xffffffff);

        lineColor(screen,camera_x+realx_2_screen(vertices[i].x), realy_2_screen(vertices[i].y), camera_x+realx_2_screen(vertices[0].x), realy_2_screen(vertices[0].y), 0xffffffff);
    }
    else if(shape->GetType() == e_circleShape)
    {
        b2CircleShape* circle = (b2CircleShape*)shape;
        const float32 k_segments = 16.0f;
        const float32 k_increment = 2.0f * b2_pi / k_segments;
        float32 theta = 0.0f;
        float32 radius = circle->GetRadius();
        b2Vec2 center(xf.position.x, xf.position.y);
        for (int32 i = 0; i < k_segments; ++i)
        {
            b2Vec2 v1 = center + radius * b2Vec2(cosf(theta), sinf(theta));
            v1.x = camera_x+realx_2_screen(v1.x);
            v1.y = realy_2_screen(v1.y);
            theta += k_increment;
            b2Vec2 v2 = center + radius * b2Vec2(cosf(theta), sinf(theta));
            v2.x = camera_x+realx_2_screen(v2.x);
            v2.y = realy_2_screen(v2.y);

            lineColor(screen,v1.x, v1.y, v2.x, v2.y, 0x000000ff);
        }
    }
};

int castle_game::get_chars_alive()
{
    int alive = 0;

    loopi(ents.size())
        if ((ents[i]->type == TYPE_CHAR) && (ents[i]->status == ALIVE)) alive++;

    return alive;

};

// options
void castle_game::load_options()
{

    FILE* f = fopen(make_path("btcopts.dat"),"rb");
    if(!f) return;

    int rversion, rvolume, rplaymus, rplaysound, rsavecounter;

    fread(&rversion,sizeof(int),1,f);
    fread(&rvolume,sizeof(int),1,f);
    fread(&rplaymus,sizeof(int),1,f);
    fread(&rplaysound,sizeof(int),1,f);
    fread(&rsavecounter,sizeof(int),1,f);

    fclose(f);

#ifdef _WII_
    rversion = swap4(rversion);
    rvolume = swap4(rvolume);
    rplaysound = swap4(rplaysound);
    rsavecounter = swap4(rsavecounter);
#endif

    // set the vars
    snd->can_play_music = rplaymus;
    snd->can_play_sounds = rplaysound;
    save_counter = rsavecounter;
    snd->volume = rvolume;
};

void castle_game::save_options()
{
    FILE* f = fopen(make_path("btcopts.dat"),"wb");
    if(!f) return;

    int rversion, rvolume, rplaymus, rplaysound, rsavecounter;

    // set the vars
    rversion = GAME_VERSION;
    rplaymus = snd->can_play_music;
    rplaysound = snd->can_play_sounds;
    rsavecounter = save_counter;
    rvolume = snd->volume;
#ifdef _WII_
    rversion = swap4(rversion);
    rvolume = swap4(rvolume);
    rplaymus = swap4(rplaymus);
    rplaysound = swap4(rplaysound);
    rsavecounter = swap4(rsavecounter);
#endif

    fwrite(&rversion,sizeof(int),1,f);
    fwrite(&rvolume,sizeof(int),1,f);
    fwrite(&rplaymus,sizeof(int),1,f);
    fwrite(&rplaysound,sizeof(int),1,f);
    fwrite(&rsavecounter,sizeof(int),1,f);

    fclose(f);


};

int castle_game::load_save(char* name)
{
    char savename[255] = {0};
    sprintf(savename,"saves/%s",name);


    FILE* f = fopen(make_path(savename),"rb");
    if(!f) return 2;

    int rversion, rlevel, rscore;

    fread(&rversion,sizeof(int),1,f);
    fread(&rlevel,sizeof(int),1,f);
    fread(&rscore,sizeof(int),1,f);

    fclose(f);

#ifdef _WII_
    rversion = swap4(rversion);
    rlevel = swap4(rlevel);
    rscore = swap4(rscore);
#endif

    // set the games vars
    current_level = rlevel;
    score = rscore;

    return 1;

}

int castle_game::create_save()
{
    save_counter++;

    char savename[255] = {0};
    sprintf(savename,"saves/save%d.bts",save_counter);


    FILE* f = fopen(make_path(savename),"wb");
    if(!f) return 2;

    int version = GAME_VERSION;
    int rversion, rlevel, rscore;

    rversion = version;
    rscore = start_score;
    rlevel = current_level;

#ifdef _WII_
    rversion = swap4(rversion);
    rlevel = swap4(rlevel);
    rscore = swap4(rscore);
#endif

    fwrite(&rversion,sizeof(int),1,f);
    fwrite(&rlevel,sizeof(int),1,f);
    fwrite(&rscore,sizeof(int),1,f);

    fclose(f);

    return 1;
};

void castle_game::update(unsigned long ms)
{
    const float32 timeStep          = 1.0f / 60.0f;
    const int32 iterations          = 10;

    // Simulation stuff
    if (simulate)
    {
        world->Step(timeStep, iterations);
        world->Validate();
    }

    // Status
    switch(status)
    {

        case GS_NEWGAME_FROM_LOAD:
            can_ply_snd = true;
            num_rocks = win_time = 0;
            reset();
            snd->play_sound("sound/level-start.wav");
            start_score = score;
            status = GS_PLAYING;
        break;

        case GS_LOADGAME: // todo

            guis[GS_LOADGAME]->render();
            // clicked
            status = guis[GS_LOADGAME]->hit_test(status);

            if (status == MESSAGE_1) status = GS_TITLE;

            if (status == MESSAGE_2) // load the save
            {
                // TODO
                char* fn = guis[GS_LOADGAME]->selected;
                if (!fn)
                {
                    status = GS_TITLE;
                    return;
                }

                load_save(fn);

                status = GS_NEWGAME_FROM_LOAD;

            }

        break;

        case GS_OPTIONS:

            // render
            guis[GS_OPTIONS]->render();
            // clicked
            status = guis[GS_OPTIONS]->hit_test(status);

        break;

        case GS_EXIT:

            render(); // render loop

            // ents
            loopi(ents.size()) ents[i]->update();
            // Trebuchet

            treb->update(ms);

            simulate = false;

            // level editor render
            guis[GS_EXIT]->render();

            // clicked
            status = guis[GS_EXIT]->hit_test(status);

        break;

        case GS_TITLE:
            simulate = false;
            // music for title screen
            snd->play_music("sound/main-menu-music.ogg");

            // render title
            guis[GS_TITLE]->render();

            // clicked
            status = guis[GS_TITLE]->hit_test(status);
            if (status==GS_PLAYING) reset();

        break;

        case GS_NEXT_LVL:
            can_ply_snd = true;
            current_level++;
            num_rocks = win_time = 0;
            reset();
            snd->play_sound("sound/level-start.wav");
            start_score = score;
            status = GS_PLAYING;
        break;

        case GS_RETRY: // same as GS_NEXT_LVL but don't inc current_level
            can_ply_snd = true;
            num_rocks = win_time = 0;
            reset();
            snd->play_sound("sound/level-start.wav");
            status = GS_PLAYING;
        break;

        case GS_FAIL: // failed the level
           // wait a while before popping dialog
            if (!win_time)
            {
                win_time = ms;
            }else{
                // game render, bg etc
                render(); // render loop

                // ents
                loopi(ents.size()) ents[i]->update();
                // Trebuchet
                treb->update(ms);
                if ((ms - win_time) > 10000) // 10 sec wait if going to fail!
                {
                    if (can_ply_snd)
                    {
                        snd->play_sound("sound/fail.wav");
                        can_ply_snd = false;
                    }

                    simulate = false;

                    // level editor render
                    guis[GS_FAIL]->render();

                    // clicked
                    status = guis[GS_FAIL]->hit_test(status);

                }
            }
        break;

        case GS_WIN:

            // wait a while before popping dialog
            if (!win_time)
            {
                win_time = ms;
            }else{

                // game render, bg etc
                render(); // render loop

                // ents
                loopi(ents.size()) ents[i]->update();

                if ((ms - win_time) > 2000) // 2 sec wait
                {
                    if (can_ply_snd)
                    {
                        snd->play_sound("sound/level-finish-success.wav");
                        can_ply_snd = false;
                    }

                    simulate = false;

                    // level editor render
                    guis[GS_WIN]->render();

                    // clicked
                    status = guis[GS_WIN]->hit_test(status);

                }
            }
        break;

        case GS_NEWGAME:
            num_rocks = score = 0;
            current_level = 0;
            status = GS_NEXT_LVL;
        break;

        case GS_PLAYING:

            if (g_real_keys[SDLK_ESCAPE])
            {
                status = GS_EXIT;
                return;
            }

            // music test
            snd->play_music("sound/level-background-noise.ogg");

            // game render, bg etc
            render(); // render loop

            // ents
            loopi(ents.size()) ents[i]->update();


            if (treb->status == TREB_READY)
            {
                guis[GS_GAMEOPTIONS]->render();
                if (guis[GS_GAMEOPTIONS]->hit_test(status))
                {
                     g_real_keys[SDLK_b] = 0;
                }
            }

            // Trebuchet
            treb->update(ms);

            // killed all characters
            if (get_chars_alive() <= 0) status = GS_WIN;
            if ((num_rocks >= total_rocks) && treb->status == TREB_CANRELOAD) status = GS_FAIL;

            simulate = true;

        break;

        case GS_LEVELEDIT_TEST_CLOSE:

             if (treb)
             {
                 delete treb;
                 treb = 0;
             }

             status = GS_LEVELEDIT;
        break;

        case GS_LEVELEDIT_TEST_INIT:

            if (!treb) treb = new castle_trebuchet(this);
            treb->status = TREB_PREVIEW;//TREB_LOADING;

            status = GS_LEVELEDIT_TEST;
        break;

        case GS_LEVELEDIT_TEST:
            // game render, bg etc
            render(); // render loop

            // ents
            loopi(ents.size()) ents[i]->update();


            if (treb->status == TREB_READY)
            {
                guis[GS_GAMEOPTIONS]->render();
                if (guis[GS_GAMEOPTIONS]->hit_test(status))
                {
                     g_real_keys[SDLK_b] = 0;
                }
            }

            // Trebuchet
            treb->update(ms);

            simulate = true;

            // level editor update
            guis[GS_LEVELEDIT]->render();

            // clicked
            status = guis[GS_LEVELEDIT]->hit_test(status);

        break;

        case GS_LEVELEDIT:
            camera_x = -MAX_SCREEN_X; // no scrolling

            // game render, bg etc
            render(); // render loop

            // ents
            loopi(ents.size()) ents[i]->update();

            // level editor render
            guis[GS_LEVELEDIT]->render();

            // clicked
            status = guis[GS_LEVELEDIT]->hit_test(status);


        break;

        case GS_ABOUT:
            // render
            guis[GS_ABOUT]->render();
            // clicked
            status = guis[GS_ABOUT]->hit_test(status);
        break;

    }

    render_overlay();

    SDL_Flip(screen); // flip buffer
};

void castle_game::render_overlay()
{
     // -- helper

    if (treb)
    {

#ifdef _WII_
/* REMOVED --- ANNOYING AFTER A WHILE
        SDL_Rect ds = {0,0,0,0};

        unsigned long timer = SDL_GetTicks();

        if (status == GS_PLAYING &&
            (treb->status == TREB_READY ||
            treb->status == TREB_LAUNCHING ||
            treb->status == TREB_PREVIEW ||
            treb->status == TREB_CANRELOAD)
            )
        {
            if ((timer-bbuton_time) > HALF_SECOND)
            {
                if (!bbuton_time_start) bbuton_time_start = timer;

                ds.x = (SCREEN_WIDTH - 100) - (bbutton->w / 2);
                ds.y = (SCREEN_HEIGHT / 2) - (bbutton->h / 2);
                SDL_BlitSurface(bbutton,0,screen,&ds);

                if ((timer-bbuton_time_start) > HALF_SECOND)
                {
                    bbuton_time = timer;
                    bbuton_time_start = 0;
                }


            }
        }
*/
#endif
        if (treb->status == TREB_READY && status == GS_PLAYING)
        {
            char txt[255] = {0};
            sprintf(txt,"Score: %d",score);
            fnts->change_color(17,33,13);
            fnts->text(screen,txt,450,20,400);
            sprintf(txt,"Shots Left: %d",total_rocks-num_rocks);
            fnts->text(screen,txt,450,40,400);
            sprintf(txt,"Level: %d",current_level);
            fnts->text(screen,txt,450,60,400);

            draw_cursor(event.motion.x,event.motion.y);
        }
    }

    // -- Always draw on top
    if (status != GS_PLAYING) draw_cursor(event.motion.x,event.motion.y);
};

void castle_game::render()
{

    // background first
    SDL_Rect ds = {0,0,0,0};
    SDL_Rect sr = {0,0,640,480};

    if (current_bg == 1) background = tx->texture_lookup("imgs/bg/bg1.bmp");
    else if (current_bg == 2) background = tx->texture_lookup("imgs/bg/bg2.bmp");
    else if (current_bg == 3) background = tx->texture_lookup("imgs/bg/bg3.bmp");

    // buffer 1 / buffer 3
    ds.x = camera_x;
    if (camera_x < -SCREEN_WIDTH) ds.x = camera_x + SCREEN_WIDTH * 2;

    if (ds.x > 0) ds.w = 640 - ds.x;
    SDL_BlitSurface(background,&sr,screen,&ds);

    // buffer 2
    ds.x = camera_x + SCREEN_WIDTH;
    if (ds.x > 0) ds.w = 640 - ds.x;
    if (ds.x > -SCREEN_WIDTH) SDL_BlitSurface(background,&sr,screen,&ds);

    // --- eof background !

};

void castle_game::draw_cursor(int x,int y)
{
    if (!cursor) cursor = tx->texture_lookup("imgs/gui/cursor.png");

    SDL_Rect r = { x,y,cursor->w,cursor->h };
    SDL_BlitSurface( cursor,0, screen,&r);

};
