#include "globals.h"
#include "castle_game.h"
#include "castle_trebuchet.h"
#include "castle_rocks.h"

#define TIMEOUT (7000)
#define SPREAD  (2.5f)
#define SCROLL_SPEED (10)
castle_trebuchet::castle_trebuchet(castle_game* cg) : cgame(cg)
, body_frame(0), body_arm(0), body_counter(0), joint_armbody(0),
joint_armcounter(0), num_ammo(1), project_type(0)
{
    loopi(MAX_LOAD) joint_rope[i] = 0;
    float sw, sh, sx, sy;

    image_data_arm = cgame->tx->texture_lookup("imgs/sprites/arm.png");
    image_data_frame = cgame->tx->texture_lookup("imgs/sprites/treb_body.png");
    image_data_cweight = cgame->tx->texture_lookup("imgs/sprites/counterw.png");

    // just world co-ords as user never positions the trebuchet

    b2BodyDef bd;
    bd.allowSleep = true;
    bd.userData = 0; // Nothing!

    sd.filter.categoryBits = 0x0002;
    sd.filter.maskBits = 0x0001;

    // trebuchet arm
    sd.density = 30.0f;
    sd.friction = 0.2f;
    sd.restitution = 0.4f;
    sd.SetAsBox(18.60f, 1.10f);
    bd.position.Set(38.59f, -61.90f);
    bd.angle = DEGTORAD(65);
    body_arm = cgame->world->CreateBody(&bd);
    body_arm->CreateShape(&sd);
    body_arm->SetMassFromShapes();

  // trebuchet body
    sd.density = 0;//static ....
    sd.friction = 0.2;
    sd.restitution = 0.4;
    sd.SetAsBox(2.0f, 5.0f);
    bd.position.Set(42.0f, -62.0f);
    bd.angle = DEGTORAD(0);
    body_frame = cgame->world->CreateBody(&bd);
    body_frame->CreateShape(&sd);
    body_frame->SetMassFromShapes();

    // treb counter weight
    sd.density = 10.01f;
    sd.friction = 5.2;
    sd.restitution = 0.1;
    bd.angularDamping = 10.0f; // stop the counter weight from spinning around :)
    sd.SetAsBox(5.0f, 5.0f);
    bd.position.Set(44.5f, -51.0f);
    body_counter = cgame->world->CreateBody(&bd);
    body_counter->CreateShape(&sd);
    body_counter->SetMassFromShapes();

    // store the default values
    counter_pos.x = 44.5f;
    counter_pos.y = -51.0f;
    counter_angle = 0;

    // joints
    b2RevoluteJointDef jointDef;
    b2Vec2 pos;

    // ARM and BODY
    pos = body_frame->GetPosition();
    jointDef.Initialize(body_frame,body_arm, b2Vec2(pos.x,pos.y+5.5f));
    jointDef.collideConnected = false;
    jointDef.enableLimit = true;
    jointDef.lowerAngle = DEGTORAD(-200);
    jointDef.upperAngle = DEGTORAD(-20);

    // create the arm/body joint
    joint_armbody = cgame->world->CreateJoint(&jointDef);


    // ARM and COUNTER W

    pos = body_arm->GetPosition();
    jointDef.Initialize(body_arm,body_counter, b2Vec2(pos.x+6.5,pos.y+16.0));
    jointDef.collideConnected = false;
    jointDef.enableLimit = false;
    joint_armcounter = cgame->world->CreateJoint(&jointDef);

    // rope anchor pos on arm
    pos = body_arm->GetPosition();
    pos.y -= 9.5f;
    pos.x -= 13.5f;

    rope_pos = pos;

    loopi(MAX_LOAD) current_ammo[i] = 0;

    status = TREB_LOADING;

};

castle_trebuchet::~castle_trebuchet()
{
    if (joint_armbody)
    {
        cgame->world->DestroyJoint(joint_armbody);
        joint_armbody = 0;
    }

    if (joint_armcounter)
    {
        cgame->world->DestroyJoint(joint_armcounter);
        joint_armcounter = 0;
    }

    if (body_arm)
    {
        cgame->world->DestroyBody(body_arm);
        body_arm = 0;
    }

    if (body_frame)
    {
        cgame->world->DestroyBody(body_frame);
        body_frame = 0;
    }

};

