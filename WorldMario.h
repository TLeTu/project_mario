#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define WMARIO_BBOX_WIDTH 16
#define WMARIO_BBOX_HEIGHT 14

#define ID_ANI_WMARIO 1003100

class CWorldMario : public CGameObject
{
public:
	CWorldMario(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	virtual void Move(float mx, float my);
};

