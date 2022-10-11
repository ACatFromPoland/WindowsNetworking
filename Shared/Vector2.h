#pragma once
class Vector2
{
public:
	union
	{
		struct { float x, y; };
		float xy[2];
	};
};