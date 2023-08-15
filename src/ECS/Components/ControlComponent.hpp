#pragma once

struct ControlComponent
{
	bool controlled = false;

	ControlComponent() = default;

	ControlComponent(bool cont)
		: controlled(cont)
	{

	}
};