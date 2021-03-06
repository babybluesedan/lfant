/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

// Internal
#include <lfant/Component.h>

// External

class btTypedConstraint;
class btRigidBody;
class btMotionState;
class btTransform;
class btVector3;
class btCollisionShape;

namespace lfant {

namespace physics {
	class MotionState;
}

class Collider;
class Geometry;


/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Physics
 *	 @{
 */

/**	This class manages physics functionality through the Physics class.
 *		Adds Rigidbody functionality to the Owner Entity, allowing it to
 *		be affected by gravity, forces, collision, constraints, etc. This
 *		makes it fully physics-interactive. For soft-bodies and cloth, look
 *		at the Softbody class.
 *	@todo
 *		Organize Rigidbody some... it's a bit messy.
 */
class Rigidbody : public Component
{
	friend class Physics;
	friend class physics::MotionState;
public:

	// Nested classes
	enum class Mode : byte
	{
		Discrete,
		Continuous,
		ContinuousDynamic
	};

	// ctor and dtor
	Rigidbody();
	virtual ~Rigidbody();

	static void ScriptBind();

	virtual void Serialize(Properties* prop);


	virtual void Enable(bool on);

	// Methods
	btTypedConstraint* GetConstraint(int idx);
	void RemoveConstraint(int idx);

	void ApplyForce(vec3 force, vec3 pos);
	void ApplyCentralForce(vec3 force);

	/**
	 *	Applies a constant acceleration.
	 */
	void Accelerate(vec3 force);

	/**
	 *	Returns the (current) mass of this Rigidbody.
	 */
	float GetMass() const;

	/**
	 *	Sets the mass of this Rigidbody.
	 */
	void SetMass(float mass);

	/**
	 *	Returns this Rigidbody's density.
	 *	\return mass / volume
	 */
	float GetDensity() const;

	/**
	 *	Sets the mass to (density * volume)
	 */
	void SetDensity(float density);

	/**
	 *	Returns the object's current velocity.
	 */
	vec3 GetVelocity() const;

	/**
	 *	Sets the velocity of this object directly.
	 */
	void SetVelocity(vec3 vel);
	void SetAngularVelocity(vec3 vel);

	/**
	 *	Returns the average of the three components of this object's velocity.
	 */
	float GetSpeed() const;

	float GetFriction() const;
	void SetFriction(float f);

	void SetMaxSpeed(float speed) { maxSpeed = speed; }
	float GetMaxSpeed() { return maxSpeed; }

	void SetTrigger(bool is);
	bool IsTrigger() const;

	// Variables
	Mode mode;

	bool useWorldGravity = true;
	bool usePointGravity = false;

	float drag = 1.0f;

	vec3 centerOfMass;

	float gravityMult = 1.0f;

protected:
	virtual void Init();
	virtual void Update();
	virtual void Deinit();


	// Slots
	void OnSetPos(vec3 pos);
	void OnSetRot(quat rot);
	void OnSetScale(vec3 scale);
	void OnSetMesh(Geometry* mesh);
	void OnAddComponent(Component* comp);
	void OnRemoveComponent(Component* comp);

	/// The current mass of this object, in kg.
	float mass = 1.0f;
	bool isTrigger = false;
	float maxSpeed = 0.0f;

	ptr<btRigidBody> body;
	ptr<btMotionState> motionState;
	Collider* collider = nullptr;
	vec3 inertia = vec3(0);
};

/** @} */
/** @} */
}
