#pragma once

#include "Player.h"
class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	bool init();

	void update();

	void resetWindow();

	void playerMove();

	void drawFrom();

	void selectorServer();

	void waitHost();
	
	void waitGeust();

	void serverSetUp();

	void clientSetUp(char * adr);

	static void revcData(void * Sock, void * player);

	void sendData();

	void gameExit();

	bool getBisGameActive() const { return m_bisGameActive; }
private:
	Player * m_pPlayer;
	Player * m_pUser;

	//소켓의 버전 정보
	WSADATA wsaData;
	//소켓
	SOCKET hServSock;
	//서버 주소
	SOCKADDR_IN servAddr;

	//클라이언트 소켓
	SOCKET hClntSock;

	//클라이언트 소켓 주소
	SOCKADDR_IN clntAddr;

	//스레드
	std::thread m_thread;

	bool m_bisGameActive;

};



