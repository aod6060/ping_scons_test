#include "SDL_mouse.h"
#include "sys.h"


namespace engine {
    void Input::init() {

    }

    void Input::handleEvent(Context* context) {

    }

    void Input::update() {

    }

    void Input::release() {

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
    bool Input::isMouseButtonReleased(const Keyboard& key) {

    }

    bool Input::isMouseButtonPressedOnce(const Keyboard& key) {

    }

    bool Input::isMouseButtonPressed(const Keyboard& key) {

    }

    bool Input::isMouseButtonReleasedOnce(const Keyboard& key) {

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