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
#include <lfant/Subsystem.h>
#include <lfant/Joint.h>
#include <lfant/Vertex.h>
#include <lfant/Rigidbody.h>

// External

class btManifoldPoint;
struct btCollisionObjectWrapper;
class btDynamicsWorld;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btPersistentManifold;

namespace lfant {

namespace physics {
class DebugRenderer;
}

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
	string name = "";
	vec3* point = nullptr;
	float force = 1.0f;

	GravPoint(string name, vec3* pt, float fc) :
		name(name), point(pt), force(fc)
	{
	}

	GravPoint() {}
};

struct ContactPoint
{
	vec3 pos;
	vec3 normal;
};

struct Collision
{
	Rigidbody* first = nullptr;
	Rigidbody* second = nullptr;
	std::deque<ContactPoint> contacts;
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

	Physics(Game* game);
	virtual ~Physics();


	virtual void Serialize(Properties* prop);

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Deinit();

	//void AddConstraint(ConstraintType ct, Entity *p1, Entity *p2);
	void AddJoint(Joint* joint);
	void RemoveJoint(Joint* joint);

	void AddRigidbody(Rigidbody* ent);
	void RemoveRigidbody(Rigidbody* ent);

	vec3 GetGravity() const;
	void SetGravity(vec3 grav);

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
	void SetGravityPoint(string name, vec3* point, float force);
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
	std::deque<RaycastHit> RaycastAll(vec3 origin, vec3 direction, float distance = 0.0f);

protected:
	virtual void ApplyGravity()
	{
	}

private:
//	static bool OnCollide(string func, btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0,
//						  const btCollisionObjectWrapper* colObj1, int partId1, int index1);
	static void OnCollide(string func, btPersistentManifold* manifold);

//	static bool OnCollideEnter(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0,
//							   const btCollisionObjectWrapper* colObj1, int partId1, int index1);
	static void OnCollideEnter(btPersistentManifold* manifold);

	static bool OnCollideStay(btManifoldPoint& cp, void* body0, void* body1);

//	static bool OnCollideExit(void* userPersistentData);
	static void OnCollideExit(btPersistentManifold* manifold);

	static void OnTick(btDynamicsWorld* world, double delta);

	ptr<btDiscreteDynamicsWorld> world;
	ptr<btBroadphaseInterface> broadphase;
	ptr<btCollisionDispatcher> dispatcher;
	ptr<btConstraintSolver> solver;
	ptr<btDefaultCollisionConfiguration> collisionConfig;

	ptr<physics::DebugRenderer> debugRenderer;

	std::deque<Rigidbody*> bodies;
	std::deque<GravPoint> gravityPoints;
//	vec3 initGravity = vec3(0, -9.81, 0);
};

/** @} */
/** @} */
}
