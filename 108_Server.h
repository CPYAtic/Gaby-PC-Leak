#pragma once

struct BlacklistInst
{
	char IPv4[INET6_ADDRSTRLEN];
};

class Server
{
	SOCKET s;
	HANDLE lThread = nullptr;

	void ListenAsync();
	static void Listen(Server* _this);

public:
	std::vector<Peer*> PeerList;
	bool AddPeer(Peer* peer);
	bool CheckPeer(Peer* peer);
	bool CheckPeer(char IPv4[INET6_ADDRSTRLEN]);
	bool RemovePeer(Peer* peer);
	bool RemovePeer(char IPv4[INET6_ADDRSTRLEN]);
	Server()
	{
		lThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Listen, this, 0, 0);
		if (lThread == INVALID_HANDLE_VALUE || lThread == 0)
			lThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Listen, this, 0, 0);

		PeerList.clear();
	}
	~Server()
	{
		PeerList.clear();
		closesocket(s);
		TerminateThread(lThread, 0);
		WSACleanup();
	}
};