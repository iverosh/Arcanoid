#include "Game.hpp"

vector <Vector2i> Game::genegare_blocks_positions(const int count) const
{
	srand(time(nullptr));
	vector <Vector2i> result;
	int x, y;
	bool flag = false;
	while (result.size() != count)
	{
		flag = false;
		x = rand() % count_of_blocks_x;
		y = rand() % count_of_blocks_y;
		for (int i = 0; i < result.size(); i++)
		{
			if (x == result[i].x && y == result[i].y)
				flag = true;
		}
		if (!flag)
			result.push_back(Vector2i(x, y));


	}
	return result;

}

Game::Game()
{


	window.setFramerateLimit(120);

	float space_x = window_width * 0.8 / count_of_blocks_x - block_width;
	float space_y = window_height * 0.3 / count_of_blocks_y - block_height;

	unique_ptr <Block> block;
	unique_ptr <BonusBlock> bonus_block;

	Vector2f pos = platform.get_position_for_the_ball(ball_radius);
	Ball first_ball(pos);
	balls.push_back(first_ball);

	ground.set_position(0, window_height - 20);
	ground.set_size(window_width, 30);
	ground.shape.setFillColor(Color::Green);

	vector <Vector2i> positions = genegare_blocks_positions(7);
	for (int y = 0; y < count_of_blocks_y; y++)
	{
		for (int x = 0; x < count_of_blocks_x; x++)
		{
			bool is_simple_block = true;
			for (int i = 0; i < positions.size(); i++)
			{
				if (x == positions[i].x && y == positions[i].y)
				{
					is_simple_block = false;
					if (i != 6)
					{
						bonus_block = make_unique<BonusBlock>(x, y, space_x, space_y, BonusType(i));
						bonus_blocks.push_back(move(bonus_block));
					}
					else
					{
						block = make_unique<UnbreakableBlock>(x, y, space_x, space_y);
						blocks.push_back(move(block));
					}
				}


			}
			if (is_simple_block)
			{
				block = make_unique<Block>(x, y, space_x, space_y);
				blocks.push_back(move(block));
			}
		}

	}
}

void Game::collisions_checking()
{
	for (auto& ball : balls)
	{
		ball.platform_collision(platform, is_sticky_bonus_active, is_sticky_bonus_applied);
		for (int i = 0; i < blocks.size(); i++)
			ball.block_collision(*blocks[i], player.points);
		for (int i = 0; i < bonus_blocks.size(); i++)
			ball.block_collision(*bonus_blocks[i], player.points);
	}
	if (balls.size() > 1)
		balls[0].other_ball_collision(balls[1]);

}

void Game::moving()
{
	float elapsed = clock.restart().asMilliseconds();

	platform.position_of_platform(Mouse::getPosition(window), window.getSize());

	for (auto& ball : balls)
		ball.ball_position(elapsed, ground_bound, platform.left());

	for (int i = 0; i < falling_bonuses.size(); i++)
	{
		falling_bonuses[i]->fall_down(elapsed, platform, player.points);
		if (falling_bonuses[i]->is_active)
		{
			falling_bonuses[i]->time.restart();
			active_bonuses.push_back(move(falling_bonuses[i]));
		}
	}
}

void Game::bonuses_updating()
{
	for (int i = 0; i < active_bonuses.size(); i++)
		active_bonuses[i]->bonus_control(*this);
	
	for (int i = 0; i < bonus_blocks.size(); i++)
		if (bonus_blocks[i]->is_dead)
			falling_bonuses.push_back(move(bonus_blocks[i]->bonus));
}

void Game::erasing()
{
	falling_bonuses.erase(remove_if(begin(falling_bonuses), end(falling_bonuses), [](unique_ptr<Bonus>& bonus) {if (bonus.get() == nullptr) return 1; return bonus->is_felt; }), end(falling_bonuses));

	active_bonuses.erase(remove_if(begin(active_bonuses), end(active_bonuses), [](unique_ptr<Bonus>& bonus) {return 1 - bonus->is_active; }), end(active_bonuses));

	blocks.erase(remove_if(begin(blocks), end(blocks), [](unique_ptr<Block>& block) { return block->is_dead; }), end(blocks));

	bonus_blocks.erase(remove_if(begin(bonus_blocks), end(bonus_blocks), [](unique_ptr<BonusBlock>& block) { return block->is_dead; }), end(bonus_blocks));

	balls.erase(remove_if(begin(balls), end(balls), [](Ball& ball) {return ball.is_dead; }), end(balls));
}

void Game::update()
{
	if (is_button_pressed)
	{
		for (auto& ball : balls)
			ball.is_sticky = 0;
		is_button_pressed = 0;
	}
	moving();
	collisions_checking();
	bonuses_updating();
	player.update_string();
	erasing();
	if ((balls.size() == 0 && falling_bonuses.size() == 0) || (blocks.size() == 1 && falling_bonuses.size() == 0 && bonus_blocks.size() == 0))
		game_over();
}

void Game::draw()
{
	window.draw(player.text);

	if (!is_game_over)
	{
		if (ground_bound)
			window.draw(ground.shape);

		for (Ball& ball : balls)
			window.draw(ball.ball);
		window.draw(platform.shape);

		for (int i = 0; i < falling_bonuses.size(); i++)
			window.draw(falling_bonuses[i]->shape);

		for (int i = 0; i < blocks.size(); i++)
			window.draw(blocks[i]->shape);

		for (int i = 0; i < bonus_blocks.size(); i++)
			window.draw(bonus_blocks[i]->shape);
	}
}


void Game::game_over()
{
	is_game_over = 1;
	player.make_over_text();
}
