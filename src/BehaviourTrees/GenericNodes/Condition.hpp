#pragma once
#include <vector>
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename Tree, typename Function>
	struct ConditionNode;

	template <typename Time, typename ... Args, typename Function>
	struct ConditionNode<BehaviourTree<Time, Args...>, Function>
		: Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::finished;
		using typename node_type::status;

		ConditionNode(Function&& function)
			: conditionFunction(std::move(function))
		{}

		status Tick(Time dt, Args ... args) override
		{
			return finished{ conditionFunction(dt, args...) };
		}

	private:
		bool started = false;
		Function conditionFunction;

	};

	template <typename Tree, typename Function>
	node_ptr<Tree> Condition(Function&& function)
	{
		return std::make_unique<ConditionNode<Tree, Function>>(std::move(function));
	}

}