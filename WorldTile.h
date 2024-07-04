#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CWorldTile : public CGameObject
{
protected:
	float cellWidth;
	float cellHeight;
	int spriteId;
	int sceneID;
	int isPath;

public:
	CWorldTile(float x, float y, int isPath, int sceneID, int spriteId) :CGameObject(x, y)
	{
		this->cellWidth = 16;
		this->cellHeight = 16;
		this->spriteId = spriteId;
		this->sceneID = sceneID;
		this->isPath = isPath;
	}

	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsPath() { return isPath; }
	int GetSceneID() { return sceneID; }
};
