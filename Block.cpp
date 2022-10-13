#include "Game.hpp"


Block::Block(int x, int y, float space_x, float space_y)
{
	set_position(window_width / 10 + x * (block_width + space_x), window_height / 10 + y * (block_height + space_y));
	shape.setSize(Vector2f(block_width, block_height));
	shape.setFillColor(Color(0, 0, 255));
}

void Block::hit(int& points)
{
	health--;
	points++;
	if (health == 0)
		death(points);
}

void Block::death(int& points)
{
	points += 5;
	is_dead = 1;
}

void SpeedBonus::bonus_control(Game& game)
{
	if (game.balls.size() == 0)
	{
		is_active = 0;
		return;
	}
	if (time.getElapsedTime().asSeconds() <= 10)
	{
		if (fabs(game.balls[0].speed.x) == ball_speed)
		{
			for (auto& ball : game.balls)
				ball.change_speed(1);
			return;
		}
	}
	else
	{
		for (auto& ball : game.balls)
			ball.change_speed(0);
		is_active = 0;
	}
}

void PlatformBonus::bonus_control(Game& game)
{
	if (time.getElapsedTime().asSeconds() <= 10)
	{
		if (fabs(game.platform.get_size().x) == platform_width)
		{
			game.platform.change_size(1);
			return;
		}
	}
	else
	{
		game.platform.change_size(0);
		is_active = 0;
	}

}

void GroundBonus::bonus_control(Game& game)
{
	is_active = 0;
	game.ground_bound = 1;

}

void TrajectoryBonus::bonus_control(Game& game)
{
	if (is_applied == 0)
	{
		for (auto &ball : game.balls)
			ball.ball.setFillColor(Color(0, 0, 122));
		is_applied = 1;
	}
	else
	{
		if (time.getElapsedTime().asSeconds() >= changing_time)
		{
			cout << "\nchanged";
			Vector2i traektory_change;
			if (rand() % 2)
				traektory_change.x = -1;
			else
				traektory_change.x = 1;
			
			if (rand() % 2)
				traektory_change.y = -1;
			else
				traektory_change.y = 1;

			for (auto& ball : game.balls)
			{
				ball.speed.x *= traektory_change.x;
				ball.speed.y *= traektory_change.y;
				ball.ball.setFillColor(Color::Red);
			}
			is_active = 0;
		}
	
	}

}

void StickyBonus::bonus_control(Game& game)
{
	if (game.is_sticky_bonus_applied == 1)
	{
		is_active = 0;
		game.is_sticky_bonus_active = 0;
		return;
	}
	if (game.is_sticky_bonus_active == 0)
		game.is_sticky_bonus_active = 1;
	
}

void BallBonus::bonus_control(Game& game)
{
	Ball ball(game.platform.get_position_for_the_ball(ball_radius));
	game.balls.push_back(ball);
	is_active = 0;
}


UnbreakableBlock::UnbreakableBlock(int x, int y, float space_x, float space_y) :Block(x, y, space_x, space_y)
{
	shape.setFillColor(Color(128, 128, 128));
}

void UnbreakableBlock::hit(int& points)
{
	return;
}

Bonus::Bonus(Vector2f pos)
{
	shape.setSize(Vector2f(block_width, block_height));
	shape.setPosition(pos);
}

void Bonus::fall_down(const float elapsed, const Platform platform, int& points)
{
	if (platform.right() >= left() && platform.left() <= right() &&
		platform.bottom() >= top() && platform.top() <= bottom())
	{
		is_active = 1;
		is_felt = 1;
		points += 5;
	}
	if (top() > window_height)
	{
		is_felt = 1;
		points -= 5;
	}

	shape.move(Vector2f(0, falling_speed * elapsed / 1000));
}

SpeedBonus::SpeedBonus(Vector2f pos) : Bonus(pos)
{
	shape.setFillColor(Color::Red);
};

PlatformBonus::PlatformBonus(Vector2f pos) : Bonus(pos)
{
	shape.setFillColor(Color::Cyan);
}

GroundBonus::GroundBonus(Vector2f pos) : Bonus(pos)
{
	shape.setFillColor(Color::Magenta);
}

TrajectoryBonus:: TrajectoryBonus(Vector2f pos) : Bonus(pos)
{
	changing_time = rand() % 5 + 3;
	shape.setFillColor(Color(0, 0, 122));
}

StickyBonus::StickyBonus(Vector2f pos) : Bonus(pos)
{
	shape.setFillColor(Color(199, 234, 70));
}

BallBonus::BallBonus(Vector2f pos) : Bonus(pos)
{
	shape.setFillColor(Color(75, 0, 130));
}

BonusBlock::BonusBlock(int x, int y, float space_x, float space_y, BonusType type) :Block(x, y, space_x, space_y)
{

	set_position(window_width / 10 + x * (block_width + space_x), window_height / 10 + y * (block_height + space_y));
	shape.setSize(Vector2f(block_width, block_height));
	shape.setFillColor(Color::Yellow);
	health = block_health;
	is_dead = 0;

	switch (type)
	{
	case Speed_:
		bonus = make_unique<SpeedBonus>(shape.getPosition());
		break;
	case Platform_:
		bonus = make_unique<PlatformBonus>(shape.getPosition());
		break;
	case Ground_:
		bonus = make_unique<GroundBonus>(shape.getPosition());
		break;
	case Trajectory_:
		bonus = make_unique<TrajectoryBonus>(shape.getPosition());
		break;
	case Sticky_:
		bonus = make_unique<StickyBonus>(shape.getPosition());
		break;
	case Ball_:
		bonus = make_unique<BallBonus>(shape.getPosition());
		break;
	default:
		break;
	}
}
