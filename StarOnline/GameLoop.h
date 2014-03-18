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

	//������ ���� ����
	WSADATA wsaData;
	//����
	SOCKET hServSock;
	//���� �ּ�
	SOCKADDR_IN servAddr;

	//Ŭ���̾�Ʈ ����
	SOCKET hClntSock;

	//Ŭ���̾�Ʈ ���� �ּ�
	SOCKADDR_IN clntAddr;

	//������
	std::thread m_thread;

	bool m_bisGameActive;

};



