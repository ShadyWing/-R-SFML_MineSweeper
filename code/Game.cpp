#pragma once
#include"Game.h"
using std::cout;
using std::endl;

Game::Game()
{
	gridSize = GRIDSIZE;

	gameOver = false;
	gameQuit = false;
	gamelvl = 2;
	imgBGNo = 1;
	imgSkinNo = 1;
	Volume = 50;
	srand(time(NULL));

	window_width = 860;
	window_height = 600;
	window.create(sf::VideoMode(window_width, window_height), L"SFML扫雷 by PJJ");
	window.setFramerateLimit(60);

	VideoMode mode = VideoMode::getDesktopMode();
	Vector2i p = { 0,0 };
	p.x = mode.width / 2 - window.getSize().x / 2;
	p.y = mode.height / 2 - window.getSize().y / 2;
	window.setPosition(p);
}

Game::~Game(){}


void Game::Run()
{
	LoadMediaData();

	while (window.isOpen() && !gameQuit)
	{
		Initial();

		bkMusic.play();
		bkMusic.setLoop(true);

		while (window.isOpen() && !gameOver)
		{
			Input();

			Logic();

			Draw();
		}
	}
}

void Game::Initial()
{
	switch (gamelvl)
	{
	case 1:
		stageWidth = LVL1_WIDTH;
		stageHeight = LVL1_HEIGHT;
		mMineNum = LVL1_NUM;
		break;
	case 2:
		stageWidth = LVL2_WIDTH;
		stageHeight = LVL2_HEIGHT;
		mMineNum = LVL2_NUM;
		break;
	case 3:
		stageWidth = LVL3_WIDTH;
		stageHeight = LVL3_HEIGHT;
		mMineNum = LVL3_NUM;
		break;
	default:
		break;
	}

	isGameOverState = ncNO;
	mFlagCalc = 0;
	isGameBegin = false;
	mTime = 0;
	mouseFlag = mNULL;
	mLcounter = 0;
	isLRFlag = false;

	if(bkMusic.getStatus() != SoundSource::Status::Playing)
		bkMusic.play();

	mCornPoint.x = (window_width - stageWidth * GRIDSIZE) / 2;
	mCornPoint.y = (window_height - stageHeight * GRIDSIZE) / 2;

	IniData();
}

void Game::IniData()
{
	int i, j;

	for (j = 0; j < stageHeight; j++)
		for (i = 0; i < stageWidth; i++)
		{
			mGameData[j][i].mState = ncUNDOWN;
			mGameData[j][i].isPress = false;
		}
}

void Game::LoadMediaData()
{
	sbWin.loadFromFile("data/Audios/pass.ogg");
	sbBoom.loadFromFile("data/Audios/bomb.ogg");
	soundWin.setBuffer(sbWin);
	soundBoom.setBuffer(sbBoom);
	bkMusic.openFromFile("data/Audios/bgm.ogg");

	soundWin.setVolume(Volume);
	soundBoom.setVolume(Volume);
	bkMusic.setVolume(Volume);
	
	btnEasy.setTex("data/images/btn/btnEasy.png", 60, 36);
	btnNormal.setTex("data/images/btn/btnNormal.png", 60, 36);
	btnHard.setTex("data/images/btn/btnHard.png", 60, 36);
	btnSkin.setTex("data/images/btn/btnSkin.png", 60, 36);
	btnBG.setTex("data/images/btn/btnBG.png", 60, 36);
	btnRestart.setTex("data/images/btn/btnRestart.png", 60, 36);
	btnQuit.setTex("data/images/btn/btnQuit.png", 60, 36);

	std::stringstream ss;
	ss << "data/images/BK0" << imgBGNo << ".jpg";
	if (!tBackground.loadFromFile(ss.str()))
		cout << "BK image 没有找到" << endl;

	ss.str("");

	ss << "data/images/Game" << imgSkinNo << ".jpg";
	if (!tTiles.loadFromFile(ss.str()))
		cout << "Game Skin image 没有找到" << endl;

	if (!tNum.loadFromFile("data/images/num.jpg"))
		cout << "num.jpg 没有找到" << endl;

	if (!tTimer.loadFromFile("data/images/jishiqi.jpg"))
		cout << "jishiqi.jpg 没有找到" << endl;

	if (!tCounter.loadFromFile("data/images/jishuqi.jpg"))
		cout << "jishuqi.jpg 没有找到" << endl;

	if (!tGameOver.loadFromFile("data/images/gameover.jpg"))
		cout << "gameover.jpg 没有找到" << endl;

	sBackground.setTexture(tBackground);
	sTiles.setTexture(tTiles);
	sNum.setTexture(tNum);
	sTimer.setTexture(tTimer);
	sCounter.setTexture(tCounter);
	sGameOver.setTexture(tGameOver);
}

