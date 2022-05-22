#pragma once
#include "button.h"

RectButton::RectButton()
{
	width = 0;
	height = 0;
	left = 0;
	top = 0;
	Orderdir = 1;	//1 纵向，2 横向
	StatusType = 3; //默认三种状态
	Variant = 1;
	isPressed = false;
	isLocked = false;
	isDisable = false;
}

RectButton::~RectButton() {}

void RectButton::setTex(const std::string& filename,
	const int& _width, const int& _height, const int &_left, const int &_top,
	const int &_StatusType, const int &_Orderdir)
{
	if (_StatusType != 3)
		StatusType = _StatusType;
	if (_Orderdir != 1)
		Orderdir = _Orderdir;
	if (!buttonTex.loadFromFile(filename))
		std::cout << filename << " not found" << std::endl;

	left = _left;
	top = _top;
	width = _width;
	height = _height;

	int block_height = buttonTex.getSize().y / StatusType;
	int block_width = buttonTex.getSize().x;
	buttonSp.setTexture(buttonTex);
	buttonSp.setTextureRect(IntRect(0, 0, block_width, block_height));
	buttonSp.setOrigin(left, top);
}

void RectButton::setVariant(const int i)
{
	// 若纵向排列
	if (Orderdir == 1)
	{
		int block_height = buttonTex.getSize().y / StatusType;
		int block_width = buttonTex.getSize().x;
		buttonSp.setTextureRect(IntRect(0, (i - 1) * block_height, block_width, block_height));
	}
	// 若横向排列
	else if (Orderdir == 2)
	{
		float block_width = buttonTex.getSize().x / StatusType;
		float block_height = buttonTex.getSize().y;
		buttonSp.setTextureRect(IntRect((i - 1) * block_width, 0, block_width, block_height));
	}
}

void RectButton::setPosition(const int &_x, const int& _y)
{
	buttonSp.setPosition(_x, _y);
}

void RectButton::setOrderdir(int i)
{
	Orderdir = i;
}

void RectButton::setOrigin(float _x, float _y)
{
	buttonSp.setOrigin(Vector2f(_x + left, _y + top));
}

void RectButton::setOrigin(Vector2f _origin)
{
	buttonSp.setOrigin(Vector2f(_origin.x + left, _origin.y + top));
}

Vector2f RectButton::getOrigin()
{
	return buttonSp.getOrigin();
}

Vector2f RectButton::getSize()
{
	return Vector2f(width, height);
}

Sprite RectButton::getSprite()
{
	return buttonSp;
}

/////////////////////////
/// 按钮可用
/// 
/// 返回		TRUE
/// 否则		FALSE
/////////////////////////
bool RectButton::getStatus()
{
	return !isDisable;
}

void RectButton::getLock()
{
	if (isLocked)
		std::cout << "Locked\n";
	else
		std::cout << "Unlocked\n";
}

int RectButton::getVariant()
{
	return Variant;
}

void RectButton::Lock(bool _status)
{
	if (_status)
		isLocked = true;
	else
		isLocked = false;
}

void RectButton::Enable(bool _status)
{
	if (_status)
		isDisable = false;
	else
		isDisable = true;
}

bool RectButton::isOverlap(Vector2i mPoint)
{
	int x = buttonSp.getPosition().x - buttonSp.getOrigin().x;
	int y = buttonSp.getPosition().y - buttonSp.getOrigin().y;

	if (mPoint.x >= x + left
		&& mPoint.x <= x + left + width
		&& mPoint.y >= y + top
		&& mPoint.y <= y + top + height)
		return true;
	return false;
}

bool RectButton::updateStatus(const Vector2i &mPoint, Event event)
{
	if (isDisable)
	{ 
		setVariant(4);
		return false;
	}

	if (isLocked)
	{
		setVariant(1);
		return false;
	}

	if (isOverlap(mPoint))
	{
		setVariant(2);

		if (event.type == Event::MouseButtonPressed
			&& event.mouseButton.button == Mouse::Left)
		{
			isPressed = true;
			setVariant(3);
		}
		else if (Mouse::isButtonPressed(Mouse::Left) && isPressed == true)
			setVariant(3);
		else if (event.type == Event::MouseButtonReleased
			&& event.mouseButton.button == Mouse::Left
			&& isPressed == true)
		{
			isPressed = false;
			return true;
		}
	}
	else if (event.type == Event::MouseButtonReleased
		&& event.mouseButton.button == Mouse::Left)
	{
		isPressed = false;
		return false;
	}
	else
		setVariant(1);

	return false;
}