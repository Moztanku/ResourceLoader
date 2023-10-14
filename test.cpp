#include <Resources.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace textures = Resources::textures;

sf::SoundBuffer make_soundBuffer(const std::string& path)
{
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile(path))
        throw std::runtime_error("Failed to load sound " + path);
    return buffer;
}

int main()
{
    // initialize the window

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // limit the framerate
    window.setFramerateLimit(60);

    // create loop
    while (window.isOpen())
    {
        // handle events

        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window

            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color

        window.clear(sf::Color::Black);

        // draw everything here...

        sf::RectangleShape shape;
        shape.setSize({600,300});
        shape.setTexture(&textures::skinface);
        shape.setTextureRect({0,0, static_cast<int>(textures::skinface.getSize().x), static_cast<int>(textures::skinface.getSize().y)});

        shape.setPosition(100,100);

        window.draw(shape);
        // end the current frame

        window.display();
    }
}