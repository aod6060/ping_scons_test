#include "game.h"
#include "glm/ext/matrix_transform.hpp"


namespace game {

    // Game
    void GameApplication::init(engine::Context* context) {
        this->context = context;
    }

    void GameApplication::handleEvents() {
    }

    void GameApplication::update() {

        if(context->input.isKeyPressed(engine::Input::Keyboard::KEYS_LEFT)) {
            pos.x -= 64.0f * context->getDelta();
        } else if(context->input.isKeyPressed(engine::Input::Keyboard::KEYS_RIGHT)) {
            pos.x += 64.0f * context->getDelta();
        }

        if(context->input.isKeyPressed(engine::Input::Keyboard::KEYS_UP)) {
            pos.y -= 64.0f * context->getDelta();
        } else if(context->input.isKeyPressed(engine::Input::Keyboard::KEYS_DOWN)) {
            pos.y += 64.0f * context->getDelta();
        }
    }

    void GameApplication::render() {
        context->render.startFrame();
        context->render.clear(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        context->render.bindShader();

        context->render.setView(glm::mat4(1.0f));

        context->render.setModel(
            glm::translate(glm::mat4(1.0f), glm::vec3(this->pos, 0.0f)) *
            glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 32.0f, 0.0f))
        );

        context->render.draw();

        context->render.unbindShader();

        context->render.endFrame();
    }

    void GameApplication::release() {
        context = nullptr;
    }
}