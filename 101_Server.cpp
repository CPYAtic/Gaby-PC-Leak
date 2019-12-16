#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#include "Console.h"

#include "Peers.h"
#include "Server.h"

#pragma comment(lib, "Ws2_32.lib")

void Server::ListenAsync()
{
	WSADATA wsa;
	SOCKET newsock;
	sockaddr_in server, client;
		
	Console::Print(Console::Light, 0, "Initializing Winsock...\n");

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		Console::PrintErr(0, "Failed to initialize WSA : %d\n", WSAGetLastError());
		return;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(13006);

	Console::Print(Console::Light, 0, "Creating a socket...\n");
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		Console::PrintErr(0, "Could not create a socket : %d\n", WSAGetLastError());
		return;
	}

	Console::Print(Console::Light, 0, "Binding...\n");
	if (bind(s, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		Console::PrintErr(0, "Bind failed : %d\n", WSAGetLastError());
		return;
	}

	Console::Print(Console::Lime, 0, "Listening...\n");
	Console::Separator(40, 0, Console::Light);

	while (true)
	{
		listen(s, 100);
		int c = sizeof(sockaddr_in);

		newsock = accept(s, (sockaddr *)&client, &c);

		if (newsock != INVALID_SOCKET)
		{
			sockaddr_storage addr;
			char ipstr[INET6_ADDRSTRLEN];
			int port = 0;
			
			socklen_t len = sizeof(addr);
			getpeername(newsock, (sockaddr*)&addr, &len);

			if (addr.ss_family == AF_INET)
			{
				sockaddr_in *s = (sockaddr_in*)&addr;
				port = ntohs(s->sin_port);
				inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
			}

			Peer* peer = new Peer(newsock, ipstr);
			if (peer == nullptr)
			{
				Console::PrintErr(0, "Failed to create a peer instance.\n");
				Console::Print(Console::Yellow, 0, "Retrying...");

				peer = new Peer(newsock, ipstr);
				if (peer == nullptr)
				{
					Console::PrintErr(0, "Repeatedly failed to create a peer instance.\n");
					Console::Print(Console::Red, 0, "Terminating this connection. Please try again.\n");
					closesocket(newsock);
				}
			}

			if (!Peers.Add(peer))
			{
				Console::PrintErr(0, "Failed to add peer to the list of peers.\n");
				Console::Print(Console::Red, 0, "Terminating this connection. Please try again.\n");
				peer->~Peer();
			}
		}
		else
		{
			Console::PrintErr(0, "Failed to accept peer : %d\n", WSAGetLastError());
		}

		newsock = INVALID_SOCKET;
	}
}

void Server::Listen(Server* _this)
{
	_this->ListenAsync();
}

#pragma region //--------------------------PEER LIST--------------------------//

bool Server::PeerList::Add(Peer* peer)
{
	if (peer == nullptr)
	{
		Console::PrintErr(0, "Server::PeerList::Add : peer in nullptr.\n");
		return false;
	}

	Peer** newlist = new Peer*[count + 1];
	if (newlist == nullptr)
	{
		Console::PrintErr(0, "Failed to allocate memory for a peer.\n");
		Console::Print(Console::Yellow, 0, "Retrying...\n");
		Sleep(100);

		newlist = new Peer*[count + 1];
		if (newlist == nullptr)
		{
			Console::PrintErr(0, "Repeatedly failed to allocate memory for a peer.\n");
			return false;
		}
	}

	if (list && count) for (unsigned long i = 0; i < count; ++i) newlist[i] = list[i];
	else if (count != (unsigned long)list)
	{
		Console::PrintErr(0, "Something went wrong with peer list. Count = %d\n", count);
		count = 0;
		return false;
	}

	newlist[count++] = peer;

	if (list) delete[] list;
	list = newlist;

	return true;
}

bool Server::PeerList::Check(Peer* peer)
{
	if (peer == nullptr)
	{
		Console::PrintErr(0, "Server::PeerList::Check : peer in nullptr.\n");
		return false;
	}

	if (count == 0 || list == nullptr)
	{
		if (count != (unsigned long)list)
		{
			Console::PrintErr(0, "Something doesn't add up with the peer list. Count = %d\n", count);
			count = 0;
			return false;
		}
		else return false;
	}

	for (unsigned long i = 0; i < count; ++i)
	{
		if (list[i] == peer) return true;
	}

	return false;
}

bool Server::PeerList::Check(char IPv4[INET6_ADDRSTRLEN])
{
	if (IPv4 == nullptr)
	{
		Console::PrintErr(0, "Server::PeerList::Check : IPv4 in nullptr.\n");
		return false;
	}

	if (count == 0 || list == nullptr)
	{
		if (count != (unsigned long)list)
		{
			Console::PrintErr(0, "Something doesn't add up with the peer list. Count = %d\n", count);
			count = 0;
			return false;
		}
		else return false;
	}

	for (unsigned long i = 0; i < count; ++i)
	{
		if (strcmp(list[i]->IPv4, IPv4) == 0) return true;
	}

	return false;
}

// You have to free the peer's instance yourself
bool Server::PeerList::Remove(Peer* peer)
{	
	if (peer == nullptr)
	{
		Console::PrintErr(0, "Server::PeerList::Remove : peer in nullptr.\n");
		return false;
	}

	if (!Check(peer)) return true;

	if (count == 0 || list == nullptr)
	{
		if (count != (unsigned long)list)
		{
			Console::PrintErr(0, "Something went wrong with peer list (2). Count = %d\n", count);
			count = 0;
			return false;
		}
		else
		{
			Console::PrintWarn(0, "Can not remove a non existing peer.\n");
			return false;
		}
	}

	if (count == 1)
	{
		count = 0;
		list[0]->~Peer();

		delete[] list;
		list = nullptr;

		return true;
	}

	Peer** newlist = new Peer*[count - 1];
	if (newlist == nullptr)
	{
		Console::PrintErr(0, "Failed to allocate memory for a new peer list.\n");
		Console::Print(Console::Yellow, 0, "Retrying...\n");
		Sleep(100);

		newlist = new Peer*[count - 1];
		if (newlist == nullptr)
		{
			Console::PrintErr(0, "Repeatedly failed to allocate memory for a new peer list.\n");
			return false;
		}
	}

	for (unsigned long i = 0, j = 0; i < count; ++i)
	{
		if (list[i] != peer) newlist[j++] = list[i];
	}
	
	delete[] list;
	list = newlist;

	return true;
}

bool Server::PeerList::Remove(char IPv4[INET6_ADDRSTRLEN])
{
	if (IPv4 == nullptr)
	{
		Console::PrintErr(0, "Server::PeerList::Remove : IPv4 in nullptr.\n");
		return false;
	}

	if (!Check(IPv4)) return true;

	for (unsigned long i = 0; i < count; ++i)
	{
		if (strcmp(IPv4, list[i]->IPv4) == 0)
		{
			Peer* peer = list[i];
			if (Remove(peer))
			{
				Console::PrintErr(0, "Failed to remove a peer from the peer list.\n");
				return false;
			}
			peer->~Peer();
			return true;
		}
	}

	Console::PrintErr(0, "Something went wrong while removing a peer from the peer list.\n");
	return false;
}

#pragma endregion