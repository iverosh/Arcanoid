#include "Player.hpp"

Platform::Platform()
{
	shape.setFillColor(Color(0, 255, 0));
	shape.setPosition(400, 800);
	shape.setSize(Vector2f(platform_width, platform_height));
}

Vector2f Platform::get_position_for_the_ball (float radius) const
{
	Vector2f result;
	result = shape.getPosition();
	result.x += shape.getSize().x / 2 - radius;
	result.y -= 2 * radius;
	return result;
}

void Platform::position_of_platform(Vector2i mouse_position, Vector2u window_size)
{
	float x_position = left();
	if (mouse_position.x >= shape.getSize().x / 2 && mouse_position.x <= window_size.x - shape.getSize().x / 2)
		shape.setPosition(mouse_position.x - shape.getSize().x / 2, shape.getPosition().y);
	else
	{
		if (mouse_position.x < shape.getSize().x / 2)
			shape.setPosition(0, shape.getPosition().y);
		else
			shape.setPosition(window_size.x - shape.getSize().x, shape.getPosition().y);
	}
	if (x_position == left())
	{
		speed = 0;
		return;
	}
	if (x_position > left())
		speed = -1;
	else
		speed = 1;
}


void Platform::change_size(int raise)
{
	if (raise)
		shape.setSize(Vector2f(platform_width * 2, platform_height));
	else
		shape.setSize(Vector2f(platform_width, platform_height));
}

Vector2f Platform::get_size() const
{
	return shape.getSize();
}

Vector2f Platform::get_position() const
{
	return shape.getPosition();
}

Player::Player()
{

	font.loadFromFile("Elfboyclassic.ttf");
	text.setFont(font);
	text.setString("Points: 0");
	text.setPosition(1000, 800);
	text.setCharacterSize(50);
}

void Player::update_string()
{
	text.setString("Points: " + to_string(points));
}



void Player::make_over_text()
{
	text.setString("Game over\nYour score: " + to_string(points));
	text.setCharacterSize(80);
	text.setPosition(100, 400);

}


