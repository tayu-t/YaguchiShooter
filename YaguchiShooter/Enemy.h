#pragma once

class Play;


class Enemy
{
private:
	Vec2 pre_pos;
	Vec2 pos{0,0};
	int hp{ 1 };

	RectF hit_box{ 0,0 };
	Vec2 speed{ 0,0 };
	double width{ 30 };
	double height{ 50 };
	int index{1};
	double gravity{1090};
	bool grounded{ false };
	Play* play;
public:
	bool exist = true;
	double score = 0;

	Enemy(Play* play,int index, Vec2 pos);
	void update();
	void update1();
	void update2();
	void draw();
	void damage();


	inline RectF& get_hit_box() noexcept { return hit_box; }
	inline const Vec2 get_pre_pos() noexcept { return pre_pos; }
	inline const Vec2 get_pos() noexcept { return pos; }
	inline void set_pos(Vec2 t)noexcept {
		pos = t;
		hit_box = RectF(Arg::center(pos), width, height);
	}

	inline void set_speed(Vec2 t) noexcept { speed = t; }
	inline Vec2 get_speed()noexcept { return speed; }
	inline int get_index()noexcept { return index; }
	inline bool& get_grounded() noexcept { return grounded; }
};



struct DeathEffect : IEffect
{
	Vec2 pos;
	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit DeathEffect(const Vec2& pos)
		:pos(pos) {}

	bool update(double t) override
	{
		t += Scene::DeltaTime();
		double a = 0.3 + (0.8 - t) / 0.8;

		TextureAsset(U"circle").scaled(4 * EaseOutExpo(t)).drawAt(pos, ColorF(1, a));
		return (t < 0.8);
	}
};

struct DeathEffect2 : IEffect
{
	Vec2 pos;
	Vec2 speed{ 5,-200 };
	int index;
	double theta{0};
	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit DeathEffect2(const Vec2& pos,int index)
		:pos(pos),index(index) {
		speed.x = Random(-10, 10);
	}

	bool update(double t) override
	{
		speed.y += 980 * Scene::DeltaTime();
		pos += speed * Scene::DeltaTime();
		theta += speed.x * Scene::DeltaTime();
		ScopedRenderStates2D sanpra{ SamplerState::ClampNearest };

		if (index == 3) {
			TextureAsset(U"yaguchi").scaled(0.3).rotated(theta).drawAt(pos);
		}
		else {
			TextureAsset(U"yaguchi").scaled(0.20).rotated(theta).drawAt(pos);
		}

		return (pos.y < Scene::Size().y + 100);
	}
};



