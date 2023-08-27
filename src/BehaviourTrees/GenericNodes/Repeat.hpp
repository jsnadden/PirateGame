#pragma once
#include <optional>
#include "src/BehaviourTrees/Node.hpp"
#include "src/BehaviourTrees/GenericNodes/Success.hpp"

namespace BT
{

	template <typename Tree>
	struct RepeatNode;

	template <typename Time, typename ... Args>
	struct RepeatNode<BehaviourTree<Time, Args...>>
		: Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::running;
		using typename node_type::finished;
		using typename node_type::status;

		RepeatNode(node_ptr<tree_type> ch, std::optional<std::size_t> max)
			: child(std::move(ch))
			, maxRepeats(max)
		{}

		void Start(Args ... args) override
		{
			child->Start(args...);
			childStarted = true;
			counter = 0;
		}

		status Tick(Time dt, Args ... args) override
		{
			if (!childStarted)
			{
				child->Start(args...);
				childStarted = true;
			}

			auto result = child->Tick(dt, args...);

			if (auto out = std::get_if<finished>(&result)) // child finished
			{

				if (!(out->result))
					return finished{ false };
				else
				{
					childStarted = false;

					if (++counter == maxRepeats) // out of repeats
						return finished{ true };

					return running{};
				}
			}
			return result;

		}

		bool Interrupt(Args ... args) override
		{
			return child->Interrupt(args...);
		}

	private:
		node_ptr<BehaviourTree<Time, Args...>> child;
		bool childStarted = false;
		std::optional<std::size_t> maxRepeats;
		std::size_t counter = 0;
	};

	template <typename Tree>
	node_ptr<Tree> Repeat(node_ptr<Tree> child, std::optional<std::size_t> maxRepeats = std::nullopt)
	{
		return std::make_unique<RepeatNode<Tree>>(std::move(child), maxRepeats);
	}

	template <typename Tree>
	node_ptr<Tree> RepeatForever(node_ptr<Tree> child)
	{
		return Repeat<Tree>(Success<Tree>(std::move(child)));
	}

}