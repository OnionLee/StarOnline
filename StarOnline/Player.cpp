#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

//�÷��̾ �����̴� gotoxy



//�ʱ�ȭ
bool Player::init(int x, int y, bool player)
{
	m_pPos.x = x;
	m_pPos.y = y;
	m_bisPlayer = player;

	return true;
}

//������Ʈ
void Player::update()
{
	gotoxy(m_pPos.x, m_pPos.y);
}

//�׸���
void Player::render()
{
	

	if (m_bisPlayer)
		printf("��");
	else
		printf("��");
}

//������ = �ܼ��� ���ΰ� ������ 2��
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
