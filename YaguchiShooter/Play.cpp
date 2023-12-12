#include "stdafx.h"
#include "Play.h"

Play::Play()
{

	

	//地面
	blocks << Block(this, RectF(-ground_afford, scene_size.y-20, scene_size.x+ ground_afford, scene_size.y - 20), normal);

	//中央
	blocks << Block(this, RectF(Arg::center(scene_size/2 + Vec2{0,180}), 200, 20), normal);

	//空中
	double l = 250;
	double y = 370;

	blocks << Block(this, RectF(Vec2{0- ground_afford,y}, l + ground_afford, 20), normal);
	blocks << Block(this, RectF(Vec2{ scene_size.x-l,y }, l + ground_afford, 20), normal);

	enemy_pop_poss[0] = Vec2{ -ground_afford + 100,300 };
	enemy_pop_poss[1] = Vec2{ scene_size.x + ground_afford - 100,300 };
	enemy_pop_poss[2] = Vec2{ scene_size.x + ground_afford - 100,scene_size.y - 80 };
	enemy_pop_poss[3] = Vec2{ -ground_afford + 100,scene_size.y -80 };
	reset();
}

void Play::update()
{
	scene_del = Scene::DeltaTime();


	if (play_scene == title) {

		
		if (KeySpace.down()) {
			play_scene = play;
			reset();
		}
	}
	else if (play_scene == play) {

		
		if (board_pos.y <= -200) {

			yaguchichan.update();

			see_score += (true_score - see_score) /8;
			if (true_score - see_score <= 5) {
				see_score = true_score;
			}
			
			for (auto& shot : yaguchichanshots) {
				shot.pos.x += shot.speed * scene_del;

				for (auto& block : blocks) {
					if (block.get_hit_box().intersects(RectF(Arg::center(shot.pos), shot.width, shot.height))) {
						shot.exist = false;
						break;
					}
				}

				if (shot.pos.x + shot.width / 2 < 0 || shot.pos.x - shot.width / 2 > scene_size.x) {
					shot.exist = false;
				}


			}



			yaguchichanshots.remove_if([](YaguchiChanShot shot) {
				return shot.exist == false;
			});

			

			enemy_pop_timer += scene_del;
			if (enemy_pop_timer >= enemy_span) {
				enemy_pop_timer -= enemy_span;
				enemy_span -= 0.4;
				enemy_span = Max(enemy_span, 2.3);
				for (int i = 0; i < enemy_pop_num; i++) {
					if (RandomBool(0.8)) {
						
						enemies << Enemy(this, 1, enemy_pop_poss[Random(0, 3)] + Vec2{ Random(-20,20),0 });
					}
					else if(RandomBool(0.6)){

						enemies << Enemy(this, 2, enemy_pop_poss[Random(0, 3)] + Vec2{ Random(-20,20),0 });
					}
					else {
						enemies << Enemy(this, 3, enemy_pop_poss[Random(0, 3)] + Vec2{ Random(-20,20),-15 });
					}
				}
			}


			enemy_pop_num_timer += scene_del;
			if (enemy_pop_num_timer >= enemy_pop_num_span) {
				enemy_pop_num_timer -= enemy_pop_num_span;
				enemy_pop_num_span -= 0.2;
				enemy_pop_num_span = Max(enemy_pop_num_span, 5.0);

				enemy_pop_num++;
			}

			if (enemy_pop_num >= 3 ) {

				rock_timer += scene_del;
				if (rock_timer >= rock_span) {
					rock_timer -= rock_span;
					rock_spon_pos_index = Random(0, 3);
					rock_timer = 0;
					rocks << Rock(enemy_pop_poss[rock_spon_pos_index] + Vec2{ 0,-50 }, Vec2{ enemy_pop_poss[rock_spon_pos_index].x < 0 ? 200 : -200 ,0 });

				}
			
				/*
				if (be_rock_warnig == false) {
					if (rock_timer >= rock_span) {
						rock_spon_pos_index = Random(0, 3);
						be_rock_warnig = true;
						rock_timer = 0;
					}
				}
				else {
					if (rock_timer >= 3) {
						rocks << Rock(enemy_pop_poss[rock_spon_pos_index] + Vec2{ 0,-50 }, Vec2{ enemy_pop_poss[rock_spon_pos_index].x < 0 ? 200 : -200 ,0});
						rock_timer = 0;
						be_rock_warnig = false;
					}
				}*/
			}

			for (auto& rock : rocks) {
				rock.update();
			}
			rocks.remove_if([](Rock rock) {
				return rock.exist == false;
			});

			
			for (auto& enemy : enemies) {
				if (enemy.exist) {
					enemy.update();
					for (auto& shot : yaguchichanshots) {
						if (shot.exist) {

							if (enemy.get_hit_box().intersects(RectF(Arg::center(shot.pos), shot.width, shot.height))) {
								shot.exist = false;
								enemy.damage();
								break;
							}
						}
					}

					
				}
			}

			enemies.remove_if([](Enemy enemy) {
				return enemy.exist == false;
			});


			intersect();

			if (yaguchichan.get_finish()) {
				play_scene = result;
			}
		}
		else {
			board_pos.y -= 400*scene_del;
			enemy_pop_timer = enemy_span - 1;

		}

		


	}
	else if (play_scene == result) {

		if (board_pos.y < scene_size.y / 2 -160 ) {
			board_pos.y += 400 * scene_del;
			if (true_score > high_score) {
				high_score = true_score;
			}
		}
		else {
			board_pos.y = scene_size.y / 2 -160 ;

			if (KeySpace.down()) {
				play_scene = title;

				reset();
			}
		}

	}

	
}

