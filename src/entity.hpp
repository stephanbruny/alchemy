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

            virtual void OnContact(Entity* e) {}
            virtual void OnContactReleased(Entity* e) {}

            virtual void OnCollisionEnter() {}
            virtual void OnCollisionRelease() {}

            ~Entity () {
                delete this->sprite;
            }
    };

    class EntityContactListener : public b2ContactListener {
        void BeginContact(b2Contact* contact) {
            Entity* bodyUserDataA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            Entity* bodyUserDataB = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            if ( bodyUserDataA && bodyUserDataB ) {
                bodyUserDataA->OnContact(bodyUserDataB);
                bodyUserDataB->OnContact(bodyUserDataA);
            } else {
                if (bodyUserDataA) bodyUserDataA->OnCollisionEnter();
                if (bodyUserDataB) bodyUserDataB->OnCollisionEnter();
            }
        }
    
        void EndContact(b2Contact* contact) {
            Entity* bodyUserDataA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            Entity* bodyUserDataB = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
            if ( bodyUserDataA && bodyUserDataB ) {
                bodyUserDataA->OnContactReleased(bodyUserDataB);
                bodyUserDataB->OnContactReleased(bodyUserDataA);
            } else {
                if (bodyUserDataA) bodyUserDataA->OnCollisionRelease();
                if (bodyUserDataB) bodyUserDataB->OnCollisionRelease();
            }
    };
}