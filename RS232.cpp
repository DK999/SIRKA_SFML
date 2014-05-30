///***Source for RS232 wrapper class***///
//--------------------------------------//
/*  This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "RS232.h"
#include <iostream>

using namespace RS232;

#ifndef __LINUX__
//windows definitions

const char SerialConnection::m_comPorts[16][10] = {"\\\\.\\COM0",  "\\\\.\\COM1",  "\\\\.\\COM2",  "\\\\.\\COM3",
												    "\\\\.\\COM4",  "\\\\.\\COM5",  "\\\\.\\COM6",  "\\\\.\\COM7",
													"\\\\.\\COM8",  "\\\\.\\COM9", "\\\\.\\COM10", "\\\\.\\COM11",
													"\\\\.\\COM12", "\\\\.\\COM13", "\\\\.\\COM14", "\\\\.\\COM15"};

SerialConnection::~SerialConnection()
{
	//close any open ports
	for(unsigned i = 0; i < 16; i++)
	{
		if(m_comPortHandles[i] != INVALID_HANDLE_VALUE)
			CloseHandle(m_comPortHandles[i]);
	}
}

int SerialConnection::OpenPort(const unsigned short port, unsigned baud)
{
	//m_check valid number
	if(port < 0 || port > 15)
	{
		std::cout << port << ": Invalid COM port number." << std::endl;
		return 1;
	}

	//create baudrate string
	switch(baud)
	{
    case 110:
		strcpy(m_baudRate, "baud=110 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 300:
		strcpy(m_baudRate, "baud=300 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 600:
		strcpy(m_baudRate, "baud=600 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 1200:
		strcpy(m_baudRate, "baud=1200 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 2400:
		strcpy(m_baudRate, "baud=2400 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 4800:
		strcpy(m_baudRate, "baud=4800 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 9600:
		strcpy(m_baudRate, "baud=9600 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 19200:
		strcpy(m_baudRate, "baud=19200 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 38400:
		strcpy(m_baudRate, "baud=38400 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 57600:
		strcpy(m_baudRate, "baud=57600 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 115200:
		strcpy(m_baudRate, "baud=115200 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 128000:
		strcpy(m_baudRate, "baud=128000 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 256000:
		strcpy(m_baudRate, "baud=256000 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 500000:
		strcpy(m_baudRate, "baud=500000 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 1000000:
		strcpy(m_baudRate, "baud=1000000 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 2000000:
		strcpy(m_baudRate, "baud=2000000 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    case 3000000:
		strcpy(m_baudRate, "baud=3000000 data=8 parity=N stop=1 dtr=on rts=on");
        break;
    default:
		std::cout << baud << ": Invalid baud rate" << std::endl;
        return 1;
	}

	//get a handle to requested COM port
	m_comPortHandles[port] = CreateFileA(m_comPorts[port],
										GENERIC_READ | GENERIC_WRITE,
										0, //no share
										0, //no security
										OPEN_EXISTING,
										0, //no threads
										0); //no template

	//m_check if port was created successfully
	if(m_comPortHandles[port] == INVALID_HANDLE_VALUE)
	{
		std::cout << "Unable to open COM port " << port << std::endl;
		return 1;
	}
	//clear the settings struct and apply to port
	memset(&m_portSettings, 0, sizeof(m_portSettings));
	m_portSettings.DCBlength = sizeof(m_portSettings);

	if(!BuildCommDCBA(m_baudRate, &m_portSettings))
	{
		std::cout << "Unable to apply COM port DCB settings." << std::endl;
		CloseHandle(m_comPortHandles[port]);
		return 1;
	}

	if(!SetCommState(m_comPortHandles[port], &m_portSettings))
	{
		std::cout << "Unable to apply COM port cfg settings." << std::endl;
		CloseHandle(m_comPortHandles[port]);
		return 1;
	}

	//update timeout struct
	m_commTimeouts.ReadIntervalTimeout = 0;
	m_commTimeouts.ReadTotalTimeoutConstant = 1;
	m_commTimeouts.ReadTotalTimeoutMultiplier = 1;
	m_commTimeouts.WriteTotalTimeoutConstant = 1;
	m_commTimeouts.WriteTotalTimeoutMultiplier = 1;

	//apply timeout settings
	if(!SetCommTimeouts(m_comPortHandles[port], &m_commTimeouts))
	{
		std::cout << "Unable to apply COM port timeout settings." << std::endl;
		CloseHandle(m_comPortHandles[port]);
		return 1;
	}
	//success!!
	return 0;
}

int SerialConnection::ReadByte(const unsigned short port, unsigned char& byte)
{
	if(m_comPortHandles[port] != INVALID_HANDLE_VALUE)
	{
		int readResult;
		ReadFile(m_comPortHandles[port], &byte, 1, (LPDWORD)((void*)&readResult), 0);
		if(readResult < 0) return -1;
		return 0;
	}
	return 1;
}

int SerialConnection::ReadByteArray(const unsigned short port, unsigned char* buffer, unsigned short size)
{
	if(m_comPortHandles[port] != INVALID_HANDLE_VALUE)
	{
		int readSize;
		//limit size to 4kb
		if(size > 4096) size = 4096;

		ReadFile(m_comPortHandles[port], buffer, size, (LPDWORD)((void*)&readSize), 0);

		return readSize;
	}
	return -1;
}

int SerialConnection::SendByte(const unsigned short port, unsigned char byte)
{
	if(m_comPortHandles[port] != INVALID_HANDLE_VALUE)
	{
		int sendResult;
		WriteFile(m_comPortHandles[port], &byte, 1, (LPDWORD)((void*)&sendResult), 0);
		if(sendResult < 0) return 1;
		return 0;
	}
	return 1;
}

int SerialConnection::SendByteArray(const unsigned short port, unsigned char* buffer, unsigned short size)
{
	if(m_comPortHandles[port] != INVALID_HANDLE_VALUE)
	{
		int sendResult;
		if(WriteFile(m_comPortHandles[port], buffer, size, (LPDWORD)((void*)&sendResult), 0))
		{
			return sendResult;
		}
		return -1;
	}
	return -1;
}

int SerialConnection::ClosePort(const unsigned short port)
{
	if(m_comPortHandles[port] != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_comPortHandles[port]);
		return 0;
	}
	return 1;
}

#else
//linux

//being base 0 sometimes requesting COM4 means you need to pass 3 as the port number - m_check this when trying to open a new port
//and you get an error saying port is unavailable.
const char SerialConnection::m_comPorts[30][16] = {"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
													"/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
													"/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
													"/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
													"/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1",
													"/dev/rfcomm0","/dev/rfcomm1","/dev/ircomm0","/dev/ircomm1"};

SerialConnection::~SerialConnection()
{
	//close any open ports on the way out...
	//for(unsigned i = 0; i < 30; i++)
	//{
	//	if(m_comPortHandles[i] != -1)
	//		ClosePort(i);
	//}
}

int SerialConnection::OpenPort(const unsigned short port, unsigned baud)
{
	int baudrate, status;

	//m_check valid port number
	if(port < 0 || port > 29)
	{
		std::cout << port << ": Invalid port number." << std::endl;
		return 1;
	}

	//set baudrate
	switch(baud)
	{
	case 50:
		baudrate = B50;
		break;
	case 75:
		baudrate = B75;
		break;
	case 110:
		baudrate = B110;
		break;
	case 134:
		baudrate = B134;
		break;
	case 150:
		baudrate = B150;
		break;
	case 200:
		baudrate = B200;
		break;
	case 300:
		baudrate = B300;
		break;
	case 600:
		baudrate = B600;
		break;
	case 1200:
		baudrate = B1200;
		break;
	case 1800:
		baudrate = B1800;
		break;
	case 2400:
		baudrate = B2400;
		break;
	case 4800:
		baudrate = B4800;
		break;
	case 9600:
		baudrate = B9600;
		break;
	case 19200:
		baudrate = B19200;
		break;
	case 38400:
		baudrate = B38400;
		break;
	case 57600:
		baudrate = B57600;
		break;
	case 115200:
		baudrate = B115200;
		break;
	case 230400:
		baudrate = B230400;
		break;
	case 460800:
		baudrate = B460800;
		break;
	case 500000:
		baudrate = B500000;
		break;
	case 576000:
		baudrate = B576000;
		break;
	case 921600:
		baudrate = B921600;
		break;
	case 1000000:
		baudrate = B1000000;
		break;
	default: std::cout << baud << ": Invalid baudrate." << std::endl;
		return 1;
	}

	//attempt to open port
	m_comPortHandles[port] = open(m_comPorts[port], O_RDWR | O_NOCTTY | O_NDELAY);
	if(m_comPortHandles[port] == -1)
	{
		//C++ equivalant of this?
		perror("SerConn::OpenPort: Unable to open port");
		return 1;
	}
	//apply port settings
	error = tcgetattr(m_comPortHandles[port], m_oldPortSettings + port);
	if(error == -1)
	{
		close(m_comPortHandles[port]);
		perror("SerConn::OpenPort: Unable to read port settings");
		return 1;
	}
	//clear and set port settings struct
	memset(&m_newPortSettings, 0, sizeof(m_newPortSettings));
	m_newPortSettings.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
	m_newPortSettings.c_iflag = IGNPAR;
	m_newPortSettings.c_oflag = 0;
	m_newPortSettings.c_lflag = 0;
	m_newPortSettings.c_cc[VMIN] = 0; //block until n bytes rcd
	m_newPortSettings.c_cc[VTIME] = 0; //block until timer expires

	error = tcsetattr(m_comPortHandles[port], TCSANOW, &m_newPortSettings);
	if(error == -1)
	{
		close(m_comPortHandles[port]);
		perror("SerConn::OpenPort: Unable to apply port settings");
		return 1;
	}

	//m_check port status
	if(ioctl(m_comPortHandles[port], TIOCMGET, &status) == -1)
	{
		perror("SerConn::OpenPort: Unable to get port status");
		return 1;
	}
	//enable dtr and rts
	status |= TIOCM_DTR;
	status |= TIOCM_RTS;
	//update port status
	if(ioctl(m_comPortHandles[port], TIOCMSET, &status) == -1)
	{
		perror("SerConn::OpenPort: Unable to update port status");
		return 1;
	}
	return 0;
}

int SerialConnection::ReadByte(const unsigned short port, unsigned char& byte)
{
	if(m_comPortHandles[port] == -1)
	{
		perror("SerConn::ReadByte: Invalid port specified");
		return 1;
	}

	read(m_comPortHandles[port], &byte, 1);
	return 0;
}

int SerialConnection::ReadByteArray(const unsigned short port, unsigned char* buffer, unsigned short size)
{
	if(m_comPortHandles[port] == -1)
	{
		perror("SerConn::ReadByteArray: Invalid port specified");
		return -1;
	}

#ifndef __STRICT_ANSI__ //-ansi flag in GCC
	if(size > SSIZE_MAX) size = (int)SSIZE_MAX;
#else
	if(size > 4096) size = 4096;
#endif

	return read(m_comPortHandles[port], buffer, size);
}

int SerialConnection::SendByte(const unsigned short port, unsigned char byte)
{
	if(m_comPortHandles[port] == -1)
	{
		perror("SerConn::SendByte: Invalid port specified");
		return -1;
	}
	int n = write(m_comPortHandles[port], &byte, 1);
	if(n < 0) return -1;
	return 0;
}

int SerialConnection::SendByteArray(const unsigned short port, unsigned char* buffer, unsigned short size)
{
	if(m_comPortHandles[port] == -1)
	{
		perror("SerConn::SendByteArray: Invalid port specified");
		return -1;
	}
	return write(m_comPortHandles[port], buffer, size);
}

int SerialConnection::ClosePort(const unsigned short port)
{
	int status;
	if(ioctl(m_comPortHandles[port], TIOCMGET, &status) == -1)
	{
		perror("SerConn::ClosePort: Unable to get port status");
	}
	//turn off DTR and RTS
	status &= ~TIOCM_DTR;
	status &= ~TIOCM_RTS;

	if(ioctl(m_comPortHandles[port], TIOCMSET, &status) == -1)
	{
		perror("SerConn::ClosePort: Unable to set port status");
	}

	close(m_comPortHandles[port]);
	tcsetattr(m_comPortHandles[port], TCSANOW, m_oldPortSettings + port);
	return 0;
}

#endif //__LINUX__
