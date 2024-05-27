#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "Collision.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "SceneLoader.h"
//#include "Koopas.h"

typedef CSceneLoader* LPSCENELOADER;

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> enemies;
	vector<LPSCENELOADER> sceneLoaders;

	int ScenePart = 0;
	float loadPositionX, loadPositionY;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void AddGameObject(LPGAMEOBJECT obj);
	virtual void GetPlayerPosition(float& x, float& y);
	virtual void SetScenePart(int part);

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;


