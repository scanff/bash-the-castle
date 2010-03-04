#include "globals.h"
#include "castle_game.h"
#include "gui/gui_button.h"
#include "gui/gui_keyboard.h"
#include "castle_character.h"
#include "castle_block.h"
#include "castle_gui.h"
#include "castle_gui_level.h"
#include "castle_level.h"
#include "castle_gui_save.h"
#include "castle_gui_load.h"

castle_gui_level::castle_gui_level(castle_game* _g) :
    current(0), highlight(0), save(false),  save_gui(0), load_gui(0), load(false),play_level(false)
{
    cgame = _g;
    loopi(LB_MAX) btns[i] = 0;

    btns[LB_EXIT] = new gui_button(cgame,40,20,NULL,0);
    btns[LB_EXIT]->tx = cgame->tx;
    btns[LB_EXIT]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_EXIT]->set_text("Exit");
    btns[LB_EXIT]->font_sz = 1;
    btns[LB_EXIT]->pad_y = 8;
    btns[LB_EXIT]->pad_x = 10;
    btns[LB_EXIT]->text_color = 0xDDB90F;
    btns[LB_EXIT]->text_color_over = 0xffffff;

    btns[LB_SAVE] = new gui_button(cgame,180,20,NULL,0);
    btns[LB_SAVE]->tx = cgame->tx;
    btns[LB_SAVE]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_SAVE]->set_text("Save Level");
    btns[LB_SAVE]->font_sz = 1;
    btns[LB_SAVE]->pad_y = 8;
    btns[LB_SAVE]->pad_x = 10;
    btns[LB_SAVE]->text_color = 0xDDB90F;
    btns[LB_SAVE]->text_color_over = 0xffffff;


    btns[LB_CLEAR] = new gui_button(cgame,320,20,NULL,0);
    btns[LB_CLEAR]->tx = cgame->tx;
    btns[LB_CLEAR]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_CLEAR]->set_text("Clear All");
    btns[LB_CLEAR]->font_sz = 1;
    btns[LB_CLEAR]->pad_y = 8;
    btns[LB_CLEAR]->pad_x = 10;
    btns[LB_CLEAR]->text_color = 0xDDB90F;
    btns[LB_CLEAR]->text_color_over = 0xffffff;


    btns[LB_UNDO] = new gui_button(cgame,460,20,NULL,0);
    btns[LB_UNDO]->tx = cgame->tx;
    btns[LB_UNDO]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_UNDO]->set_text("Undo");
    btns[LB_UNDO]->font_sz = 2;
    btns[LB_UNDO]->pad_y = 8;
    btns[LB_UNDO]->pad_x = 10;
    btns[LB_UNDO]->text_color = 0xDDB90F;
    btns[LB_UNDO]->text_color_over = 0xffffff;

    btns[LB_TEST] = new gui_button(cgame,180,70,NULL,0);
    btns[LB_TEST]->tx = cgame->tx;
    btns[LB_TEST]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_TEST]->set_text("Test");
    btns[LB_TEST]->font_sz = 2;
    btns[LB_TEST]->pad_y = 8;
    btns[LB_TEST]->pad_x = 10;
    btns[LB_TEST]->text_color = 0xDDB90F;
    btns[LB_TEST]->text_color_over = 0xffffff;

    btns[LB_LOAD] = new gui_button(cgame,40,70,NULL,0);
    btns[LB_LOAD]->tx = cgame->tx;
    btns[LB_LOAD]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_LOAD]->set_text("Load");
    btns[LB_LOAD]->font_sz = 1;
    btns[LB_LOAD]->pad_y = 8;
    btns[LB_LOAD]->pad_x = 10;
    btns[LB_LOAD]->text_color = 0xDDB90F;
    btns[LB_LOAD]->text_color_over = 0xffffff;

    btns[LB_PLAY] = new gui_button(cgame,320,70,NULL,0);
    btns[LB_PLAY]->tx = cgame->tx;
    btns[LB_PLAY]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_PLAY]->set_text("Play");
    btns[LB_PLAY]->pad_y = 8;
    btns[LB_PLAY]->pad_x = 10;
    btns[LB_PLAY]->text_color = 0xDDB90F;
    btns[LB_PLAY]->text_color_over = 0xffffff;

    // play test
    btns[LB_PLAY_CLOSE] = new gui_button(cgame,500,40,NULL,0);
    btns[LB_PLAY_CLOSE]->tx = cgame->tx;
    btns[LB_PLAY_CLOSE]->set_images("imgs/gui/button_out.png","imgs/gui/button_over.png",0,0);
    btns[LB_PLAY_CLOSE]->set_text("Exit");
    btns[LB_PLAY_CLOSE]->pad_y = 8;
    btns[LB_PLAY_CLOSE]->pad_x = 10;
    btns[LB_PLAY_CLOSE]->text_color = 0xDDB90F;
    btns[LB_PLAY_CLOSE]->text_color_over = 0xffffff;

    // placement buttons

    btns[LB_BG1] = new gui_button(cgame,40,420,NULL,0);
    btns[LB_BG1]->tx = cgame->tx;
    btns[LB_BG1]->set_images("imgs/gui/le_bg1.png","imgs/gui/le_bg1.png",0,0);

    btns[LB_BG2] = new gui_button(cgame,85,420,NULL,0);
    btns[LB_BG2]->tx = cgame->tx;
    btns[LB_BG2]->set_images("imgs/gui/le_bg2.png","imgs/gui/le_bg2.png",0,0);

    btns[LB_BG3] = new gui_button(cgame,130,420,NULL,0);
    btns[LB_BG3]->tx = cgame->tx;
    btns[LB_BG3]->set_images("imgs/gui/le_bg3.png","imgs/gui/le_bg3.png",0,0);


    btns[LB_KING] = new gui_button(cgame,200,420,NULL,0);
    btns[LB_KING]->tx = cgame->tx;
    btns[LB_KING]->set_images("imgs/gui/le_king.png","imgs/gui/le_king.png",0,0);

    btns[LB_QUEEN] = new gui_button(cgame,245,420,NULL,0);
    btns[LB_QUEEN]->tx = cgame->tx;
    btns[LB_QUEEN]->set_images("imgs/gui/le_queen.png","imgs/gui/le_queen.png",0,0);

    btns[LB_JESTER] = new gui_button(cgame,290,420,NULL,0);
    btns[LB_JESTER]->tx = cgame->tx;
    btns[LB_JESTER]->set_images("imgs/gui/le_jester.png","imgs/gui/le_jester.png",0,0);

    btns[LB_SOLDIER] = new gui_button(cgame,335,420,NULL,0);
    btns[LB_SOLDIER]->tx = cgame->tx;
    btns[LB_SOLDIER]->set_images("imgs/gui/le_soldier.png","imgs/gui/le_soldier.png",0,0);

    btns[LB_PRINCESS] = new gui_button(cgame,380,420,NULL,0);
    btns[LB_PRINCESS]->tx = cgame->tx;
    btns[LB_PRINCESS]->set_images("imgs/gui/le_princess.png","imgs/gui/le_princess.png",0,0);


    btns[LB_WOOD1] = new gui_button(cgame,40,120,NULL,0);
    btns[LB_WOOD1]->tx = cgame->tx;
    btns[LB_WOOD1]->set_images("imgs/gui/le_wood1.png","imgs/gui/le_wood1.png",0,0);

    btns[LB_WOOD2] = new gui_button(cgame,40,165,NULL,0);
    btns[LB_WOOD2]->tx = cgame->tx;
    btns[LB_WOOD2]->set_images("imgs/gui/le_wood2.png","imgs/gui/le_wood2.png",0,0);

    btns[LB_WOOD3] = new gui_button(cgame,85,120,NULL,0);
    btns[LB_WOOD3]->tx = cgame->tx;
    btns[LB_WOOD3]->set_images("imgs/gui/le_wood3.png","imgs/gui/le_wood3.png",0,0);

    btns[LB_BARREL1] = new gui_button(cgame,85,165,NULL,0);
    btns[LB_BARREL1]->tx = cgame->tx;
    btns[LB_BARREL1]->set_images("imgs/gui/le_barrel1.png","imgs/gui/le_barrel1.png",0,0);

    btns[LB_ROOF1] = new gui_button(cgame,40,210,NULL,0);
    btns[LB_ROOF1]->tx = cgame->tx;
    btns[LB_ROOF1]->set_images("imgs/gui/le_roof1.png","imgs/gui/le_roof1.png",0,0);

    btns[LB_STUMP1] = new gui_button(cgame,85,210,NULL,0);
    btns[LB_STUMP1]->tx = cgame->tx;
    btns[LB_STUMP1]->set_images("imgs/gui/le_stump1.png","imgs/gui/le_stump1.png",0,0);

    btns[LB_STONE1] = new gui_button(cgame,40,255,NULL,0);
    btns[LB_STONE1]->tx = cgame->tx;
    btns[LB_STONE1]->set_images("imgs/gui/le_stone1.png","imgs/gui/le_stone1.png",0,0);



};

