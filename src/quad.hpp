#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

using namespace std;

namespace Alchemy {
    class Quad : public sf::Drawable, public sf::Transformable {
        public:

            // add functions to play with the entity's geometry / colors / texturing...

        private:

            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
            {
                states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

                states.texture = &m_texture;

                target.draw(m_vertices, states);
            }

            sf::VertexArray m_vertices;
            sf::Texture m_texture;
    };
};