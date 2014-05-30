/***Header file for RS232 class which acts as a C++ wrapper for OS specific serial functions***/
//------------------------------------------------------------------------------------------------//
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

#ifndef RS232_H_
#define RS232_H_

#include <cstring>

#ifndef __LINUX__
//windows
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

#else
//linux
#include <cstdio>

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#endif //__LINUX__

//-------------Class Declaration----------------//
namespace RS232
{
	class SerialConnection
	{
	public:
		~SerialConnection(); //closes any ports on destruction

		//attempts to open port at given baud. Port numbers start at 0, returns 1 if error
		int OpenPort(const unsigned short port, unsigned baud);
		//attempts to read a single byte from port. returns 0 if successful
		//else returns 1 if failed or no bytes to read. received data is stored in byte. blocking
		int ReadByte(const unsigned short port, unsigned char& byte);
		//fills buffer with any received bytes up to size and returns number of bytes received
		//or -1 if port is invalid. blocking.
		int ReadByteArray(const unsigned short port, unsigned char* buffer, unsigned short size);
		//attempts to send byte on port if port is open. returns 1 on failure
		int SendByte(const unsigned short port, unsigned char byte);
		//attempts to send an array of bytes up to size on port if port is open. returns -1 on error
		//else returns number of bytes sent. blocking.
		int SendByteArray(const unsigned short port, unsigned char* buffer, unsigned short size);
		//closes port if port is open
		int ClosePort(const unsigned short port);

	private:
#ifndef __LINUX__
		//HANDLEs of available COM ports
		HANDLE m_comPortHandles[16];
		//struct to hold port settings when opening a new port
		DCB m_portSettings;
		//timeout struct
		COMMTIMEOUTS m_commTimeouts;

		static const char m_comPorts[16][10];
		char m_baudRate[64];
#else
		//linux members
		int m_comPortHandles[30], error;
		termios m_newPortSettings, m_oldPortSettings[30];
		static const char m_comPorts[30][16];

#endif //__LINUX__
	};
};




#endif //RS232_H_
