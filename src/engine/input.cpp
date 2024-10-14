#include "sys.h"


namespace engine {

    void Input::init() {
        // Keyboard
        this->keys.resize(Input::Keyboard::KEYS_MAX_SIZE);

        std::for_each(this->keys.begin(), this->keys.end(), [](InputState& state) {
            state = InputState::IS_RELEASED;
        });

        // Mouse Buttons
        this->mouseButtons.resize(Input::MouseButtons::MBS_MAX_SIZE);

        std::for_each(this->mouseButtons.begin(), this->mouseButtons.end(), [](InputState& state) {
            state = InputState::IS_RELEASED;
        });

        // Mouse Position
        this->position = glm::vec2(0.0f, 0.0f);

        // Mouse Relative
        this->relative = glm::vec2(0.0f, 0.0f);
    }

    void Input::handleEvent(Context* context) {
        if(context->event.type == SDL_KEYDOWN) {
            if(this->keys[context->event.key.keysym.scancode] == InputState::IS_RELEASED) {
                this->keys[context->event.key.keysym.scancode] = InputState::IS_PRESSED_ONCE;
            }
        } else if(context->event.type == SDL_KEYUP) {
            if(this->keys[context->event.key.keysym.scancode] == InputState::IS_PRESSED) {
                this->keys[context->event.key.keysym.scancode] = InputState::IS_RELEASED_ONCE;
            }
        } else if(context->event.type == SDL_MOUSEBUTTONDOWN) {
            if(this->mouseButtons[context->event.button.button - 1] == InputState::IS_RELEASED) {
                this->mouseButtons[context->event.button.button - 1] = InputState::IS_PRESSED_ONCE;
            }
        } else if(context->event.type == SDL_MOUSEBUTTONUP) {
            if(this->mouseButtons[context->event.button.button - 1] == InputState::IS_PRESSED) {
                this->mouseButtons[context->event.button.button - 1] = InputState::IS_RELEASED_ONCE;
            }
        } else if(context->event.type == SDL_MOUSEMOTION) {
            this->position.x = context->event.motion.x;
            this->position.y = context->event.motion.y;
            this->relative.x = context->event.motion.xrel;
            this->relative.y = context->event.motion.yrel;
            this->time = 0.0f;
        }


    }

    void Input::update(Context* context) {
        std::function<void(InputState&)> updateState = [](InputState& state) {
            if(state == InputState::IS_PRESSED_ONCE) {
                state = InputState::IS_PRESSED;
            }

            if(state == InputState::IS_RELEASED_ONCE) {
                state = InputState::IS_RELEASED;
            }
        };

        std::function<glm::vec2(glm::vec2, float t)> toZero = [](glm::vec2 a, float t) {
            glm::vec2 b = glm::vec2(0.0);
            return (b - a) * (t * t) + a;
        };

        // Keyboard
        std::for_each(keys.begin(), keys.end(),updateState);

        // Mouse Buttons
        std::for_each(this->mouseButtons.begin(), this->mouseButtons.end(), updateState);

        if(time < maxTime) {
            time += context->getDelta();
        }

        relative = toZero(relative, time * (1.0 / maxTime));
    }

    void Input::release() {
        this->keys.clear();
        this->mouseButtons.clear();
    }


    // Keyboard
    bool Input::isKeyReleased(const Keyboard& key) {
        return this->keys[key] == InputState::IS_RELEASED || this->keys[key] == InputState::IS_RELEASED_ONCE;
    }

    bool Input::isKeyPressedOnce(const Keyboard& key) {
        return this->keys[key] == InputState::IS_PRESSED_ONCE;
    }

    bool Input::isKeyPressed(const Keyboard& key) {
        return this->keys[key] == InputState::IS_PRESSED || this->keys[key] == InputState::IS_PRESSED_ONCE;
    }

    bool Input::isKeyReleasedOnce(const Keyboard& key) {
        return this->keys[key] == InputState::IS_RELEASED_ONCE;
    }


    // Mouse
    bool Input::isMouseButtonReleased(const MouseButtons& mb) {
        return this->mouseButtons[mb] == InputState::IS_RELEASED || this->mouseButtons[mb] == InputState::IS_RELEASED_ONCE;
    }

    bool Input::isMouseButtonPressedOnce(const MouseButtons& mb) {
        return this->mouseButtons[mb] == InputState::IS_PRESSED_ONCE;
    }

    bool Input::isMouseButtonPressed(const MouseButtons& mb) {
        return this->mouseButtons[mb] == InputState::IS_PRESSED || this->mouseButtons[mb] == InputState::IS_PRESSED_ONCE;
    }

    bool Input::isMouseButtonReleasedOnce(const MouseButtons& mb) {
        return this->mouseButtons[mb] == InputState::IS_RELEASED_ONCE;
    }


    glm::vec2 Input::getPosition() {
        return this->position;
    }

    glm::vec2 Input::getRelative() {
        return this->relative;
    }


    void Input::setGrab(bool grab) {
        SDL_SetRelativeMouseMode((grab) ? SDL_TRUE : SDL_FALSE);
    }

    bool Input::getGrab() {
        return SDL_GetRelativeMouseMode() ? true : false;
    }

    void Input::toggleGrab() {
        this->setGrab(!this->getGrab());
    }

}