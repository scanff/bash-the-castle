#include "globals.h"
#include "castle_game.h"
#include "castle_block.h"

castle_block::~castle_block()
{
    wld->world->DestroyBody(body);
    body = 0;

    srand(10000);
};

castle_block::castle_block(castle_game* cg, int x, int y, int t,float a)
: wld(cg), x_px(x), y_px(y)
{
    cgame = cg;
    angle = a;
    world = cg->world;
    type = TYPE_BLOCK; // caste_ent
    status = ALIVE;
    sub_type = t;
    r_x = x - MAX_SCREEN_X;
    r_y = y;

    float sw, sh, sx, sy;

    switch(t)
    {

        case WOOD_V:

            image_data = wld->tx->texture_lookup("imgs/sprites/wood1.png");

            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);
            sd.density = 8.5f;
            sd.friction = 3.1f;

        break;

        case WOOD_H:
            image_data = wld->tx->texture_lookup("imgs/sprites/wood2.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);
            sd.density = 8.0f;
            sd.friction = 4.1f;

        break;

         case WOOD_3:

            image_data = wld->tx->texture_lookup("imgs/sprites/wood3.png");

            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);
            sd.density = 9.5f;
            sd.friction = 1.1f;

        break;
        case ROCK_V:
            image_data = wld->tx->texture_lookup("imgs/sprites/rock_v.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);
            sd.density = 11.0f;
            sd.friction = 6.0f;

        break;

        case STUMP_V:
            image_data = wld->tx->texture_lookup("imgs/sprites/stump_v.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);
            sd.density = 1.50f;
            sd.friction = 1.1f;

        break;

        case ROOF_1:

            sd.density = 11.0f;
            sd.friction = 4.0f;

            image_data = wld->tx->texture_lookup("imgs/sprites/roof_1.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.vertexCount = 3;
            sd.vertices[0].Set(-sw, 0.0f);
            sd.vertices[1].Set(sw, 0.0f);
            sd.vertices[2].Set(0.0f, sh*2);


        break;

        case BARREL_1:
            image_data = wld->tx->texture_lookup("imgs/sprites/barrel1.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);
            sd.density = 1.4f;
            sd.friction = 0.1f;

        break;
    }


    sx = x/SCALE;
    sy = y/(-SCALE);


    b2BodyDef bd;
    bd.allowSleep = true;
    bd.position.Set(sx, sy);
    bd.userData = this;
    bd.angle = a;
    body = wld->world->CreateBody(&bd);
    body->CreateShape(&sd);
    body->SetMassFromShapes();

    // add to game
    cg->ents.push_back(this);
};



void castle_block::update()
{
    if (status == DEAD) return;

    if (body->GetPosition().x > cgame->ground_w ||
        body->GetPosition().x < 0.0f
        )
    {
        status = DEAD;
        return;
    }

   // wld->drawB2Shape(body,body->GetShapeList());
    wld->drawB2image(this,body);

    if (status == HIT)
    {
        int r = (int)(rand() % 4);

        switch(r)
        {
            case 0:
                cgame->snd->play_sound("sound/boom1.wav");
            break;
            case 1:
                cgame->snd->play_sound("sound/boom2.wav");
            break;
            case 2:
                cgame->snd->play_sound("sound/boom3.wav");
            break;
            case 3:
                cgame->snd->play_sound("sound/boom4.wav");
            break;
        }

        status = ALIVE;
    }
};
