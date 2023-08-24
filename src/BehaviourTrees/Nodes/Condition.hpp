#pragma once
#include <vector>
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename BehaviourTree, typename Function>
	struct ConditionNode;

	template <typename Time, typename Event, typename ... Args, typename Function>
	struct ConditionNode<BehaviourTree<Time, Event, Args...>, Function>
		: Node<BehaviourTree<Time, Event, Args...>>
	{
		using tree_type = BehaviourTree<Time, Event, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::finished;
		using typename node_type::status;

		FallbackNode(Function&& function)
			: conditionFunction(std::move(function))
		{}

		status Tick(Time dt, Args ... args) override
		{
			finished{ conditionFunction(dt, args...) };
		}

	private:
		bool started = false;
		Function conditionFunction;

	};

	template <typename BehaviourTree, typename Function>
	node_ptr<BehaviourTree> Condition(Function&& function)
	{
		return std::make_unique<ConditionNode<BehaviourTree, Function>>(std::move(function));
	}

}