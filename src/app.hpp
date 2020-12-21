#pragma once

#include <SFML/Graphics.hpp>

namespace Alchemy {
    class GameApplication {
        public:
            GameApplication() {

            }

            virtual void OnLoad() {

            }

            virtual void OnUpdate(float dt) {

            }

            virtual void OnDraw(sf::RenderWindow* context) {

            }

            virtual void OnQuit () {

            }

            ~GameApplication() {

            }
    };
}