#include "Game.hpp"



int main()
{
	
	Game game;
	
	
	while(game.window.isOpen()) 
	{
		Event ev;
		while(game.window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				game.window.close();
			if (ev.type == Event::KeyPressed)
				game.is_button_pressed = 1;

			
		}
		game.window.clear();
		
		
		game.update();
		
		

		game.draw();
		
		game.window.display();
		
	}

	return 0;
}