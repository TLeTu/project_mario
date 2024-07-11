#include "UI_Life.h"
#include "debug.h"
#include "Sprites.h"

void CUILife::Render()
{
	num1->Render();
	num2->Render();
}

void CUILife::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUILife::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
}

CUILife::CUILife(float x, float y)
{
	num1 = new CUINum(x + 8, y);
	num2 = new CUINum(x + 16, y);

}

void CUILife::SetUIPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	this->num1->SetPosition(x + 8, y);
	this->num2->SetPosition(x + 16, y);
}

void CUILife::SetLife(int life)
{
	int num1 = life / 10;
	int num2 = life % 10;

	this->num1->SetState(num1);
	this->num2->SetState(num2);
}

