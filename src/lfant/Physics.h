/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-29 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
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

#include <lfant/Subsystem.h>
#include <lfant/Joint.h>
#include <lfant/Vertex.h>
#include <lfant/Rigidbody.h>

class btManifoldPoint;
struct btCollisionObjectWrapper;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;

namespace lfant
{
class Entity;
class Rigidbody;
class Joint;

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Physics
 *	 @{
 */

struct GravPoint
{
	string name;
	vec3* point;
	float force;

	GravPoint(string name, vec3* pt, float fc) :
		name(name), point(pt), force(fc)
	{
	}
};

/**
 *	Physics
 */
class Physics : public Subsystem
{
	friend class Rigidbody;
public:

	struct RaycastHit
	{
		Vertex vertex;
		float distance;
		//Collider* collider;
		Entity* entity;
	};

	Physics();
	virtual ~Physics();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	//void AddConstraint(ConstraintType ct, Entity *p1, Entity *p2);
	void AddJoint(Joint* joint);
	void RemoveJoint(Joint* joint);

	void AddRigidbody(Rigidbody* ent);
	void RemoveRigidbody(Rigidbody* ent, bool destroy = false);

	/**
	 *	Returns the gravity point with the given name.
	 *	@param name The name of the GravPoint to return.
	 */
	GravPoint* GetGravityPoint(string name);

	/**
	 *	Adds or sets the gravity point with the given name.
	 *	\details
	 *		If a gravity point with the given name exists, then its point
	 *		and force are set according to the other parameters.
	 *	@param name The name of the gravity point to set.
	 *	@param point The locational point of gravity.
	 *	@param force The gravitational force of this point.
	 */
	void SetGravityPoint(string name, vec3 point, float force);
	void SetGravityPoint(string name, vec3& point, float force);
	void SetGravityPoint(string name, float force);

	/**
	 *	Casts a ray and returns the first collider it intersects with.
	 *	@param origin The point to begin the ray at.
	 *	@param direction The direction to cast the ray.
	 *	@param distance The distance to cast the ray. Defaults to infinity as 0.0f.
	 */
	RaycastHit Raycast(vec3 origin, vec3 direction, float distance = 0.0f);

	/**
	 *	Casts a ray and returns all the colliders it intersects with.
	 *	@param origin The point to begin the ray at.
	 *	@param direction The direction to cast the ray.
	 *	@param distance The distance to cast the ray. Defaults to infinity as 0.0f.
	 */
	vector<RaycastHit> RaycastAll(vec3 origin, vec3 direction, float distance = 0.0f);

protected:
	virtual void ApplyGravity()
	{
	}

private:
	static bool OnCollide(string func, btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0,
	                      const btCollisionObjectWrapper* colObj1, int partId1, int index1);

	static bool OnCollideEnter(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0,
	                           const btCollisionObjectWrapper* colObj1, int partId1, int index1);

	static bool OnCollideStay(btManifoldPoint& cp, void* body0, void* body1);

	static bool OnCollideExit(void* userPersistentData);

	btDiscreteDynamicsWorld* world;
	btBroadphaseInterface* broadphase;
	btCollisionDispatcher* dispatcher;
	btConstraintSolver* solver;
	btDefaultCollisionConfiguration* collisionConfig;

	vector<GravPoint> gravityPoints;
};

/** @} */
/** @} */
}
