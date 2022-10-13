#pragma once
#include "Rectangle.hpp"

const int platform_width = 250;
const int platform_height = 20;
const int window_height = 950;
const int window_width = 1300;
const float ball_radius = 30;
const float ball_speed = 500;

class Platform : public Rectagle
{
public:
	int speed = 0;
	Platform();
	Vector2f get_position_for_the_ball(float radius) const;
	
	void position_of_platform(Vector2i mouse_position, Vector2u window_size);
	
	void change_size(int raise);

	Vector2f get_size() const;
	Vector2f get_position() const;


};



class Player
{
public:
	int points = 0;
	Font font;
	Text text;
	Player();
	
	void update_string();


	void make_over_text();


};

