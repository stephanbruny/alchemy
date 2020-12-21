#include <iostream>

#include "conf.hpp"
#include "src/game.hpp"

#include GAME_MAIN

int main() {
    auto game = new Alchemy::Game(ALCHEMY_WINDOW_WIDTH, ALCHEMY_WINDOW_HEIGHT);
    auto app = new DemoGame();
    return game->RunGame(app);
}
