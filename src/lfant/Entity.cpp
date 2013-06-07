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


#include <lfant/Entity.h>

// Internal
#include <lfant/Time.h>

#include <lfant/Game.h>
#include <lfant/Scene.h>
#include <lfant/TypeInfo.h>

#include <lfant/Console.h>
#include <lfant/util/String.h>
#include <lfant/Transform.h>

#include <lfant/Component.h>
#include <lfant/ScriptSystem.h>
#include <lfant/Camera.h>
#include <lfant/Random.h>

// External
#include <stdarg.h>

namespace lfant
{

Entity::Entity()
{
}

Entity::~Entity()
{
}

/*******************************************************************************
*
*		Generic Entity functions
*
*******************************************************************************/

void Entity::Init()
{
	Log("Entity::Init: Touch.");
	Object::Init();

	if(!transform)
	{
		transform = AddComponent<Transform>();
	}

	if(lifeTime <= 0.0f)
	{
		useLifeTime = false;
	}

	if(id == 0)
	{
		// Generate new id?
		id = random::Range<uint64_t>(0, ULLONG_MAX);
	}
}

void Entity::Update()
{
	if(lifeTime > 0.0f)
	{
		useLifeTime = true;
	}
	if(useLifeTime == true)
	{
		lifeTime -= game->time->deltaTime;
		if(lifeTime <= 0.0f)
		{
			Destroy();
		}
	}

	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->Update();
		}
	}
	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->PostUpdate();
		}
	}

	for(auto& child : children)
	{
		if(child->active)
		{
			child->Update();
		}
	}
}

void Entity::AddChild(Entity* ent)
{
	children.push_front(ent);
}

void Entity::RemoveChild(Entity* ent)
{
	//	children.remove(ptr<Entity>(ent));
	for(uint i = 0; i < children.size(); ++i)
	{
		if(children[i] == ent)
		{
			children.erase(children.begin()+i);
			return;
		}
	}
}

void Entity::AddComponent(Component* comp, Properties *prop)
{
	components.push_back(comp);
	comp->owner = this;
	if(prop)
	{
		comp->Load(prop);
	}
	comp->Init();
	TriggerEvent("AddComponent", comp);
	TriggerEvent("SetComponent"+RemoveScoping(Type(comp)), comp);
}

void Entity::UnsafeDestroy()
{
	Object::Destroy();

	Log("Entity::Destroy: Destroying ", components.size()," components.");
	for(auto& compo : components)
	{
		compo->OnDestroy();
	}
//	components.clear();

	Log("Entity::Destroy: Destroying ", children.size()," children.");
	for(auto& child : children)
	{
		child->UnsafeDestroy();
	}
}

void Entity::Destroy()
{
	Log("Entity::Destroy: Touch.");
	Object::Destroy();
	Log("Entity::Destroy: super called.");

	for(auto& compo : components)
	{
		compo->Destroy();
	}
	//	components.clear();
	Log("Entity::Destroy: Components destroyed.");

	for(auto& child : children)
	{
		child->Destroy();
	}

	if(!parent)
	{
		Log("Entity::Destroy: Removing from scene.");
		game->scene->RemoveEntity(this);
	}
	else
	{
		Log("Entity::Destroy: Removing from parent.");
		parent->RemoveChild(this);
	}
}

Entity* Entity::Clone(string name, Entity* parent)
{
	Entity* ent = game->scene->Spawn(name, parent);
	//ent->components = components;
	/// @todo clone children here
	//ent->Init();
	return ent;
}

Component* Entity::GetComponent(string type)
{
	string unscoped = RemoveScoping(type);
	for(auto& comp : components)
	{
		if(Type(comp) == type || Type(comp) == unscoped || RemoveScoping(Type(comp)).find(unscoped) != -1)
		{
			return comp;
		}
	}
	return nullptr;
}

/*******************************************************************************
*
*		Component System functions
*
*******************************************************************************/

