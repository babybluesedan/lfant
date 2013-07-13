/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/
#pragma once

#include <lfant/stdafx.h>

// External
//#include <boost/uuid/uuid.hpp>
//#include <boost/uuid/uuid_generators.hpp>
#include <forward_list>

// Internal
#include <lfant/Object.h>
//#include <lfant/Component.h>
#include <lfant/TypeInfo.h>
#include <lfant/Game.h>
#include <lfant/ptr.h>
#include <lfant/Properties.h>

namespace lfant {

class Component;
class Item;
class Transform;
class Entity;

bool EntityActive(Entity* ent);

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *	This class is the basis for all Entities.
 *		Handles basic object and entity duties, like initialization,
 *		updating every frame, then cleaning up on destruction. Cloning is also
 *		supported by default. Entities can have functionality added to them
 *		by adding Components to their list of Component instances. Those
 *		are modules of functionality and data ready to be added onto an Entity.
 *		This prefers aggregation over inheritance.
 *	@todo
 *		Organize...
 */
class Entity : public Object
{
	friend class Scene;
	friend class Component;
	friend class Renderer;

public:
	Entity();
	Entity(const Entity& ent);
	virtual ~Entity();

	Entity& operator=(const Entity& other);

	virtual void Destroy();

	virtual void Save(Properties* prop) const;
	virtual void Load(Properties* prop);

	void Load(Properties* prop, bool init);

	static void Bind() __attribute__((constructor));

	/**
	 *	Adds a new component by instancing the given type.
	 *	@tparam C The class of component to add.
	 */
	template<typename C>
	C* AddComponent()
	{
		C* comp = new C();
		AddComponent(comp);
		TriggerEvent("SetComponent"+type::Unscope(type::Name(comp)), comp);
		return comp;
	}

	Component* AddComponent(string type);

	void AddComponent(Component* comp);

	/**
	 *	Removes a component from this Entity
	 *	@tparam C The type of component to remove.
	 */
	template<typename C>
	void RemoveComponent();
	void RemoveComponent(string type);
	void RemoveComponent(Component* comp);

	/**
	 *	Clones an identical instance of this Entity.
	 */
	Entity* Clone(string name = "", Entity* parent = nullptr) const;
	void Clone(Entity* ent, string name = "", Entity* parent = nullptr) const;

	template<typename C>
	C* GetComponent()
	{
		for(auto& comp : components)
		{
			if(C* c = dynamic_cast<C*>(comp.get()))
			{
				return c;
			}
		}
		return nullptr;
	}

	Component* GetComponent(string type);

	template<typename C>
	deque<C*> GetComponents()
	{
		deque<C*> comps;
		for(auto& comp : components)
		{
			if(type::Name(comp) == type::Name<C>())
			{
				comps += dynamic_cast<C*>(comp.get());
			}
		}
		return comps;
	}

	Entity* GetChild(string name, bool recursive = false);

	bool HasTag(string tag);

	uint32_t GetId() const { return id; }

	string GetLayer();
	void SetLayer(string layer);

	Entity* GetParent();
	void SetParent(Entity* ent);

	Transform* transform;

	/// Whether to update this Entity or not.
	bool active = true;

	string name = "Entity";

	/// The identifying tags used for grouping.
	deque<string> tags;

	float lifetime = 0.0f;

protected:

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy() {}

	void AddChild(Entity* ent);
	void RemoveChild(Entity* ent);
	void UnsafeDestroy();

	template<typename... P>
	void TriggerEventWithChildren(string name, P... args)
	{
		TriggerEvent(name, args...);
		for(auto& c : children)
		{
			if(!EntityActive(c)) continue;
			c->TriggerEventWithChildren(name, args...);
		}
	}

	template<typename... P>
	void TriggerEventWithParent(string name, P... args)
	{
		TriggerEvent(name, args...);
		if(parent && EntityActive(parent))
		{
			parent->TriggerEvent(name, args...);
		}
	}

private:

	Component* AddComponent(string type, Properties* prop);

	deque< ptr<Entity> > children;
	deque< ptr<Component> > components;
	bool useLifeTime = false;

	/// Scene-unique identifier.
	uint32_t id = 0;

	/// The layer of this entity for primarily display filtering
	string layer = "Main";

	Entity* parent = nullptr;
};

/** @} */
/** @} */
}
