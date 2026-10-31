#pragma once
#include "box2d/box2d.h"
#include "box2d/id.h"
#include <memory>

namespace physics {
    class world{
        std::shared_ptr<b2WorldId> ptr;
        public:
            world(b2Vec2 gravity={0,10});
            void simulate(int fps=60, int subStepCount=4);
            b2WorldId get();
    };
    class body{
        std::shared_ptr<b2BodyId> ptr;
        public:
            body(world& w, b2Vec2 pos, b2BodyType type);
            void attachBox(float w, float h);
            b2BodyId get();
    };
}