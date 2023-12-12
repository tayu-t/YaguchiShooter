#include "stdafx.h"
#include "YaguchiChan.h"
#include"Play.h"

void YaguchiChan::update()
{
	pre_pos = pos;
	if (hp >= 1) {


		update_key_move();

		if (KeyV.down()) {
			play->get_yaguchichanshot() << YaguchiChanShot(pos + Vec2{ direction * width / 2 ,0 }, direction * 350);
			play->get_effect().add<ThrowEffect>(this, direction);
		}

		if (muteki_timer > 0) {
			muteki_timer -= play->get_scene_del();
		}

		hit_box = RectF(Arg::center(pos), width, height);
	}
	else {

		if (pos.y - height <= Scene::Size().y)
		{
			speed.y += gravity*0.6 * play->get_scene_del();

			pos += speed * play->get_scene_del();
		}
		else {
			finish = true;
		}


	}
}



void YaguchiChan::update_key_move()
{
	
	bool t =true;
	if (KeyRight.pressed()) {
		speed.x += key_acc*play->get_scene_del();
		direction = 1;
		t = false;
	}

	if (KeyLeft.pressed()) {
		speed.x -= key_acc * play->get_scene_del();
		direction = -1;
		t = false;
	}

	if (KeySpace.down() && grounded) {
		speed.y = -540;
	}

	if (grounded && t) { speed.x *= 0.07; }
	

	speed.x = Clamp<double>(speed.x, -x_speed_max, x_speed_max);

	if (speed.y <= 0) {
		speed.y += gravity * play->get_scene_del();
	}
	else {

		speed.y += gravity*1.2 * play->get_scene_del();
	}
	//speed.y = Max<double>(speed.y, 600);

	pos += speed * play->get_scene_del();
}



void YaguchiChan::draw()
{

//	hit_box.draw(Palette::Skyblue);
	
	TextureAsset(U"yaguchichan").mirrored(direction == -1).scaled(0.2).drawAt(pos);

	for (int i = 0; i < start_hp; i++) {

		TextureAsset(U"emptyhp").scaled(5).draw(Vec2{ 0,50 * i });
		if (hp >= i + 1) {

			TextureAsset(U"hp").scaled(5).draw(Vec2{ 0,50 * i });
		}
	}
}

void YaguchiChan::damage(int d)
{
	if (muteki_timer <= 0) {
		hp-= d;

		if (hp <= 0) {
			speed.y = -360;
			speed.x = 0;
		}
		else {
			play->get_effect().add<SweatEffect>(this, direction);
		}

		muteki_timer = muteki_time;
	}
}
