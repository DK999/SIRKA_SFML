#include <SFML/Graphics.hpp>
#include <vector>
#include <stdint.h>
#include <cstring>
#include <sstream>

#pragma once

class Initiator
{
public:
    void setRectangles(std::vector<sf::RectangleShape*> *RectVec,  uint8_t m_number_of_devices);
    void setValues(std::vector<sf::Text*> *Values, sf::Font *font,  uint8_t m_number_of_devices);
    void setHeadlines(std::vector<sf::Text*> *Headlines, sf::Font *font,  uint8_t m_number_of_devices, uint8_t *m_devicenumber);
    void setOptions(std::vector<sf::Text*> *Options, sf::Font *font);
    std::string convertInt(int number);
};
