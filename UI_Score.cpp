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

void CUIScore::SetScore(int score)
{
	int num1 = score / 1000000;
	int num2 = (score % 1000000) / 100000;
	int num3 = (score % 100000) / 10000;
	int num4 = (score % 10000) / 1000;
	int num5 = (score % 1000) / 100;
	int num6 = (score % 100) / 10;
	int num7 = score % 10;

	this->num1->SetState(num1);
	this->num2->SetState(num2);
	this->num3->SetState(num3);
	this->num4->SetState(num4);
	this->num5->SetState(num5);
	this->num6->SetState(num6);
	this->num7->SetState(num7);
}