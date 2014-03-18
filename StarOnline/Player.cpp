#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

//플레이어를 움직이는 gotoxy



//초기화
bool Player::init(int x, int y, bool player)
{
	m_pPos.x = x;
	m_pPos.y = y;
	m_bisPlayer = player;

	return true;
}

//업데이트
void Player::update()
{
	gotoxy(m_pPos.x, m_pPos.y);
}

//그리기
void Player::render()
{
	

	if (m_bisPlayer)
		printf("☆");
	else
		printf("★");
}

//움직임 = 콘솔은 세로가 가로의 2배
void Player::move(int x, int y)
{

	m_pPos.x += x*2;
	m_pPos.y += y;


	if (m_pPos.x < 2)
	{
		m_pPos.x = 2;
	}

	if (m_pPos.x > MAXPOSX)
	{
		m_pPos.x = MAXPOSX;
	}

	if (m_pPos.y < 1)
	{
		m_pPos.y = 1;
	}

	if (m_pPos.y > MAXPOSY)
	{
		m_pPos.y = MAXPOSY;
	}

}
