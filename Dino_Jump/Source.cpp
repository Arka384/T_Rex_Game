#include "Utils.h"
float another = 0;
int main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(window_width, window_height), "Test", Style::Close);
	Clock clk;
	Time t;

	float timer = 0;
	int last_score = 0;
	init_ground();
	init_clouds();
	init_dino();
	init_obstacles();
	init_numberes();

	SoundBuffer hurt, jump, scored;
	Sound Hurt, Jump, Scored;
	title.loadFromFile("Resources/dino.png");
	Title.setTexture(title);
	Title.setScale(Vector2f(2.4, 2.4));
	Title.setPosition(window_width / 2 - Title.getGlobalBounds().width / 2, 100);

	hurt.loadFromFile("Resources/sounds/hurt.wav");
	Hurt.setBuffer(hurt);
	jump.loadFromFile("Resources/sounds/jump.wav");
	Jump.setBuffer(jump);
	Jump.setVolume(20.f);
	scored.loadFromFile("Resources/sounds/100.wav");
	Scored.setBuffer(scored);



	while (window.isOpen())
	{
		t = clk.restart();
		dt = t.asSeconds();

		Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::Closed:
				window.close();
			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Enter))
					if (game_state == 0 || game_state == 2) {
						reset();
						game_state = 1;	//switch to play state
						trex.sprite.setTextureRect(IntRect(trex.pos_x, 0, 88, 93));
					}
				if (Keyboard::isKeyPressed(Keyboard::Up) && trex.y + trex.sprite.getGlobalBounds().height >= window_height - 80) {
					Jump.play();
					trex.velocity_y = -900;
				}
				break;
			default:
				break;
			}
		}

		switch (game_state)
		{
		case 1:	//play state
			timer += dt;
			another += dt;

			ground_update();
			cloud_update();
			update_obstacles(Hurt);
			game_updates();
			trex.sprite.setPosition(30, trex.y);

			if (timer > trex.animate_timer) {
				if (trex.y + trex.sprite.getGlobalBounds().height < window_height - 80)
					trex.sprite.setTextureRect(IntRect(1338, 2, 88, 94));
				else
					runnig();
				update_score();
				if (score.score - last_score == 100) {
					Scored.play();
					for (int i = 0; i < 2; i++) {
						gr[i].ground_scroll_speed += 50;
						cloud[i].cloud_scroll_speed += 10;
					}
					last_score = score.score;
				}
				timer = 0;
			}

			break;
		case 2:
			trex.sprite.setTextureRect(IntRect(1778, 4, 86, 92));
			break;
		default:
			break;
		}



		window.clear();
		render(window);
		window.display();
	}
}

//////////////////////////////////////
void init_dino()
{
	trex.sprite.setTexture(sprite_sheet);
	trex.sprite.setTextureRect(IntRect(75, 0, 90, 96));
	trex.sprite.setPosition(30, trex.y);
}

void init_obstacles()
{
	tree_rects[0] = IntRect(446, 1, 102, 72);	//small-3
	tree_rects[1] = IntRect(650, 2, 152, 100);	//big-3
	tree_rects[2] = IntRect(650, 2, 52, 100);	//big-1
	tree_rects[3] = IntRect(850, 2, 102, 100);	//mixed

	for (int i = 0; i < 4; i++)
	{
		tree[i].y = window_height - 170;
		if (i == 0)
			tree[i].y = window_height - 150;
		tree[i].sprite.setTexture(sprite_sheet);
		tree[i].sprite.setTextureRect(tree_rects[i]);
		tree[i].x = window_width + 200;
		tree[i].sprite.setPosition(tree[i].x, tree[i].y);
	}

	trees.push_back(tree[rand() % 4]);

}

