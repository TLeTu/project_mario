#include "CameraPoint.h"

CCameraPoint::CCameraPoint(float x, float y) : CGameObject(x, y)
{

}

void CCameraPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - CAMERAPOINT_BBOX_WIDTH / 2;
	t = y - CAMERAPOINT_BBOX_HEIGHT / 2;
	r = l + CAMERAPOINT_BBOX_WIDTH;
	b = t + CAMERAPOINT_BBOX_HEIGHT;
}

void CCameraPoint::Render()
{
	RenderBoundingBox();
}

void CCameraPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}