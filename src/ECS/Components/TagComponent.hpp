#pragma once
#include <string>

struct TagComponent
{
private:

	std::string tag;

public:

	TagComponent()
		: tag("unnamed")
	{
	}

	TagComponent(std::string t)
		: tag(t)
	{
	}

};