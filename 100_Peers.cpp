#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <ctime>

#include "Console.h"

#include "Peers.h"

void Peer::Communicate()
{
	srand(clock());

	if (!SendConfirmMessage())
	{
		Console::PrintWarn(0, "Communication with %s failed!\n", IPv4);
		return;
	}

	char buff[1024];
	int len = 1024;

	while (AwaitData(buff, len))
	{
		if (len == 20 && memcmp(buff + 2, "\x50\x00", 2) == 0 && !verified)
		{
			// verify the player
			// ...

			bool ret = AcceptVerifyInfo();
			
			if (!ret) Console::PrintErr(0, "Failed to verify peer %s\n", IPv4);

			verified = ret;
		}
		else if (len == 8 && memcmp(buff + 2, "\x20\x00", 2) == 0)
		{
			if (!AnswerPush(*(WORD*)buff)) Console::PrintErr(0, "Failed to send a push response to %s\n", IPv4);
		}
		else
		{
			//Console::PrintWarn(0, "Recieved an unknown packet from %s ; Size = %d , Name = %02X %02X\n", IPv4, len, buff[2], buff[3]);
		}
		len = 1024;
	}

	Console::PrintWarn(0, "Exiting the thread associated with %s\n", IPv4);
	return;
}

void Peer::ComThread(Peer* _this)
{
	_this->Communicate();
}

bool Peer::AwaitData(char* data, int& len)
{
	clock_t timer = clock();

	ZeroMemory(data, len);

	int size = 0;
	unsigned long count = 1, i = 0;
	do
	{
		int length = recv(Socket, data + size, 4, MSG_WAITALL);

		if (length == 4)
		{
			if (size == 0) count = (unsigned long)(data[2] >> 4);

			size += length;
			if (++i == count || size >= len)
			{
				len = size;
				return true;
			}
		}
		else if (length == 0)
		{
			len = size;
			Console::PrintWarn(0, "Connection with %s was closed by the peer.\n", IPv4);
			return false;
		}
		else if (length == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
	} while (clock() - timer <= 10000);

	Console::Print(Console::Red, 0, "Connection with %s timed out.\n", IPv4);
	return false;
}

bool Peer::SendConfirmMessage()
{
	struct ConfirmMessage // XX XX 30 00 5X 64 36 XX XX XX XX XX
	{
		WORD id = 0;
		WORD packetType = 0x0030;
		DWORD unknown1 = 0x01366450;
		DWORD unknown2 = 0x010EDB00;
	};

	ConfirmMessage msg;
	msg.id = rand() % 0xFFFF;
	msg.unknown1 += rand() % 0xF;
	msg.unknown2 = rand();

	memcpy(&msg, "\x03\x00\x30\x00\x53\x64\x36\x01\x00\xDB\x0E\x01", 12);

	if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
	{
		if (sent == sizeof(msg))
		{
			return true;
		}
		else if (sent == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
		else
		{
			Console::PrintWarn(0, "Failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");

			if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
			{
				if (sent == sizeof(msg))
				{
					return true;
				}
				else if (sent == SOCKET_ERROR)
				{
					Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
					return false;
				}
				else
				{
					Console::PrintWarn(0, "Repeatedly failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
					return false;
				}
			}
		}
	}

	return false;
}

bool Peer::AcceptVerifyInfo()
{
	struct PushResponse // XX XX 00 20 ?? ?? ?? ??
	{
		WORD id = 0;
		WORD packetType = 0x0020;
		DWORD unknown = 0x00207D50;
	};

	PushResponse msg;
	msg.unknown += rand() % 0xF;
	
	if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
	{
		if (sent == sizeof(msg))
		{
			return true;
		}
		else if (sent == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
		else
		{
			Console::PrintWarn(0, "Failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");

			if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
			{
				if (sent == sizeof(msg))
				{
					return true;
				}
				else if (sent == SOCKET_ERROR)
				{
					Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
					return false;
				}
				else
				{
					Console::PrintWarn(0, "Repeatedly failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
					return false;
				}
			}
		}
	}

	return false;
}

bool Peer::AnswerPush(WORD connID)
{
	struct PushResponse // ?? ?? 00 20 1X 12 XX XX
	{
		WORD id = 0;
		WORD packetType = 0x0020;
		WORD unknown1 = 0x1210;
		WORD unknown2 = 0;
	};

	PushResponse msg;
	msg.id = connID;
	msg.unknown1 += rand() % 0xF;

	if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
	{
		if (sent == sizeof(msg))
		{
			return true;
		}
		else if (sent == SOCKET_ERROR)
		{
			Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
			return false;
		}
		else
		{
			Console::PrintWarn(0, "Failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");

			if (int sent = send(Socket, (char*)&msg, sizeof(msg), 0))
			{
				if (sent == sizeof(msg))
				{
					return true;
				}
				else if (sent == SOCKET_ERROR)
				{
					Console::PrintWarn(0, "Connection with %s was aborted : %d\n", IPv4, WSAGetLastError());
					return false;
				}
				else
				{
					Console::PrintWarn(0, "Repeatedly failed to send confirm message to %s : %d\n", IPv4, WSAGetLastError());
					return false;
				}
			}
		}
	}

	return false;
}