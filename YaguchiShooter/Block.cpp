#include "stdafx.h"
#include "Block.h"
#include"Play.h"

void Block::update()
{
}

void Block::draw()
{
	switch (nature)
	{
	case normal:
		hit_box.draw(Palette::Brown);
		for (int i = 0; i < tex_num; i++) {
			Vec2 tex_pos = hit_box.pos + Vec2{ 50 * i , 0 };

			TextureAsset(U"normal").draw(tex_pos);

		}
		break;
	case slime:
		for (int i = 0; i < tex_num; i++) {
			Vec2 tex_pos = hit_box.pos + Vec2{ 50 * i , 0 };

			TextureAsset(U"slime").draw(tex_pos);

		}
		break;
	default:
		break;
	}
	
}
