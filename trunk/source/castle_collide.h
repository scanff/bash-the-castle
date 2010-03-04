#ifndef CASTLE_COLLIDE_H_INCLUDED
#define CASTLE_COLLIDE_H_INCLUDED


#include "castle_ent.h"
#include "castle_rocks.h"

class contact_listener : public b2ContactListener
{
    public:

    void Add(const b2ContactPoint* point)
    {
       // float bottom;

        b2Body* b1 = 0;
        b2Body* b2 = 0;

        b1 = point->shape1->GetBody();
        b2 = point->shape2->GetBody();

        if (b1 && b2)
        {
            castle_ent* e1 = (castle_ent*)(b1->GetUserData());
            castle_ent* e2 = (castle_ent*)(b2->GetUserData());

            if (!e1 || !e2) return;
            if (e1->status == DEAD || e2->status == DEAD) return;

            b2Vec2 lv = e1->body->GetLinearVelocity();
            b2Vec2 lv2 = e2->body->GetLinearVelocity();

            //block on block
            if (e1->type == TYPE_BLOCK && e2->type == TYPE_BLOCK)
            {
                if (fabs(lv2.x) > 3.0f || fabs(lv2.y) > 3.0f ||
                    fabs(lv.x) > 3.0f || fabs(lv.y) > 3.0f
                    ) e1->status = HIT;
            }

            //rock on block
            if (e1->type == TYPE_ROCK && e2->type == TYPE_BLOCK)
            {
                if (fabs(lv.x) > 5.0f || fabs(lv.y) > 5.0f)
                {
                    if (e1->status != DEAD)
                    {
                        e1->status = HIT;
                        e1->hit = e2;
                    }
                }
            }else if (e1->type == TYPE_BLOCK && e2->type == TYPE_ROCK){
                if (fabs(lv2.x) > 5.0f || fabs(lv2.y) > 5.0f)
                {
                    if (e2->status != DEAD)
                    {
                        e2->status = HIT;
                        e2->hit = e1;
                    }
                }
            }

            //rock on rock
            if (e1->type == TYPE_ROCK && e2->type == TYPE_ROCK)
            {
                if (fabs(lv.x) > 3.0f || fabs(lv.y) > 3.0f ||
                    fabs(lv2.x) > 3.0f || fabs(lv2.y) > 3.0f)
                {
                    if (e2->status != DEAD && e1->status != DEAD)
                    {
                        e1->status = HIT;
                        e1->hit = e2;
                    }
                }
                else if (e1->sub_type == PROJECT_BOMB)
                {
                    if (e2->status != DEAD && e1->status != DEAD)
                    {
                        e1->status = HIT;
                        e1->hit = e2;
                    }
                }
                else if  (e2->sub_type == PROJECT_BOMB)
                {
                    if (e2->status != DEAD && e1->status != DEAD)
                    {
                        e2->status = HIT;
                        e2->hit = e1;
                    }
                }
            }

            // character collide
            if (e1->type == TYPE_CHAR && e1->status == ALIVE)
            {

                if (fabs(lv2.x) > 2.0f || fabs(lv2.y) > 2.0f)
                 {
                    e1->status = HIT;
                    return;
                 }
            }

            if (e2->type == TYPE_CHAR && e2->status == ALIVE)
            {
                b2Vec2 lv = e1->body->GetLinearVelocity();
                b2Vec2 lv2 = e2->body->GetLinearVelocity();

                 if (fabs(lv.x) > 2.0f || fabs(lv.y) > 2.0f)
                 {
                    e2->status = HIT;
                    return;
                 }
            }

        }
    };

    void Persist(const b2ContactPoint* point) {};

    void Remove(const b2ContactPoint* point) {};

    void Clear() {};

  /*  float GetBottom(b2Body* body)
    {
        b2Shape* shape = body->GetShapeList();
        b2XForm xf = body->GetXForm();
        b2PolygonShape* poly = (b2PolygonShape*)shape;
        const b2OBB& obb = poly->GetOBB();
        b2Vec2 h = obb.extents;
        b2Vec2 vs[4];

        vs[0].Set(-h.x, -h.y);
        vs[1].Set( h.x, -h.y);
        vs[2].Set( h.x,  h.y);
        vs[3].Set(-h.x,  h.y);

        loopi(4)
        {
            vs[i] = obb.center + b2Mul(obb.R, vs[i]);
            vs[i] = b2Mul(xf, vs[i]);
        }

        float v1, v2;

        vs[0].y < vs[1].y ? v1 = vs[0].y : v1 = vs[1].y;
        vs[2].y < vs[3].y ? v2 = vs[2].y : v1 = vs[3].y;

        return v1 < v2 ? v1 : v2;

    };
*/
};


#endif // CASTLE_COLLIDE_H_INCLUDED
