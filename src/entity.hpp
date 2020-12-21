#pragma once

#include <string>
#include "../lib/box2d/include/box2d/box2d.h"
#include <SFML/Graphics.hpp>
#include "message-bus.hpp"
#include "sprite-object.hpp"
#include "animation.hpp"

using namespace std;

namespace Alchemy {

    typedef struct SpriteConfig {
        sf::Texture* texture;
        Animation* animation;
        int width;
        int height;
        b2World* world;
    };

    class Entity : public Actor {
        private:
            SpriteObject* sprite;
        public:
            Entity (SpriteConfig spriteConfig, MessageBus* messageBus) : Actor(messageBus) {
                this->sprite = new SpriteObject(
                    spriteConfig.texture,
                    spriteConfig.animation,
                    spriteConfig.width, spriteConfig.height, spriteConfig.world
                );
            }

            virtual void OnUpdate(float dt) { }

            virtual void OnDraw(RenderWindow* context) {
                this->sprite->OnDraw(context);
            }

            ~Entity () {
                delete this->sprite;
            }
    };
}