castle_gui_level::~castle_gui_level()
{
    delete_save_ents();

    if (save_gui) delete save_gui;

    loopi(LB_MAX)
    {
       delete btns[i];
       btns[i] = 0;
    }
};

void castle_gui_level::delete_save_ents()
{
    loopi(s_ents.size()) delete s_ents[i];

    s_ents.resize(0);

};

void castle_gui_level::save_cur_ents()
{
    delete_save_ents(); // clear

    loopi(cgame->ents.size())
    {
        castle_ent* e = cgame->ents[i];

        save_ents* x = new save_ents;

        b2Vec2 p = e->body->GetPosition();
        float a = e->body->GetAngle();

        x->x = p.x;
        x->y = p.y;
        x->a = a;

        s_ents.push_back(x);
    }
};

void castle_gui_level::restore_ents()
{
    loopi(cgame->ents.size())
    {
        castle_ent* e = cgame->ents[i];

        b2Vec2 p;
        float a;

        p.x = s_ents[i]->x;
        p.y = s_ents[i]->y;
        a = s_ents[i]->a;

        e->body->SetXForm(p,a);

    }
};


int lastbut = 0;
int lasttype = 0;
int castle_gui_level::hit_test(int s)
{
    if (play_level)
    {
        if(btns[LB_PLAY_CLOSE]->hit_test(&event,0)==B_CLICK)
        {
            cgame->simulate = false;

            play_level = false;

            // remove the rocks simulation added
            loopi(cgame->ents.size())
            {
                if (cgame->ents[i]->type == TYPE_ROCK)
                {
                    delete cgame->ents[i];
                    cgame->ents[i] = 0;
                }
            }

            cgame->reorder_ents();

            restore_ents();

            return GS_LEVELEDIT_TEST_CLOSE;
        }
        return s;
    }


    if (load)
    {
        int lr = load_gui->hit_test(s);

        if (lr == MESSAGE_1)
        {
            load = false;

            delete load_gui;
            load_gui = 0;

            return s;
        }

        if (lr == MESSAGE_2)
        {
            load = false;
            char* fn = load_gui->selected;
            cgame->load_level(fn,true);

            delete load_gui;
            load_gui = 0;

            return s;
        }

        return s;
    }

    if (save)
    {
        int sr = save_gui->hit_test(s);
        if (sr == MESSAGE_1)
        {
            save = false;

            delete save_gui;
            save_gui = 0;

            return s;
        }
        if (sr == MESSAGE_2)
        {
            save = false;
            char* fn = save_gui->gui_keyb->dest_buffer;
            cgame->level->save_level(fn);

            delete save_gui;
            save_gui = 0;

            return s;
        }

        return s;
    }

    // press button 2
    if (g_real_keys[SDLK_2] && !g_keys_last_state[SDLK_2])
    {

        if (current)
        {
            // remove the rocks simulation added
            loopi(cgame->ents.size())
            {
                if (cgame->ents[i] == current)
                {   // found
                    delete cgame->ents[i];
                    cgame->ents[i] = 0;
                    break;
                }
            }

            cgame->reorder_ents();

            current = 0;
            return s;
        }
    }

    // A or B
    if(lasttype==event.type && lastbut==event.button.button) return s; // why?? get event twice without it
    lasttype = event.type;
    lastbut = event.button.button;

    if ((event.type == SDL_MOUSEBUTTONDOWN) &&
        (event.button.state == SDL_PRESSED) &&
        (event.button.button == SDL_BUTTON_LEFT))
    {

        castle_ent* temp = current;

        current = 0;

        if(btns[LB_EXIT]->hit_test(&event,0)==B_CLICK)
        {
            delete_save_ents();
            return GS_TITLE;
        }

        if(btns[LB_UNDO]->hit_test(&event,0)==B_CLICK)
        {
            if(cgame->ents.size() > 0) {
              castle_ent* a = (castle_ent*)cgame->ents.back();
              delete a;
              cgame->ents.pop_back();
            }
        }

        if(btns[LB_SAVE]->hit_test(&event,0)==B_CLICK)
            save = true;

        if(btns[LB_TEST]->hit_test(&event,0)==B_CLICK)
        {
            cgame->simulate = !cgame->simulate;

            if (cgame->simulate)
                save_cur_ents(); // save position info so we can restore after simulate !!!
            else restore_ents();

        }

        if(btns[LB_PLAY]->hit_test(&event,0)==B_CLICK)
        {
            save_cur_ents(); // save position info so we can restore after simulate !!!
            // save current level as temp file so we can reload later

            play_level = true;
            return GS_LEVELEDIT_TEST_INIT;

        }

        if(btns[LB_CLEAR]->hit_test(&event,0)==B_CLICK)
        {
            delete_save_ents();
            cgame->clear_ents();
        }

        if(btns[LB_BG1]->hit_test(&event,0)==B_CLICK)
            cgame->current_bg = 1;

        if(btns[LB_BG2]->hit_test(&event,0)==B_CLICK)
            cgame->current_bg = 2;

        if(btns[LB_BG3]->hit_test(&event,0)==B_CLICK)
            cgame->current_bg = 3;

        if(btns[LB_LOAD]->hit_test(&event,0)==B_CLICK)
            load = true;

        // add king to level!
        if(btns[LB_KING]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_character(cgame,event.motion.x,event.motion.y,CHAR_KING);
        }

        if(btns[LB_QUEEN]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_character(cgame,event.motion.x,event.motion.y,CHAR_QUEEN);
        }

        if(btns[LB_JESTER]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_character(cgame,event.motion.x,event.motion.y,CHAR_JESTER);
        }

        if(btns[LB_SOLDIER]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_character(cgame,event.motion.x,event.motion.y,CHAR_SOLDIER);
        }

        if(btns[LB_PRINCESS]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_character(cgame,event.motion.x,event.motion.y,CHAR_PRINCESS);
        }

        if(btns[LB_WOOD1]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_block(cgame,event.motion.x,event.motion.y,WOOD_V,0);
        }

        if(btns[LB_WOOD2]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_block(cgame,event.motion.x,event.motion.y,WOOD_H,0);
        }

        if(btns[LB_WOOD3]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_block(cgame,event.motion.x,event.motion.y,WOOD_3,0);
        }

        if(btns[LB_BARREL1]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_block(cgame,event.motion.x,event.motion.y,BARREL_1,0);
        }
        if(btns[LB_ROOF1]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_block(cgame,event.motion.x,event.motion.y,ROOF_1,0);
        }
        if(btns[LB_STUMP1]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_block(cgame,event.motion.x,event.motion.y,STUMP_V,0);
        }

        if(btns[LB_STONE1]->hit_test(&event,0)==B_CLICK && (!current))
        {
            angle = 0.0f;
            current = new castle_block(cgame,event.motion.x,event.motion.y,ROCK_V,0);
        }


        if (!current && !temp)
        {
            // hit test on ents !
           current = hit_ent();
        }

        if (temp == current) current = 0;

    }else if (  (event.button.state == SDL_PRESSED) &&
                (event.button.button == SDL_BUTTON_RIGHT) && current)
    {

        if (current->type == TYPE_BLOCK)
        {
            angle += 0.05f;
        }

    }else if (  (event.button.state == SDL_PRESSED) &&
                (event.button.button == SDL_BUTTON_MIDDLE) && current)
    {
            // delete selected ... TODO
    }



    return s; // nothing
}

