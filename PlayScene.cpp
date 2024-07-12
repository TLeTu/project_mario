#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include <sstream>
#include <string>
#include <map>


#include "UI_BG.h"
#include "UI_Number.h"
#include "UI_Time.h"
#include "UI_ClearText.h"
#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Box.h"
#include "Koopas.h"
#include "Brick.h"
#include "Goomba.h"
#include "Mario.h"
#include "EdgeDetector.h"
#include "Decoration.h"
#include "Floor.h"
#include "Mushroom.h"
#include "Collision.h"
#include "Piranha.h"
#include "Fireball.h"
#include "SceneLoader.h"
#include "Tube.h"
#include "Flytrap.h"
#include "Button.h"
#include "LuckyBox.h"
#include "CameraPoint.h"
#include "Wall.h"
#include "WorldMario.h"
#include "WorldTile.h"
#include "Leaf.h"
#include "UI.h"
#include "DeadFloor.h"


#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	UI = NULL;
	loadPositionX = 0;
	loadPositionY = 0;
	ScenePart = 0;
	endStart = -1;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;
	CGameObject* enemy = NULL;
	CGameObject* coinBrick = NULL;
	CWorldTile* tile = NULL;
	CSceneLoader* sceneLoader = NULL;
	CCameraPoint* cameraPoint = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		int mtype = atoi(tokens[3].c_str());
		if (mtype)
		{
			obj = new CMario(x, y);
			player = (CMario*)obj;
		}
		else
		{
			obj = new CWorldMario(x, y);
			player = (CWorldMario*)obj;
		}

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GOOMBA:
	{
		int type = atoi(tokens[3].c_str());
		enemy = new CGoomba(x, y, type);
		break;
	}
	case OBJECT_TYPE_BRICK:
	{
		int type = atoi(tokens[3].c_str());
		int isCoin = atoi(tokens[4].c_str());
		if (isCoin)
		{
			coinBrick = new CBrick(x, y, type, isCoin);
		}
		else obj = new CBrick(x, y, type, isCoin); 
		break;
	}
	case OBJECT_TYPE_LUCKYBOX:
	{
		obj = new CLuckyBox(x, y);
		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_KOOPAS:
	{
		int type = atoi(tokens[3].c_str());
		int color = atoi(tokens[4].c_str());
		enemy = new CKoopas(x, y, type, color);
		break;

	}
	case OBJECT_TYPE_EDGEDETECTOR: enemy = new CEdgeDetector(x, y); break;
	case OBJECT_TYPE_MUSHROOM: obj = new CMushroom(x, y); break;
	case OBJECT_TYPE_FLYTRAP: enemy = new CFlytrap(x, y); break;
	case OBJECT_TYPE_PIRANHA:
	{
		int type = atoi(tokens[3].c_str());
		enemy = new CPiranha(x, y, type);
		break;

	}
	case OBJECT_TYPE_FIREBALL: enemy = new CFireball(x, y); break;
	case OBJECT_TYPE_DECORATION: 
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CDecoration(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_TUBE:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CTube(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	case OBJECT_TYPE_UI:
	{
		if (UI != NULL)
		{
			DebugOut(L"[ERROR] UI object was created before!\n");
			return;
		}
		UI = new CUI(x, y);

		DebugOut(L"[INFO] UI object has been created!\n");
		break;
	}

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}
	case OBJECT_TYPE_WALL:
	{
		
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CWall(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_WORLD_TILE:
	{
		int isPath = atoi(tokens[3].c_str());
		int sceneId = atoi(tokens[4].c_str());
		int spriteId = atoi(tokens[5].c_str());

		tile = new CWorldTile(
			x, y, isPath, sceneId, spriteId
		);

		break;
	}
	
	case OBJECT_TYPE_FLOOR:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CFloor(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	}

	case OBJECT_TYPE_DEADFLOOR:
	{
		
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CDeadFloor(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		break;
	
	}

	case OBJECT_TYPE_PORTAL:
	{
		int scene_id = atoi(tokens[3].c_str());
		float px = (float)atof(tokens[4].c_str());
		float py = (float)atof(tokens[5].c_str());
		obj = new CPortal(x, y, scene_id, px, py);
		break;
	}
	case OBJECT_TYPE_BOX: 
	{
		int type = atoi(tokens[3].c_str());
		obj = new CBox(x, y, type);
		break;
	}
	case OBJECT_TYPE_SCENELOADER:
	{
		int part = atoi(tokens[3].c_str());
		sceneLoader = new CSceneLoader(x, y, part);
		break;
	}
	case OBJECT_TYPE_BUTTON: obj = new CButton(x, y); break;
	case OBJECT_TYPE_CAMERAPOINT:
	{

		cameraPoint = new CCameraPoint(x, y);
		break;
	}


	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	};

	// General object setup
	if (obj != NULL) 
	{
		obj->SetPosition(x, y);
		objects.push_back(obj);
	}
	if (sceneLoader != NULL)
	{
		sceneLoader->SetPosition(x, y);
		sceneLoaders.push_back(sceneLoader);
	}
	if (enemy != NULL)
	{
		enemy->SetPosition(x, y);
		enemies.push_back(enemy);
	}
	if (coinBrick != NULL)
	{
		coinBrick->SetPosition(x, y);
		coinBricks.push_back(coinBrick);
	}
	if (cameraPoint != NULL)
	{
		cameraPoint->SetPosition(x, y);
		cameraPoints.push_back(cameraPoint);
	}
	if (tile != NULL)
	{
		tile->SetPosition(x, y);
		tiles.push_back(tile);
	}
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();
	LoadSaveFile();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (auto object : objects)
	{
		if (object != player)
		{
			coObjects.push_back(object);
		}
	}

	for (size_t i = 0; i < sceneLoaders.size(); i++)
	{
		coObjects.push_back(sceneLoaders[i]);
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		coObjects.push_back(enemies[i]);
	}

	for (size_t i = 0; i < coinBricks.size(); i++)
	{
		coObjects.push_back(coinBricks[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < coinBricks.size(); i++)
	{
		coinBricks[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < sceneLoaders.size(); i++)
	{
		if (sceneLoaders[i]->GetPart() == ScenePart + 1)
		{
			sceneLoaders[i]->GetPosition(loadPositionX, loadPositionY);
		}
	}
	for (size_t i = 0; i < enemies.size(); i++)
	{
		float ox, oy;
		enemies[i]->GetPosition(ox, oy);
		if (ox <= loadPositionX)
		{
			enemies[i]->Update(dt, &coObjects);
		}
	}
	for (size_t i = 0; i < cameraPoints.size(); i++)
	{
		cameraPoints[i]->Update(dt, &coObjects);
	}


	if (player == NULL) return;

	UI->Update(dt, &coObjects);

	if (player->GetState() == MARIO_STATE_END)
	{
			if (GetTickCount64() - endStart > 5000)
			{
				CGame::GetInstance()->GameEnd();
			}
	}
	
	UpdateCameraPosition();

	UpdateUIPosition();

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (size_t i = 0; i < sceneLoaders.size(); i++)
	{
		sceneLoaders[i]->Render();
	}
	for (size_t i = 0; i < tiles.size(); i++)
	{
		tiles[i]->Render();
	}

	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->GetType() == "piranha")
		{
			enemies[i]->Render();
		}
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}

	for (size_t i = 0; i < coinBricks.size(); i++)
	{
		coinBricks[i]->Render();
	}


	for (size_t i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->GetType() != "piranha")
		{
			enemies[i]->Render();
		}
	}

	for (size_t i = 0; i < cameraPoints.size(); i++)
	{
		cameraPoints[i]->Render();
	}

	UI->Render();

}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();

	for (it = coinBricks.begin(); it != coinBricks.end(); it++)
	{
		delete (*it);
	}
	coinBricks.clear();

	for (it = enemies.begin(); it != enemies.end(); it++)
	{
		delete (*it);
	}
	enemies.clear();

	vector<LPSCENELOADER>::iterator lt;
	for (lt = sceneLoaders.begin(); lt != sceneLoaders.end(); it++)
	{
		delete (*lt);
	}
	sceneLoaders.clear();

	for (it = cameraPoints.begin(); it != cameraPoints.end(); it++)
	{
		delete (*it);
	}

	cameraPoints.clear();

	vector<LPWORLDTILE>::iterator wt;
	for (wt = tiles.begin(); wt != tiles.end(); wt++)
	{
		delete (*wt);
	}

	tiles.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{

	SaveFile();
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	for (int i = 0; i < enemies.size(); i++)
		delete enemies[i];

	enemies.clear();

	for (int i = 0; i < coinBricks.size(); i++)
		delete coinBricks[i];

	coinBricks.clear();

	for (int i = 0; i < sceneLoaders.size(); i++)
		delete sceneLoaders[i];

	sceneLoaders.clear();

	for (int i = 0; i < cameraPoints.size(); i++)
		delete cameraPoints[i];

	cameraPoints.clear();

	for (int i = 0; i < tiles.size(); i++)
		delete tiles[i];

	tiles.clear();


	delete UI;
	UI = NULL;
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	for (it = coinBricks.begin(); it != coinBricks.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	for (it = enemies.begin(); it != enemies.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	vector<LPSCENELOADER>::iterator lt;
	for (lt = sceneLoaders.begin(); lt != sceneLoaders.end(); lt++)
	{
		LPSCENELOADER o = *lt;
		if (o->IsDeleted())
		{
			delete o;
			*lt = NULL;
		}
	}

	for (it = cameraPoints.begin(); it != cameraPoints.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	vector<LPWORLDTILE>::iterator wt;
	for (wt = tiles.begin(); wt != tiles.end(); wt++)
	{
		LPWORLDTILE o = *wt;
		if (o->IsDeleted())
		{
			delete o;
			*wt = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());

	coinBricks.erase(
		std::remove_if(coinBricks.begin(), coinBricks.end(), CPlayScene::IsGameObjectDeleted),
		coinBricks.end());


	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(), CPlayScene::IsGameObjectDeleted),
		enemies.end());

	sceneLoaders.erase(
		std::remove_if(sceneLoaders.begin(), sceneLoaders.end(), CPlayScene::IsGameObjectDeleted),
		sceneLoaders.end());

	cameraPoints.erase(
		std::remove_if(cameraPoints.begin(), cameraPoints.end(), CPlayScene::IsGameObjectDeleted),
		cameraPoints.end());

	tiles.erase(
		std::remove_if(tiles.begin(), tiles.end(), CPlayScene::IsGameObjectDeleted),
		tiles.end());

}

void CPlayScene::AddGameObject(LPGAMEOBJECT obj)
{
	objects.push_back(obj);
}

void CPlayScene::GetPlayerPosition(float& x, float& y)
{
	player->GetPosition(x, y);
}

void CPlayScene::SetScenePart(int part)
{
	ScenePart = part;
	Render();
}

void CPlayScene::BrickToCoin()
{
	float bx, by;
	if (coinBricks.size() > 0)
	{
		for (int i = 0; i < coinBricks.size(); i++)
		{
			coinBricks[i]->GetPosition(bx, by);
			LPGAMEOBJECT coin = new CCoin(bx, by);
			coinBricks[i]->Delete();
			coin->SetState(COIN_STATE_TOBRICK);
			AddGameObject(coin);
		}
	}
	else return;
}

void CPlayScene::CoinToBrick(LPGAMEOBJECT obj)
{
	coinBricks.push_back(obj);
}

LPGAMEOBJECT CPlayScene::GetEnemiesInRange(float x, float y)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		float ex, ey;
		enemies[i]->GetPosition(ex, ey);
		if (abs(ex - x) <= 8 && abs(ey - y) <= 8)
		{
			return enemies[i];
		}
	}
	return NULL;
}

LPGAMEOBJECT CPlayScene::GetBrickInRange(float x, float y)
{
	for (int i = 0; i < coinBricks.size(); i++)
	{
		float bx, by;
		coinBricks[i]->GetPosition(bx, by);
		if (abs(bx - x) <= 8 && abs(by - y) <= 8)
		{
			return coinBricks[i];
		}
	}
	return NULL;
}

void CPlayScene::SetPlayerPosition(float x, float y)
{
	player->SetPosition(x, y);
}

bool CPlayScene::CheckTile(float mx, float my)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		float tx, ty;
		tiles[i]->GetPosition(tx, ty);
		if (tx == mx && ty == my)
		{
			return tiles[i]->IsPath();
		}
	}
	return false;
}

int CPlayScene::GetTileId(float x, float y)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		float tx, ty;
		tiles[i]->GetPosition(tx, ty);
		if (tx == x && ty == y)
		{
			return tiles[i]->GetSceneID();
		}
	}
	return -1;
}

