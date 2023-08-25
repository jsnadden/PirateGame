#pragma once
#include <tuple>



namespace BT
{

	template <typename Time, typename ... Args>
	struct BehaviourTree
	{
		using time_type = Time;
		using args_type = std::tuple<Args...>;
	};

}