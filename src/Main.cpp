#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Game.hpp"
#include <memory>
#include <iostream>

#include "ResourceManager.hpp"

int main(int argc, char* argv[])
{
    // ResourceManager Must be Instantiated here -- DO NOT CHANGE
    ResourceManager::init(argv[0]);

    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Survive");
    window.setKeyRepeatEnabled(false);
    
    std::unique_ptr<Game> pGame = std::make_unique<Game>();
    if (!pGame->initialise())
    {
        std::cerr << "Game Failed to initialise" << std::endl;
        return 1;
    }
    
    sf::Clock clock;
	sf::Color background = sf::Color(10, 55, 2, 255);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                    // "close requested" event: we close the window
                    window.close();
                    break;
                case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						break;
					}
					if (event.key.code == sf::Keyboard::U)
					{
						// cheat code
						pGame->openUpgradeMenu();
						break;
					}
                    pGame->onKeyPressed(event.key.code);
                    break;
                case sf::Event::KeyReleased:
                    pGame->onKeyReleased(event.key.code);
                    break;
                default:
                    break;
            }
        }
        
        sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();
        pGame->update(elapsedTime.asSeconds());
        
        // clear the window
        window.clear(background);
        
        window.draw(*pGame.get());
        
        // end the current frame
        window.display();
    }
    
    return 0;
}
