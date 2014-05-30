#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <cstring>
#include <sstream>
#include "RS232.h"
#include "crc16.hpp"

#pragma once

class BMX055
{
public:
    BMX055( int port, int baud);
    ~BMX055();

    void getData(std::vector<sf::RectangleShape*> *RectVec, std::vector<sf::Text*> *Values);
    uint8_t getNumberOfDevices();
    uint8_t getGyroResolution();
    uint8_t getAccResolution();
    uint8_t getMeasurements();
    uint8_t* getDeviceNumbers();
    void changeAcc();
    void changeGyro();
    void changeMeasurements();
    void init_device(void);
    std::string convertInt(int number);

private:
    RS232::SerialConnection m_serial;
    CRC16 m_check;
    uint8_t m_number_of_devices, m_devicenumber[20], m_buffer[30], m_command[8];
    uint8_t volatile m_measurements;
    uint8_t m_gyro_res, m_acc_res;
    uint16_t m_port, m_baud,m_crc_ex, m_crc_int;
    int16_t m_sensor_data[10];
    float m_rect_correction;
};
