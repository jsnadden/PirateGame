#pragma once

struct Animation
{
	int row = 0;
	int nFrames = 1;
	float speed = 1.0f;

	Animation()
	{
		row = 0;
		nFrames = 1;
		speed = 1.0f;
	}

	Animation(int r, int f, float s)
	{
		row = r;
		nFrames = f;
		speed = s;
	}
};