void castle_trebuchet::render()
{

    /*  -- DEBUG LINES
    cgame->drawB2Shape(body_frame,body_frame->GetShapeList());
    cgame->drawB2Shape(body_arm,body_arm->GetShapeList());
    cgame->drawB2Shape(body_counter,body_counter->GetShapeList());
    */

    // rope line
    if (status == TREB_READY || status == TREB_LAUNCHING)
    {
        loopi(num_ammo)
        {
            if (joint_rope[i])
            {
                b2Vec2 s = joint_rope[i]->GetAnchor1();
                b2Vec2 d = joint_rope[i]->GetAnchor2();
                lineColor(cgame->screen,s.x*SCALE, s.y*-SCALE, d.x*SCALE, d.y*-SCALE, 0x512C01ff);
            }
        }
    }

    // counter
    image_data = image_data_cweight;
    cgame->drawB2image(this,body_counter);

    // arm
    image_data = image_data_arm;
    cgame->drawB2image(this,body_arm);

    // body .. basic overlay
    SDL_Rect ds = {cgame->camera_x +60,240,0,0};
    SDL_BlitSurface(image_data_frame,0,cgame->screen,&ds);

   /* -- DEBUG LINES
    b2Vec2 s = joint_armbody->GetAnchor1();
    b2Vec2 d = joint_armbody->GetAnchor2();
    lineColor(cgame->screen,s.x*SCALE, s.y*-SCALE, d.x*SCALE, d.y*-SCALE, 0xff0000ff);
    b2Vec2 s = joint_armcounter->GetAnchor1();
    b2Vec2 d = joint_armcounter->GetAnchor2();
    lineColor(cgame->screen,s.x*SCALE, s.y*-SCALE, d.x*SCALE, d.y*-SCALE, 0xff0000ff);
    */
};

// reload with new rock type
void castle_trebuchet::load_rocks(int newtype)
{
    //remove current rocks from game
    loopi(cgame->ents.size())
    {
        loopj(num_ammo)
        {
            if (current_ammo[j] == cgame->ents[i])
            {
                delete cgame->ents[i];
                cgame->ents[i] = 0;
                current_ammo[j] = 0;
            }
        }
    }
    // reorder cgame->ents
    cgame->reorder_ents();

    project_type = newtype;

    if (project_type == PROJECT_BIG_ROCK ||
        project_type == PROJECT_BOMB)
        num_ammo = 1;
    else if (project_type == PROJECT_SMALL_ROCK) num_ammo = 3;

    status = TREB_LOADING;
};

void castle_trebuchet::update(unsigned long ms)
{
    switch(status)
    {
        case TREB_PREVIEW:
        preview();
        break;
        case TREB_LOADING:
        start();
        break;
        case TREB_READY:
        ready();
        break;
        case TREB_LAUNCHING:
        launching();
        break;
        case TREB_FIRE:
        fire(ms);
        break;
        case TREB_DONE:
        done(ms);
        break;
        case TREB_CANRELOAD:
        can_reload();
        break;
        case TREB_FAIL:
        fail();
        break;
    };

    // trebuchet render
    render();
};

void castle_trebuchet::can_reload()
{
    if (g_real_keys[SDLK_b] && !g_keys_last_state[SDLK_b])
    {
        status = TREB_LOADING;
    }
}

//failed to launch in time
void castle_trebuchet::fail()
{
    float y = RADTODEG(body_arm->GetAngle());

    if (y < -100.0f)
        body_arm->SetAngularVelocity(1.0f); // push back

    if  (((SDL_GetTicks()-start_time) > 2000) // time out 2 secs
        ) // done .. ?
    {
        status = TREB_CANRELOAD;
    }
};

void castle_trebuchet::done(unsigned long ms)
{
    int x = cgame->camera_x;

    if (x > -MAX_SCREEN_X) cgame->camera_x-=SCROLL_SPEED;
    else x = -MAX_SCREEN_X;

    if (x < -SCREEN_WIDTH)
    {
         //change the rocks collision group
        loopi(num_ammo)
        {
            if (current_ammo[i]->status != DEAD)
            {
                b2Shape* s = current_ammo[i]->castle_ent::body->GetShapeList();

                b2FilterData cg = s->GetFilterData();
                cg.categoryBits = 0x0001;
                cg.maskBits = 0xFFFF;
                cg.groupIndex = 0;
                s->SetFilterData(cg);
                cgame->world->Refilter(s);
            }
        }

    }


    if  (((ms-start_time) > TIMEOUT) // time out 7 secs
        ) // done .. ?
    {
        status = TREB_CANRELOAD;
    }


};

