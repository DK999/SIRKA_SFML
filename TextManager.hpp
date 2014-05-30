#include <SFML/Graphics.hpp>
#include <vector>
#include <stdint.h>
#include <cstring>
#include <sstream>

#pragma once

class TextManager
{
public:
    TextManager();
    void changeAcc(std::vector<sf::Text*> *Resolutions, uint8_t Res);
    void changeGyro(std::vector<sf::Text*> *Resolutions, uint8_t Res);
    void changeMeasurements(std::vector<sf::Text*> *Resolutions, uint8_t Meas);
    void changeTime(std::vector<sf::Text*> *Resolutions, sf::Time Time);
    std::string convertInt(int number);
private:
    std::string m_GyroText[5], m_AccText[4], m_MeasurementText[4];
};
