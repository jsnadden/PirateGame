#pragma once
#include <optional>
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{
	template <typename BehaviourTree, typename Function>
	struct ActionNode;

	template <typename Time, typename Event, typename ... Args, typename Function>
	struct ActionNode<BehaviourTree<Time, Event, Args...>, Function>
		: Node<tree<Time, Event, Args...>>
	{
		using tree_type = BehaviourTree<Time, Event, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::finished;
		using typename node_type::status;

		ActionNode(Function&& function)
			: action(std::move(function))
		{}

		status Tick(Time dt, Args ... args) override
		{
			action(dt, args...);
			return finished{ true };
		}

	private:
		Function action;
	};

	template <typename BehaviourTree, typename Function>
	node_ptr<BehaviourTree> action(Function&& function)
	{
		return std::make_unique<ActionNode<BehaviourTree, Function>>(std::move(function));
	}


}