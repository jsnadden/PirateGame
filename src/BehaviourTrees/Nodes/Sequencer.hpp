#pragma once
#include <vector>
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename BehaviourTree>
	struct SequencerNode;

	template <typename Time, typename Event, typename ... Args>
	struct SequencerNode<BehaviourTree<Time, Event, Args...>>
		: Node<BehaviourTree<Time, Event, Args...>>
	{
		using tree_type = BehaviourTree<Time, Event, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::running;
		using typename node_type::finished;
		using typename node_type::status;

		SequencerNode(std::vector<node_ptr<tree_type>> ch)
			: children(std::move(ch))
		{}

		void Start(Args ...) override
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
					if (!out->result) // child failure
						return finished{ false };

					// child success
					++index;
					started = false;
					return running{};
				}
				else // child running or waiting
					return result;
			}
			// completed sequence
			return finished{ true };
		}

		bool Interrupt(Event const& event, Args ... args) override
		{
			if (index < children.size())
				return children[index]->Interrupt(event, args...);

			return false;
		}

	private:
		std::vector<node_ptr<BehaviourTree<Time, Event, Args...>>> children;
		std::size_t index = 0;
		bool started = false;
	};

	template <typename BehaviourTree, typename ... Children>
	node_ptr<BehaviourTree> Sequencer(node_ptr<BehaviourTree> child, Children ... siblings)
	{
		std::vector<node_ptr<BehaviourTree>> children;
		children.push_back(std::move(child));
		(children.push_back(std::move(siblings)), ...);
		return std::make_unique<SequencerNode<BehaviourTree>>(std::move(children));
	}




}