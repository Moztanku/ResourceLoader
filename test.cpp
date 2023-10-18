#include <Resources.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <memory>

#include <lib.hpp>
namespace textures = Resources::textures;
// namespace sfx = Resources::

#include <format>
#include <iostream>

int main()
{
    lib::myClass::getInstance();
    lib::test();

    // initialize the window

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // limit the framerate
    window.setFramerateLimit(60);

    sf::RectangleShape shape;
    shape.setSize({600,300});
    shape.setTexture(&textures::skinface);
    shape.setTextureRect({0,0, static_cast<int>(textures::skinface.getSize().x), static_cast<int>(textures::skinface.getSize().y)});
    shape.setPosition(100,100);

    sf::Sound sound{Resources::sounds::air_raid};
    sound.setPitch(0.5f);
    sound.setVolume(10.f);

    auto& music = *Resources::music::I_Miss_You;
    music.setPitch(0.25f);
    music.setVolume(20.f);

    // create loop
    while (window.isOpen())
    {
        if(sound.getStatus() != sf::Sound::Status::Playing){
            sound.play();
        }
        
        // handle events

        if(Resources::music::I_Miss_You->getStatus() != sf::Music::Status::Playing){
            Resources::music::I_Miss_You->play();
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color

        window.clear(sf::Color::Black);

        // draw everything here...

        window.draw(shape);
        // end the current frame

        window.display();
    }
}