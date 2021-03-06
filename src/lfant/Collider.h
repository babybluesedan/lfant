/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

#include <lfant/stdafx.h>

// External

// Internal

#include <lfant/Component.h>

class btCollisionShape;

namespace lfant
{
class Rigidbody;

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Physics
 *	 @{
 */

class Collider : public Component
{
	friend class Rigidbody;
	friend class CharacterController;
public:
	bool trigger;
	//SurfaceMaterial* material;
	vec3 center;

protected:
	Collider()
	{
	}
	virtual ~Collider()
	{
	}


	virtual void Serialize(Properties* prop);

	virtual void Init();
	virtual void OnSetScale(vec3 scale);

	virtual btCollisionShape* GetShape() = 0;

	vec3 GetSize() const;
	virtual void SetSize(vec3 size);

	Rigidbody* rigidbody;
	vec3 size = vec3(1);

private:

};

/** @} */
/** @} */
}
