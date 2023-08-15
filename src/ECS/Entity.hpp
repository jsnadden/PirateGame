#pragma once
#include <entt.hpp>

class Entity
{
private:

	entt::registry* enttReg{ nullptr };
	entt::entity enttId{ entt::null };

public:

	Entity() = default;

	Entity(entt::registry* reg, entt::entity ent)
		: enttReg(reg), enttId(ent)
	{
	}

	template <typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		assert(!this->HasComponent<T>());

		return enttReg->emplace<T>(this->enttId, std::forward<Args>(args)...);
	}

	template <typename T>
	bool HasComponent()
	{
		return enttReg->all_of<T>(this->enttId);
	}

	template <typename T>
	T& GetComponent()
	{
		assert(this->HasComponent<T>());

		return enttReg->get<T>(this->enttId);
	}

	operator bool() const { return this->enttId != entt::null; }

};

