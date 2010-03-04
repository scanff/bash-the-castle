#include "globals.h"
#include "castle_game.h"
#include "castle_ent.h"
#include "castle_rocks.h"

castle_rocks::~castle_rocks()
{
    wld->world->DestroyBody(body);
    body = 0;
};

castle_rocks::castle_rocks(castle_game* w, int x, int y,int rt) : wld(w)
{
    b2CircleDef     sd;
    b2BodyDef       bd;
    hit = 0;
    cgame = w;
    world = w->world;
    status = ALIVE;
    type = TYPE_ROCK;
    sub_type = rock_type = rt;
    switch(rock_type)
    {
        case PROJECT_BIG_ROCK: // 1 big

            image_data = wld->tx->texture_lookup("imgs/sprites/rock.png");

            sd.density = 5.5f; // pretty heavy
            sd.radius = (image_data->w/2)/SCALE;
            sd.restitution = 0.01; // not bouncy
            sd.friction = 50.0f;
            // same as trebuchet
            sd.filter.categoryBits = 0x0002;
            sd.filter.maskBits = 0x0001;

            bd.isBullet = true;
            bd.allowSleep = false;
            bd.position.Set(42.0f, -80.0f);

        break;

        case PROJECT_SMALL_ROCK: // small

            image_data = wld->tx->texture_lookup("imgs/sprites/rocksm.png");

            sd.density = 4.0f; // pretty heavy
            sd.radius = (image_data->w/2)/SCALE;
            sd.restitution = 0.01; // not bouncy
            sd.friction = 20.0f;
            // same as trebuchet
            sd.filter.categoryBits = 0x0002;
            sd.filter.maskBits = 0x0001;

            bd.isBullet = true;
            bd.allowSleep = false;
            bd.position.Set(42.0f, -80.0f);

        break;

          case PROJECT_BOMB: // bomb

            image_data = wld->tx->texture_lookup("imgs/sprites/bomb.png");

            sd.density = 4.0f; // pretty heavy
            sd.radius = (image_data->w/2)/SCALE;
            sd.restitution = 0.01; // not bouncy
            sd.friction = 20.0f;
            // same as trebuchet
            sd.filter.categoryBits = 0x0002;
            sd.filter.maskBits = 0x0001;

            bd.isBullet = true;
            bd.allowSleep = false;
            bd.position.Set(42.0f, -80.0f);


        break;
    }

    bd.userData = this;
    body = wld->world->CreateBody(&bd);
    body->CreateShape(&sd);
    body->SetMassFromShapes();

    // add to game
    w->ents.push_back(this);
};


void castle_rocks::update()
{

    if (status == DEAD) return;

    if (body->GetPosition().x > cgame->ground_w ||
        body->GetPosition().x < 0.0f
        )
    {
        status == DEAD;
        return;
    }

   // wld->drawB2Shape(body,body->GetShapeList());

    wld->drawB2image(this,body);

    if (status == HIT && rock_type != PROJECT_BOMB)
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
    else if (rock_type == PROJECT_BOMB)
    {

        // apply force to the block we hit
        if (hit)
        {
            if (hit->type != TYPE_BLOCK && hit->type != TYPE_CHAR) return;
            cgame->snd->stop_sound("sound/hiss.wav");
            cgame->snd->play_sound("sound/explode.wav");


            hit->body->ApplyImpulse(b2Vec2(7000,0),hit->body->GetWorldCenter());

            status = DEAD;
            // remove
            wld->world->DestroyBody(body);
            body = 0;
        }
    }




};
