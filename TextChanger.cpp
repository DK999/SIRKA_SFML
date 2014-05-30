#include "TextManager.hpp"

TextManager::TextManager()
{
    m_GyroText[0] = "<G> Gyro : 61,0 m°/s /LSB";
    m_GyroText[1] = "<G> Gyro : 30,5 m°/s /LSB";
    m_GyroText[2] = "<G> Gyro : 15,3 m°/s /LSB";
    m_GyroText[3] = "<G> Gyro :  7,6 m°/s /LSB";
    m_GyroText[4] = "<G> Gyro :  3,8 m°/s /LSB";
    m_AccText[0] = "<A> Acc : 0,98mg /LSB";
    m_AccText[1] = "<A>  Acc : 1,95mg /LSB";
    m_AccText[2] = "<A>  Acc : 3,91mg /LSB";
    m_AccText[3] = "<A>  Acc : 7,81mg /LSB";
    m_MeasurementText[0] = "<M> Meas per Cycle: 16";
    m_MeasurementText[1] = "<M> Meas per Cycle:  8";
    m_MeasurementText[2] = "<M> Meas per Cycle:  4";
    m_MeasurementText[3] = "<M> Meas per Cycle:  1";
}

void TextManager::changeAcc(std::vector<sf::Text*> *Resolutions, uint8_t Res)
{
    std::vector<sf::Text*>::iterator Resit = Resolutions->begin()+1;
    sf::Text *Res_Pointer = *Resit;

    Res_Pointer->setString(m_AccText[Res]);

}

void TextManager::changeGyro(std::vector<sf::Text*> *Resolutions, uint8_t Res)
{
    std::vector<sf::Text*>::iterator Resit = Resolutions->begin()+2;
    sf::Text *Res_Pointer = *Resit;

    Res_Pointer->setString(m_GyroText[Res]);

}

void TextManager::changeMeasurements(std::vector<sf::Text*> *Resolutions, uint8_t Meas)
{
    std::vector<sf::Text*>::iterator Resit = Resolutions->begin();
    sf::Text *Res_Pointer = *Resit;

    Res_Pointer->setString(m_MeasurementText[Meas]);

}

void TextManager::changeTime(std::vector<sf::Text*> *Resolutions, sf::Time Time)
{
    std::vector<sf::Text*>::iterator Textit = Resolutions->begin()+3;
    sf::Text *Text_Pointer = *Textit;
    std::string m_Text = " Elapsed time (µs) :  ";
    m_Text.replace(m_Text.begin()+21,m_Text.end(),convertInt(Time.asMicroseconds()));
    Text_Pointer->setString(m_Text);


}

std::string TextManager::convertInt(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}
