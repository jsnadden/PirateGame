#pragma once
#include <optional>
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{
	template <typename Tree, typename Function>
	struct ActionNode;

	template <typename Time, typename ... Args, typename Function>
	struct ActionNode<BehaviourTree<Time, Args...>, Function>
		: Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;
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

	template <typename Tree, typename Function>
	node_ptr<Tree> Action(Function&& function)
	{
		return std::make_unique<ActionNode<Tree, Function>>(std::move(function));
	}


}