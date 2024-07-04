#include "WorldTile.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"


void CWorldTile::Render()
{
	CSprites::GetInstance()->Get(this->spriteId)->Draw(x, y);
	/*RenderBoundingBox();*/
}

void CWorldTile::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth;
	b = t + this->cellHeight;
}