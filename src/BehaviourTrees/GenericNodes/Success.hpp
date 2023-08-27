#pragma once
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename Tree>
	struct SuccessNode;

	template <typename Time, typename ... Args>
	struct SuccessNode<BehaviourTree<Time, Args...>>
		: Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::finished;
		using typename node_type::status;

		SuccessNode(node_ptr<tree_type> ch)
			: child(std::move(ch))
		{}

		void Start(Args ... args) override
		{
			child->Start(args...);
		}

		status Tick(Time dt, Args ... args) override
		{
			auto result = child->Tick(dt, args...);
			if (std::get_if<finished>(&result))
				return finished{ true };
			return result;
		}

		bool Interrupt(Args ... args) override
		{
			return child->Interrupt(args...);
		}

	private:
		node_ptr<BehaviourTree<Time, Args...>> child;
	};

	template <typename Tree>
	node_ptr<Tree> Success(node_ptr<Tree> child)
	{
		return std::make_unique<SuccessNode<Tree>>(std::move(child));
	}

}