void init_ground()
{
	sprite_sheet.loadFromFile("Resources/dino_game.png");
	for (int i = 0; i < 2; i++)
	{
		gr[i].sprite.setTexture(sprite_sheet);
		gr[i].sprite.setTextureRect(IntRect(0, 102, gr[i].sprite_width, 26));
		gr[i].sprite.setPosition(i * 2400, window_height - 100);
	}

	//gamve over itmes are initialized here
	gover.setTexture(sprite_sheet);
	gover.setTextureRect(IntRect(952, 28, 382, 24));
	gover.setPosition(window_width / 2 - gover.getGlobalBounds().width / 2, 100);
	gover_icon.setTexture(sprite_sheet);
	gover_icon.setTextureRect(IntRect(2, 2, 72, 64));
	gover_icon.setScale(Vector2f(0.7, 0.7));
	gover_icon.setPosition(window_width / 2 - gover_icon.getGlobalBounds().width / 2, 150);

}

void init_clouds()
{
	for (int i = 0; i < 2; i++)
	{
		cloud[i].sprite.setTexture(sprite_sheet);
		cloud[i].sprite.setTextureRect(IntRect(165, 0, 94, 30));
		cloud[i].y = rand() % (cloud[i].max_y - cloud[i].min_y + 1) + cloud[i].min_y;
		cloud[i].x = rand() % (window_width * 2 - window_width + 1) + window_width + cloud[i - 1].x / 2;
		cloud[i].sprite.setPosition(cloud[i].x, cloud[i].y);
	}
}

void init_numberes()
{
	for (int i = 0; i < 10; i++)
		numbers[i] = IntRect(954 + (i * 20), 2, 20, 21);

	for (int i = 0; i < 5; i++)
	{
		score.digits[i].setTexture(sprite_sheet);
		score.digits[i].setTextureRect(numbers[0]);
		score.digits[i].setPosition((window_width - 50) - (i * 25), 20);
	}
}
//////////////////////////////////////

void ground_update()
{
	for (int i = 0; i < 2; i++)
		gr[i].sprite.move(Vector2f(-gr[i].ground_scroll_speed*dt, 0));;

	if (gr[0].sprite.getPosition().x < -2400)
		gr[0].sprite.setPosition(gr[1].sprite.getPosition().x + gr[1].sprite_width, window_height - 100);
	if (gr[1].sprite.getPosition().x < -2400)
		gr[1].sprite.setPosition(gr[0].sprite.getPosition().x + gr[0].sprite_width, window_height - 100);
}

void cloud_update()
{
	for (int i = 0; i < 2; i++) {
		cloud[i].x = cloud[i].sprite.getPosition().x;
		cloud[i].y = cloud[i].sprite.getPosition().y;
		cloud[i].sprite.move(Vector2f(-cloud[i].cloud_scroll_speed*dt, 0));
	}

	if (cloud[0].x + 100 < 0)
	{
		cloud[0].x = rand() % (window_width * 2 - window_width + 1) + window_width + cloud[1].x / 4;
		cloud[0].sprite.setPosition(cloud[0].x, cloud[0].y);
	}
	if (cloud[1].x + 100 < 0)
	{
		cloud[1].x = rand() % (window_width * 2 - window_width + 1) + window_width + cloud[0].x / 4;
		cloud[1].sprite.setPosition(cloud[1].x, cloud[1].y);
	}
}

void update_obstacles(Sound &hurt)
{
	for (auto i = trees.begin(); i != trees.end(); i++) {
		i->sprite.move(Vector2f(-i->scrool_speed*dt, 0));

		if (trex.sprite.getPosition().x + trex.sprite.getGlobalBounds().width - 30 > i->sprite.getPosition().x &&
			trex.sprite.getPosition().x + 30 < i->sprite.getPosition().x + i->sprite.getGlobalBounds().width &&
			trex.sprite.getPosition().y + trex.sprite.getGlobalBounds().height - 30 > i->sprite.getPosition().y) {
			hurt.play();
			game_state = 2;	//gameover state
		}

	}

	float m = 1.2, n = 2.5;
	if (score.score > 400) {
		m = 0.8; n = 2.0;
	}
	else if (score.score > 600) {
		m = 0.5, n = 1.0;
	}
	else if (score.score > 800) {
		m = 0.2; n = 0.6;
	}

	if (another > randf(m, n))
	{
		int j = rand() % 4;
		tree[j].scrool_speed = gr[0].ground_scroll_speed;
		trees.push_back(tree[j]);
		another = 0;
	}

	if (trees.size() > 5)
		trees.pop_front();
}

