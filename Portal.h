#pragma once

#include "GameObject.h"

#define PORTAL_BBOX_WIDTH 32
#define PORTAL_BBOX_HEIGHT 16

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

public:
	CPortal(float x, float y, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }

	int GetSceneId() { return scene_id;  }
};