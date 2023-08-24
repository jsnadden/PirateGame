#pragma once
#include <tuple>



namespace BT
{

	template <typename Time, typename Event, typename ... Args>
	struct BehaviourTree
	{
		using time_type = Time;
		using event_type = Event;
		using args_type = std::tuple<Args...>;
	};

}