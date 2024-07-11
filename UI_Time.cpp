#include "UI_Time.h"
#include "debug.h"
#include "Sprites.h"

void CUITime::Render()
{
	num1->Render();
	num2->Render();
	num3->Render();

}

void CUITime::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUITime::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
}

CUITime::CUITime(float x, float y)
{
	num1 = new CUINum(x + 8, y);
	num2 = new CUINum(x + 16, y);
	num3 = new CUINum(x + 24, y);

}

void CUITime::SetUIPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	this->num1->SetPosition(x + 8, y);
	this->num2->SetPosition(x + 16, y);
	this->num3->SetPosition(x + 24, y);

}
