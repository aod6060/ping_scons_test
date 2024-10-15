#include "../sys.h"
#include "entity.h"
#include "glm/ext/matrix_transform.hpp"


namespace engine {
    namespace entity {
        glm::mat4 Transform::toMatrix() {
            return 
                glm::translate(glm::mat4(1.0), glm::vec3(this->position, 0.0f)) *
                glm::scale(glm::mat4(1.0), glm::vec3(this->size, 0.0f));
        }

    }
}
