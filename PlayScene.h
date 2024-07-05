#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "Collision.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "WorldTile.h"
#include "SceneLoader.h"
//#include "Koopas.h"

typedef CSceneLoader* LPSCENELOADER;
typedef CWorldTile* LPWORLDTILE;

class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> enemies;
	vector<LPSCENELOADER> sceneLoaders;
	vector<LPGAMEOBJECT> coinBricks;
	vector<LPGAMEOBJECT> cameraPoints;
	vector<LPWORLDTILE> tiles;

	int ScenePart;
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
	virtual void BrickToCoin();
	virtual void CoinToBrick(LPGAMEOBJECT obj);
	virtual void GetPlayerPosition(float& x, float& y);
	virtual void SetPlayerPosition(float x, float y);
	virtual void SetScenePart(int part);
	virtual void ResetScenePart() { ScenePart = 0; }
	virtual LPGAMEOBJECT GetEnemiesInRange(float x, float y);

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	virtual bool CheckTile(float mx, float my);
	virtual int GetTileId(float x, float y);
};

typedef CPlayScene* LPPLAYSCENE;


