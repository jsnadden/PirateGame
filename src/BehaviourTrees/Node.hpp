#pragma once
#include <variant>
#include <memory>
#include "src/BehaviourTrees/BehaviourTree.hpp"

namespace BT
{

	template <typename BehaviourTree>
	struct Node;

	template <typename Time, typename Event, typename ... Args>
	struct Node<BehaviourTree<Time, Event, Args...>>
	{
		using tree_type = BehaviourTree<Time, Event, Args...>;

		struct running
		{};

		struct finished
		{
			bool result;
		};

		struct waiting
		{
			Time duration;
		};

		using status = std::variant<running, finished, waiting>;

		virtual void Start(Args ...) {}
		virtual status Tick(Time dt, Args ... args) = 0;
		virtual bool Interrupt(Event const&, Args ...) { return false; }

		virtual ~node() {}
	};

	template <typename BehaviourTree>
	using node_ptr = std::unique_ptr<Node<BehaviourTree>>;

}
