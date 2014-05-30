#include "BMX055.hpp"

#define PREAMBLE_ONE 0
#define PREAMBLE_TWO 1
#define LENGTH 2
#define ADDRESS 3
#define COMMAND 4
#define PARAMETER 5
#define CRCL 6
#define CRCH 7

BMX055::BMX055(int port, int baud):
    m_number_of_devices(0), m_measurements(0x10), m_gyro_res(0), m_acc_res(0), m_port(port), m_baud(baud),
    m_crc_ex(0), m_crc_int(0), m_rect_correction(-0.1)
{
    int i;
    for ( i = 0; i < 20; i++)                   //_________________________________
        m_devicenumber[i] = 0;                  //
    for ( i = 0; i < 30; i++)                   //  Initialize everything with zero
        m_buffer[i] = 0;                        //
    for ( i = 0; i < 10; i++)                   //
        m_sensor_data[i] = 0;                   //__________________________________

    m_command[PREAMBLE_ONE] = 0xAA;             // PREAMBLE
    m_command[PREAMBLE_TWO] = 0xAA;             // PREAMBLE
    m_command[ADDRESS] = 0x00;                  // set Address to 00
    m_command[LENGTH] = 8;                      // set FRAME LENGTH
    m_command[COMMAND] = 0x14;                  // Choose "send_hello" command
    m_command[PARAMETER] = 0x00;                // No Parameter needed
                                                //__________________________________

    if(m_serial.OpenPort(port,baud) == 0)
    {
        int nr = 0;

         for ( int i = 0; i < 20; i++)          // m_check for 20 Sensors
        {
            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );

             m_serial.SendByteArray(port,m_command,sizeof(m_command));    // Send Hello Byte
             if(m_serial.ReadByteArray(port,m_buffer,sizeof(m_buffer)))   // Read if answer available
                {   /* If Device responds, increase number of devices and note address */
                    m_number_of_devices++;                            // If answer, increase nr of devices
                    m_devicenumber[nr++] = (int)m_command[ADDRESS];           // save address to devicenumber
                }
                m_command[ADDRESS]++;                                       // increase address
        }
    }else
    std::cout<<"Fehler bei COM PORT Initialisierung"<<std::endl;    // show failure
}

void BMX055::getData(std::vector<sf::RectangleShape*> *RectVec, std::vector<sf::Text*> *Values)
{
    int j, bytes_received;                                                  // Count-Value and storage for amout of bytes received
    std::vector<sf::RectangleShape*>::iterator Rectit = RectVec->begin();   // Iterator for Vector of Rectangles
    sf::RectangleShape *Vec_Pointer;                                        // Pointer to Vector-Address
    std::vector<sf::Text*>::iterator Textit = Values->begin();              // Iterator vor Vector of Values
    sf::Text *Values_Pointer;                                               // Pointer to Vector-Address

    m_command[LENGTH] = 8;

        for ( int i = 0; i < m_number_of_devices; i++)                      // Send to every found Sensor Device
        {
            m_command[PARAMETER] = m_measurements;                          // Use user defined amount Measurements
            m_command[COMMAND] = 0x00;                                      // Set command "get all data"
            m_command[ADDRESS]=(uint8_t) m_devicenumber[i];                 // store device address

            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );

            m_serial.SendByteArray(m_port,m_command,sizeof(m_command));       // send "get all data" command to sensor
            bytes_received = m_serial.ReadByteArray(m_port,m_buffer,sizeof(m_buffer)); // read answer from sensor

            m_crc_ex = (uint16_t) m_buffer[bytes_received-1];               // Store CRC-m_checksum
            m_crc_ex <<= 8;
            m_crc_ex |= m_buffer[bytes_received-2];
            m_crc_int = 0;                                                  // Initialize CRC-Value with zero
            j = 4;

            for( int l = 0; l < bytes_received-2; l++)                      // Create CRC-m_checksum
                m_crc_int = m_check.CreateCRC(m_crc_int,m_buffer[l]);

            for ( int k = 0; k < (bytes_received-2)/2 ; k++)                // Store data
            {
                m_sensor_data[k] = (int8_t) m_buffer[j+1];                  // Make 16Bit Variable out of two 8 Bit Variables
                m_sensor_data[k]<<=8;
                m_sensor_data[k] |= m_buffer[j];
                j+=2;
            }

            if( m_crc_int == m_crc_ex )                                     // if m_checksum is equal
            {
                for ( j = 0; j < 9; j++)                                    // Do 9 times ( number of Bars per Device )
                {
                    Vec_Pointer = *Rectit++;                                // point to bar in Vector
                    Vec_Pointer->setSize(sf::Vector2f(20, m_sensor_data[j]*m_rect_correction));   // set new value to bar
                    Values_Pointer = *Textit++;                             // point to Value in Vector
                    Values_Pointer->setString(convertInt(m_sensor_data[j]));// Store value into string
                }

            }
            else
            {   std::cout<<"CRC failed - CRC-INT = "<<m_crc_int<<" CRC-EX = "<<m_crc_ex<<std::endl;   // Show message to console
                i--;                                    // retry sensor
            }

        }

}

