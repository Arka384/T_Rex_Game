
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>
#include <list>
using namespace sf;
constexpr int window_width = 1000;
constexpr int window_height = 400;

Texture sprite_sheet, title;
Sprite gover, gover_icon, Title;


typedef struct ground {
	Sprite sprite;
	int ground_scroll_speed = 600;
	int sprite_width = 2400;
}ground;
ground gr[2];

typedef struct clouds {
	Sprite sprite;
	int cloud_scroll_speed = 150;
	int x, y;
	int max_y = 150;
	int min_y = 50;
}clouds;
clouds cloud[2];

typedef struct dino {
	Sprite sprite;
	int pos_x = 1514;
	float y = window_height - 180;
	float animate_timer = 0.1;
	float velocity_y = 0;
	float accleration_y = 0;
}dino;
dino trex;

typedef struct obstacles {
	Sprite sprite;
	int x, y;
	int scrool_speed = 600;
}obstacles;
obstacles tree[4];
std::list<obstacles> trees;

typedef struct scoreing {
	int score;
	Sprite digits[5];
	int d[5] = { 0 };
	bool flag[5] = { false };
}scoring;
scoring score;

float dt = 0;
int game_state = 0;
IntRect tree_rects[4], numbers[10];

void init_ground(void);
void init_clouds(void);
void init_dino(void);
void init_obstacles(void);
void init_numberes(void);

void ground_update(void);
void cloud_update(void);
void update_obstacles(Sound &);
void game_updates(void);
void runnig(void);
void update_score(void);

void render(RenderWindow &);
void reset(void);
float randf(float, float);