castle_ent* castle_gui_level::hit_ent()
{
    b2Vec2 p;
    p.x = (event.motion.x + X_OFFSET + MAX_SCREEN_X) / SCALE;
    p.y = (event.motion.y + Y_OFFSET) / -SCALE;

	b2AABB aabb;
	b2Vec2 d;

	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;

    loopi(cgame->ents.size())
	{
		b2Body* bdy = cgame->ents[i]->body;
        b2Shape* shape = bdy->GetShapeList();

        if (shape->TestPoint(bdy->GetXForm(), p)) return cgame->ents[i];
	}

	return 0;
};

void castle_gui_level::render()
{
    if (play_level)
    {

        btns[LB_PLAY_CLOSE]->obj_state = B_OUT; //reset
        btns[LB_PLAY_CLOSE]->hit_test(&event,0);
        btns[LB_PLAY_CLOSE]->draw();

        return;
    }

    if (save)
    {
        if (!save_gui)
             save_gui = new castle_gui_save(cgame);

        save_gui->render();
        return;
    }
    if (load)
    {
        if (!load_gui)
            load_gui = new castle_gui_load(cgame,make_path("levels/"));

        load_gui->render();
        return;
    }
    // buttons
    loopi(LB_MAX)
    {
        if (i == LB_PLAY_CLOSE) continue;

        btns[i]->obj_state = B_OUT; //reset
        btns[i]->hit_test(&event,0);
        btns[i]->draw();
    }

    // placing a block or char
    pos.x = (event.motion.x + MAX_SCREEN_X)  / SCALE;
    pos.y = event.motion.y / -SCALE;
    if (current)
    {
        current->body->SetXForm(pos, angle);
        // update the ent
        current->r_x = event.motion.x;
        current->r_y = event.motion.y;
        current->angle = angle;
        // world x y

        current->w_x = pos.x;
        current->w_y = pos.y;

        cgame->drawB2Shape(current->castle_ent::body,current->body->GetShapeList());
    }

    if ((highlight = hit_ent())) cgame->drawB2Shape(highlight->castle_ent::body,highlight->body->GetShapeList());

     // render ground
    cgame->drawB2Shape(cgame->ground,cgame->ground->GetShapeList());

};


