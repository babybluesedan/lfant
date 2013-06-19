/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2013-06-04 by Taylor Snead
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

// External

// Internal
#include <lfant/Component.h>
//#include <galaga/Item.h>

namespace lfant {
namespace galaga {

//class Item; // inherits Component
// @todo Should items be their own components on an entity
//		or inherit Item to be on that entity
//		or independent of the Component system.
// Requirements:
//	- Positional change when equipped. Possibly for unequiped as well. (Item-specific)
//	- 

class Inventory;

class Item : public lfant::Component
{
	friend class Inventory;
	DECLARE_COMP(Item)
public:
	Item();
	virtual ~Item();

	void Save(Properties* prop);
	void Load(Properties* prop);

	void Init();

	void Use(byte mode);

protected:
	void Equip(bool val);
	void EndUse();

	float mass = 1.0f;
	vec3 initialPosition {0};
	vec3 equippedPosition {0};
	bool equipped = false;
	Inventory* inventory = nullptr;

private:
};

/** @addtogroup Galaga
 *	@{
 */
/** @addtogroup Entities
 *	@{
 */

/**
 *
 */
class Inventory : public lfant::Component
{
	friend class Item;
	DECLARE_COMP(Inventory)
public:
	Inventory();
	~Inventory();

	void Save(Properties* prop);
	void Load(Properties* prop);

	void Init();

	void AddItem(Item* item);
	void RemoveItem(Item* item);
	void RemoveItem(string name);
	void RemoveItem(uint32_t idx);

	Item* GetItem(string name);
	Item* GetItem(uint32_t idx);

	void Equip(string name);
	void Equip(uint32_t idx);
	void UseItem(byte mode);

protected:

	void EndUseItem();

	deque<Item*> items;

	/// Maximum amount of items allowed to be held.
	uint32_t itemLimit = 0;

	/// Maximum total weight of items allowed.
	float massLimit = 0.0f;

	float mass = 0.0f;

	uint32_t equippedItem = 0;

private:

};

/// @}
/// @}
}
}
