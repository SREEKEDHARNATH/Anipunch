#include "physics.hpp"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/id.h"
#include "box2d/math_types.h"
#include "box2d/types.h"
#include <iostream>

void delW(b2WorldId* id){
    if (id){
        if (b2World_IsValid(*id)){
            b2DestroyWorld(*id);
        }
        delete id;
    }
}
void delB(b2BodyId* id){
    if (id){
        if (b2Body_IsValid(*id)){
            b2DestroyBody(*id);
        }
        delete id;
    }
}

namespace physics {
    world::world(b2Vec2 gravity){
        b2WorldId* id = new b2WorldId();
        b2WorldDef def=b2DefaultWorldDef();
        def.gravity=gravity;
        auto i=b2CreateWorld(&def);
        if (!b2World_IsValid(i)){
            std::cerr << "Failed to create world\n";
            delete id;
            return;
        }
        *id=i;
        ptr.reset(id, delW);
    }
    void world::simulate(int fps, int subStepCount){
        if (ptr.get()){
            b2World_Step(get(), 1.0f/fps, subStepCount);
        }
    }
    b2WorldId world::get(){
        return *(ptr.get());
    }

    body::body(world& w, b2Vec2 pos, b2BodyType type){
        b2BodyId* id = new b2BodyId();
        b2BodyDef def=b2DefaultBodyDef();
        def.isAwake=true;
        def.position=pos;
        def.type=type;
        auto i = b2CreateBody(w.get(), &def);
        if (!b2Body_IsValid(i)){
            std::cerr << "Body Creation failed\n";
            delete id;
            return;
        }
        *id=i;
        ptr.reset(id, delB);
    }
    void body::attachBox(float w, float h){
        if (b2Body_IsValid(get())){
            b2Polygon poly=b2MakeBox(w/2, h/2);
            b2ShapeDef def=b2DefaultShapeDef();
            def.density=1;
            def.enableContactEvents=true;
            def.material.friction=0;
            b2ShapeId sh=b2CreatePolygonShape(get(), &def, &poly);
            if (!b2Shape_IsValid(sh)){
                std::cerr << "Shape Creation failed\n";
            }

        }
    }
    b2BodyId body::get(){
        return *(ptr.get());
    }
}