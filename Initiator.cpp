#include "Initiator.hpp"
/*
 *  Initializes rectangles, number depends on found devices.
 *  Creates 9 Rectangles per device with a distance of 20 pixels between sensors
 */
void Initiator::setRectangles(std::vector<sf::RectangleShape*> *RectVec,  uint8_t m_number_of_devices)
{   int rect_distance = 0;
    std::vector<sf::RectangleShape*>::iterator Rectit;      // Iterator for Vector of rectangles
    sf::RectangleShape *Vec_Pointer;                        // Pointer for Vektor

    for( int i = 0; i < m_number_of_devices * 9 ; i++)        // Create 9 Bars for each Sensor
        {
            if((i % 3 == 0) && (i != 0))
                rect_distance +=20;                             // increase distance by 20 pixels for space
            RectVec->push_back(new sf::RectangleShape(sf::Vector2f(0,0)));  // create new ractangle
            Rectit = RectVec->begin()+i;                        // point to last created element
            Vec_Pointer = *Rectit;                              // Get address of element
            if ( i % 3 == 0)
                Vec_Pointer->setFillColor(sf::Color(0,100,0,255));  // Set color for Bar 1
            else if ( i % 3 == 1)
                Vec_Pointer->setFillColor(sf::Color(138,43,226,255));   // Set color for Bar 2
            else if ( i % 3 == 2)
                Vec_Pointer->setFillColor(sf::Color::Red);              // Set color for Bar 3
            Vec_Pointer->setPosition(sf::Vector2f(rect_distance,100));  // Set Position
            rect_distance+=20;
        }

}
/*
 *  Initializes values, number depends on found devices.
 *  Creates 9 values per device with a distance of 20 pixels between sensors
 */
void Initiator::setValues(std::vector<sf::Text*> *Values, sf::Font *font, uint8_t m_number_of_devices)
{
    std::vector<sf::Text*>::iterator Textit;
    sf::Text *Values_Pointer;
    int x_val_num = 0;
    int y_val_num = 100;

    for( int i = 0; i < m_number_of_devices * 9 ; i++)        // Create 9 Values for each Sensor
        {
            Values->push_back(new sf::Text);  // create new ractangle
            Textit = Values->begin()+i;                        // point to last created element
            Values_Pointer = *Textit;                              // Get address of element

            Values_Pointer->setFont(*font);
            Values_Pointer->setCharacterSize(14);
            Values_Pointer->setColor(sf::Color(190,190,190,255));     // set color to gray
            Values_Pointer->setRotation(270.0);                       // rotate upward
            Values_Pointer->setPosition(sf::Vector2f(x_val_num,y_val_num));
            x_val_num+=20;                                      // change x position to next bar
            if( i % 3 == 2 && i != 0)                           // Put space every 4th value
                x_val_num+=20;
        }

}
/*
 *  Initializes Headlines, number depends on found devices.
 *  Creates 3 Headlines per device with a distance of 20 pixels between sensors
 *  Uses devicenumber to dynamically create Headlines for found device numbers
 */
void Initiator::setHeadlines(std::vector<sf::Text*> *Headlines, sf::Font *font, uint8_t m_number_of_devices, uint8_t *m_devicenumber)
{
    std::vector<sf::Text*>::iterator Headit;
    sf::Text *Headline_Pointer;
    std::string Gyro,Acc,Mag;
    Gyro = "Gyro ";
    Acc = "Acc ";
    Mag = "Mag ";
    int x_val_head = 20;
    int y_val_head = 0;
    int sensor_number = 0;

   for ( int i = 0; i < m_number_of_devices * 3; i++)            // Set basics of Headlines
    {
        Headlines->push_back(new sf::Text);  // create new ractangle
        Headit = Headlines->begin()+i;                        // point to last created element
        Headline_Pointer = *Headit;                              // Get address of element
        Headline_Pointer->setFont(*font);
        Headline_Pointer->setCharacterSize(14);
        Headline_Pointer->setColor(sf::Color::White);
        Headline_Pointer->setPosition(sf::Vector2f(x_val_head,y_val_head));
        x_val_head += 80;
        if(i % 3 == 0)
            {
            Gyro.replace(Gyro.end()-1,Gyro.end(),convertInt(m_devicenumber[sensor_number]));
            Headline_Pointer->setString(Gyro);
            }
        else if ( i % 3 == 1 )
            {
            Acc.replace(Acc.end()-1,Acc.end(),convertInt(m_devicenumber[sensor_number]));
            Headline_Pointer->setString(Acc);
            }
        else if ( i % 3 == 2 )
            {
            Mag.replace(Mag.end()-1,Mag.end(),convertInt(m_devicenumber[sensor_number++]));
            Headline_Pointer->setString(Mag);
            }
    }
}
/*
 *  Initializes Options
 */
void Initiator::setOptions(std::vector<sf::Text*> *Options, sf::Font *font)
{   std::string m_Text[4];
    m_Text[3] = " Elapsed time (µs) :  ";
    m_Text[2] = "<G> Gyro : 61,0 m°/s /LSB";
    m_Text[1] = "<A> Acc : 0,98mg /LSB";
    m_Text[0] = "<M> Meas per Cycle: 16";
    std::vector<sf::Text*>::iterator Optionit;
    sf::Text *Option_Pointer;
    int x_val = 0;
    for(int i = 0; i < 4 ; i++)
    {
        Options->push_back(new sf::Text);
        Optionit = Options->begin()+i;
        Option_Pointer = *Optionit;
        Option_Pointer->setFont(*font);
        Option_Pointer->setCharacterSize(14);
        Option_Pointer->setColor(sf::Color::White);
        Option_Pointer->setPosition(sf::Vector2f(x_val,1000));
        Option_Pointer->setString(m_Text[i]);
        x_val+=200;

    }
}

std::string Initiator::convertInt(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}
