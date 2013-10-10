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

namespace lfant {

bool EntityActive(Entity* ent)
{
	return ent->active;
}

Entity::Entity()
{
	transform = AddComponent<Transform>();
}

Entity::Entity(const Entity& ent)
{
	ent.Clone(this);
}

Entity::~Entity()
{
}

Entity& Entity::operator=(const Entity& other)
{
	other.Clone(this);
	return *this;
}

Entity* Entity::Clone(string name, Entity* parent) const
{
	Entity* ent = new Entity;
	Clone(ent, name, parent);
	return ent;
}

void Entity::Clone(Entity* ent, string name, Entity* parent) const
{
	if(name == "") name = this->name;
	if(!parent) parent = this->parent;

	ent->name = name;
	ent->parent = parent;
	game->scene->AddEntity(ent);

	for(auto& c : components)
	{
		c->Clone(ent);
	}
	for(auto& c : children)
	{
		c->Clone(c->name, ent);
	}
}

uint32_t GenerateId()
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

//	if(!transform)
//	{
	//	transform = AddComponent<Transform>();
//	}

	if(lifetime <= 0.0f)
	{
		useLifeTime = false;
	}

	if(id == 0)
	{
		// Generate new id?
		id = game->scene->GenerateEntityId();
	}
}

void Entity::Update()
{
	if(lifetime > 0.0f)
	{
		useLifeTime = true;
	}
	if(useLifeTime == true)
	{
		lifetime -= game->time->deltaTime;
		if(lifetime <= 0.0f)
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

void Entity::FixedUpdate()
{
	for(auto& comp : components)
	{
		if(comp->IsEnabled())
		{
			comp->FixedUpdate();
		}
	}
	for(auto& child : children)
	{
		if(child->active)
		{
			child->FixedUpdate();
		}
	}
}

void Entity::AddChild(Entity* ent)
{
	children.push_back(ent);
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

void Entity::AddComponent(Component* comp)
{
	components.push_back(comp);
	comp->owner = this;
	comp->Init();
	TriggerEvent("AddComponent", comp);
	Log("Entity::AddComponent: Calling ", "SetComponent"+type::Unscope(type::Name(comp)));
	TriggerEvent("OnSetComponent"+type::Unscope(type::Name(comp)), comp);
}

void Entity::Deinit()
{
	Object::Deinit();

	Log("Entity::Destroy: Destroying ", components.size()," components.");
	for(auto& compo : components)
	{
		compo->Deinit();
	}
//	components.clear();

	Log("Entity::Destroy: Destroying ", children.size()," children.");
	for(auto& child : children)
	{
		child->Deinit();
	}
}

void Entity::Destroy()
{
	Log("Entity::Destroy: Touch.");
	Object::Destroy();
	Log("Entity::Destroy: super called.");

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

Component* Entity::GetComponent(string type)
{
	string unscoped = type::Unscope(type);
	for(auto& comp : components)
	{
		if(type::Name(comp) == type || type::Name(comp) == unscoped || type::Unscope(type::Name(comp)).find(unscoped) != -1)
		{
			return comp;
		}
	}
	return nullptr;
}

string Entity::GetLayer()
{
	if(layer == "Main" && parent)
	{
		return parent->GetLayer();
	}
	else
	{
		return layer;
	}
}

void Entity::SetLayer(string layer)
{
	this->layer = layer;
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
			TriggerEvent("OnSetComponent"+type::Unscope(type::Name(comp)), comp);
			components.erase(components.begin()+i);
			break;
		}
	}
	Log("Removed component");
}

template<typename T>
void Entity::RemoveComponent()
{
	for(uint i = 0; i < components.size(); ++i)
	{
		Component* comp = components[i];
		if(type::Name<T>() == type::Name(comp))
		{
			TriggerEvent("RemoveComponent", comp);
			comp->Destroy();
			components.erase(components.begin()+i);
			TriggerEvent("OnSetComponent"+type::Unscope(type::Name<T>()), nullptr);
		}
	}
}

void Entity::RemoveComponent(string type)
{
	for(uint i = 0; i < components.size(); ++i)
	{
		Component* comp = components[i];
		string t = type::Name(comp);
		if(t == type || type::Unscope(t) == type)
		{
			TriggerEvent("RemoveComponent", comp);
			comp->Destroy();
			components.erase(components.begin()+i);
			TriggerEvent("OnSetComponent"+type::Unscope(t), nullptr);
		}
	}
}

Entity* Entity::GetParent()
{
	return parent;
}

void Entity::SetParent(Entity* ent)
{
	if(parent)
	{
		parent->RemoveChild(ent);
	}
	else
	{
		game->scene->RemoveEntity(this, false);
	}
	ent->AddChild(this);
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
{/*
	Script::Class<Entity, Object> inst;
	inst.Var("name", &Entity::name);
//	inst.Var("id", &Entity::id);
//	inst.Var("layer", &Entity::layer);
	inst.Var("active", &Entity::active);
	inst.Var("lifetime", &Entity::lifetime);

	inst.Func("Init", &Entity::Init);
	inst.Func("Update", &Entity::Update);
	inst.Func("Destroy", &Entity::Destroy);
	inst.Func("AddComponent", (Component* (Entity::*)(string))&Entity::AddComponent);
	inst.Func("GetComponent", (Component* (Entity::*)(string))&Entity::GetComponent);
	inst.Func("RemoveComponent", (void (Entity::*)(string))&Entity::RemoveComponent);
	inst.Func("Clone", &Entity::Clone);
	inst.Func("GetChild", &Entity::GetChild);

	inst.Prop<string>("layer", &Entity::GetLayer, &Entity::SetLayer);
	inst.Prop<uint32_t>("id", &Entity::GetId);
	inst.Prop<Entity*>("parent", &Entity::GetParent, &Entity::SetParent);
*/}


/*******************************************************************************
*
*		Saving and Loading
*
*******************************************************************************/

void Entity::Save(Properties* prop) const
{
	prop->type = "entity";
	prop->id = name;

	prop->Set("id", id);
	prop->Set("tags", tags);
	prop->Set("layer", layer);
	prop->Set("active", active);
	prop->Set("lifetime", lifetime);

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

	prop->GetId(name);
	prop->Get("id", id);
	prop->Get("tags", tags);
	prop->Get("layer", layer);
	prop->Get("active", active);
	prop->Get("lifetime", lifetime);

	Log("Entity::Load: Loaded basic properties.");

	deque<Properties*> c = prop->GetChildren("component");
	Log("Entity::Load: Component nodes loaded.");
	Component* component = nullptr;
	for(auto& comp : c)
	{
		Log("Loading component props, '"+comp->type+" "+comp->id+"'.");
		if(comp->id != "Transform")
		{
			if(component = GetComponent(comp->id))
			{
				component->Load(comp);
			}
			else
			{
				component = AddComponent(comp->id, comp);
			}
			Log("Entity::Load: Added component, owner = ", component->owner);
			if(comp->id == "Camera")
			{
				game->scene->mainCamera = dynamic_cast<Camera*>(component);
				Log("Added Camera component, now mainCamera = ", game->scene->mainCamera);
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

	string file = "";
	prop->Get("file", file);
	if(file != "")
	{
		Log("Loading entity from file path");
		ptr<Properties> fp {new Properties};
		fp->LoadFile(file);
		string type = type::Unscope(type::Name(this));
		to_lower(type);
		if(Properties* pc = fp->GetChild(type))
		{
			Load(pc, true);
		}
	}
}

void Entity::Load(Properties* prop, bool init)
{
	Load(prop);
	if(init)
	{
		Init();
	}
}

Component *Entity::AddComponent(string type)
{
	return AddComponent(type, nullptr);
}

Component* Entity::AddComponent(string type, Properties* prop)
{
	printf("Adding comp via string of type\n");
	auto val = Component::registry.Get(type);
	Component* result = nullptr;
	if(val == nullptr)
	{
		Log("No function for this component type.");
		return nullptr;
	}
	else
	{
		Log("Adding component");
		result = (*val)();
		result->owner = this;
		if(result->initBeforeLoad)
		{
			AddComponent(result);
			if(prop) result->Load(prop);
		}
		else
		{
			if(prop) result->Load(prop);
			AddComponent(result);
		}
	}

	if(type == "Camera")
	{
		game->scene->mainCamera = dynamic_cast<Camera*>(result);
	}

	return result;
}

}
