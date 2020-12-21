#pragma once

#include "animation.hpp"
#include <SFML/Graphics.hpp>

#include "../lib/box2d/include/box2d/box2d.h"

#include <iostream>

using namespace sf;

namespace Alchemy {
    class SpriteObject {
        private:
            sf::Sprite sprite;
            Animation animation;
            sf::Texture* texture;
            string currentState;
            b2World* world;
            b2Body* body;
            sf::Vector2f movement;
        public:
            
            SpriteObject (sf::Texture* texture, Animation* anim, int width, int height, b2World* world) {
                this->animation = Animation(anim);
                this->world = world;
                this->texture = texture;
                this->sprite.setTexture(*this->texture);
                this->setupBody(width, height);
            }

            void setAnimation(Animation* anim) {
                this->animation = Animation(anim);
            }

            void setScale(float x, float y) {
                this->sprite.setScale(x, y);
            }

            void setupBody (int width, int height) {
                b2BodyDef bodyDef;
                bodyDef.type = b2_dynamicBody;
                bodyDef.position.Set(0.0f, 0.0f);
                this->body = world->CreateBody(&bodyDef);
                b2PolygonShape shape;
                shape.SetAsBox(width / 2, height / 2);
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &shape;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.3f;
                this->body->CreateFixture(&fixtureDef);
            }

            void setAnimationFrame(AnimationFrame* frame) {
                this->sprite.setTextureRect(sf::IntRect(
                    frame->x,
                    frame->y,
                    frame->width,
                    frame->height                        
                ));
            }

            Vector2f getPosition() {
                auto v = this->body->GetPosition();
                return Vector2f(v.x, v.y);
            }   

            void OnUpdate(float dt) {
                if (this->animation.onUpdate(dt)) {
                    if (this->animation.currentFrame != NULL) {
                        this->setAnimationFrame(this->animation.currentFrame);
                    }
                };

                this->sprite.setPosition(this->getPosition());
            }

            void setState(string name) {
                if (this->currentState == name) return;
                this->animation.setState(name);
                this->setAnimationFrame(this->animation.currentFrame);
                this->currentState = name;
            }

            void OnDraw(RenderWindow* context) {
                context->draw(this->sprite);
            }

            void setVelocity(float x, float y) {
                this->movement.x = x;
                this->movement.y = y;
                this->body->SetLinearVelocity(b2Vec2(x, y));
            }

            sf::Vector2f getMovement () {
                return this->movement;
            }
    };
}