std::string BMX055::convertInt(int number)
{
    std::stringstream ss;
    ss << number;
    return ss.str();
}

uint8_t BMX055::getNumberOfDevices()
{
    return m_number_of_devices;
}

void BMX055::changeAcc()
{   m_command[LENGTH] = 8;
   m_acc_res++;
   if(m_acc_res >3)
       m_acc_res = 0;
   for ( int i = 0; i < m_number_of_devices; i++)
   {
        m_command[ADDRESS]=(uint8_t) m_devicenumber[i];
        if ( m_acc_res == 0)
        {   m_command[COMMAND] = 0x11;
            m_command[PARAMETER] = 0x03;
            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
            m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
        }
        else if ( m_acc_res == 1)
        {   m_command[COMMAND] = 0x11;
            m_command[PARAMETER] = 0x05;
            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
            m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
        }
        else if ( m_acc_res == 2)
        {   m_command[COMMAND] = 0x11;
            m_command[PARAMETER] = 0x08;
            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
            m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
        }
        else if( m_acc_res == 3)
        {   m_command[COMMAND] = 0x11;
            m_command[PARAMETER] = 0x0C;
            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
            m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
        }
   }
}

void BMX055::changeGyro()
{   m_command[LENGTH] = 8;
    m_gyro_res++;
    if(m_gyro_res >4)
        m_gyro_res = 0;
       for ( int i = 0; i < m_number_of_devices; i++)
        {
            m_command[ADDRESS]=(uint8_t) m_devicenumber[i];
            if ( m_gyro_res == 0)
            {   m_command[COMMAND] = 0x10;
                m_command[PARAMETER] = 0x00;
                m_crc_int = 0;
                for(int crc_count = 0; crc_count < 6 ; crc_count++)
                {
                    m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
                }
                m_command[CRCL] = (uint8_t)m_crc_int;
                m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
                m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
            }
            else if ( m_gyro_res == 1)
            {   m_command[COMMAND] = 0x10;
                m_command[PARAMETER] = 0x01;
                m_crc_int = 0;
                for(int crc_count = 0; crc_count < 6 ; crc_count++)
                {
                    m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
                }
                m_command[CRCL] = (uint8_t)m_crc_int;
                m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
                m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
            }
            else if ( m_gyro_res == 2)
            {   m_command[COMMAND] = 0x10;
                m_command[PARAMETER] = 0x02;
                m_crc_int = 0;
                for(int crc_count = 0; crc_count < 6 ; crc_count++)
                {
                    m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
                }
                m_command[CRCL] = (uint8_t)m_crc_int;
                m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
                m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
            }
            else if( m_gyro_res == 3)
            {   m_command[COMMAND] = 0x10;
                m_command[PARAMETER] = 0x03;
                m_crc_int = 0;
                for(int crc_count = 0; crc_count < 6 ; crc_count++)
                {
                    m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
                }
                m_command[CRCL] = (uint8_t)m_crc_int;
                m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
                m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
            }
            else if( m_gyro_res == 4)
            {   m_command[COMMAND] = 0x10;
                m_command[PARAMETER] = 0x04;
                m_crc_int = 0;
                for(int crc_count = 0; crc_count < 6 ; crc_count++)
                {
                    m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
                }
                m_command[CRCL] = (uint8_t)m_crc_int;
                m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
                m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
            }
        }
}

void BMX055::changeMeasurements()
{
    if(m_measurements == 0x01)
        m_measurements = 0x04;
    else if(m_measurements == 0x04)
        m_measurements = 0x08;
    else if(m_measurements == 0x08)
        m_measurements = 0x10;
    else if(m_measurements == 0x10)
        m_measurements = 0x01;
}

uint8_t BMX055::getAccResolution()
{
    return m_acc_res;
}

uint8_t BMX055::getGyroResolution()
{
    return m_gyro_res;
}

uint8_t BMX055::getMeasurements()
{
   if ( m_measurements == 0x01)
        return (0x03);
   else if ( m_measurements == 0x04 )
        return (0x02);
   else if ( m_measurements == 0x08 )
        return (0x01);
   else if ( m_measurements == 0x10 )
        return (0x00);
   return (0x00);
}

uint8_t* BMX055::getDeviceNumbers()
{
    return m_devicenumber;
}

void BMX055::init_device()
{   m_command[LENGTH] = 8;
     for ( int i = 0; i < m_number_of_devices; i++)
        {
            m_command[ADDRESS]=(uint8_t) m_devicenumber[i];
            m_command[COMMAND] = 0x10;
            m_command[PARAMETER] = 0x00;
            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
            m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
            m_command[COMMAND] = 0x11;
            m_command[PARAMETER] = 0x03;
            m_crc_int = 0;
            for(int crc_count = 0; crc_count < 6 ; crc_count++)
            {
                m_crc_int = m_check.CreateCRC(m_crc_int,m_command[crc_count]);
            }
            m_command[CRCL] = (uint8_t)m_crc_int;
            m_command[CRCH] = (uint8_t)(m_crc_int >> 8 );
            m_serial.SendByteArray(m_port,m_command,sizeof(m_command));
        }

}

BMX055::~BMX055(void)
{
    m_serial.ClosePort(m_port);
}
