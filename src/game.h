#ifndef GAME_H
#define GAME_H


#include "sys.h"

namespace game {


    struct GameApplication : public engine::IApplication {

        uint32_t count = 0;
        
        virtual void init(engine::Context* context);
        virtual void handleEvents(engine::Context* context);
        virtual void update(engine::Context* context);
        virtual void render(engine::Context* context);
        virtual void release(engine::Context* context);
    };

}

#endif