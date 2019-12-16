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

	struct PeerList
	{
		unsigned long count = 0;
		Peer** list = nullptr;

		bool Add(Peer* peer);
		bool Check(Peer* peer);
		bool Check(char IPv4[INET6_ADDRSTRLEN]);
		bool Remove(Peer* peer);
		bool Remove(char IPv4[INET6_ADDRSTRLEN]);
	};

	struct BlackList
	{
		unsigned long count = 0;
		BlacklistInst** list = nullptr;

		bool Add(BlacklistInst* bli);
		bool Check(BlacklistInst * bli);
		bool Remove(BlacklistInst* bli);
	};

public:

	PeerList Peers;
	BlackList Blacklist;

	Server()
	{
		lThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Listen, this, 0, 0);
		if (lThread == INVALID_HANDLE_VALUE || lThread == 0)
		{
			Console::PrintErr(0, "Failed to initialize the server : %d\n", GetLastError());
			Console::Print(Console::Yellow, 0, "Retrying...\n");

			lThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Listen, this, 0, 0);

			if (lThread == INVALID_HANDLE_VALUE || lThread == 0)
			{
				Console::PrintErr(0, "Repeatedly failed to initialize the server : %d\n", GetLastError());
				Console::Print(Console::Red, 0, "Please try again and ensure you run me as administrator!\n");
			}
			else
			{
				Console::Print(Console::Lime, 0, "Server initialized!\n");
			}
		}
		else
		{
			Console::Print(Console::Lime, 0, "Server initialized!\n");
		}
		Peers.count = 0;
		Peers.list = nullptr;
		Blacklist.count = 0;
		Blacklist.list = nullptr;
	}
	~Server()
	{
		if (Peers.list)
		{
			for (unsigned long i = 0; i < Peers.count; ++i)
			{
				if (Peers.list[i]) Peers.list[i]->~Peer();
			}
			delete[] Peers.list;
		}
		if (Blacklist.list)
		{
			for (unsigned long i = 0; i < Blacklist.count; ++i)
			{
				if (Blacklist.list[i]) delete Blacklist.list[i];
			}
			delete[] Blacklist.list;
		}

		closesocket(s);
		TerminateThread(lThread, 0);
		WSACleanup();
	}
};