void runnig()
{
	if (trex.pos_x == 1602)
		trex.pos_x = 1514;
	else
		trex.pos_x += 88;
	trex.sprite.setTextureRect(IntRect(trex.pos_x, 0, 88, 93));
}

void update_score()
{
	score.score++;

	///0
	if (score.d[0] == 9) {
		score.d[0] = 0;
		score.flag[1] = true;
		score.digits[0].setTextureRect(numbers[0]);
	}
	else {
		score.d[0]++;
		score.digits[0].setTextureRect(numbers[score.d[0]]);
	}
	////1
	if (score.flag[1]) {
		if (score.d[1] == 9) {
			score.d[1] = 0;
			score.flag[2] = true;
			score.digits[1].setTextureRect(numbers[1]);
		}
		else {
			score.d[1]++;
			score.digits[1].setTextureRect(numbers[score.d[1]]);
		}
		score.flag[1] = false;
	}
	////2
	if (score.flag[2]) {
		if (score.d[2] == 9) {
			score.d[2] = 0;
			score.flag[3] = true;
			score.digits[2].setTextureRect(numbers[2]);
		}
		else {
			score.d[2]++;
			score.digits[2].setTextureRect(numbers[score.d[2]]);
		}
		score.flag[2] = false;
	}
	////3
	if (score.flag[3]) {
		if (score.d[3] == 9) {
			score.d[3] = 0;
			score.flag[4] = true;
			score.digits[3].setTextureRect(numbers[3]);
		}
		else {
			score.d[3]++;
			score.digits[3].setTextureRect(numbers[score.d[3]]);
		}
		score.flag[3] = false;
	}
	////4
	if (score.flag[4]) {
		score.d[4]++;
		score.digits[4].setTextureRect(numbers[score.d[4]]);
		score.flag[4] = false;
	}

}

void game_updates()
{
	//all game runnig state updates
	static int gravity = 2500;
	float height = trex.sprite.getGlobalBounds().height;
	if (trex.y + height < window_height - 80)
		trex.velocity_y += gravity * dt;
	else if (trex.y + height > window_height - 80)
		trex.y = window_height - height - 80;

	trex.velocity_y += trex.accleration_y * dt;
	trex.y += trex.velocity_y * dt;
}

////////////////////////////////////////////
void render(RenderWindow &thisWindow)
{
	if (game_state == 0)
		thisWindow.draw(Title);
	if (game_state != 0) {
		for (int i = 0; i < 2; i++)
		{
			thisWindow.draw(gr[i].sprite);
			thisWindow.draw(cloud[i].sprite);
		}
		for (auto i = trees.begin(); i != trees.end(); i++)
			thisWindow.draw(i->sprite);
		for (int i = 0; i < 5; i++)
			thisWindow.draw(score.digits[i]);
	}

	if (game_state == 2)
	{
		thisWindow.draw(gover);
		thisWindow.draw(gover_icon);
	}

	thisWindow.draw(trex.sprite);

}

//////////////////////////////////////////
float randf(float min, float max)
{
	float scale = rand() / (float)RAND_MAX;
	return (min + scale * (max - min));
}

void reset()
{
	trees.clear();
	trees.push_back(tree[rand() % 4]);
	another = 0;

	for (int i = 0; i < 2; i++)
	{
		gr[i].ground_scroll_speed = 600;
		cloud[i].cloud_scroll_speed = 150;
	}

	score.score = 0;
	for (int i = 0; i < 5; i++) {
		score.digits[i].setTextureRect(numbers[0]);
		score.d[i] = 0;
		score.flag[i] = 0;
	}

}