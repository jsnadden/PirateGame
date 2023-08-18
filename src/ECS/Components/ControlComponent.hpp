#pragma once

struct ControlComponent
{
	bool controlled = false;

	ControlComponent()
		: controlled(true)
	{}
};