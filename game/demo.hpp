#pragma once
#include "../src/app.hpp"
#include "../src/animation.hpp"
#include "../src/sprite-object.hpp"
#include "../src/textures.hpp"
#include <math.h>
#include <iostream>
#include <map>
#include <any>

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include "../src/message-bus.hpp"

#include "../lib/box2d/include/box2d/box2d.h"

#define GLSL_VERSION 330

using namespace sf;
using namespace std;

class FooActor : public Alchemy::Actor {
    private:
        sf::Vector2f pos;
        sf::Vector2f movement;
        b2World* world;
        b2Body* body;
    public:
        FooActor (Alchemy::MessageBus* bus, b2World* world) : Alchemy::Actor (bus) {
            this->subscribe("demo");
            this->pos.x = 0.0f;
            this->pos.y = 0.0f;
            this->world = world;

            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0.0f, 0.0f);
            this->body = world->CreateBody(&bodyDef);
            b2CircleShape shape;
            shape.m_radius = 25.0f;
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.3f;
            this->body->CreateFixture(&fixtureDef);
        };

        void receive (string message, any value) override {
            std::cout << "Receive: " << message << std::endl;
            if (message == "foo") {
                std::cout << "FooActor called" << std::endl;
            }

            if (message == "input") {
                pair<float, float> input = any_cast<pair<float, float>>(value);
                this->body->ApplyLinearImpulse(
                    b2Vec2(input.first, input.second),
                    this->body->GetPosition(),
                    true
                );
            }
        }

        void OnUpdate(float dt) {
            
        }

        Vector2f getPosition() {
            auto v = this->body->GetPosition();
            return Vector2f(v.x, v.y);
        }
};

class DemoGame : public Alchemy::GameApplication {
    private:
        float _t;
        Alchemy::MessageBus messageBus;
        FooActor* foo;
        sf::CircleShape* shape;
        int joyStickIndex;
        b2World* world;
        Alchemy::Animations animations;
        Alchemy::SpriteObject* sprite;
        Alchemy::Textures* images;
    public:

        DemoGame() {
            this->_t = 0.0f;
            this->messageBus.addChannel("demo");
            this->shape = new sf::CircleShape(50.0f);
            this->joyStickIndex = -1;
            this->world = new b2World(b2Vec2(0.0f, 0.0f));
            // this->foo = new FooActor(&this->messageBus, this->world);
            this->images = new Alchemy::Textures();
            cout << "OK 1" << endl;
            this->sprite = new Alchemy::SpriteObject(
                this->images->load("build/resources/assets/hero-fem.png"),
                this->animations.load("build/resources/animation/hero.json"),
                16, 16, this->world
            );
            this->sprite->setScale(4.0f, 4.0f);
            this->sprite->setState("walk-down");
        }

        void HandleGamepad () {
            if (this->joyStickIndex != -1) {
                float x = sf::Joystick::getAxisPosition(joyStickIndex, sf::Joystick::X);
                float y = sf::Joystick::getAxisPosition(joyStickIndex, sf::Joystick::Y);
                if (abs(x) >= 5.0f || abs(y) >= 5.0f) {
                    pair<float, float> axis(x, y);
                    this->messageBus.publish("demo", "input", any(axis) );
                    this->sprite->setVelocity(x, y);
                    if (abs(x) > abs(y)) {
                        if (x > 0) this->sprite->setState("walk-right");
                        if (x < 0) this->sprite->setState("walk-left");
                    }
                    if (abs(x) < abs(y)) {
                        if (y > 0) this->sprite->setState("walk-down");
                        if (y < 0) this->sprite->setState("walk-up");
                    }
                } else {
                    auto vec = this->sprite->getMovement();
                    if (abs(vec.x) > abs(vec.y)) {
                        if (vec.x > 0) this->sprite->setState("right");
                        if (vec.x < 0) this->sprite->setState("left");
                    }
                    if (abs(vec.x) < abs(vec.y)) {
                        if (vec.y > 0) this->sprite->setState("down");
                        if (vec.y < 0) this->sprite->setState("up");
                    }
                    this->sprite->setVelocity(0, 0);
                }
            }
        }
        
        void OnLoad() {
            std::cout << "Demo Game!" << std::endl;
            this->messageBus.publish("demo", "foo", 42);
            this->shape->setFillColor(sf::Color(100, 250, 50));

            for (int i = 0; i < sf::Joystick::Count; i++) {
                if (sf::Joystick::isConnected(i)) {
                    this->joyStickIndex = i;
                    cout << "Index: " << this->joyStickIndex << endl;
                    break;
                }
            }
        }

        void OnUpdate(float dt) {
            _t += dt;
            this->HandleGamepad();
            // this->shape->setPosition(this->foo->getPosition());
            // this->foo->OnUpdate(dt);
            this->sprite->OnUpdate(dt);
            this->world->Step(dt, 8, 3);
        }

        void OnDraw(RenderWindow* context) {
            float mod = sin(_t * 32);
            // context->draw((*this->shape));
            this->sprite->OnDraw(context);
        }

        void OnQuit() {
            std::cout << "Bye bye!" << std::endl;
        }

        ~DemoGame() {
            delete this->foo;
            delete this->shape;
            delete this->world;
        }
};