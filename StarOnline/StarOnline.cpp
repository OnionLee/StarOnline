// StarOnline.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "GameLoop.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//���� ����
	GameLoop * p_loop = new GameLoop();

	if (p_loop->init())
	{
		while (p_loop->getBisGameActive())
		{
			p_loop->update();
		}

		delete p_loop;
	}
	else
	{
		return -1;
	}

int b = 0;

	return 0;
}

