#include "game.h"
#include "sys.h"


namespace game {

    // Game
    void GameApplication::init(engine::Context* context) {

    }

    void GameApplication::handleEvents(engine::Context* context) {

    }

    void GameApplication::update(engine::Context* context) {
        //glm::vec2 p = context->input.getRelative();

        //std::cout << "Mouse Relative: " << p.x << ", " << p.y << "\n";

        if(context->input.isKeyPressedOnce(engine::Input::KEYS_TAB)) {
            context->input.toggleGrab();
        }
    }

    void GameApplication::render(engine::Context* context) {

    }

    void GameApplication::release(engine::Context* context) {

    }
}