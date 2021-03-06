/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-11-01 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Collider.h>

// Internal
#include <lfant/Rigidbody.h>
#include <lfant/Transform.h>
#include "lfant/Console.h"

// External
#include <btBulletCollisionCommon.h>

namespace lfant
{

void Collider::Serialize(Properties* prop)
{
	Component::Serialize(prop);

	prop->Value("size", &size);
}

void Collider::Init()
{
	TriggerEvent("SetComponentCollider", this);
	ConnectEvent(SENDER(owner, SetScale), RECEIVER(this, OnSetScale));
	ConnectEvent(SENDER(owner, SetComponentRigidbody), rigidbody);
//	OnSetScale(vec3(1));
	SetSize(size);
}

void Collider::OnSetScale(vec3 scale)
{
//	GetShape()->setLocalScaling(vec3_cast<btVector3>(owner->transform->GetWorldScale()));
	SetSize(size);
}

vec3 Collider::GetSize() const
{
	return size;
}

void Collider::SetSize(vec3 size)
{
	btVector3 newSize = vec3_cast<btVector3>(owner->transform->GetWorldScale()*size);
	GetShape()->setLocalScaling(newSize);
	this->size = size;
}

}
