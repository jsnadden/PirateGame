#pragma once
#include "src/BehaviourTrees/Node.hpp"

namespace BT
{

	template <typename Tree, typename Function>
	struct WaitNode;

	template <typename Time, typename ... Args, typename Function>
	struct WaitNode<BehaviourTree<Time, Args...>, Function>
		: Node<BehaviourTree<Time, Args...>>
	{
		using tree_type = BehaviourTree<Time, Args...>;
		using node_type = Node<tree_type>;
		using typename node_type::finished;
		using typename node_type::waiting;
		using typename node_type::status;

		WaitNode(Function&& function)
			: waitTime(std::move(function))
		{}

		void Start(Args ... args) override
		{
			countDown = waitTime(args...);
		}

		status Tick(Time dt, Args ...) override
		{
			countDown -= dt;
			if (countDown <= 0)
				return finished{ true };
			else
				return waiting{ countDown };
		}

	private:
		Function waitTime;
		Time countDown = Time{};
	};

	template <typename Tree, typename Function>
	struct WaitHelper
	{
		static node_ptr<Tree> Make(Function&& function)
		{
			return std::make_unique<WaitNode<Tree, Function>>(std::move(function));
		}
	};

	template <typename Time, typename ... Args>
	struct WaitHelper<BehaviourTree<Time, Args...>, Time>
	{
		static node_ptr<BehaviourTree<Time, Args...>> Make(Time duration)
		{
			auto function = [duration](Args ...) { return duration; };
			return std::make_unique<WaitNode<BehaviourTree<Time, Args...>, decltype(function)>>(std::move(function));
		}
	};

	template <typename Tree, typename Function>
	node_ptr<Tree> Wait(Function&& function)
	{
		return WaitHelper<Tree, Function>::Make(std::move(function));
	}

}
