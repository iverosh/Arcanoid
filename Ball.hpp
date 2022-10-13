#pragma once
#include "Player.hpp"
#include "Block.hpp"

using namespace sf;
using namespace std;


enum Side
{
	Left ,
	Right,
	Top,
	Bottom
};


class Ball	
{
public:
	CircleShape ball;
	Vector2f speed;
	float position_relative_to_platform;
	int is_sticky = 1;
	int is_dead = 0;
	
	Ball(Vector2f pos);
	void change_speed(int raise);
	Vector2f center() const;

	void set_position(float x, float y);
	float top() const;
	float bottom() const;
	float left() const;
	float right() const;

	void ball_position(float elapsed, int& ground_bound, float platform_x);
	
	template <typename Type>
	Side where_came_from(Type rect);

	void other_ball_collision(Ball& other);
	void platform_collision(Platform platform, int is_sticky_bonus_active, int& is_sticky_bonus_applied);
	void block_collision(Block& block, int& points);
};



 