void Entity::RemoveComponent(Component* comp)
{
	Log("About to remove comp '", comp, "'.");
	for(uint i = 0; i < components.size(); ++i)
	{
		if(components[i] == comp)
		{
			TriggerEvent("RemoveComponent", comp);
			components.erase(components.begin()+i);
			break;
		}
	}
	Log("Removed component");
}

template<class T>
void Entity::RemoveComponent()
{
	for(uint i = 0; i < components.size(); ++i)
	{
		Component* comp = components[i];
		if(Type<T>() == Type(comp))
		{
			TriggerEvent("RemoveComponent", comp);
			TriggerEvent("SetComponent"+RemoveScoping(Type(comp)), comp);
			comp->Destroy();
			components.erase(components.begin()+i);
		}
	}
}

Entity* Entity::GetChild(string name, bool recursive)
{
	Entity* ent = nullptr;
	for(auto& child : children)
	{
		if(child->name == name)
		{
			ent = child;
		}
	}

	if(ent || !recursive)
	{
		return ent;
	}
	else
	{
		for(auto& child : children)
		{
			if((ent = child->GetChild(name, true)))
			{
				return ent;
			}
		}
	}
	return nullptr;
}

bool Entity::HasTag(string tag)
{
	for(auto& str : tags)
	{
		if(str == tag)
		{
			return true;
		}
	}
	return false;
}

/*******************************************************************************
*
*		Scripting
*
*******************************************************************************/


void Entity::Bind()
{
	/*
	   Script::Class obj;
	   obj.Create<Entity>("Entity", true);

	   obj.Method("void Destroy()", &Entity::Destroy);
	 */
}


/*******************************************************************************
*
*		Saving and Loading
*
*******************************************************************************/

void Entity::Save(Properties* prop)
{
	prop->type = "entity";
	prop->id = name;

	prop->Set("id", id);
	prop->Set("tags", tags);
	prop->Set("layer", layer);
	prop->Set("active", active);
	prop->Set("lifeTime", lifeTime);

	for(auto& comp : components)
	{
		comp->Save(prop->AddChild());
	}

	for(auto& child : children)
	{
		child->Save(prop->AddChild());
	}
}

void Entity::Load(Properties* prop)
{
	prop->Get("id", id);
	prop->Get("tags", tags);
	prop->Get("layer", layer);
	prop->Get("active", active);
	prop->Get("lifeTime", lifeTime);

	Log("Entity::Load: Loaded basic properties.");

	deque<Properties*> c = prop->GetChildren("component");
	Log("Entity::Load: Component nodes loaded.");
	Component* component = nullptr;
	for(auto& comp : c)
	{
		Log("Loading component props, '"+comp->type+" "+comp->id+"'.");
		if(comp->id != "Transform")
		{
			component = AddComponent(comp->id, comp);
			Log("Entity::Load: Added component, owner = ", component->owner);
			if(comp->id == "Camera")
			{
				game->scene->mainCamera = dynamic_cast<Camera*>(component);
			}
		}
		else
		{
			if(transform)
			{
				component = transform;
				component->Load(comp);
			}
			else
			{
				component = AddComponent("Transform", comp);
			}
		}
		//	component->Load(comp);
	}

	c = prop->GetChildren("entity");
	Log("Entity::Load: Children nodes loaded.");
	Entity* ent = nullptr;
	for(auto& child : c)
	{
		ent = game->scene->SpawnAndLoad(child, child->id, this);
	//	ent->Load(child);
	}

	Log("Entity::Load: Finished.");
	Init();
}

Component *Entity::AddComponent(string type, Properties *prop)
{
	printf("Adding comp via string of type\n");
	auto val = Component::componentRegistry[type];
	Component* result = nullptr;
	if(val == nullptr)
	{
		Log("No function for this component type.");
		result = nullptr;
	}
	else
	{
		Log("Found function for this type.");
		result = (this->*val)(prop);
		Log("Added component owner ptr: '", result->owner, "'.");
		result->owner = this;
	}

	if(type == "Camera" && result)
	{
		game->scene->mainCamera = dynamic_cast<Camera*>(result);
	}

	return result;
}

}
