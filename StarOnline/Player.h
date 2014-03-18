#pragma once
class Player
{
public:
	Player();
	~Player();

	bool init(int x, int y, bool player);
	void render();
	void update();

	void move(int x, int y);

	POINT getPos() const { return m_pPos; }
	void setPos(POINT val) { m_pPos = val; }

private:

	POINT m_pPos;
	bool m_bisPlayer;

};

