#pragma once
#include "Depends.h"

//player update

void automate_player(void)
{
	float dis = RAND_MAX;

	//if (y + doodle_h < tiles[ix].getPosition().y)
	for (int i = 0; i < n_tiles; i++)
	{
		tiles[i].setFillColor(Color::Green);
		if (tiles[i].getPosition().y > 0 &&
			tiles[i].getPosition().y < y + doodle_h)
		{
			float x = (y + doodle_h) - tiles[i].getPosition().y;
			if (x < dis) {
				ix = i;
				dis = x;
			}
		}
	}

	tiles[ix].setFillColor(Color::Red);
	//std::cout << ix << std::endl;
	if (tiles[ix].getPosition().x + tile_width < x)
	{
		Doodle.setTexture(doodle);
		velocity_x = -500;
	}
	else if (tiles[ix].getPosition().x > x + doodle_w)
	{
		Doodle.setTexture(doodle_2);
		velocity_x = 500;
	}

}

void update_position(void)
{
	//updates for gravity
	for (int i = 0; i < n_tiles; i++)
	{
		float tile_x = tiles[i].getPosition().x;
		float tile_y = tiles[i].getPosition().y;

		if (y + doodle_h >= tile_y && y + doodle_h <= tile_y + tile_height / 2 &&
			x + doodle_w >= tile_x && x <= tile_x + tile_width && velocity_y > 0)
		{
			//std::cout << i << std::endl;
			velocity_x = 0;
			jump.play();
			velocity_y = -800;
		}
		else
			velocity_y += gravity * dt;
	}

	//updating positions
	velocity_x += accleration_x * dt;
	velocity_y += accleration_y * dt;
	x += velocity_x * dt;
	y += velocity_y * dt;

	//checking for wall-collisions
	if (x < 0)
	{
		x = 0;
		velocity_x = -(velocity_x + 300);
	}
	if (x > W_Width - doodle_w)
	{
		x = W_Width - doodle_w;
		velocity_x = -(velocity_x - 300);
	}
	if (y < 0)
	{
		y = 0;
		velocity_y = -(velocity_y + 100);
	}

	if (y > W_Height)
	{
		//fall.play();
		game_state = 2;
		score_number.setPosition(550, 300);
		score_number.setFont(comic_sans);
		score_number.setFillColor(Color::Black);
	}
}

void update_movement(void)
{
	if (Keyboard::isKeyPressed(Keyboard::Right) && paused == false)
	{
		Doodle.setTexture(doodle_2);
		velocity_x = 400;
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && paused == false)
	{
		Doodle.setTexture(doodle);
		velocity_x = -400;
	}

	if (Keyboard::isKeyPressed(Keyboard::Enter))
	{
		if (game_state == 0)
		{
			s_start.play();
			game_state = 1;
		}
		else if (game_state == 3)
		{
			Collect.play();
			draw_help = false;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Space))
		if (game_state == 1)
		{
			Pause.play();
			if (paused == false)
				paused = true;
			else
				paused = false;

			if (JetPack.getStatus() == Sound::Status::Playing)
				JetPack.pause();
			else if (JetPack.getStatus() == Sound::Status::Paused)
				JetPack.play();
		}

}

void update_tiles(void)
{
	//new implementation
	for (int i = 0; i < n_tiles; i++)
	{
		sleep(microseconds(100));
		if (tiles[i].getPosition().y + tile_height > W_Height)
		{
			score += 2;
			int px = rand() % (W_Width - tile_width);
			tiles[i].setPosition(px, py[n_tiles - 1] - tile_width);
		}
	}

	/* previous implementation
	//I am keeping this implementation just to feel that how dumb this was :-|
	for (int i = 0; i < n_tiles; i++)
	{
		if (tiles[i].getPosition().y > W_Height)
		{
			score += 2;

			last_tile_y += tile_gap_y;
			int px, py;
			int low = last_tile_x - tile_gap_x - tile_width;
			int high = last_tile_x + tile_gap_x + tile_width;

			px = (rand() % (high - low + 1) + low);
			py = last_tile_y - tile_gap_y;
			last_tile_y = py;

			if (px > W_Width - tile_width)
			{
				px = 0;
				px -= tile_width;
			}
			else if (px < 0)
			{
				px = 0;
				px = rand() % W_Width / 2;
			}
			else if (i == 11)
				py = py - 30;
			else if (i == 0)
				py = py + 30;

			last_tile_x = px;
			tiles[i].setPosition(px, py);

		}
	}	*/

	if (y < W_Height / 2 - 50)
		for (int i = 0; i < n_tiles; i++)
			tiles[i].move(0, fabs(velocity_y)* dt - 0.01);
}