void Game::PartialReload()
{
	std::stringstream ss;
	ss << "data/images/BK0" << imgBGNo << ".jpg";
	if (!tBackground.loadFromFile(ss.str()))
		cout << "BK image 没有找到" << endl;

	ss.str("");

	ss << "data/images/Game" << imgSkinNo << ".jpg";
	if (!tTiles.loadFromFile(ss.str()))
		cout << "Game Skin image 没有找到" << endl;


	sBackground.setTexture(tBackground);

	sTiles.setTexture(tTiles);

}

void Game::MineSet(int Py, int Px, bool _LButton)
{
	int mCount, i, j, k, l;
	mCount = 0;

	do {
		bool flag = true;
		k = rand() % stageHeight;
		l = rand() % stageWidth;

		for (i = Py - 1; i < Py + 2; i++)
			for (j = Px - 1; j < Px + 2; j++)
				if (i >= 0 && i < stageHeight && j >= 0 && j < stageWidth)
					// 随机坐标若处于 9 宫格覆盖范围, 且是左键，则不要布雷
					if (k == i && l == j && _LButton)
						flag = false;

		if (flag && mGameData[k][l].mState == ncUNDOWN)
		{
			mGameData[k][l].mState = ncMINE;
			mGameData[k][l].mStateBackUp = ncMINE;
			mCount++;
		}
	} while (mCount != mMineNum);

	// 方格赋值
	for (i = 0; i < stageHeight; i++)
		for (j = 0; j < stageWidth; j++)
		{
			if (mGameData[i][j].mState != ncMINE)
			{
				mCount = 0;
				for (k = i - 1; k < i + 2; k++)
					for (l = j - 1; l < j + 2; l++)
						// 计算(i,j)周围雷的数目
						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncMINE)
								mCount++;
						}
				// 保存状态
				switch (mCount)
				{
				case 0:
					mGameData[i][j].mState = ncNULL; break;
				case 1:
					mGameData[i][j].mState = ncONE; break;
				case 2:
					mGameData[i][j].mState = ncTWO; break;
				case 3:
					mGameData[i][j].mState = ncTHREE; break;
				case 4:
					mGameData[i][j].mState = ncFOUR; break;
				case 5:
					mGameData[i][j].mState = ncFIVE; break;
				case 6:
					mGameData[i][j].mState = ncSIX; break;
				case 7:
					mGameData[i][j].mState = ncSEVEN; break;
				case 8:
					mGameData[i][j].mState = ncEIGHT; break;
				}
			}
		}

}

void Game::unCover()
{
	int i, j;
	for (j = 0; j < stageHeight; j++)
		for (i = 0; i < stageWidth; i++)
		{
			if (mGameData[j][i].isPress == false)
				if (mGameData[j][i].mState == ncMINE)
				{
					mGameData[j][i].isPress = true;
					mGameData[j][i].mState = ncUNFOUND;
				}
		}
}

void Game::undownOpen()
{
	int i, j;
	for (j = 0; j < stageHeight; j++)
		for (i = 0; i < stageWidth; i++)
		{
			if (mGameData[j][i].isPress == false)
				mGameData[j][i].isPress = true;
		}
}

void Game::Input()
{
	Event event;
	window.pollEvent(event);
	if (event.type == Event::Closed || (event.type == Event::KeyReleased && event.key.code == Keyboard::Escape))
	{
		gameQuit = true;
		window.close();
	}


	// 鼠标
	if(mGridPoint.x >= 0 && mGridPoint.x < stageWidth
		&& mGridPoint.y >= 0 && mGridPoint.y < stageHeight
		&& isGameOverState == ncNO)
	{
		// 左键
		if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left)
		{
			mouseFlag = mL;

			// 双击计时
			if(mLcounter == 0)
				mouseClickTimer.restart();
			mLcounter++;
			//cout << mLcounter << endl;
		}
		if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && mLcounter == 2)
		{
			if (mouseClickTimer.getElapsedTime().asMilliseconds() <= 300 && event.type != Event::MouseMoved)
			{
				//cout << "双" << endl;
				mouseFlag = mR;
			}
			mLcounter = 0;
		}

		// 右键
		else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Right)
		{
			mouseFlag = mR;
		}
		else if (Mouse::isButtonPressed(Mouse::Button::Right) && !isLRFlag)
		{
			mouseFlag = mPRESSING;
		}

		// 左右键同时
		if (Mouse::isButtonPressed(Mouse::Button::Left))
		{
			if (Mouse::isButtonPressed(Mouse::Button::Right))
			{
				isLRFlag = true;
				mouseFlag = mPRESSING;
			}
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Right)
			{
				mouseFlag = mR;
				isLRFlag = false;
			}
		}
		if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
			&& isLRFlag == true)
		{
			mouseFlag = mR;
			isLRFlag = false;
		}

	}

	// 按钮
	if (btnEasy.updateStatus(Mouse::getPosition(window), event))
		mouseFlag = mEASY;
	if (btnNormal.updateStatus(Mouse::getPosition(window), event))
		mouseFlag = mNORMAL;
	if (btnHard.updateStatus(Mouse::getPosition(window), event))
		mouseFlag = mHARD;
	if (btnSkin.updateStatus(Mouse::getPosition(window), event))
		mouseFlag = mSKIN;
	if (btnBG.updateStatus(Mouse::getPosition(window), event))
		mouseFlag = mBG;
	if (btnRestart.updateStatus(Mouse::getPosition(window), event))
		mouseFlag = mRESTART;
	if (btnQuit.updateStatus(Mouse::getPosition(window), event))
		mouseFlag = mQUIT;
}

