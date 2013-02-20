/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-11-02 by Taylor Snead
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

#include "Joint.hpp"

// External
#include <btBulletDynamicsCommon.h>

// Internal
#include "TypeInfo.hpp"
#include "Entity.hpp"
#include "Rigidbody.hpp"

namespace sfs
{

Joint::Joint() :
	Component(nullptr)
{
}

Joint::~Joint()
{
}

/*******************************************************************************
 *
 *		Game Loop
 *
 *******************************************************************************/

void Joint::Init()
{
	if(Rigidbody* rb = owner->GetComponent<Rigidbody>())
	{
		rigidbody = rb;
	}
}

void Joint::OnAddComponent(Component* comp)
{
	if(Type(comp) == "Rigidbody")
	{
		rigidbody = dynamic_cast<Rigidbody*>(comp);
	}
}

void Joint::OnRemoveComponent(Component* comp)
{
	if(Type(comp) == "Rigidbody")
	{
		rigidbody = nullptr;
	}
}

}