void update_mouse_menu(void)
{
	float bx = Play.getPosition().x;
	float by = Play.getPosition().y;
	float b_w = Play.getGlobalBounds().width;
	float b_h = Play.getGlobalBounds().height;

	help.setFillColor(Color::Black);
	Manual.setFillColor(Color::Black);
	Auto.setFillColor(Color::Black);
	Play.setTexture(play_b);

	int hot = mx > bx && mx < bx + b_w &&
		my > by && my < by + b_h;
	if (hot)
		Play.setTexture(play_b_on);
	if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
	{
		Collect.play();
		game_state = 0;
	}

	int hot_2 = mx > 630 && mx < 730 &&
		my > 285 && my < 335;
	if (hot_2)
		help.setFillColor(Color::Blue);
	if (hot_2 && Mouse::isButtonPressed(Mouse::Button::Left))
	{
		Collect.play();
		draw_help = true;
	}

	//updating modes
	int hot_a = mx > 400 && mx < 400 + Auto.getGlobalBounds().width &&
		my > 540 && my < 540 + Auto.getGlobalBounds().height;
	if (hot_a)
		Auto.setFillColor(Color(139, 69, 19, 255));
	if (hot_a && Mouse::isButtonPressed(Mouse::Button::Left))
	{
		mode = 1;
		Egg.setPosition(500, 528);
	}

	int hot_m = mx > 600 && mx < 600 + Manual.getGlobalBounds().width &&
		my > 540 && my < 540 + Manual.getGlobalBounds().height;
	if (hot_m)
		Manual.setFillColor(Color(139, 69, 19, 255));
	if (hot_m && Mouse::isButtonPressed(Mouse::Button::Left))
	{
		mode = 0;
		Egg.setPosition(535, 528);
	}

}

void update_mouse_over(void)
{
	int hot = mx > 260 && mx < 550 &&
		my > 550 && my < 600;
	if (hot)
		draw_menu = true;
	if (hot && Mouse::isButtonPressed(Mouse::Button::Left))
	{
		Collect.play();
		game_state = 3;
	}

	int hot_r = mx > 260 && mx < 550 &&
		my > 455 && my < 505;
	if (hot_r)
		draw_replay = true;
	if (hot_r && Mouse::isButtonPressed(Mouse::Button::Left))
	{
		Collect.play();
		game_state = 0;
	}
}

void spwan_pickups(void)
{
	timer += dt;
	if (timer > spawn_time)
	{
		timer = 0;
		int h = W_Width - Jet_new.getGlobalBounds().width;
		int sx = (rand() % (h + 1));
		Jet_new.setPosition(sx, -100);
		spawner = true;
		spawnd = true;	//one time execution variable
	}
}

void update_pickups(void)
{
	if (spawner == true)
	{
		Jet_new.move(0, move_speed*dt);
		//checking collisoin with spawn
		if (x + doodle_w > Jet_new.getPosition().x && x < Jet_new.getPosition().x + Jet_new.getGlobalBounds().width - 10 &&
			y + doodle_h > Jet_new.getPosition().y && y < Jet_new.getPosition().y + Jet_new.getGlobalBounds().height - 10)
		{
			JetPack.play();
			collided = true;
		}

		if (collided == true)
		{
			//set jetpack position
			if (Doodle.getTexture() == &doodle)
			{
				Jet_on.setScale(-1, 1);
				Jet_on.setPosition(x + doodle_w + Jet_on.getGlobalBounds().width, y);
				Jet_off.setScale(-1, 1);
				Jet_off.setPosition(x + doodle_w + Jet_on.getGlobalBounds().width, y);
			}
			if (Doodle.getTexture() == &doodle_2)
			{
				Jet_on.setScale(1, 1);
				Jet_on.setPosition(x - Jet_on.getGlobalBounds().width, y);
				Jet_off.setScale(1, 1);
				Jet_off.setPosition(x - Jet_on.getGlobalBounds().width, y);
			}
			move_speed = 0;

			//update jetpack speed
			if (jet_time_counter < 4)
			{
				jet_time_counter += dt;
				velocity_y = -1200;
			}
			else if (jet_time_counter < 5)
			{
				jet_time_counter += dt;
				velocity_y = -800;
			}
			else if (jet_time_counter < 7)
			{
				jet_time_counter += dt;
				velocity_y = -500;
			}
			else
			{
				jet_time_counter = 0;
				spawner = false;
				collided = false;
				move_speed = 120;
			}

			if (spawnd == true)
				spawn_time += 20;
			spawnd = false;
		}
	}
}