void Play::draw()
{
	RectF(0, 0, scene_size).draw(Arg::top = Palette::Cornflowerblue, Arg::bottom = Palette::Paleturquoise);

	ScopedRenderStates2D sanpra{ SamplerState::ClampNearest };

	for (auto& b : blocks) {
		b.draw();
	}

	

	for (auto& shot : yaguchichanshots) {
		TextureAsset(U"yaguchichanshot").mirrored(shot.speed <= 0).drawAt(shot.pos);
		TextureAsset(U"yaguchichanshot").mirrored(shot.speed <= 0).drawAt(shot.pos - Sign(shot.speed)* Vec2{ 9,0 },ColorF(1,0.4));
		TextureAsset(U"yaguchichanshot").mirrored(shot.speed <= 0).drawAt(shot.pos - Sign(shot.speed) * Vec2{ 13,0 }, ColorF(1, 0.2));
	}
	for (auto& rock : rocks) {
		rock.draw();
	}

	yaguchichan.draw();

	effect.update();

	for (auto& enemy : enemies) {
		enemy.draw();
	}


	if (play_scene == title) {
		TextureAsset(U"board").scaled(8.5).drawAt(board_pos);

		FontAsset(U"2")(U"Fight T.Y !!").drawAt( board_pos - Vec2{0,(245 - 160) }, Palette::Red);

		FontAsset(U"1")(U"HighScore").drawAt(30,board_pos - Vec2{ 0,(185 - 160) },Palette::Black);
		FontAsset(U"1")(U"{}"_fmt(int(high_score))).drawAt(board_pos - Vec2{ 0,(130 - 160) }, Palette::Red);
		FontAsset(U"1")(U"Press SpaceKey").drawAt(25,board_pos - Vec2{ 0,(60- 160) }, Palette::Black);

	}
	else if (play_scene == play) {
		FontAsset(U"1")(U"Score").drawAt(scene_size.x/2 ,50, Palette::Black);
		FontAsset(U"1")(U"{}"_fmt(int(see_score))).drawAt(scene_size.x/2 ,90, Palette::Yellow);



		if (board_pos.y > -200){
			TextureAsset(U"board").scaled(8.5).drawAt(board_pos);

			FontAsset(U"2")(U"Fight T.Y !!").drawAt(board_pos - Vec2{ 0,(245 - 160) }, Palette::Red);

			FontAsset(U"1")(U"HighScore").drawAt(30, board_pos - Vec2{ 0,(185 - 160) }, Palette::Black);
			FontAsset(U"1")(U"{}"_fmt(int(high_score))).drawAt(board_pos - Vec2{ 0,(130 - 160) }, Palette::Red);
			FontAsset(U"1")(U"Press SpaceKey").drawAt(25, board_pos - Vec2{ 0,(60 - 160) }, Palette::Black);
		}
		else {
			if (see_score < 1000) {
				FontAsset(U"2")(U"[←→]:move!").drawAt(30, scene_size.x / 2 - 240, 50, Palette::Black);
				FontAsset(U"2")(U"[Space]:Jump!").drawAt(30, scene_size.x / 2 - 240, 90, Palette::Black);
				FontAsset(U"2")(U"[V]:Attack!").drawAt(30, scene_size.x / 2 - 240, 130, Palette::Black);
			}

		}




	}
	else if (play_scene == result) {


		TextureAsset(U"board").scaled(8.5).drawAt(board_pos);

		FontAsset(U"1")(U"Score").drawAt(board_pos - Vec2{ 0,(245 - 160) }, Palette::Black);
		FontAsset(U"1")(U"{}"_fmt(int(true_score))).drawAt(board_pos - Vec2{ 0,(197 - 160) }, Palette::Red);
		FontAsset(U"1")(U"HighScore").drawAt(30, board_pos - Vec2{ 0,(150 - 160) }, Palette::Black);
		FontAsset(U"1")(U"{}"_fmt(int(high_score))).drawAt(board_pos - Vec2{ 0,(102 - 160) }, Palette::Red);
		FontAsset(U"1")(U"Press SpaceKey").drawAt(25, board_pos - Vec2{ 0,(60 - 160) }, Palette::Black);


	}

}

