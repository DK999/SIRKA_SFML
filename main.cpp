#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "BMX055.hpp"
#include "GUI.hpp"
#include "Initiator.hpp"
#include "TextManager.hpp"

using namespace std;

int main()
{   vector<sf::RectangleShape*> RectVec;
    vector<sf::Text*> Values, Option, Headline;
    sf::Font font;
    sf::RenderWindow window(sf::VideoMode(1280, 1024), "SIRKA");
    sf::Clock clock;

     if (!font.loadFromFile("arialbd.ttf"))              // Initialize Arialbd as Font
        cout<<"ERROR loading font!"<<endl;
    else
    {   BMX055 Sensor(2, 1000000);  // Port 2, 3Mbaud/s
        Initiator Init;
        GUI Display;
        TextManager TManager;
        Init.setRectangles(&RectVec, Sensor.getNumberOfDevices());
        Init.setValues(&Values, &font, Sensor.getNumberOfDevices());
        Init.setHeadlines(&Headline, &font, Sensor.getNumberOfDevices(),Sensor.getDeviceNumbers());
        Init.setOptions(&Option,&font);
        Sensor.init_device();

        sf::Event event;
         while (window.isOpen())
        {

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                if ( event.type == sf::Event::KeyPressed)
                {
                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();
                else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    {
                        Sensor.changeAcc();
                        TManager.changeAcc(&Option,Sensor.getAccResolution());
                    }
                else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::G))
                    {
                        Sensor.changeGyro();
                        TManager.changeGyro(&Option,Sensor.getGyroResolution());
                    }
                else if ( sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                    {
                        Sensor.changeMeasurements();
                        TManager.changeMeasurements(&Option,Sensor.getMeasurements());
                    }
                }
            }
        clock.restart();
        Sensor.getData(&RectVec,&Values);
        TManager.changeTime(&Option,clock.getElapsedTime());
        Display.showData(&RectVec,&Values,&Headline,&Option,&window);

        }
    }


    RectVec.clear();
    Values.clear();
    Headline.clear();
    Option.clear();

    return 0;
}
