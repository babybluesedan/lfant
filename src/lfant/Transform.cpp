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
#include <glm/gtx/euler_angles.hpp>

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
	vec3 rot = radians(rotation);
	rot.y = -rot.y;
	return quat(rot);
//	return quat(radians(rotation));
}

void Transform::SetRotationQuat(quat rot)
{
//	rotationQuat = rot;
//	rotation = degrees(eulerAngles(rot));
//	TriggerEvent("SetRotation", rotation);
	SetRotation(degrees(eulerAngles(rot)));
	TriggerEvent("SetRotation", rot);
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
	return matrix[3].xyz;
	/*
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldPosition() + GetPosition();
	}
	else
	{
		return GetPosition();
	}
	*/
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
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldRotationQuat() * GetRotationQuat();
	//	return GetRotationQuat() * owner->parent->transform->GetWorldRotationQuat();
	}
	else
	{
		return GetRotationQuat();
	}
}

void Transform::SetWorldRotationQuat(quat rot)
{
	SetRotation(degrees(eulerAngles(rot)) - parent->GetWorldRotation());
}

vec3 Transform::GetWorldRotation()
{
	if(owner->parent)
	{
		return owner->parent->transform->GetWorldRotation() + GetRotation();
	}
	else
	{
		return rotation;
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
	SetMatrix();
//	SetDirection();
}

void Transform::SetMatrix()
{
	vec3 scl = GetScale();
	if(scl == vec3(0))
	{
		matrix = mat4(0);
		return;
	}
	
	vec3 pos = GetPosition();
//	pos.x = -pos.x;
//	vec3 rot = radians(GetRotation());
	quat rot = GetRotationQuat();
	matrix = mat4(1);

	if(owner->parent)
	{
		matrix = owner->parent->transform->matrix;
	}
	else
	{
	//	scl.x = -scl.x;
	}

//	matrix = glm::translate(mat4(), pos) * mat4_cast(rot) * glm::scale(mat4(), scl);
	
	if(pos != vec3(0))
	{
		matrix = glm::translate(matrix, pos);
	}
	
//	if(rot != vec3(0))
	{
	//	matrix = glm::rotate(matrix, rot.x, vec3(1,0,0));
	//	matrix = glm::rotate(matrix, rot.y, vec3(0,1,0));
	//	matrix = glm::rotate(matrix, rot.z, vec3(0,0,1));
		matrix *= mat4_cast(rot);
	}
	
	if(scl != vec3(1))
	{
		matrix = glm::scale(matrix, scl);
	}
	
/*	if(owner->parent)
	{
		matrix = owner->parent->transform->GetMatrix() * matrix;
	//	matrix = matrix * owner->parent->transform->GetMatrix();
	}*/
}

mat4 Transform::GetMatrix()
{
//	mat4 matrix = this->matrix;
//	matrix[3].x = -matrix[3].x;
	return matrix;
}

void Transform::SetDirection()
{
/*
	vec3 rot = radians(GetWorldRotation());

//	rot.y = -rot.y;	
//	rot.x = -rot.x;
	
	direction = vec3(
		cos(rot.x) * sin(rot.y),
		sin(rot.x),
		cos(rot.x) * cos(rot.y)
		);
//	direction.x = -direction.x;
	
	right = vec3(
		sin(rot.y - pi / 2.0f),
		0,
		cos(rot.y - pi / 2.0f)
		);	
*/
//	mat4 matrix = GetWorldMatrix();
//	right = vec3(matrix[0].xyz);
//	up = vec3(matrix[1].xyz);
//	direction = vec3(matrix[2].xyz);

//	right = vec3(matrix[0][0], matrix[1][0], matrix[2][0]);
//	up = vec3(matrix[0][1], matrix[1][1], matrix[2][1]);
//	direction = vec3(matrix[0][2], matrix[1][2], matrix[2][2]);
//	up = cross(right, direction);
}

void Transform::Translate(vec3 pos)
{
//	Log("Translating by ", lexical_cast<string>(vec3(-pos.x, pos.y, pos.z)));
	SetPosition(GetPosition() + pos);
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

vec3 Transform::GetDirection()
{
	return matrix[2].xyz;
//	return vec3(-matrix[2].x, matrix[2].y, -matrix[2].z);
//	return direction;
}

vec3 Transform::GetRight()
{
//	Log("Getting right, ", lexical_cast<string>(right));
	return matrix[0].xyz;
//	return vec3(-matrix[0].x, matrix[0].y, -matrix[0].z);
//	return right;
}

vec3 Transform::GetUp()
{
	return matrix[1].xyz;
//	return cross(GetDirection(), GetRight());
//	return up;
}

}
