#include "Player.hpp"


const int block_health = 1;

const int block_width = 100;
const int block_height = 50;

const int falling_speed = 250;

class Game;

class Block : public Rectagle
{
public:

	int health = block_health;
	int is_dead = 0;

	Block(int x, int y, float space_x, float space_y);

	virtual ~Block() = default;

	virtual void hit(int& points);

	void death(int& points);

};


class UnbreakableBlock : public Block
{
public:
	UnbreakableBlock(int x, int y, float space_x, float space_y);
	void hit(int& points);
};



class Bonus : public Rectagle
{
public:
	int is_active = 0;
	int is_felt = 0;
	Clock time;

	Bonus(Vector2f pos);
	void fall_down(const float elapsed, const Platform platform, int& points);
	virtual void bonus_control(Game &game) = 0;
	virtual ~Bonus() = default;
};



class SpeedBonus : public Bonus
{
public:
	SpeedBonus(Vector2f pos);

	void bonus_control(Game &game);
};

class PlatformBonus : public Bonus
{
public:
	PlatformBonus(Vector2f pos);
	void bonus_control(Game& game);
};

class GroundBonus : public Bonus
{
public:
	GroundBonus(Vector2f pos);
	void bonus_control(Game& game);
};

class TrajectoryBonus : public Bonus
{
public:
	int is_applied = 0;
	int changing_time;
	TrajectoryBonus(Vector2f pos);
	void bonus_control(Game& game);
};





class StickyBonus : public Bonus
{
public:

	StickyBonus(Vector2f pos);
	void bonus_control(Game& game);

};

class BallBonus : public Bonus
{
public:
	BallBonus(Vector2f pos);
	void bonus_control(Game& game);

};

enum BonusType
{
	Ball_,
	Speed_,
	Platform_,
	Ground_,
	Trajectory_,
	Sticky_

};


class BonusBlock : public Block
{
public:
	
	unique_ptr <Bonus> bonus;

	BonusBlock(int x, int y, float space_x, float space_y, BonusType type);
};





