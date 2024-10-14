#include "game.h"
#include "glm/ext/matrix_transform.hpp"


namespace game {

    // Game
    void GameApplication::init(engine::Context* context) {
    }

    void GameApplication::handleEvents(engine::Context* context) {
    }

    void GameApplication::update(engine::Context* context) {
    }

    void GameApplication::render(engine::Context* context) {
        context->render.clear(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        context->render.startFrame();

        context->render.setView(glm::mat4(1.0f));

        context->render.setModel(
            glm::translate(glm::mat4(1.0f), glm::vec3(32.0f, 32.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 32.0f, 0.0f))
        );

        context->render.draw();
        
        context->render.endFrame();
    }

    void GameApplication::release(engine::Context* context) {
    }
}