#include "stdafx.h"
#include "GameLoop.h"

bool g_bisGameActive;

GameLoop::GameLoop()
{

}

GameLoop::~GameLoop()
{

}

//�ʱ�ȭ �Ͽ� ���ϰ� ����
bool GameLoop::init()
{
	g_bisGameActive = m_bisGameActive = true;

	m_pPlayer = new Player();
	m_pUser = new Player();

	if (!m_pPlayer->init(6, 3, true) || !m_pUser->init(10, 4, false))
	{
		return false;
	}

	selectorServer();

	m_thread = std::thread(&GameLoop::revcData,&hClntSock,m_pUser);

	return true;
	
}

void GameLoop::update()
{
	//ȭ�� �ʱ�ȭ
	resetWindow();
	//�÷��̾� Ű �Է�
	playerMove();

int a =0;

	//�÷��̾�� ������Ʈ
	m_pPlayer->update();
	m_pPlayer->render();

	m_pUser->update();
	m_pUser->render();

	//������ ����
	sendData();

	Sleep(100);
}

void GameLoop::playerMove()
{
	//Ű���� �Է��� ������
	if (_kbhit())
	{
		auto Key = _getch();

		switch (Key)
		{
		case 'a':
			m_pPlayer->move(-1, 0);
			break;
		case 'd':
			m_pPlayer->move(1, 0);
			break;
		case 's':
			m_pPlayer->move(0, 1);
			break;
		case 'w':
			m_pPlayer->move(0, -1);
			break;
		case 'x':
			gameExit();
		}
	}
}

void GameLoop::drawFrom()
{
	gotoxy(0, 0);
	printf("����������������������������������������������\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("��                                          ��\n");
	printf("����������������������������������������������\n");
	printf("�̵��� a,s,d,f, ����� x\n");
}

void GameLoop::resetWindow()
{
	system("cls");
	drawFrom();
}

void GameLoop::selectorServer()
{
	char Selector;
	printf("HOST = 1, GEUST = 0 : ");
	scanf_s("%c", &Selector);

	if (Selector == '1')
	{
		waitGeust();
	}
	else
	{
		waitHost();
	}
}

void GameLoop::waitHost()
{
	fflush(stdin);

	char buf[32] = { '\0' };
	printf("�����Ǹ� �Է� �� �ּ���...\n");
	gets_s( buf);
	clientSetUp(buf);

}

void GameLoop::waitGeust()
{
	printf("�Խ�Ʈ�� ��ٸ��� ���Դϴ�...\n");
	serverSetUp();
}

void GameLoop::serverSetUp()
{
	//���� ����
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("error");
		return;
	}

	//���� ����(TCP ���� UDP����)
	hServSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//������ ����
	servAddr = { 0 };
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//���ε�
	result = bind(hServSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));

	if (result != 0)
	{
		printf("error");
		return ;
	}

	//���� ���� �� �ִ� ���·� ��ȯ
	listen(hServSock, 10);

	clntAddr = { 0 };
	int size = sizeof(SOCKADDR_IN);

	//���� ������ ���� ���
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &size);

	if (hClntSock == SOCKET_ERROR)
	{
		printf("error");
		return ;
	}

	printf("���� ����!!!\n");

	return ;
}

void GameLoop::clientSetUp(char * adr)
{
	//���� ����
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("error");
		return;
	}

	//���� ����(TCP ���� UDP����)
	hClntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//������ ����
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = inet_addr(adr);

	//���� �ɶ� ���� ���
	//�ð��� �ʹ� �����Ǹ� Ÿ�Ӿƿ��� �ɸ���.
	result = connect(hClntSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
	if (result == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return ;
	}
	

	printf("���� ����!!!\n");


	return;
}

void GameLoop::gameExit()
{
	//������ ����� ���� ���� ����
	g_bisGameActive = m_bisGameActive = false;

	//������ ����
	m_thread.join();
}

void GameLoop::revcData(void * Sock, void * player)
{
	//POINT�����͸� �޾� �ðŶ�,
	int len = sizeof(POINT);
	int count = 0;
	char buf[sizeof(POINT)];

	SOCKET Socket = *(SOCKET*)Sock;
	Player * p_Player2 = (Player *)player;

	//������ ����
	while (g_bisGameActive)
	{
		while (len != count)
		{
			count += recv(Socket, buf + count, len - count, 0);
		}
		count = 0;
		p_Player2->setPos(*(POINT*)(buf));
	}

	return;
}

void GameLoop::sendData()
{
	char buf[sizeof(POINT)];
	POINT value = m_pPlayer->getPos();

	for (int i = 0; i < sizeof(POINT); i++)
	{
		buf[i] = ((char *)&value)[i];
	}

	send(hClntSock, buf, sizeof(buf), 0);
}


