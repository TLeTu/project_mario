#include "SceneLoader.h"
#include "debug.h"

#include "Collision.h"

#include <cmath>


CSceneLoader::CSceneLoader(float x, float y, int part) :CGameObject(x, y)
{
	vx = 0;
	vy = 0;
	this->part = part;
}

void CSceneLoader::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x - SCENELOADER_BBOX_WIDTH / 2;
	top = y - SCENELOADER_BBOX_HEIGHT / 2;
	right = left + SCENELOADER_BBOX_WIDTH;
	bottom = top + SCENELOADER_BBOX_HEIGHT;
}


void CSceneLoader::Render()
{
	//RenderBoundingBox();
}
