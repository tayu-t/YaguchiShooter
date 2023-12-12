#pragma once
#include"Enemy.h"
#include"YaguchiChan.h"
#include"Block.h"

enum PlayScene
{
	title,
	play,
	result,
};

class Rock
{
private:
	Vec2 pos;
	Vec2 pre_pos;
	Vec2 speed;
	
	double width{ 64 };
	double height{ 64 };
	RectF hit_box;
	double theta{ 0_deg };
	double gravity{ 1090 };
public:
	bool exist = true;
	Rock(Vec2 pos, Vec2 speed) :pos(pos), speed(speed) {};
	void update() {
		double scene_del = Scene::DeltaTime();
		speed.y += gravity * scene_del;

		pos += speed * scene_del;
		if (pos.y + height / 2 >= Scene::Size().y - 20.1) {
			pos.y = Scene::Size().y - 20.1 - height / 2;
		}
		theta += (speed.x*2 / width) * scene_del;
		
		if (speed.x > 0 && pos.x - width / 2 > Scene::Size().x) {
			exist = false;
		}
		else if (speed.x < 0 && pos.x + width / 2 < 0) {
			exist = false;
		}


		hit_box = RectF(Arg::center(pos), width, height);
	};
	void draw() {
		//hit_box.draw(Palette::Skyblue);
		ScopedRenderStates2D sanpra{ SamplerState::ClampNearest };

		TextureAsset(U"rock").scaled(2).rotated(theta).drawAt(pos);
	}
	inline RectF& get_hit_box() noexcept { return hit_box; }
	inline const Vec2 get_pre_pos() noexcept { return pre_pos; }
	inline const Vec2 get_pos() noexcept { return pos; }
	inline void set_pos(Vec2 t)noexcept {
		pos = t;
		hit_box = RectF(Arg::center(pos), width, height);
	}

	inline void set_speed(Vec2 t) noexcept { speed = t; }
	inline Vec2 get_speed()noexcept { return speed; }
};

class Play
{
private:
	double i = 30.1_deg;
	Effect effect;
	PlayScene play_scene{ title };
	Vec2 scene_size = Scene::Size();
	YaguchiChan yaguchichan{this};
	Array<YaguchiChanShot> yaguchichanshots;
	Array<Rock> rocks;
	int rock_spon_pos_index;
	//bool be_rock_warnig{ false };
	double rock_span{ 10 };
	double rock_timer{ 0 };

	double see_score{ 0 };
	int true_score{ 0 };
	int high_score{ 0 };

	Vec2 board_pos{ 0,0 };

	Array<Enemy> enemies;

	double left_wall{ 0 };
	double right_wall{ double(Scene::Size().x)};

	Array<Block> blocks;

	double scene_del;

	int enemy_pop_num{ 1 };
	double enemy_pop_num_span{ 10 };
	double enemy_pop_num_timer{ 0 };
	double enemy_span{ 5 };
	Vec2 enemy_pop_poss[4];//左上,右上,右下,左下
	double enemy_pop_timer{ 0 };
	double ground_afford{ 200 };//enemyを召喚するためのもの
public:
	Play();
	void update();
	void draw();
	void intersect();
	void reset();

	Array<YaguchiChanShot>& get_yaguchichanshot() noexcept{ return yaguchichanshots; }
	double get_scene_del() noexcept { return scene_del; }
	Effect& get_effect() noexcept { return effect; }
	int& get_true_score() noexcept { return true_score; }
};

