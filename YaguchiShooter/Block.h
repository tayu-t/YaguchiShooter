#pragma once

class Play;

enum Block_Nature
{
	normal,
	slime,


	num,
};

class Block
{
private:
	Vec2 pos{ 0,0 };
	double width{ 80 };
	double height{ 20 };
	RectF hit_box;
	Play* play;
	Block_Nature nature;
	double e{0.1};
	int tex_num{0};
public:

	Block(Play* play,const RectF& r,Block_Nature n) : play(play),nature(n){
		hit_box = r;
		pos = hit_box.center();
		width = hit_box.w;
		tex_num = width / 50;
		height = hit_box.h;
		switch (n)
		{
		case normal:
			e = 0;
			break;
		case slime:
			e = 0.8;
			break;
		default:
			break;
		}
	}

	void update();
	void draw();

	const double get_e()noexcept { return e; }
	inline RectF& get_hit_box() noexcept { return hit_box; }

};

