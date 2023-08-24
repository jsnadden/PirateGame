#pragma once
#include <vector>
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename BehaviourTree, typename Function>
	struct SelectorNode;

	template <typename Time, typename Event, typename ... Args, typename Function>
	struct SelectorNode<BehaviourTree<Time, Event, Args...>, Function>
		: Node<BehaviourTree<Time, Event, Args...>>
	{
		using tree_type = BehaviourTree<Time, Event, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::running;
		using typename node_type::finished;
		using typename node_type::status;

		SelectorNode(std::vector<node_ptr<tree_type>> ch, Function function)
			: children(std::move(ch)), selectorFunction(std::move(function))
		{}

		void Start(Args ...) override
		{
			index = 0;
			started = false;
		}

		status Tick(Time dt, Args ... args) override
		{
			index = selectorFunction(dt, args...);

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
					started = false;
					return result;
				}
				else // child running or waiting
					return result;
			}

			// invalid selection
			return finished{ false };
		}

		bool Interrupt(Event const& event, Args ... args) override
		{
			if (index < children.size())
				return children[index]->Interrupt(event, args...);

			return false ;
		}

	private:
		std::vector<node_ptr<BehaviourTree<Time, Event, Args...>>> children;
		std::size_t index = 0;
		Function selectorFunction;
		bool started = false;
	};

	template <typename BehaviourTree, typename ... Children, typename Function>
	node_ptr<BehaviourTree> Selector(node_ptr<BehaviourTree> child, Children ... siblings, Function function)
	{
		std::vector<node_ptr<BehaviourTree>> children;
		children.push_back(std::move(child));
		(children.push_back(std::move(siblings)), ...);
		return std::make_unique<SelectorNode<BehaviourTree>, Function>(std::move(children), std::move(function));
	}




}