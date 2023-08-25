#pragma once
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename Tree>
	struct InverterNode;

	template <typename Time, typename ... Args>
	struct InverterNode<BehaviourTree<Time, Args...>>
		: Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::running;
		using typename node_type::finished;
		using typename node_type::status;

		InverterNode(node_ptr<tree_type> ch)
			: child(std::move(ch))
		{}

		void Start(Args ... args) override
		{
			child->Start(args...);
			started = true;
		}

		status Tick(Time dt, Args ... args) override
		{
			if (!started)
			{
				started = true;
			}

			auto result = child->Tick(dt, args...);
			if (auto out = std::get_if<finished>(&result)) // child finished
			{
				started = false;

				if (out->result) // child success
					return finished{ false };
				else // child failure
					return finished{ true };
			}
			return result; // child running or waiting
		}

		bool Interrupt(Args ... args) override
		{
			return child->Interrupt(args...);
		}

	private:
		node_ptr<BehaviourTree<Time, Args...>> child;
		bool started = false;
	};

	template <typename Tree>
	node_ptr<Tree> Inverter(node_ptr<Tree> child)
	{
		return std::make_unique<InverterNode<Tree>>(std::move(child));
	}

}