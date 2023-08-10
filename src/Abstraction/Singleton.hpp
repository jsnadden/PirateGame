#pragma once

#include "Uncopyable.hpp"

template <class T>
class Singleton
{
public:
	static T* Pointer()
	{
		static T t;
		return &t;
	}
};