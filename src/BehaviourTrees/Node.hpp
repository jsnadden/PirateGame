#pragma once
#include <variant>
#include <memory>
#include "src/BehaviourTrees/BehaviourTree.hpp"

namespace BT
{

	template <typename BehaviourTree>
	struct Node;

	template <typename Time, typename ... Args>
	struct Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;

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
		virtual bool Interrupt(Args ... args) { return false; }

		virtual ~Node() {}
	};

	template <typename Tree>
	using node_ptr = std::unique_ptr<Node<Tree>>;

}
