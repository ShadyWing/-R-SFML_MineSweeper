#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class RectButton
{
public:
	RectButton();
	~RectButton();

	void setTex(const std::string& filename,
		const int &_width, const int &_height, const int& _left = 0, const int& _top = 0,
		const int& _StatusType = 3, const int &_Orderdir = 1);
	void setVariant(const int i);
	void setPosition(const int &_x, const int &_y);
	void setOrderdir(int i);
	void setOrigin(float _x, float _y);
	void setOrigin(Vector2f _origin);

	Sprite getSprite();
	bool getStatus();
	Vector2f getOrigin();
	Vector2f getSize();
	int getVariant();
	void getLock();

	void Lock(bool _status);
	void Enable(bool _status);
	bool isOverlap(Vector2i mPoint);
	bool updateStatus(const Vector2i &mPoint, Event event);
private:
	Texture buttonTex;
	Sprite buttonSp;
	int left, top, width, height;
	int Orderdir;
	int Variant;
	int StatusType;
	bool isPressed;
	bool isLocked;
	bool isDisable;
};