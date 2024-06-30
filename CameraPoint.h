#pragma once

#include "GameObject.h"

#define CAMERAPOINT_BBOX_WIDTH 16
#define CAMERAPOINT_BBOX_HEIGHT 16


class CCameraPoint : public CGameObject
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);


	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

public:
	virtual void Render();
	CCameraPoint(float x, float y);
};