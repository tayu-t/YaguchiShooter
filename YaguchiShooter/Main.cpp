# include <Siv3D.hpp> // Siv3D v0.6.12
#include"Play.h"




void Main()
{
	FontAsset::Register(U"1", 40, Typeface::Icon_Awesome_Brand);
	FontAsset::Register(U"2", 50, Typeface::Mplus_Bold);

	TextureAsset::Register(U"yaguchi", Resource(U"yaguchi.png"));
	TextureAsset::Register(U"yaguchichan", Resource(U"yaguchichan.png"));
	TextureAsset::Register(U"throweffect", Resource(U"throweffect.png"));

	TextureAsset::Register(U"yaguchichanshot", Resource(U"yaguchichan_shot.png"));//20*32
	TextureAsset::Register(U"normal", Resource(U"normal.png"));
	TextureAsset::Register(U"board", Resource(U"board.png"));
	TextureAsset::Register(U"sweat", Resource(U"sweat.png"));
	TextureAsset::Register(U"hp", Resource(U"hp.png"));
	TextureAsset::Register(U"emptyhp", Resource(U"emptyhp.png"));
	TextureAsset::Register(U"circle", Resource(U"circle.png"));
	TextureAsset::Register(U"rock", Resource(U"rock.png"));
	


	Play play;


	while (System::Update())
	{
		play.update();
		play.draw();
	}
}
