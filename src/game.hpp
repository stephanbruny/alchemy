#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


#define DISABLE_CPP17_FILESYSTEM
#include "include/tileson.hpp"
#include "app.hpp"
#include "animation.hpp"

using namespace std;

namespace Alchemy {
    class Game {
        private:
            int _windowWidth;
            int _windowHeight;
        public:

            Game (int width, int height) {
                this->_windowWidth = width;
                this->_windowHeight = height;
            }

            int RunGame(GameApplication* app) {
                sf::RenderWindow window(sf::VideoMode(this->_windowWidth, this->_windowHeight), "Alchemy");

                app->OnLoad();

                sf::Clock clock;

                auto renderingTask = [&](){
                    window.setActive(true);
                    window.setFramerateLimit(60);
                    window.setVerticalSyncEnabled(true);

                    while (window.isOpen())
                    {
                        window.clear(sf::Color::Blue);
                        app->OnDraw(&window);
                        window.display();
                    }
                };

                sf::Thread renderWorker(renderingTask);

                window.setActive(false);

                renderWorker.launch();

                while (window.isOpen())    // Detect window close button or ESC key
                {
                    sf::Event event;
                    while (window.pollEvent(event))
                    {
                        // "close requested" event: we close the window
                        if (event.type == sf::Event::Closed)
                            window.close();
                    }

                    sf::Time elapsed = clock.restart();
                    
                    float dt = elapsed.asSeconds();

                    app->OnUpdate(dt);

                    sf::sleep(sf::milliseconds(1));
                }

                app->OnQuit();

                return 0;

            }
    };
}