#include "WorldMario.h"

CWorldMario::CWorldMario(float x, float y) : CGameObject()
{
}

void CWorldMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CWorldMario::Render()
{
	CAnimations::GetInstance()->Get(100310)->Render(x, y);
}

void CWorldMario::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - WMARIO_BBOX_WIDTH/2;
	t = y - WMARIO_BBOX_HEIGHT;
	r = l + WMARIO_BBOX_WIDTH;
	b = t + WMARIO_BBOX_HEIGHT;
}

void CWorldMario::Move(float mx, float my)
{
	x += mx;
	y += my;
}

bool CWorldMario::CheckMoveDirection(float mx, float my)
{
	return CGame::GetInstance()->GetCurrentScene()->CheckTile(x + mx, y + my);
}