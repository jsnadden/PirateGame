#pragma once

class Maths
{
public:

	static int divide(int x, int y) {
		int q = x / y;
		int r = x % y;
		if ((r != 0) && ((r < 0) != (y < 0))) --q;
		return q;
	}

	static int mod(int x, int y) {
		int r = x % y;
		if ((r != 0) && ((r < 0) != (y < 0))) { r += y; }
		return r;
	}
};