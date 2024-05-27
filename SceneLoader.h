#pragma once

#include "GameObject.h"

#define SCENELOADER_BBOX_WIDTH 16
#define SCENELOADER_BBOX_HEIGHT 500


class CSceneLoader : public CGameObject
{
protected:
	int part;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};


	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

public:

	int GetPart() { return this->part; };
	virtual void Render();
	CSceneLoader(float x, float y, int part);
};