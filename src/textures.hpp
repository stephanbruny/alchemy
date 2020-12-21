#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

using namespace std;

namespace Alchemy {

    class Textures {
        private:
            map<string, sf::Texture*> textures;

        public:
            Textures () {

            }

            sf::Texture* load(string filename) {
                auto maybeImage = this->textures.find(filename);
                if (maybeImage != this->textures.end()) {
                    return maybeImage->second;
                }
                sf::Texture* image = new sf::Texture();
                if (!image->loadFromFile(filename)) {
                    throw "Image not found";
                }
                this->textures.insert(pair<string, sf::Texture*>(
                    filename,
                    image
                ));
                return image;
            };

            void clearAll () {
                for (auto iter = this->textures.begin(); iter != this->textures.end(); iter++) {
                    delete iter->second;
                    this->textures.erase(iter);
                }
                this->textures.clear();
            }
    };

}