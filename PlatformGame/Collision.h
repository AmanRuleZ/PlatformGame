#ifndef Collision_h
#define Collision_h

#include <SDL.h>

class ColliderComponent;


class Collision
{	
public:
	static bool ColGround(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool ColOnGround(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool ColLeft(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool ColOnLeft(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool ColRight(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool ColOnRight(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool ColUp(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool AABB(const SDL_Rect & rectA, const SDL_Rect & rectB);
	static bool AABB2(const SDL_Rect & rectA, const SDL_Rect & rectB);
};



#endif