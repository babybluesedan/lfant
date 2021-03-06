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

// Internal
#include <lfant/Component.h>

// External
#include <glm/gtc/quaternion.hpp>

namespace lfant {

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();


	virtual void Serialize(Properties* prop);

	static void ScriptBind();

	virtual void Init();
	virtual void Update();

	dvec3 GetPosition() const;
	void SetPosition(dvec3 pos);
	dvec3 GetWorldPosition() const;
	void SetWorldPosition(dvec3 pos);

	quat GetRotationQuat() const;
	void SetRotationQuat(quat q);
	quat GetWorldRotationQuat();
	void SetWorldRotationQuat(quat q);

	vec3 GetRotation() const;
	void SetRotation(vec3 rot);
	vec3 GetWorldRotation() const;
	void SetWorldRotation(vec3 rot);

	vec3 GetScale() const;
	void SetScale(vec3 scl);
	vec3 GetWorldScale() const;
	void SetWorldScale(vec3 scl);

//	void Translate(vec3 pos);
	void Translate(dvec3 pos);
	void Rotate(vec3 rot);
	void Rotate(vec3 axis, float value);
	void Scale(vec3 scl);

	void SetDirection();
	void SetMatrix();
	const mat4& GetMatrix();

	vec3 GetDirection() const;
	vec3 GetRight() const;
	vec3 GetUp() const;

	vec3 GetRelativeWorldPosition() const;
	vec3 GetRelativeWorldPosition(Entity* ent) const;

	dvec3 position = dvec3(0.0);
	vec3 rotation = vec3(0);
	vec3 scale = vec3(1);

protected:

	void OnSetWorldPos();

	ivec3 GetRelativeSector() const;

	quat rotationQuat;
	mat4 matrix = mat4(1);

//	ivec3 sector;

/*	vec3 up;
	vec3 right;
	vec3 direction;*/

//	Transform* parent = nullptr;
//	bool updateMatrix = false;
//	u8vec3 rotOrder {2,0,1};


private:

};

}
