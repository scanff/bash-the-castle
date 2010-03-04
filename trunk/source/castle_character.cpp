#include "globals.h"
#include "castle_game.h"
#include "castle_character.h"
#include "castle_trebuchet.h"
castle_character::~castle_character()
{
    wld->world->DestroyBody(body);
    body = 0;
};

castle_character::castle_character(castle_game* cg, int x, int y, int t)
: wld(cg), x_px(x), y_px(y)
{
    cgame = cg;
    world = cg->world;
    type = TYPE_CHAR;
    status = ALIVE;
    sub_type = t;
    r_x = x - MAX_SCREEN_X;
    r_y = y;

    float sw, sh, sx, sy;

    switch(t)
    {

        case CHAR_KING:

            image_data = image_char_alive = wld->tx->texture_lookup("imgs/sprites/king.png");
            image_char_dead = wld->tx->texture_lookup("imgs/sprites/kingd.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);

        break;
        case CHAR_QUEEN:

            image_data = image_char_alive = wld->tx->texture_lookup("imgs/sprites/queen.png");
            image_char_dead = wld->tx->texture_lookup("imgs/sprites/queend.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);

        break;
        case CHAR_JESTER:

            image_data = image_char_alive = wld->tx->texture_lookup("imgs/sprites/jester.png");
            image_char_dead = wld->tx->texture_lookup("imgs/sprites/jesterd.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);

        break;
        case CHAR_SOLDIER:

            image_data = image_char_alive = wld->tx->texture_lookup("imgs/sprites/soldier.png");
            image_char_dead = wld->tx->texture_lookup("imgs/sprites/soldierd.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);

        break;

        case CHAR_PRINCESS:
            image_data = image_char_alive = wld->tx->texture_lookup("imgs/sprites/princess.png");
            image_char_dead = wld->tx->texture_lookup("imgs/sprites/princessd.png");
            sw = wld->screenx_2_real(image_data->w)/2;
            sh = wld->screenx_2_real(image_data->h)/2;

            sd.SetAsBox(sw, sh);
        break;
    }

    sd.density = 0.5f;
    sd.friction = 2.1f;
    sx = wld->screenx_2_real(x);
    sy = wld->screeny_2_real(y);


    b2BodyDef bd;
    bd.allowSleep = true;
    bd.position.Set(sx, sy);
    bd.userData = this;

    body = wld->world->CreateBody(&bd);
    body->CreateShape(&sd);
    body->SetMassFromShapes();

    // add to game
    cg->ents.push_back(this);


};

void castle_character::update()
{
    if (body->GetPosition().x > cgame->ground_w ||
        body->GetPosition().x < 0.0f
        )
    {
        status == DEAD;
        return;
    }

    if (status == ALIVE)
    {
        image_data = image_char_alive;
    }
    else if (status == HIT)
    {
        if (!cgame->treb)
        {
            status = ALIVE;
            return;
        }
        // can't die in these treb states
        if (cgame->treb->status == TREB_PREVIEW ||
            cgame->treb->status == TREB_LOADING ||
            cgame->treb->status == TREB_LAUNCHING)
        {
            status = ALIVE;
            return;
        }

        image_data = image_char_dead;
        switch(sub_type)
        {
            case CHAR_KING:
                wld->snd->play_sound("sound/male1.wav");
            break;
            case CHAR_QUEEN:
                wld->snd->play_sound("sound/female1.wav");
            break;
            case CHAR_JESTER:
                wld->snd->play_sound("sound/male2.wav");
             break;
            case CHAR_SOLDIER:
                wld->snd->play_sound("sound/male1.wav");
            break;
            case CHAR_PRINCESS:
                wld->snd->play_sound("sound/female2.wav");
            break;
        };
        wld->score += 10; // increase score
        status = DEAD;
    }else if (status == DEAD){
        image_data = image_char_dead;
    }

    //wld->drawB2Shape(body,body->GetShapeList());
    wld->drawB2image(this,body);
};
