#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>
#include "button.h"
using namespace sf;

#define GRIDSIZE 25
#define LVL1_WIDTH 9
#define LVL1_HEIGHT 9
#define LVL1_NUM 10
#define LVL2_WIDTH 16
#define LVL2_HEIGHT 16
#define LVL2_NUM 40
#define LVL3_WIDTH 30
#define LVL3_HEIGHT 16
#define LVL3_NUM 99

typedef enum GRIDSTATE
{
	ncNULL,
	ncUNDOWN,
	ncMINE,
	ncONE,
	ncTWO,
	ncTHREE,
	ncFOUR,
	ncFIVE,
	ncSIX,
	ncSEVEN,
	ncEIGHT,
	ncFLAG,
	ncQ,
	ncX,
	ncBOMBING,
	ncUNFOUND,
};

typedef enum GAMEOVERSTATE
{
	ncNO,
	ncWIN,
	ncLOSE
};

typedef enum MOUSECLICK
{
	mNULL,
	mL,
	mR,
	mPRESSING,
	mEASY,
	mNORMAL,
	mHARD,
	mSKIN,
	mBG,
	mRESTART,
	mQUIT
};

class BLOCK
{
public:
	int mState;
	int mStateBackUp;
	bool isPress;
};


class Game
{
public:
	Game();
	~Game();

	RenderWindow window;
	bool gameOver, gameQuit;

	int window_width, window_height, stageWidth, stageHeight,
		gridSize, mMineNum, mFlagCalc;
	int gamelvl, mTime;
	int imgBGNo, imgSkinNo;
	int mLcounter;
	bool isLRFlag;
	int Volume;

	BLOCK mGameData[LVL3_HEIGHT][LVL3_WIDTH];
	bool isGameBegin;
	int isGameOverState;
	Vector2i mCornPoint, mGridPoint;
	MOUSECLICK mouseFlag;

	Texture tBackground, tTiles,
		tNum, tTimer, tCounter, tGameOver;

	Sprite sBackground, sTiles,
		sNum, sTimer, sCounter, sGameOver;

	RectButton btnEasy, btnNormal, btnHard, 
		btnBG, btnSkin, btnRestart, btnQuit;

	SoundBuffer sbWin, sbBoom;
	Sound soundWin, soundBoom;
	Music bkMusic;

	Clock gameClock, mouseClickTimer;

	void Run();

	void Initial();
	void IniData();
	void LoadMediaData();
	void PartialReload();
	void MineSet(int Py, int Px, bool _Lbutton);

	void Input();
	void LButtonDown();
	void RButtonDown();
	void Pressing();
	void unCover();
	void undownOpen();
	void NullClick(int j, int i);
	Vector2i getMouseGrid();

	void Logic();
	void isWin();

	void Draw();
	void DrawGrid();
	void DrawButton();
	void DrawScore();
	void DrawTimer();
	void DrawGameEnd();
};

