#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#pragma once

class GUI
{
public:
    void showData(std::vector<sf::RectangleShape*> *RectVec,std::vector<sf::Text*> *Values, std::vector<sf::Text*> *Headlines,std::vector<sf::Text*> *Option, sf::RenderWindow *Window);
private:

};
