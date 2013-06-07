/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-12-19 by Taylor Snead
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

namespace lfant {
namespace galaga {

/** @addtogroup Galaga
 *	@{
 */
/** @addtogroup Entities
 *	@{
 */

/**
 *
 */
class Player : public lfant::Component
{
	DECLARE_COMP(Player)
public:
	Player()
	{
	}
	virtual ~Player()
	{
	}

	virtual void Init();
	virtual void Update();

	virtual void Move(string axis, float value);

	void Load(Properties* prop);
	void Save(Properties *prop);

	void OnSetRigidbody(Component* comp);

protected:

	float movementSpeed = 3.0f;
	float lookSpeed = 0.01f;
	float jumpHeight = 2.0f;
	float bulletSpeed = 5.0f;
	bool mouseLook = true;

	int meshCount = 0;
	ivec2 lastMouse;
	bool haveRigidbody = false;

private:

};

/// @}
/// @}
}
}
