#include "UI_Score.h"
#include "debug.h"
#include "Sprites.h"

void CUIScore::Render()
{
	num1->Render();
	num2->Render();
	num3->Render();
	num4->Render();
	num5->Render();
	num6->Render();
	num7->Render();
}

void CUIScore::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x;
	bottom = y;
}

void CUIScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
}

CUIScore::CUIScore(float x, float y)
{
	num1 = new CUINum(x + 8, y);
	num2 = new CUINum(x + 16, y);
	num3 = new CUINum(x + 24, y);
	num4 = new CUINum(x + 32, y);
	num5 = new CUINum(x + 40, y);
	num6 = new CUINum(x + 48, y);
	num7 = new CUINum(x + 56, y);

}

void CUIScore::SetUIPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	this->num1->SetPosition(x + 8, y);
	this->num2->SetPosition(x + 16, y);
	this->num3->SetPosition(x + 24, y);
	this->num4->SetPosition(x + 32, y);
	this->num5->SetPosition(x + 40, y);
	this->num6->SetPosition(x + 48, y);
	this->num7->SetPosition(x + 56, y);

}