void Game::LButtonDown()
{
	int i, j;
	i = mGridPoint.x;
	j = mGridPoint.y;

	if (isGameBegin == false)
	{
		isGameBegin = true;
		gameClock.restart();
		MineSet(j, i, true);
	}
	if (mGameData[j][i].mState != ncFLAG)
		if (mGameData[j][i].isPress == false)
		{
			mGameData[j][i].isPress = true;
			if (mGameData[j][i].mState == ncMINE)
			{
				isGameBegin = false;
				isGameOverState = ncLOSE;
				mGameData[j][i].mState = ncBOMBING;
				unCover();
				undownOpen();
			}
			if (mGameData[j][i].mState == ncNULL)
				NullClick(j, i);
		}
}

void Game::RButtonDown()
{
	int i, j, k, l;
	i = mGridPoint.x;
	j = mGridPoint.y;

	if (mGameData[j][i].isPress == false)
	{
		if (isGameBegin)
		{
			mGameData[j][i].isPress = true;
			mGameData[j][i].mStateBackUp = mGameData[j][i].mState;
			mGameData[j][i].mState = ncFLAG;
			mFlagCalc++;
		}
		else
		{
			isGameBegin = true;
			gameClock.restart();
			MineSet(j, i, false);
			mGameData[j][i].isPress = true;
			mGameData[j][i].mStateBackUp = mGameData[j][i].mState;
			mGameData[j][i].mState = ncFLAG;
			mFlagCalc++;
		}
	}
	else
	{
		if (mGameData[j][i].mState == ncFLAG)
		{
			mGameData[j][i].isPress = true;
			mGameData[j][i].mState = ncQ;
			mFlagCalc--;
		}
		else if (mGameData[j][i].mState == ncQ)
		{
			mGameData[j][i].isPress = false;
			mGameData[j][i].mState = mGameData[j][i].mStateBackUp;
		}
		else if (mGameData[j][i].mState != ncQ && mGameData[j][i].mState != ncFLAG)
		{
			if (mGameData[j][i].isPress == true)
			{
				int flagNum = 0;
				for (k = j - 1; k < j + 2; k++)
					for (l = i - 1; l < i + 2; l++)
						if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
						{
							if (mGameData[k][l].mState == ncFLAG)
							{
								flagNum++;
								if (mGameData[k][l].mStateBackUp != ncMINE)
								{
									isGameOverState = ncLOSE;
									isGameBegin = false;
									unCover();
									undownOpen();
								}
							}
						}
				if (flagNum + 2 == mGameData[j][i].mState)
				{
					for (k = j - 1; k < j + 2; k++)
						for (l = i - 1; l < i + 2; l++)
							if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
							{
								if (mGameData[k][l].mState != ncFLAG)
								{
									if (mGameData[k][l].isPress == false)
									{
										mGameData[k][l].isPress = true;
										if (mGameData[k][l].mState == ncMINE)
										{
											isGameOverState = ncLOSE;
											isGameBegin = false;
											mGameData[k][l].mState = ncBOMBING;
											unCover();
											undownOpen();
										}
										if (mGameData[k][l].mState == ncNULL)
											NullClick(k, l);
									}
								}
							}
				}
			}
		}
	}
}