void CPlayScene::UpdateCameraPosition()
{
	if (CGame::GetInstance()->GetCurrentSceneId() != 1)
	{
		float cx, cy;
		float cmx, _;
		if (cameraPoints.size() != 0)
		{
			cameraPoints[0]->GetPosition(cmx, _);
		}
		else cmx = -1;

		CGame* game = CGame::GetInstance();
		player->GetPosition(cx, cy);
		if (cx < cmx || cmx == -1)
		{
			cx -= game->GetBackBufferWidth() / 2;
			if (cx < 0) cx = 0;
			cy -= game->GetBackBufferHeight() / 4;
			if (cy > 0) cy = 0;
			CGame::GetInstance()->SetCamPos(cx, cy);
		}
	}
	else CGame::GetInstance()->SetCamPos(0, 0);
}

void CPlayScene::UpdateUIPosition()
{
	if (UI != NULL)
	{
		float cx, cy;
		CGame::GetInstance()->GetCamPos(cx, cy);

		UI->SetUIPosition(cx + 108, cy + 220);
	}
}

void CPlayScene::LoadSaveFile()
{
	if (UI == NULL) return;
	if (player == NULL) return;
	ifstream f;
	f.open(L"savefile.txt");

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		if (line[0] == '#') continue;
		SetSaveValue(line);
	}

	f.close();
}

