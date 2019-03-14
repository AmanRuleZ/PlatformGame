#include "Collision.h"
#include "ColliderComponent.h"
#include <iostream>

bool Collision::ColGround(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (rectA.y + rectA.h >= rectB.y && rectA.y + rectA.h <= rectB.y + rectB.h 
		&& ( (rectA.x >= rectB.x+5 && rectA.x <= rectB.x + rectB.w-5) || (rectA.x +rectA.w >= rectB.x +5&& rectA.x +rectA.w <= rectB.x + rectB.w-5)))
		return true;

	return false;
}

bool Collision::ColOnGround(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (rectA.y + rectA.h >= rectB.y -3  && rectA.y + rectA.h <= rectB.y + rectB.h -3
		&& ((rectA.x + rectA.w / 2 >= rectB.x - 15 && rectA.x + rectA.w / 2 <= rectB.x + rectB.w + 15)))
		return true;

	return false;
}

bool Collision::ColLeft(const SDL_Rect & rectA, const SDL_Rect & rectB)
{	
	if (rectA.x <= rectB.x + rectB.w && rectA.x >= rectB.x && 
		((rectA.y >= rectB.y && rectA.y <= rectB.y + rectB.h - 25) || (rectA.y + rectA.h >= rectB.y +25 && rectA.y + rectA.h <= rectB.y + rectB.h)))
		return true;

	return false;
}

bool Collision::ColOnLeft(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (rectA.x -10  <= rectB.x + rectB.w && rectA.x - 10>= rectB.x &&
		((rectA.y >= rectB.y && rectA.y <= rectB.y + rectB.h) || (rectA.y + rectA.h >= rectB.y && rectA.y + rectA.h <= rectB.y + rectB.h)))
		return true;

	return false;
}

bool Collision::ColRight(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (rectA.x + rectA.w >= rectB.x && rectA.x + rectA.w <= rectB.x + rectB.w &&
		((rectA.y >= rectB.y && rectA.y <= rectB.y + rectB.h -25) || (rectA.y + rectA.h >= rectB.y + 25 && rectA.y + rectA.h <= rectB.y + rectB.h)))
		return true;

	return false;
}

bool Collision::ColOnRight(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (rectA.x + rectA.w +10 >= rectB.x && rectA.x + rectA.w  +10<= rectB.x + rectB.w &&
		((rectA.y >= rectB.y && rectA.y <= rectB.y + rectB.h) || (rectA.y + rectA.h  >= rectB.y && rectA.y + rectA.h <= rectB.y + rectB.h)))
		return true;

	return false;
}

bool Collision::ColUp(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if(rectA.y <= rectB.y+ rectB.h && rectA.y >= rectB.y
		&& ((rectA.x >= rectB.x + 5 && rectA.x <= rectB.x + rectB.w - 5) || (rectA.x + rectA.w >= rectB.x + 5 && rectA.x + rectA.w <= rectB.x + rectB.w - 5)))
		return true;

	return false;
}

bool Collision::AABB(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (
		rectA.x + rectA.w >= rectB.x &&
		rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y &&
		rectB.y + rectB.h >= rectA.y
		)
	{
		return true;
	}

	return false;

}

bool Collision::AABB2(const SDL_Rect & rectA, const SDL_Rect & rectB)
{
	if (
		rectA.x + rectA.w >= rectB.x &&
		rectB.x + rectB.w >= rectA.x &&
		rectA.y + rectA.h >= rectB.y &&
		rectB.y + rectB.h >= rectA.y
		)
	{
		return true;
	}
	cout << rectB.x + rectB.w << "  " << rectA.x << endl;
	//cout << rectA.x << +"  " << rectA.y << "  x  " << rectB.x << "  " << rectB.y << endl;
	//cout << (rectA.x + rectA.w >= rectB.x) << "  " << (rectB.x + rectB.w >= rectA.x) << "  " << (rectA.y + rectA.h >= rectB.y) << "  " << (rectB.y + rectB.h >= rectA.y) << endl;
	return false;

}