void Game::Pressing()
{
	int i, j, k, l;
	i = mGridPoint.x;
	j = mGridPoint.y;

	if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
	{
		if (mGameData[j][i].mState != ncFLAG
			&& mGameData[j][i].mState != ncQ
			&& mGameData[j][i].isPress == true)
		{
			for (k = j - 1; k < j + 2; k++)
				for (l = i - 1; l < i + 2; l++)
					if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth
						&& !(k == j && l == i))
					{
						if (mGameData[k][l].isPress != true)
						{
							mGameData[k][l].mStateBackUp = mGameData[k][l].mState;
							mGameData[k][l].mState = ncX;
						}
					}
			Event event;
			window.pollEvent(event);
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Right)
			{
				mouseFlag = mNULL;
				if (mGameData[j][i].isPress == true)
				{
					RButtonDown();
				}
			}
		}
	}
}

void Game::NullClick(int j, int i)
{
	int k, l;
	for (k = j - 1; k < j + 2; k++)
		for (l = i - 1; l < i + 2; l++)
			if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
			{
				if (mGameData[k][l].isPress == false)
				{
					mGameData[k][l].isPress = true;
					if (mGameData[k][l].mState == ncNULL)
						NullClick(k, l); 
				}
			}
}

Vector2i Game::getMouseGrid()
{
	int i, j;
	Vector2i mPoint = Mouse::getPosition(window);
	if (mPoint.x < mCornPoint.x || mPoint.y - mCornPoint.y < 0)
	{
		i = -1;
		j = -1;
	}
	else
	{
		i = (mPoint.x - mCornPoint.x) / gridSize;
		j = (mPoint.y - mCornPoint.y) / gridSize;
	}
	return Vector2i(i, j);
}

void Game::Logic()
{
	if (mouseClickTimer.getElapsedTime().asMilliseconds() > 300)
		mLcounter = 0;

	mGridPoint.x = getMouseGrid().x;
	mGridPoint.y = getMouseGrid().y;

	for (int k = 0; k < stageHeight; k++)
	{
		for (int l = 0; l < stageWidth; l++)
			if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth
				&& mGameData[k][l].mState == ncX)
			{
				mGameData[k][l].mState = mGameData[k][l].mStateBackUp;
			}
	}

	switch (mouseFlag)
	{
	case mL:
		LButtonDown();
		mouseFlag = mNULL;
		break;
	case mR:
		RButtonDown();
		mouseFlag = mNULL;
		break;
	case mPRESSING:
		Pressing();
		mouseFlag = mNULL;
		break;
	case mEASY:
		gamelvl = 1;
		Initial();
		mouseFlag = mNULL;
		break;
	case mNORMAL:
		gamelvl = 2;
		Initial();
		mouseFlag = mNULL;
		break;
	case mHARD:
		gamelvl = 3;
		Initial();
		mouseFlag = mNULL;
		break;
	case mSKIN:
		imgSkinNo++;
		if (imgSkinNo > 6)
			imgSkinNo = 1;
		PartialReload();
		mouseFlag = mNULL;
		break;
	case mBG:
		imgBGNo++;
		if (imgBGNo > 7)
			imgBGNo = 1;
		PartialReload();
		mouseFlag = mNULL;
		break;
	case mRESTART:
		Initial();
		mouseFlag = mNULL;
		break;
	case mQUIT:
		bkMusic.stop();
		gameQuit = true;
		window.close();
		mouseFlag = mNULL;
		break;
	default:break;
	}

	if (isGameOverState == ncLOSE && bkMusic.getStatus() == SoundSource::Status::Playing)
	{
		bkMusic.stop();
		soundBoom.play();
	}
	else if (isGameOverState == ncWIN && bkMusic.getStatus() == SoundSource::Status::Playing)
	{
		bkMusic.stop();
		soundWin.play();
	}

	isWin();
}

void Game::isWin()
{
	int i, j, c = 0;
	if (isGameOverState != ncLOSE)
	{
		for (i = 0; i < stageWidth; i++)
			for (j = 0; j < stageHeight; j++)
			{
				if (mGameData[j][i].isPress == false || mGameData[j][i].mState == ncFLAG)
					c++;
			}
	}
	if (c == mMineNum)
	{
		isGameBegin = false;
		isGameOverState = ncWIN;
		undownOpen();
	}
}

void Game::Draw()
{
	window.clear();
	window.draw(sBackground);

	DrawGrid();

	DrawButton();

	DrawTimer();
	
	DrawScore();

	if (isGameOverState)
		DrawGameEnd();

	window.display(); //把显示缓冲区的内容，显示在屏幕上。SFML 采用的是双缓冲机制
}

