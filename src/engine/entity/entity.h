#ifndef ENTITY_H
#define ENTITY_H

#include "glm/ext/vector_float2.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <vector>


namespace engine {

    namespace entity {
        struct Transform {
            glm::vec2 position;
            glm::vec2 size;

            glm::mat4 toMatrix();
        };

        enum ComponentType {
            CT_MAX_SIZE
        };

        struct Entity;

        struct IComponent {
            virtual void init(Entity* entity) = 0;
            virtual void handleEvent() = 0;
            virtual void update() = 0;
            virtual void render() = 0;
            virtual void release() = 0;

            virtual ComponentType getType() = 0;
        };


        struct IScene;

        struct Entity {
            IScene* scene;
            std::string name;
            Transform transform;

            Entity* parent = nullptr;
            std::vector<Entity*> childeren;

            void init(IScene* scene);
            void init(IScene* scene, Entity* parent);

            void handleEvent();
            void update();
            void render();
            void release();

            void appendChild(Entity* child);
            void removeChild(Entity* child);
            Entity* getChild(uint32_t idx);
            uint32_t getChilderenSize();


        };

    }
}

#endif