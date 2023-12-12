#include "stdafx.h"
#include "Enemy.h"
#include"Play.h"

Enemy::Enemy(Play* play,int index, Vec2 pos)
	:play(play),index(index),pos(pos)
{
	pre_pos = pos;
	switch (index)
	{
	case 1:

		score = 100;

		hp = 1;
		if (pos.x <= 0) {
			speed = Vec2{Random(90,200),0};
		}
		else {

			speed = Vec2{ -Random(90,200),0 };
		}
		break;
	case 2:
		score = 200;

		hp = 1;
		if (pos.x <= 0) {
			speed = Vec2{ Random(90,200),0 };
		}
		else {

			speed = Vec2{ -Random(90,200),0 };
		}
	
		break;
	case 3:

		hp = 5;

		score = 400;
		width *= 1.5;
		height *= 1.5;
		if (pos.x <= 0) {
			speed = Vec2{ Random(50,110),0 };
		}
		else {

			speed = Vec2{ -Random(50,110),0 };
		}


		break;
	default:
		break;
	}
	hit_box = RectF(Arg::center(pos), width, height);
}

void Enemy::update()
{
	pre_pos = pos;


	switch (index)
	{
	case 1:
		update1();
		break;
	case 2:
		update2();
		break;
	case 3:
		update1();
		break;
	default:
		break;
	}
}

void Enemy::update1()
{

	speed.y += gravity * play->get_scene_del();

	pos += speed * play->get_scene_del();
	if (pos.y + height / 2 >= Scene::Size().y - 20.1) {
		pos.y = Scene::Size().y - 20.1 - height / 2;
	}

	if (speed.x > 0 && pos.x-width/2 > Scene::Size().x) {
		exist = false;
	}
	else if (speed.x < 0 && pos.x + width/2< 0) {
		exist = false;
	}


	hit_box = RectF(Arg::center(pos), width, height);
}

void Enemy::update2()
{
	if (grounded) {
		speed.y = -520;
	}


	speed.y += gravity * play->get_scene_del();

	pos += speed * play->get_scene_del();
	

	if (speed.x > 0 && pos.x - width / 2 > Scene::Size().x) {
		exist = false;
	}
	else if (speed.x < 0 && pos.x + width / 2 < 0) {
		exist = false;
	}


	hit_box = RectF(Arg::center(pos), width, height);
}



void Enemy::draw()
{
	//hit_box.draw(Palette::Skyblue);
	ScopedRenderStates2D sanpra{ SamplerState::ClampNearest };

	if (index == 3) {
		TextureAsset(U"yaguchi").mirrored(speed.x <= 0).scaled(0.3).drawAt(pos);
	}
	else {
		TextureAsset(U"yaguchi").mirrored(speed.x <= 0).scaled(0.20).drawAt(pos);
	}
}

void Enemy::damage()
{
	hp--;
	if (hp <= 0) {
		play->get_effect().add<DeathEffect>(pos);
		play->get_true_score() += score;
		exist = false;
	}
}


