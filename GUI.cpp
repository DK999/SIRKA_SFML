#include "GUI.hpp"

void GUI::showData(std::vector<sf::RectangleShape*> *RectVec,std::vector<sf::Text*> *Values, std::vector<sf::Text*> *Headlines,std::vector<sf::Text*> *Option, sf::RenderWindow *window)
{
    std::vector<sf::RectangleShape*>::iterator Rectit = RectVec->begin();   // Let iterator show at beginning of vector
    std::vector<sf::Text*>::iterator Valit = Values->begin();
    std::vector<sf::Text*>::iterator Headit = Headlines->begin();
    std::vector<sf::Text*>::iterator Optionit = Option->begin();
    sf::RectangleShape *Vec_Pointer;                                        // Pointer for vector elements
    sf::Text *Values_pointer, *Headlines_pointer, *Options_pointer;

    window->clear();

    for( Rectit = RectVec->begin(); Rectit < RectVec->end(); Rectit++)
    {
        Vec_Pointer = *Rectit;
        window->draw(*Vec_Pointer);
    }
    for( Valit = Values->begin(); Valit < Values->end(); Valit++)
    {
        Values_pointer = *Valit;
        window->draw(*Values_pointer);
    }
    for( Headit = Headlines->begin(); Headit < Headlines->end(); Headit++)
    {
        Headlines_pointer = *Headit;
        window->draw(*Headlines_pointer);
    }
    for( Optionit = Option->begin(); Optionit < Option->end(); Optionit++)
    {
        Options_pointer = *Optionit;
        window->draw(*Options_pointer);
    }

    window->display();
}
