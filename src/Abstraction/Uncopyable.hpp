#pragma once

class Uncopyable
{
private:
	Uncopyable(const Uncopyable& uc);

	const Uncopyable& operator=(const Uncopyable& uc);

protected:
	Uncopyable() {}
};