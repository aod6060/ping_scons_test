#ifndef GAME_H
#define GAME_H


#include "../engine/sys.h"

namespace game {


    struct GameApplication : public engine::IApplication {
        engine::Context* context = nullptr;
        
        uint32_t count = 0;
        glm::vec2 pos = glm::vec2(0.0f);

        virtual void init(engine::Context* context);
        virtual void handleEvents();
        virtual void update();
        virtual void render();
        virtual void release();
    };

}

#endif