void Game::DrawGrid()
{
	for (int j = 0; j < stageHeight; j++)
		for (int i = 0; i < stageWidth; i++)
		{
			if (mGameData[j][i].isPress == true)
			{
				sTiles.setTextureRect(IntRect(mGameData[j][i].mState * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				sTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
			}
			else
			{
				if(mGameData[j][i].mState == ncX)
					sTiles.setTextureRect(IntRect(mGameData[j][i].mState * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				else
					sTiles.setTextureRect(IntRect(ncUNDOWN * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
				sTiles.setPosition(mCornPoint.x + i * GRIDSIZE, mCornPoint.y + j * GRIDSIZE);
			}
			window.draw(sTiles);
		}
}

void Game::DrawButton()
{
	Vector2i LeftCorner;
	int ButtonWidth = 60;
	int ButtonHeight = 36;
	int detaX;
	detaX = (window_width - ButtonWidth * 7) / 8; //7 个按钮在界面上等分宽度
	LeftCorner.y = window_height - GRIDSIZE * 3; //指定高度

	//ButtonRectEasy
	LeftCorner.x = detaX;
	btnEasy.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(btnEasy.getSprite());

	//ButtonRectNormal
	LeftCorner.x = detaX * 2 + ButtonWidth;
	btnNormal.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(btnNormal.getSprite());

	//ButtonRectHard
	LeftCorner.x = detaX * 3 + ButtonWidth * 2;
	btnHard.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(btnHard.getSprite());

	//ButtonRectSkin
	LeftCorner.x = detaX * 4 + ButtonWidth * 3;
	btnSkin.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(btnSkin.getSprite());

	//ButtonRectBG
	LeftCorner.x = detaX * 5 + ButtonWidth * 4;
	btnBG.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(btnBG.getSprite());

	//ButtonRectRestart
	LeftCorner.x = detaX * 6 + ButtonWidth * 5;
	btnRestart.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(btnRestart.getSprite());

	//ButtonRectQuit
	LeftCorner.x = detaX * 7 + ButtonWidth * 6;
	btnQuit.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(btnQuit.getSprite());
}

void Game::DrawScore()
{
	Vector2i LeftCorner;
	LeftCorner.x = window_width - sCounter.getLocalBounds().width * 1.25;
	LeftCorner.y = sCounter.getLocalBounds().height * 0.5;
	sCounter.setPosition(LeftCorner.x, LeftCorner.y); //计数器纹理的贴图位置
	window.draw(sCounter);

	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sCounter.getLocalBounds().width - NumSize;
	LeftCorner.y = LeftCorner.y + sCounter.getLocalBounds().height * 0.5 - NumSize * 0.5;

	int mScore = mMineNum - mFlagCalc;
	//绘制个位数的数字
	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);
	//绘制十位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);
	//绘制百位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
	sNum.setPosition(LeftCorner.x, LeftCorner.y);
	window.draw(sNum);
}

void Game::DrawTimer()
{
	Vector2i LeftCorner;
	LeftCorner.x = sTimer.getLocalBounds().width * 0.25;
	LeftCorner.y = sTimer.getLocalBounds().height * 0.5;
	sTimer.setPosition(LeftCorner.x, LeftCorner.y); //计数器纹理的贴图位置
	window.draw(sTimer);

	if (isGameBegin)
		mTime = gameClock.getElapsedTime().asSeconds();
	int mScore = mTime;

	if (mScore > 999)
		mScore = 999;

	int NumSize = sNum.getLocalBounds().height;
	LeftCorner.x = LeftCorner.x + sTimer.getLocalBounds().width - NumSize * 1.5;
	LeftCorner.y = LeftCorner.y + sTimer.getLocalBounds().height * 0.5 - NumSize * 0.5;
	//绘制个位数的数字
	int a = mScore % 10;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
	//绘制十位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
	//绘制百位数的数字
	mScore = mScore / 10;
	a = mScore % 10;
	LeftCorner.x = LeftCorner.x - NumSize;
	sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));//纹理上取数字纹理
	sNum.setPosition(LeftCorner.x, LeftCorner.y);//摆好位置
	window.draw(sNum);
}

void Game::DrawGameEnd()
{
	Vector2i LeftCorner;
	int ButtonWidth = 200;
	int ButtonHeight = sGameOver.getLocalBounds().height;
	LeftCorner.x = (window_width - ButtonWidth) / 2; //指定顶点坐标
	LeftCorner.y = (window_height - ButtonHeight) / 2;//指定顶点坐标

	sGameOver.setPosition(LeftCorner.x, LeftCorner.y); //设置按钮的位置坐标

	if (isGameOverState == ncWIN)
		sGameOver.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight)); //读取按钮的纹理区域
	if (isGameOverState == ncLOSE)
		sGameOver.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight)); //读取按钮的纹理区域

	window.draw(sGameOver);
}

