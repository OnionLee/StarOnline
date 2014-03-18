#include "stdafx.h"
#include "GameLoop.h"

bool g_bisGameActive;

GameLoop::GameLoop()
{

}

GameLoop::~GameLoop()
{

}

//초기화 하여 리턴값 전달
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
	//화면 초기화
	resetWindow();
	//플레이어 키 입력
	playerMove();

int a =0;

	//플레이어들 업데이트
	m_pPlayer->update();
	m_pPlayer->render();

	m_pUser->update();
	m_pUser->render();

	//데이터 전송
	sendData();

	Sleep(100);
}

void GameLoop::playerMove()
{
	//키보드 입력이 있을시
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
	printf("┌─────────────────────┐\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("│                                          │\n");
	printf("└─────────────────────┘\n");
	printf("이동은 a,s,d,f, 종료는 x\n");
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
	printf("아이피를 입력 해 주세요...\n");
	gets_s( buf);
	clientSetUp(buf);

}

void GameLoop::waitGeust()
{
	printf("게스트를 기다리는 중입니다...\n");
	serverSetUp();
}

void GameLoop::serverSetUp()
{
	//버전 생성
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("error");
		return;
	}

	//소켓 설정(TCP 할지 UDP할지)
	hServSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//아이피 설정
	servAddr = { 0 };
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//바인드
	result = bind(hServSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));

	if (result != 0)
	{
		printf("error");
		return ;
	}

	//연결 받을 수 있는 상태로 전환
	listen(hServSock, 10);

	clntAddr = { 0 };
	int size = sizeof(SOCKADDR_IN);

	//연결 받을때 까지 대기
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &size);

	if (hClntSock == SOCKET_ERROR)
	{
		printf("error");
		return ;
	}

	printf("연결 성공!!!\n");

	return ;
}

void GameLoop::clientSetUp(char * adr)
{
	//버전 생성
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("error");
		return;
	}

	//소켓 설정(TCP 할지 UDP할지)
	hClntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//아이피 설정
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = inet_addr(adr);

	//연결 될때 까지 대기
	//시간이 너무 오래되면 타임아웃이 걸린다.
	result = connect(hClntSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
	if (result == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return ;
	}
	

	printf("연결 성공!!!\n");


	return;
}

void GameLoop::gameExit()
{
	//쓰레드 종료용 공유 변수 설정
	g_bisGameActive = m_bisGameActive = false;

	//쓰레드 조인
	m_thread.join();
}

void GameLoop::revcData(void * Sock, void * player)
{
	//POINT데이터만 받아 올거라서,
	int len = sizeof(POINT);
	int count = 0;
	char buf[sizeof(POINT)];

	SOCKET Socket = *(SOCKET*)Sock;
	Player * p_Player2 = (Player *)player;

	//스레드 종료
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


