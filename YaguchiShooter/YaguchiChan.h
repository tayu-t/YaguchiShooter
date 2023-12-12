#pragma once

class Play;
class YaguchiChan;

struct YaguchiChanShot
{
	Vec2 pos;
	double speed;
	bool exist{ true };
	static constexpr double width{ 20 };
	static constexpr double height{ 32 };

};



class YaguchiChan
{
private:
	int direction{ 1 };//→:1←:-1
	Vec2 pre_pos{ 0,0 };
	Vec2 pos{ 0,100 };
	double key_acc{ 700 };
	Vec2 speed{ 0,0 };
	double x_speed_max{ 280 };
	int start_hp{ 5 };


	int hp{ 10 };
	const double muteki_time = 1.5;
	double muteki_timer = 0;
	static constexpr double scale{ 1.2 };
	double height{ 45*scale };
	double width{ 30*scale };
	RectF hit_box;
	Play* play;
	double gravity{1090 };
	bool grounded{ false };
	bool finish{ false };
public:
	YaguchiChan(Play* play) :play(play)
	{
	};
	void reset() {
		pos = Vec2{ Scene::Size().x / 2, 434 };
		hit_box = RectF(Arg::center(pos), width, height);
		hp = start_hp;
		speed = Vec2{ 0,0 };
		grounded = false;
		muteki_timer = 0;
		direction = 1;
		finish = false;
	}

	double get_muteki_timer()noexcept { return muteki_timer; }
	void update();
	
	void update_key_move();
	void draw();
	void damage(int d = 1);
	bool get_finish() { return finish; }
	int get_hp() { return hp; }

	inline RectF& get_hit_box() noexcept { return hit_box; }
	inline const Vec2 get_pre_pos() noexcept { return pre_pos; }
	inline const Vec2 get_pos() noexcept { return pos; }
	inline void set_pos(Vec2 t)noexcept {
		pos = t;
		hit_box = RectF(Arg::center(pos), width, height);
	}

	inline void set_speed(Vec2 t) noexcept { speed = t; }
	inline Vec2 get_speed()noexcept { return speed; }

	inline bool& get_grounded() noexcept { return grounded; }
};


struct ThrowEffect : IEffect
{
	Vec2 pos;
	int direction;
	YaguchiChan* yaguchichan;
	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit ThrowEffect(YaguchiChan* yaguchichan, int direciton)
		:yaguchichan(yaguchichan)
		, direction{ direciton } {}

	bool update(double t) override
	{
		pos = yaguchichan->get_pos() + Vec2{ direction * yaguchichan->get_hit_box().w / 2 ,0 } + direction * Vec2{ 9,0 };
		t += Scene::DeltaTime();
		double a = 0.3+(0.5 - t) / 0.5;
		TextureAsset(U"throweffect").scaled(1.4).mirrored(direction == -1).drawAt(pos, ColorF(1, a));
		return (t < 0.5);
	}
};

struct SweatEffect : IEffect
{
	Vec2 pos;
	int direction;
	YaguchiChan* yaguchichan;
	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit SweatEffect(YaguchiChan* yaguchichan, int direciton)
		:yaguchichan(yaguchichan)
		, direction{ direciton } {}

	bool update(double t) override
	{
		pos = yaguchichan->get_pos() + Vec2{ -direction * yaguchichan->get_hit_box().w / 2 ,-40 } + direction * Vec2{ -9,0 };
		t += Scene::DeltaTime();
		double a =  (1.3 - t) / 1.3;
		TextureAsset(U"sweat").scaled(1).mirrored(direction == -1).drawAt(pos, ColorF(1, a));
		TextureAsset(U"sweat").scaled(1).mirrored(direction == -1).drawAt(pos + Vec2{15,0}, ColorF(1, a));
		return (t < 1.3);
	}
};