void castle_trebuchet::fire(unsigned long ms)
{
    cgame->num_rocks++; // fired a rock

    if (current_ammo[0]->rock_type == PROJECT_BOMB)
        cgame->snd->play_sound("sound/hiss.wav");

    start_time = ms;
    float a = body_arm->GetAngularVelocity();
    b2Vec2 p,s;

    loopi(num_ammo)
    {
        float x_scale, y_scale;

        switch(project_type)
        {
            case PROJECT_BIG_ROCK:
                x_scale = 1.11f;
                y_scale = 1.0f;
            break;
            case PROJECT_SMALL_ROCK:
                x_scale = 0.91f;
                y_scale = 0.9f;
            break;
            case PROJECT_BOMB:
                x_scale = 0.91f;
                y_scale = 0.9f;
            break;

        }

        s = current_ammo[i]->body->GetPosition();
        current_ammo[i]->body->SetXForm(s,0);

        current_ammo[i]->body->SetAngularVelocity(a);
        p.x = current_ammo[i]->body->GetLinearVelocity().x * x_scale;
        p.y = current_ammo[i]->body->GetLinearVelocity().y * y_scale;

        current_ammo[i]->body->SetLinearVelocity(p);

        cgame->world->DestroyJoint(joint_rope[i]);
        joint_rope[i] = 0;
    }

    status = TREB_DONE;
}
void castle_trebuchet::launching()
{
    float y = RADTODEG(body_arm->GetAngle());

    if (y < -115.0f)
    {
        /* FAIL ! */
        b2Vec2 p;

        p.x = 0.0f;
        p.y = 10.0f;

        loopi(num_ammo)
        {

            //p.y = 10.0f + (rand() % 3);
            current_ammo[i]->body->SetLinearVelocity(p);

            if (joint_rope)
            {
                cgame->world->DestroyJoint(joint_rope[i]);
                joint_rope[i] = 0;
            }

        }

        cgame->num_rocks++; // oops
        cgame->snd->play_sound("sound/shootfail.wav");

        start_time = SDL_GetTicks();
        status = TREB_FAIL;

        return;

    }

    loopi(num_ammo)
    {
        if (i != 0)
        {
            b2Vec2 p = current_ammo[0]->body->GetPosition();
            p.y += (float)(i * SPREAD);
            float a = current_ammo[0]->body->GetAngle();
            current_ammo[i]->body->SetXForm(p,a);
        }
    }

    if (g_real_keys[SDLK_b] && !g_keys_last_state[SDLK_b])
        status = TREB_FIRE;
}

void castle_trebuchet::ready()
{
    // to do any prey launch stuff
    if (g_real_keys[SDLK_b] && !g_keys_last_state[SDLK_b])
    {
        cgame->snd->play_sound("sound/treb-launch.wav");
        status = TREB_LAUNCHING;
        body_arm->SetAngularVelocity(-5.00f);
    }
}

// preview the castle
void castle_trebuchet::preview()
{
    cgame->simulate = false;
    cgame->camera_x = -MAX_SCREEN_X;
    if (g_real_keys[SDLK_b] && !g_keys_last_state[SDLK_b])
        status = TREB_LOADING;

};

void castle_trebuchet::start()
{
    // reset trebuchet to start
    int a = (int)(RADTODEG(body_arm->GetAngle()));
    if (a != 45)
    {
        body_arm->SetXForm(body_arm->GetPosition(),DEGTORAD(45));
        return;
    }

    // move to treb.
    if (cgame->camera_x  > 0 || cgame->camera_x == 0)
    {
        cgame->camera_x = 0;

        loopi(num_ammo)
        {
            int t  = 0;
           // if (!current_ammo[i])
            {

                current_ammo[i] = new castle_rocks(cgame,0,0,project_type); // create rock

                // create joint between projectile and arm
                b2DistanceJointDef jointDef;
                jointDef.Initialize(body_arm, current_ammo[i]->body, rope_pos, current_ammo[i]->body->GetPosition());
                jointDef.collideConnected = false; // for now
                joint_rope[i] = cgame->world->CreateJoint(&jointDef);
            }
        }

        status = TREB_READY;

        cgame->simulate =false;
    }else{
        cgame->camera_x+=SCROLL_SPEED; // re-pos camera
    }
};
