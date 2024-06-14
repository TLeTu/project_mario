#include "Button.h"

CButton::CButton(float x, float y) : CGameObject(x, y)
{
	SetState(BUTTON_STATE_NORMAL);
}

void CButton::Render()
{
	int ani_id = -1;
	if (GetState() == BUTTON_STATE_NORMAL)
	{
		ani_id = ID_ANI_BUTTON;
	}
	else ani_id = ID_ANI_PRESSED;
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ani_id)->Render(x, y);
	//RenderBoundingBox();
}

void CButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == BUTTON_STATE_PRESSED)
	{
		l = x - BUTTON_BBOX_WIDTH / 2;
		t = y - BUTTON_BBOX_HEIGHT_PRESSED / 2;
		r = l + BUTTON_BBOX_WIDTH;
		b = t + BUTTON_BBOX_HEIGHT_PRESSED;
	}
	else
	{
		l = x - BUTTON_BBOX_WIDTH / 2;
		t = y - BUTTON_BBOX_HEIGHT / 2;
		r = l + BUTTON_BBOX_WIDTH;
		b = t + BUTTON_BBOX_HEIGHT;
	}
}

void CButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CButton::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BUTTON_STATE_NORMAL:
		return;
	case BUTTON_STATE_PRESSED:
		return;
	}
}
