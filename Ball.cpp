#include "Ball.hpp"

Ball::Ball(Vector2f pos)
{
	ball.setRadius(ball_radius);
	ball.setPosition(pos);
	ball.setFillColor(Color(255, 0, 0));
	speed = Vector2f(ball_speed, -ball_speed);
	position_relative_to_platform = platform_width / 2 - ball_radius;
}

void Ball::change_speed(int raise)
{
	if (raise)
		speed = Vector2f(speed.x * 2, speed.y * 2);
	else
		speed = Vector2f(speed.x / 2, speed.y / 2);
}


Vector2f Ball::center() const
{
	Vector2f result;
	result = ball.getPosition();
	result.x += ball_radius;
	result.y += ball_radius;
	return result;
}

void Ball::set_position(float x, float y) { ball.setPosition(x, y); }

float Ball::top() const { return ball.getPosition().y; }

float Ball::bottom() const { return ball.getPosition().y + 2 * ball_radius; }

float Ball::left() const { return ball.getPosition().x; }

float Ball::right() const { return ball.getPosition().x + 2 * ball_radius; }

void Ball::ball_position(float elapsed, int& ground_bound, float platform_x)
{
	float speed_abs = abs(speed.x);
	if (!is_sticky)
		ball.move(speed.x * elapsed / 1000, speed.y * elapsed / 1000);
	else
		set_position(position_relative_to_platform + platform_x, top());
	if (left() <= 0)
	{
		speed.x = speed_abs;
		set_position(0, top());
	}
	if (right() >= window_width)
	{
		speed.x = -speed_abs;
		set_position(window_width - 2 * ball_radius, top());
	}
	if (top() <= 0)
	{
		speed.y = speed_abs;
		set_position(left(), 0);
	}

	if (bottom() >= window_height - 20)
	{
		if (ground_bound)
		{
			speed.y = -speed_abs;
			set_position(left(), window_height - 20 - 2 * ball_radius);
			ground_bound = 0;
		}
		else
			is_dead = 1;
	}
}

template <typename Type>
Side Ball::where_came_from(Type rect)
{
	vector <double> gaps;
	gaps.push_back(fabs(right() - rect.left()));
	gaps.push_back(fabs(left() - rect.right()));
	gaps.push_back(fabs(bottom() - rect.top()));
	gaps.push_back(fabs(top() - rect.bottom()));
	double min = gaps[0];
	Side res = Side(0);

	for (int i = 1; i < 4; i++)
	{
		if (min > gaps[i])
		{
			min = gaps[i];
			res = Side(i);
		}
	}
	return res;
}

void Ball::other_ball_collision(Ball& other)
{
	if (other.right() >= left() && other.left() <= right() &&
		other.bottom() >= top() && other.top() <= bottom())
	{
		if (left() == other.left())
		{
			speed.y *= -1;
			other.speed.y *= -1;
		}
		else if (top() == other.top())
		{
			speed.x *= -1;
			other.speed.x *= -1;
		}
		else
		{
			speed.x *= -1;
			speed.y *= -1;
			other.speed.x *= -1;
			other.speed.y *= -1;
		}
		switch (where_came_from(other))
		{
		case Left:
			other.ball.move(-10, 0);
		case Right:
			other.ball.move(10, 0);
		case Top:
			other.ball.move(0, -10);
		case Bottom:
			other.ball.move(0, 10);

		default:
			break;
		}
	}
}

void Ball::platform_collision(Platform platform, int is_sticky_bonus_active, int& is_sticky_bonus_applied)
{
	if (platform.right() >= left() && platform.left() <= right() &&
		platform.bottom() >= top() && platform.top() <= bottom())
	{
		if (is_sticky_bonus_active)
		{
			position_relative_to_platform = left() - platform.left();
			is_sticky = 1;
			is_sticky_bonus_applied = 1;
		}
		Side side = where_came_from(platform);
		if (side == Left)
		{
			speed.x *= -1;
			set_position(platform.left() - 2 * ball_radius, top());
		}
		else if (side == Right)
		{
			speed.x *= -1;
			set_position(platform.right(), top());
		}
		else if (side == Top)
		{
			speed.y *= -1;
			set_position(left(), platform.top() - 2 * ball_radius);
		}
		else if (side == Bottom)
		{
			speed.y *= -1;
			set_position(left(), platform.bottom());
		}
		if (platform.speed != 0)
			speed.x = abs(speed.x) * platform.speed;

	}
}

void Ball::block_collision(Block& block, int& points)
{

	if (block.right() >= left() && block.left() <= right() &&
		block.bottom() >= top() && block.top() <= bottom())
	{
		Side side = where_came_from(block);
		if (side == Left)
		{
			speed.x *= -1;
			set_position(block.left() - 2 * ball_radius - 2, top());
		}
		else if (side == Right)
		{
			speed.x *= -1;
			set_position(block.right() + 2, top());
		}
		else if (side == Top)
		{
			speed.y *= -1;
			set_position(left(), block.top() - 2 * ball_radius - 2);
		}
		else if (side == Bottom)
		{
			speed.y *= -1;
			set_position(left(), block.bottom() + 2);
		}
		block.hit(points);
	}
}