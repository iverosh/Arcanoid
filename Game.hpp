#pragma once	
#include "Ball.hpp"


const int count_of_blocks_x = 10;
const int count_of_blocks_y = 5;



class Game
{
public:
	RenderWindow window{ {window_width, window_height}, "Arcanoid"};
	Player player;
	
	vector <unique_ptr <Block>> blocks;
	vector <unique_ptr<BonusBlock>> bonus_blocks;
	vector <unique_ptr<Bonus>> falling_bonuses;
	vector <unique_ptr<Bonus>> active_bonuses;
	vector<Ball> balls;
	
	Rectagle ground;
	Platform platform;
	Clock clock;

	int is_game_over = 0;
	int ground_bound = 0;
	int is_button_pressed = 0;
	int is_sticky_bonus_active = 0;
	int is_sticky_bonus_applied = 0;
	
	vector <Vector2i> genegare_blocks_positions(const int count) const;

	Game();

	void collisions_checking();

	void moving();

	void bonuses_updating();

	void erasing();
	
	void update();
	
	void draw();

	void game_over();	
};