void CPlayScene::SetSaveValue(string line)
{
		vector<string> tokens = split(line);
	
		if (tokens.size() < 1) return;
	
		int type = atoi(tokens[0].c_str());
		int value = (int)atof(tokens[1].c_str());
	
		switch (type)
		{
		case 0:
			UI->SetScore(value);
			break;
		case 1:
			UI->SetMoney(value);
			break;
		case 2:
			UI->SetLife(value);
			break;
		case 3:
			if (value == 100) break;
			UI->SetCardSlot(1, value);
			break;
		case 4:
			if (value == 100) break;
			UI->SetCardSlot(2, value);
			break;
		case 5:
			if (value == 100) break;
			UI->SetCardSlot(3, value);
			break;
		case 6:
			if (CGame::GetInstance()->GetCurrentSceneId() != 1)
				((CMario*)player)->SetLevel(value);
			break;
		}
}

void CPlayScene::SaveFile()
{
	if (UI == NULL) return;
	if (player == NULL) return;

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	std::map<int, int> saveData;
	std::ifstream inputFile(L"savefile.txt");

	if (inputFile.is_open()) {
		std::string line;
		while (std::getline(inputFile, line)) {
			if (line[0] == '#') continue; // Skip comments
			std::istringstream iss(line);
			int type, value;
			if (!(iss >> type >> value)) { break; } // Error
			saveData[type] = value;
		}
		inputFile.close();
	}

	// Update the necessary values
	saveData[0] = UI->GetSore();
	saveData[1] = UI->GetMoney();
	saveData[2] = UI->GetLife();
	saveData[3] = UI->GetCardSlot(1);
	saveData[4] = UI->GetCardSlot(2);
	saveData[5] = UI->GetCardSlot(3);
	if (CGame::GetInstance()->GetCurrentSceneId() != 1)
		saveData[6] = ((CMario*)player)->GetLevel();

	// Write the updated data back to the file
	std::ofstream outputFile(L"savefile.txt");
	outputFile.clear();
	outputFile << "# Save file\n";
	outputFile << "# Type Value\n";
	outputFile << "# 0: score, 1: money, 2: lives, 3: card1, 4: card2, 5: card3, 6: mariolevel\n";

	for (const auto& pair : saveData) {
		outputFile << pair.first << "\t" << pair.second << "\n";
	}

	outputFile.close();
}

void CPlayScene::GameEnd(float lx, float ly, int cardType)
{
	endStart = GetTickCount64();
	CUIClearText* clearText = new CUIClearText(lx, ly - 50);
	clearText->SetCard(cardType);
	clearText->SetCardSlotPosition(lx +80, ly - 50);
	AddGameObject(clearText);
}