/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-26 by Taylor Snead
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
#include <lfant/Transform.h>

// External

// Internal

#include <lfant/Entity.h>
#include <lfant/util/Math.h>

#include <lfant/Console.h>

namespace lfant
{

Transform::Transform()
{
}

Transform::~Transform()
{

}

void Transform::Save(Properties *prop)
{
	Component::Save(prop);

	prop->Set("position", position);
	prop->Set("rotation", rotation);
	prop->Set("scale", scale);
}

void Transform::Load(Properties *prop)
{
	Log("Loading transform from type '"+prop->type+"', id '"+prop->id+"'.");
	Component::Load(prop);

	prop->Get("position", position);
	prop->Get("rotation", rotation);
	prop->Get("scale", scale);

	Log("Loaded position: "+lexical_cast<string>(position));
}

vec3 Transform::GetPosition()
{
	return position;
}

void Transform::SetPosition(vec3 pos)
{
	position = pos;
	TriggerEvent("SetPosition", position);
}

quat Transform::GetRotationQuat()
{
	return quat(rotation);
}

void Transform::SetRotationQuat(quat rot)
{
	rotationQuat = rot;
	rotation = degrees(eulerAngles(rot));
	TriggerEvent("SetRotation", rotation);
}

vec3 Transform::GetRotation()
{
	return rotation;
}

void Transform::SetRotation(vec3 rot)
{
	rollover(rot.x, 0.0f, 360.0f);
	rollover(rot.y, 0.0f, 360.0f);
	rollover(rot.z, 0.0f, 360.0f);

	rotation = rot;
//	rotationQuat = quat(rotation);
//	Log("Setting rotation to ", lexical_cast<string>(rotation));

	TriggerEvent("SetRotation", rot);
//	TriggerEvent("SetRotation", rotationQuat);
}

vec3 Transform::GetScale()
{
	return scale;
}

void Transform::SetScale(vec3 scl)
{
	scale = scl;
	TriggerEvent("SetScale", scale);
}

vec3 Transform::GetWorldPosition()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldPosition() + GetPosition();
	}
	else
	{
		return GetPosition();
	}
}

void Transform::SetWorldPosition(vec3 pos)
{
	if(owner->parent)
	{
		SetPosition(pos - owner->parent->transform->GetWorldPosition());
	}
	else
	{
		SetPosition(pos);
	}
}

quat Transform::GetWorldRotationQuat()
{
	return quat(GetWorldRotation());
}

void Transform::SetWorldRotationQuat(quat rot)
{
	SetRotation(eulerAngles(rot) - parent->GetWorldRotation());
}

vec3 Transform::GetWorldRotation()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldRotation() + GetRotation();
	}
	else
	{
		return GetRotation();
	}
}

void Transform::SetWorldRotation(vec3 rot)
{
	if(owner->parent)
	{
		SetRotation(rot - owner->parent->transform->GetWorldRotation());
	}
	else
	{
		SetRotation(rot);
	}
}

vec3 Transform::GetWorldScale()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldScale() * GetScale();
	}
	else
	{
		return GetScale();
	}
}

void Transform::SetWorldScale(vec3 scl)
{
	if(owner->parent)
	{
		SetScale(scl - owner->parent->transform->GetWorldScale());
	}
	else
	{
		SetScale(scl);
	}
}

void Transform::Update()
{
//	Log("Transform updating");
	SetDirection();
//	SetMatrix();
}

void Transform::SetMatrix()
{
	/*
//	mat4 matrix;
	matrix = mat4(1.0f);
	matrix = glm::translate(mat4(1.0f), GetWorldPosition());
//	matrix *= glm::mat4_cast(GetWorldRotationQuat());
	vec3 rot = radians(GetWorldRotation());
	matrix = glm::rotate(matrix, rot.x, vec3(1,0,0));
	matrix = glm::rotate(matrix, rot.y, vec3(0,1,0));
	matrix = glm::rotate(matrix, rot.z, vec3(0,0,1));
//	matrix *= mat4_cast(rotationQuat);
//	glm::rotate
	matrix = glm::scale(matrix, GetWorldScale());
//	return matrix;
//	Log("Transform position: ", lexical_cast<string>(vec3(matrix[3].xyz)));
*/
}

mat4 Transform::GetMatrix()
{
	vec3 scl = GetWorldScale();
	if(scl == vec3(0))
	{
		return mat4(0);
	}
	
	vec3 pos = GetWorldPosition();
	vec3 rot = radians(GetWorldRotation());
	mat4 matrix = mat4(1.0f);
	
	if(pos != vec3(0))
	{
	//	Log("pos = ", lexical_cast<string>(pos));
		matrix = glm::translate(matrix, pos);
	//	Log("matrix = ", lexical_cast<string>(matrix));
	}
	
	if(rot != vec3(0))
	{
	//	Log("rot = ", lexical_cast<string>(rot));
		matrix = glm::rotate(matrix, rot.x, vec3(1,0,0));
		matrix = glm::rotate(matrix, rot.y, vec3(0,1,0));
		matrix = glm::rotate(matrix, rot.z, vec3(0,0,1));
	//	Log("matrix = ", lexical_cast<string>(matrix));
	}
	
	if(scl != vec3(1))
	{
		matrix = glm::scale(matrix, scl);
	//	Log("matrix = ", lexical_cast<string>(matrix));
	}
	
	return matrix;
}

void Transform::SetDirection()
{
	vec3 rot = radians(GetWorldRotation());
//	Log("Cam rotation: ", lexical_cast<string>(rot));
	direction = vec3(
		cos(rot.x) * sin(rot.y),
		sin(rot.x),
		cos(rot.x) * cos(rot.y)
		);

	right = vec3(
		sin(rot.y - pi / 2.0f),
		0,
		cos(rot.y - pi / 2.0f)
		);

	up = cross(right, direction);
//	right.x *= -1;
}

vec3 Transform::GetWorldRotatedPosition()
{
	if(!owner->parent)
	{
		return vec3(0);
	}

	vec3 newpos(0);
	vec3 parrot = owner->parent->transform->GetWorldRotation();

	newpos.x = position.x*cos(parrot.x) - position.y*sin(parrot.y);
	newpos.y = position.y*cos(parrot.y) - position.x*sin(parrot.x);

	return newpos;
}

void Transform::Translate(vec3 pos)
{
//	Log("Translating by ", lexical_cast<string>(pos));
	SetPosition(position + pos);
}

void Transform::Rotate(vec3 rot)
{
	SetRotation(rotation + rot);

	//rotationQuat = quat(rotation);
	//_rotationQuat = rotate( _rotationQuat, rot.x, xdir );
	//_rotationQuat = rotate( _rotationQuat, rot.y, ydir );
	//_rotationQuat = rotate( _rotationQuat, rot.z, zdir );
//	TriggerEvent("SetRotation", rotation);
}

void Transform::Scale(vec3 scl)
{
	SetScale(scale * scl);
//	TriggerEvent("SetScale", scale);
}

}
