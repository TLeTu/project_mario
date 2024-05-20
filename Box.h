#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BOX 61000
#define ID_ANI_BOX_EMPTY 62000
#define BOX_WIDTH 16
#define BOX_BBOX_WIDTH 16
#define BOX_BBOX_HEIGHT 16

#define BOX_STATE_NORMAL 100
#define BOX_STATE_EMPTY 200
#define BOX_STATE_MUSHROOM 300

class CBox : public CGameObject {
protected:
	int type;
public:
	CBox(float x, float y, int type) : CGameObject(x, y) 
	{ 
		this->type = type;
		if (type == 0) {
			this->SetState(BOX_STATE_NORMAL);
		}
		else if (type == 1)
		{
			this->SetState(BOX_STATE_MUSHROOM);
		}
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);

	void SetState(int state);
};