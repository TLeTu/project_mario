#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define WMARIO_BBOX_WIDTH 16
#define WMARIO_BBOX_HEIGHT 14

#define ID_ANI_WMARIO 1003100

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACOON	3

class CWorldMario : public CGameObject
{
protected:
	int sceneId;
	int level;
public:
	CWorldMario(float x, float y);
	void SetLevel(int l)
	{
		level = l;
	};
	int GetLevel() { return level; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	virtual void Move(float mx, float my);
	virtual bool CheckMoveDirection(float mx, float my);
	virtual void SetSceneId(int sceneId) { this->sceneId = sceneId; }
	virtual int GetSceneId() { return sceneId; }
};

