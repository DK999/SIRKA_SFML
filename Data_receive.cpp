#include "Data_receive.h"

DataReceive::DataReceive(int port,int baud):
    m_port(port)
{   m_hello[0] = 0x00;
    m_hello[1] = 0xFF;
    m_hello[2] = 0x00;
    m_get_data_string[1] = 0x00;
    m_get_data_string[2] = 0x10;
    for ( int i = 0; i < 20; i++)
    {
        m_x_acc[i] = 0;
        m_y_acc[i] = 0;
        m_z_acc[i] = 0;
        m_x_gyr[i] = 0;
        m_y_gyr[i] = 0;
        m_z_gyr[i] = 0;
        m_x_mag[i] = 0;
        m_y_mag[i] = 0;
        m_z_mag[i] = 0;
        m_devicenumber[i] = 0;
    }
    for ( int i = 0; i < 30; i++)
    {
        m_buffer[i] = 0;
    }
    m_number_of_devices = 0;
    /* Open Serial Port */
    if ( m_serial.OpenPort(m_port,baud) == 0 )
    {   int success = 0;
        int nr = 0;
        /* Getting Number of Sensors and their address */
        for ( int i = 0; i < 20; i++)
        {   /* Send HELLO to Device */
            m_serial.SendByteArray(port,m_hello,sizeof(m_hello));
            if(m_serial.ReadByteArray(port,m_buffer,sizeof(m_buffer)))
            {   /* If Device responds, increase number of devices and note address */
                m_number_of_devices++;

                m_devicenumber[nr++] = (int)m_hello[0];


            }
            m_hello[0]++;
        }

    }else
    std::cout<<"Fehler bei COM PORT Initialisierung"<<std::endl;
}

void DataReceive::getValues(std::vector<sf::RectangleShape*> *RectVec)
{   std::vector<sf::RectangleShape*>::iterator Rectit = RectVec->begin();
    sf::RectangleShape *Vec_Pointer;

    for ( int i = 0; i < m_number_of_devices ; i++)
    {
        m_get_data_string[0] = m_devicenumber[i];
        m_serial.SendByteArray(m_port,m_get_data_string,sizeof(m_get_data_string));
        m_serial.ReadByteArray(m_port,m_buffer,sizeof(m_buffer));
        m_x_acc[0] = (int8_t) m_buffer[1]; 		// Cast MSB X to int8_t and make it signed in 16Bit Array
		m_y_acc[0] <<=8;							// Shift MSB X 4 Bits left to have space for LSB X
		m_z_acc[0] |= m_buffer[0];			// Do OR with first 4 Bits of LSB X
		m_x_gyr[0] = (int8_t) m_buffer[3];		// Cast MSB Y to int8_t and make it signed in 16Bit Array
		m_y_gyr[0] <<=8;							// Shift MSB X 4 Bits left to have space for LSB X
		m_z_gyr[0] |= m_buffer[2];			// Do OR with first 4 Bits of LSB X
		m_x_mag[0] = (int8_t) m_buffer[5];		// Cast MSB Z to int8_t and make it signed in 16Bit Array
		m_y_mag[0] <<=8;							// Shift MSB Z 4 Bits left to have space for LSB Z
		m_z_mag[0] |= m_buffer[4];			// Do OR with first 4 Bits of LSB Z
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_x_acc[0]));
        Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_y_acc[0]));
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_z_acc[0]));
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_x_gyr[0]));
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_y_gyr[0]));
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_z_gyr[0]));
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_x_mag[0]));
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_y_mag[0]));
		Vec_Pointer = *Rectit++;
		Vec_Pointer->setSize(sf::Vector2f(10, m_z_mag[0]));


    }

}
void DataReceive::initVector(std::vector<sf::RectangleShape*> *RectVec)
{   int rect_distance = 0;
    std::vector<sf::RectangleShape*>::iterator Rectit;
    sf::RectangleShape *Vec_Pointer;


    for( int i = 0; i < m_number_of_devices * 3 ; i++)
        {
        RectVec->push_back(new sf::RectangleShape(sf::Vector2f(0,0)));
        Rectit = RectVec->begin()+i;
        Vec_Pointer = *Rectit;
        Vec_Pointer->setFillColor(sf::Color::Green);
        Vec_Pointer->setPosition(sf::Vector2f(rect_distance,100));
        rect_distance+=10;
        if(i%3 == 0)
            rect_distance+=10;
    }
}
