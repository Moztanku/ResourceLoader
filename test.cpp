#include <Resources.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <memory>
namespace textures = Resources::textures;
// namespace sfx = Resources::

    std::unique_ptr<sf::Music> make_music(const std::string& path)
    {
        auto music = std::make_unique<sf::Music>();
        if(!music->openFromFile(path))
            throw std::runtime_error("Failed to load music " + path);
        return music;
    }

int main()
{
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

    auto& music = *Resources::music::I_Miss_You;

    // create loop
    while (window.isOpen())
    {
        if(sound.getStatus() != sf::Sound::Status::Playing){
            sound.setPitch(1.0f + (rand() % 100) / 100.0f);
            sound.play();
        }
        
        // handle events

        if(Resources::music::I_Miss_You->getStatus() != sf::Music::Status::Playing){
            Resources::music::I_Miss_You->setPitch(1.0f + (rand() % 100) / 100.0f);
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