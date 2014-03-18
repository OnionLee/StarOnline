// StarOnline.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "GameLoop.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//게임 루프
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

