#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <ctime>
#include <algorithm>
#include "Console.h"

#include "Peers.h"
#include "Server.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma region Server
void Server::ListenAsync()
{
	WSADATA wsa;
	SOCKET newsock;
	sockaddr_in server, client;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(13005);

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return;

	if (bind(s, (sockaddr*)& server, sizeof(server)) == SOCKET_ERROR)
		return;
	while (true)
	{
		listen(s, 100);
		int c = sizeof(sockaddr_in);

		newsock = accept(s, (sockaddr*)& client, &c);

		if (newsock != INVALID_SOCKET)
		{
			sockaddr_storage addr;
			char ipstr[INET6_ADDRSTRLEN];
			int port = 0;

			socklen_t len = sizeof(addr);
			getpeername(newsock, (sockaddr*)& addr, &len);

			if (addr.ss_family == AF_INET)
			{
				sockaddr_in* s = (sockaddr_in*)& addr;
				port = ntohs(s->sin_port);
				inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
			}

			Peer* peer = new Peer(newsock, ipstr);
			if (peer == nullptr)
			{
				Console::PrintErr("Failed to create a peer instance.\n");
				Console::Print("Retrying...");

				peer = new Peer(newsock, ipstr);
				if (peer == nullptr)
				{
					Console::PrintErr("Repeatedly failed to create a peer instance.\n");
					Console::Print("Terminating this connection. Please try again.\n");
					closesocket(newsock);
				}
			}

			if (!AddPeer(peer))
			{
				Console::PrintErr("Failed to add peer to the list of peers.\n");
				Console::Print("Terminating this connection. Please try again.\n");
				peer->~Peer();
			}
		}

		newsock = INVALID_SOCKET;
	}
}

void Server::Listen(Server* _this)
{
	_this->ListenAsync();
}

bool Server::AddPeer(Peer* peer)
{
	if (peer == nullptr) return false;
	this->PeerList.push_back(peer);
	return true;
}

bool Server::CheckPeer(Peer* peer)
{
	return peer != nullptr;
}

bool Server::CheckPeer(char IPv4[INET6_ADDRSTRLEN])
{
	if (IPv4 == nullptr) return false;
	for (int i = 0; i < this->PeerList.size(); i++) if (strcmp(this->PeerList[i]->IPv4, IPv4) == 0) return true;

	return false;
}

// You have to free the peer's instance yourself
bool Server::RemovePeer(Peer* peer)
{
	if (peer == nullptr)return false;
	if (!this->CheckPeer(peer)) return true;
	this->PeerList.erase(std::remove(this->PeerList.begin(), this->PeerList.end(), peer), this->PeerList.end());
	return true;
}

bool Server::RemovePeer(char IPv4[INET6_ADDRSTRLEN])
{
	if (IPv4 == nullptr) return false;
	if (!CheckPeer(IPv4)) return true;
	for (int i = 0; i < this->PeerList.size(); i++) { if (strcmp(this->PeerList[i]->IPv4, IPv4) == 0) { RemovePeer(this->PeerList[i]); return true; } }

	return false;
}
#pragma endregion