void Play::intersect()
{
	if (yaguchichan.get_hp() >= 1) {
		//壁との衝突

		if (yaguchichan.get_hit_box().x <= left_wall) {
			yaguchichan.set_pos(Vec2(left_wall + yaguchichan.get_hit_box().w / 2, yaguchichan.get_pos().y));
			yaguchichan.set_speed(Vec2{ yaguchichan.get_speed().x * -0.01, yaguchichan.get_speed().y });
		}
		else if (yaguchichan.get_hit_box().rightX() >= right_wall) {

			yaguchichan.set_pos(Vec2(right_wall - yaguchichan.get_hit_box().w / 2, yaguchichan.get_pos().y));

			yaguchichan.set_speed(Vec2{ yaguchichan.get_speed().x * -0.01, yaguchichan.get_speed().y });
		}
		yaguchichan.get_grounded() = false;


		//blockとの衝突
		for (auto& b : blocks) {
			if (b.get_hit_box().intersects(yaguchichan.get_hit_box())) {
				double line_rate = 0.02;
				//yaguchiの下面は前のフレームではstageの上面より上にあった
				if (yaguchichan.get_pre_pos().y + yaguchichan.get_hit_box().h / 2 <= b.get_hit_box().y)
				{
					if (yaguchichan.get_hit_box().bottomY() >= b.get_hit_box().y)
					{


						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(yaguchichan.get_hit_box().x, b.get_hit_box().x);
						intersect_line_end = Min(yaguchichan.get_hit_box().rightX(), b.get_hit_box().rightX());

						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / yaguchichan.get_hit_box().w >= line_rate)
						{
							yaguchichan.set_pos(Vec2{ yaguchichan.get_pos().x,b.get_hit_box().y - 0.01 - yaguchichan.get_hit_box().h / 2 });
							yaguchichan.set_speed(Vec2{ yaguchichan.get_speed().x ,yaguchichan.get_speed().y * -b.get_e() });
							yaguchichan.get_grounded() = true;
						}
					}
				}




				//actorの上面は前のフレームではstageの下面より下にあった
				if (yaguchichan.get_pre_pos().y - yaguchichan.get_hit_box().h / 2 >= b.get_hit_box().bottomY())
				{

					if (yaguchichan.get_hit_box().y <= b.get_hit_box().bottomY())
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(yaguchichan.get_hit_box().x, b.get_hit_box().x);
						intersect_line_end = Min(yaguchichan.get_hit_box().rightX(), b.get_hit_box().rightX());

						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / yaguchichan.get_hit_box().w >= line_rate)
						{
							yaguchichan.set_pos(Vec2{ yaguchichan.get_pos().x,b.get_hit_box().bottomY() + 0.01 + yaguchichan.get_hit_box().h / 2 });
							yaguchichan.set_speed(Vec2{ yaguchichan.get_speed().x ,yaguchichan.get_speed().y * -b.get_e() });

						}
					}
				}



				//actorがstageの右にあたる

				//actorの左面は前のフレームではstageの右面より右にあった
				if (yaguchichan.get_pre_pos().x - yaguchichan.get_hit_box().w / 2 >= b.get_hit_box().rightX())
				{
					if (yaguchichan.get_hit_box().x <= b.get_hit_box().rightX())
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(yaguchichan.get_hit_box().y, b.get_hit_box().y);
						intersect_line_end = Min(yaguchichan.get_hit_box().bottomY(), b.get_hit_box().bottomY());


						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / yaguchichan.get_hit_box().h >= line_rate)
						{

							yaguchichan.set_pos(Vec2{ b.get_hit_box().rightX() + 0.01 + yaguchichan.get_hit_box().w / 2,yaguchichan.get_pos().y });
							yaguchichan.set_speed(Vec2{ yaguchichan.get_speed().x * -b.get_e(),yaguchichan.get_speed().y });

						}
					}
				}


				//actorがstageの左にあたる
				//actorの右面は前のフレームではstageの左面より左にあった
				if (yaguchichan.get_pre_pos().x + yaguchichan.get_hit_box().w / 2 <= b.get_hit_box().x)
				{
					if (yaguchichan.get_hit_box().rightX() >= b.get_hit_box().x)
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(yaguchichan.get_hit_box().y, b.get_hit_box().y);
						intersect_line_end = Min(yaguchichan.get_hit_box().bottomY(), b.get_hit_box().bottomY());


						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / yaguchichan.get_hit_box().h >= line_rate)
						{
							yaguchichan.set_pos(Vec2{ b.get_hit_box().x - 0.01 - yaguchichan.get_hit_box().w / 2,yaguchichan.get_pos().y });
							yaguchichan.set_speed(Vec2{ yaguchichan.get_speed().x * -b.get_e(),yaguchichan.get_speed().y });


						}
					}
				}

			}
		}

		//敵との衝突
		for (auto& enemy : enemies) {
			if (enemy.exist) {

				//shotのところでexist == falseとなることがあり得る
				if (yaguchichan.get_muteki_timer() <= 0 && yaguchichan.get_hp() >= 1) {

					if (enemy.get_hit_box().intersects(yaguchichan.get_hit_box())) {

						yaguchichan.set_speed(Vec2{ (yaguchichan.get_pos().x - enemy.get_pos().x) ? 1 : -1 * 200, -300 });
						yaguchichan.damage();
					}

				}
			}
		}

		for (auto& rock : rocks) {
			if (rock.exist) {

				//shotのところでexist == falseとなることがあり得る
				if (yaguchichan.get_muteki_timer() <= 0 && yaguchichan.get_hp() >= 1) {

					if (rock.get_hit_box().intersects(yaguchichan.get_hit_box())) {

						yaguchichan.set_speed(Vec2{ (yaguchichan.get_pos().x - rock.get_pos().x) ? 1 : -1 * 200, -300 });
						yaguchichan.damage(2);
					}

				}
			}
		}

	}
	//敵と地面
	//クラスでまとめろ定期

	for (auto& enemy : enemies) {

		enemy.get_grounded() = false;
		for (auto& b : blocks) {
			if (b.get_hit_box().intersects(enemy.get_hit_box())) {
				double line_rate = 0.02;
				//yaguchiの下面は前のフレームではstageの上面より上にあった
				if (enemy.get_pre_pos().y + enemy.get_hit_box().h / 2 <= b.get_hit_box().y)
				{
					if (enemy.get_hit_box().bottomY() >= b.get_hit_box().y)
					{


						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(enemy.get_hit_box().x, b.get_hit_box().x);
						intersect_line_end = Min(enemy.get_hit_box().rightX(), b.get_hit_box().rightX());

						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / enemy.get_hit_box().w >= line_rate)
						{
							enemy.set_pos(Vec2{ enemy.get_pos().x,b.get_hit_box().y - 0.1 - enemy.get_hit_box().h / 2 });
							enemy.set_speed(Vec2{ enemy.get_speed().x ,enemy.get_speed().y * -b.get_e() });
							enemy.get_grounded() = true;
						}
					}
				}




				//actorの上面は前のフレームではstageの下面より下にあった
				if (enemy.get_pre_pos().y - enemy.get_hit_box().h / 2 >= b.get_hit_box().bottomY())
				{

					if (enemy.get_hit_box().y <= b.get_hit_box().bottomY())
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(enemy.get_hit_box().x, b.get_hit_box().x);
						intersect_line_end = Min(enemy.get_hit_box().rightX(), b.get_hit_box().rightX());

						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / enemy.get_hit_box().w >= line_rate)
						{
							enemy.set_pos(Vec2{ enemy.get_pos().x,b.get_hit_box().bottomY() + 0.01 + enemy.get_hit_box().h / 2 });
							enemy.set_speed(Vec2{ enemy.get_speed().x ,enemy.get_speed().y * -b.get_e() });

						}
					}
				}



				//actorがstageの右にあたる

				//actorの左面は前のフレームではstageの右面より右にあった
				if (enemy.get_pre_pos().x - enemy.get_hit_box().w / 2 >= b.get_hit_box().rightX())
				{
					if (enemy.get_hit_box().x <= b.get_hit_box().rightX())
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(enemy.get_hit_box().y, b.get_hit_box().y);
						intersect_line_end = Min(enemy.get_hit_box().bottomY(), b.get_hit_box().bottomY());


						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / enemy.get_hit_box().h >= line_rate)
						{

							enemy.set_pos(Vec2{ b.get_hit_box().rightX() + 0.01 + enemy.get_hit_box().w / 2,enemy.get_pos().y });
							enemy.set_speed(Vec2{ enemy.get_speed().x * -1,enemy.get_speed().y });

						}
					}
				}


				//actorがstageの左にあたる
				//actorの右面は前のフレームではstageの左面より左にあった
				if (enemy.get_pre_pos().x + enemy.get_hit_box().w / 2 <= b.get_hit_box().x)
				{
					if (enemy.get_hit_box().rightX() >= b.get_hit_box().x)
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(enemy.get_hit_box().y, b.get_hit_box().y);
						intersect_line_end = Min(enemy.get_hit_box().bottomY(), b.get_hit_box().bottomY());


						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / enemy.get_hit_box().h >= line_rate)
						{
							enemy.set_pos(Vec2{ b.get_hit_box().x - 0.01 - enemy.get_hit_box().w / 2,enemy.get_pos().y });
							enemy.set_speed(Vec2{ enemy.get_speed().x * -1,enemy.get_speed().y });


						}
					}
				}

			}
		}
	}

	
	for (auto& rock : rocks) {
		//岩と地面
		for (auto& b : blocks) {
			if (b.get_hit_box().intersects(rock.get_hit_box())) {
				double line_rate = 0.02;
				//yaguchiの下面は前のフレームではstageの上面より上にあった
				if (rock.get_pre_pos().y + rock.get_hit_box().h / 2 <= b.get_hit_box().y)
				{
					if (rock.get_hit_box().bottomY() >= b.get_hit_box().y)
					{


						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(rock.get_hit_box().x, b.get_hit_box().x);
						intersect_line_end = Min(rock.get_hit_box().rightX(), b.get_hit_box().rightX());

						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / rock.get_hit_box().w >= line_rate)
						{
							rock.set_pos(Vec2{ rock.get_pos().x,b.get_hit_box().y - 0.1 - rock.get_hit_box().h / 2 });
							rock.set_speed(Vec2{ rock.get_speed().x ,rock.get_speed().y * -b.get_e() });
							
						}
					}
				}




				//actorの上面は前のフレームではstageの下面より下にあった
				if (rock.get_pre_pos().y - rock.get_hit_box().h / 2 >= b.get_hit_box().bottomY())
				{

					if (rock.get_hit_box().y <= b.get_hit_box().bottomY())
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(rock.get_hit_box().x, b.get_hit_box().x);
						intersect_line_end = Min(rock.get_hit_box().rightX(), b.get_hit_box().rightX());

						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / rock.get_hit_box().w >= line_rate)
						{
							rock.set_pos(Vec2{ rock.get_pos().x,b.get_hit_box().bottomY() + 0.01 + rock.get_hit_box().h / 2 });
							rock.set_speed(Vec2{ rock.get_speed().x ,rock.get_speed().y * -b.get_e() });

						}
					}
				}



				//actorがstageの右にあたる

				//actorの左面は前のフレームではstageの右面より右にあった
				if (rock.get_pre_pos().x - rock.get_hit_box().w / 2 >= b.get_hit_box().rightX())
				{
					if (rock.get_hit_box().x <= b.get_hit_box().rightX())
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(rock.get_hit_box().y, b.get_hit_box().y);
						intersect_line_end = Min(rock.get_hit_box().bottomY(), b.get_hit_box().bottomY());


						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / rock.get_hit_box().h >= line_rate)
						{

							rock.set_pos(Vec2{ b.get_hit_box().rightX() + 0.01 + rock.get_hit_box().w / 2,rock.get_pos().y });
							rock.set_speed(Vec2{ rock.get_speed().x * -1,rock.get_speed().y });

						}
					}
				}


				//actorがstageの左にあたる
				//actorの右面は前のフレームではstageの左面より左にあった
				if (rock.get_pre_pos().x + rock.get_hit_box().w / 2 <= b.get_hit_box().x)
				{
					if (rock.get_hit_box().rightX() >= b.get_hit_box().x)
					{
						double intersect_line_begin;//交差している部分
						double intersect_line_end;//交差している部分

						intersect_line_begin = Max(rock.get_hit_box().y, b.get_hit_box().y);
						intersect_line_end = Min(rock.get_hit_box().bottomY(), b.get_hit_box().bottomY());


						double dis_begin_end = intersect_line_end - intersect_line_begin;
						if (dis_begin_end / rock.get_hit_box().h >= line_rate)
						{
							rock.set_pos(Vec2{ b.get_hit_box().x - 0.01 - rock.get_hit_box().w / 2,rock.get_pos().y });
							rock.set_speed(Vec2{ rock.get_speed().x * -1,rock.get_speed().y });


						}
					}
				}

			}
		}

		//岩と敵
		for (auto& enemy : enemies) {
			if (enemy.exist) {
				if (enemy.get_hit_box().intersects(rock.get_hit_box())) {
					effect.add<DeathEffect2>(enemy.get_pos(), enemy.get_index());
					enemy.exist = false;
				}
			}
		}
	}
}

void Play::reset()
{
	board_pos = scene_size / 2 - Vec2{ 0,160 };
	see_score = 0;
	true_score = 0;
	rocks.clear();
	rock_timer = 0;
	enemy_pop_timer = 0;
	enemy_pop_num = 1;
	enemies.clear();
	effect.clear();
	yaguchichanshots.clear();
	yaguchichan.reset();
}

