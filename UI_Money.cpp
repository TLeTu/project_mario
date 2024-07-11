#include "UI_Money.h"
#include "debug.h"
#include "Sprites.h"

void CUIMoney::Render()
{
	num1->Render();
	num2->Render();
}

void CUIMoney::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUIMoney::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
}

CUIMoney::CUIMoney(float x, float y)
{
	num1 = new CUINum(x + 8, y);
	num2 = new CUINum(x + 16, y);

}

void CUIMoney::SetUIPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	this->num1->SetPosition(x + 8, y);
	this->num2->SetPosition(x + 16, y);
}
