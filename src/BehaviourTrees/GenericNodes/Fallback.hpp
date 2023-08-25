#pragma once
#include <vector>
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename Tree>
	struct FallbackNode;

	template <typename Time, typename ... Args>
	struct FallbackNode<BehaviourTree<Time, Args...>>
		: Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::running;
		using typename node_type::finished;
		using typename node_type::status;

		FallbackNode(std::vector<node_ptr<tree_type>> ch)
			: children(std::move(ch))
		{}

		void Start(Args ... args) override
		{
			index = 0;
			started = false;
		}

		status Tick(Time dt, Args ... args) override
		{
			if (index < children.size())
			{
				if (!started)
				{
					children[index]->Start(args...);
					started = true;
				}

				auto result = children[index]->Tick(dt, args...);
				if (auto out = std::get_if<finished>(&result)) // child finished
				{
					if (out->result) // child success
						return finished{ true };

					// child failure
					++index;
					started = false;
					return running{};
				}
				else // child running or waiting
					return result;
			}
			// out of options
			return finished{ false };
		}

		bool Interrupt(Args ... args) override
		{
			if (index < children.size())
				return children[index]->Interrupt(args...);

			return false;
		}

	private:
		std::vector<node_ptr<BehaviourTree<Time, Args...>>> children;
		std::size_t index = 0;
		bool started = false;
	};

	template <typename Tree, typename ... Children>
	node_ptr<Tree> Fallback(node_ptr<Tree> child, Children ... siblings)
	{
		std::vector<node_ptr<Tree>> children;
		children.push_back(std::move(child));
		(children.push_back(std::move(siblings)), ...);
		return std::make_unique<FallbackNode<Tree>>(std::move(children));
	}




}