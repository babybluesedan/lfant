/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-08-02 by Taylor Snead
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
#include <lfant/Range.h>
#include <lfant/Material.h>
#include <lfant/Vertex.h>
#include <lfant/Mesh.h>
#include <lfant/Geometry.h>

namespace lfant
{
class Particle;

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Particles
 *	 @{
 */

/**	This class handles a ParticleSystem Component
 *		Gives functionality for emitting particles from this transform. The
 *		emitted particles die after a certain lifetime, and while alive, follow
 *		the given parameters. The particles can be rendered either as quads
 *		(really two triangles, of course) or as point-sprites. Point-sprites
 *		are more efficient, but it's possible that some older hardware doesn't
 *		support it.
 *	@todo
 *		Rendering code for ParticleSystem.
 *		Having the ParticleSystem Component send a call to render itself.
 *		Integrate with Bullet Physics for collision detection, bounciness, friction, and maybe buoyancy.
 */
class ParticleSystem : public Renderable
{
	friend class Renderer;
	friend class Particle;
public:

	/**
	 *	A burst of particles that occurs at a certain interval.
	 */
	struct Burst
	{
		float time = 5.0f;
		float current;
		uint32 particles = 10;
	};

	/**
	 *	Supported emitter types.
	 */
	enum class EmitterType : uint8
	{
		Point = 0,
		Box,
		Sphere,
		Cone
	};

	struct ParticleVertex
	{
		vec3 position = vec3(0.0f);
		u8vec4 color {255};
		float size {1.0f};

		ParticleVertex()
		{
		}

		ParticleVertex(vec3 pos, vec4 col, float size) :
			position(pos), color(col), size(size)
		{
		}
	};

	template<typename T>
	struct Field
	{
		enum class CurveType : byte
		{
			Linear,

		};
		Range<T> start;
		Range<T> end;
		CurveType curveType = CurveType::Linear;

		void Serialize(Properties* prop);
		void Save(Properties* prop) const;
	};

	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void Init();
	virtual void Update();
	virtual void PostUpdate();
	virtual void OnDestroy();

	void BeginRender();
	void Render();
	void EndRender();

	void Save(Properties* prop) const;
	void Serialize(Properties *prop);

	/**
	 *	Emits a specific amount of particles
	 *	@param amount The amount of particles to emit.
	 */
	void Emit(uint32 amount);
	void Emit(Particle* pt = nullptr);
	void Recycle(Particle* pt);
	void Clear();
	uint32 GetCount();
	void GenerateVelocity(Particle* pt);

	// Particle properties
	Range<float> lifetime = {1.0f, 5.0f};
	Range<Range<vec4>> color = {{vec4(0.01f), vec4(1.0f)}, {vec4(0.01f), vec4(1.0f)}};
	Range<Range<float>> size = {{0.1f, 1.0f}, {0.5f, 2.0f}};
//	Range<Range<float>> speed;
	Range<Range<vec3>> velocity = {{vec3(1), vec3(5)}, {vec3(2), vec3(10)}};

	Material material;

	vec3 dimensions = vec3(1.0f);
	vec3 gravity = vec3(0.0f, 0.0f, 0.0f);

	// Emitter properties
	uint32 maxParticles = 1500;
	float radius = 1.0f;
	float angle = 30.0f;
	float startDelay;
	float time;
	float duration;
//	float timeScale = 1.0f;
	float rate = 1.5f;

	bool pausable = true;
	bool paused = false;
	bool looping = true;
	bool inheritTransform = true;

	EmitterType emitterType = EmitterType::Sphere;

	std::deque<Particle> particles;
//	Buffer<Particle> particles;
	std::deque<Burst> bursts;
	std::deque<Particle*> recycle;


protected:

	void UpdatePosition(Particle* pt);
	void OnSetPosition();

	float toEmit = 0.0f;

private:
	bool rewriteBuffer = false;
	Buffer<ParticleVertex> particleBuffer;
};

/** @} */
/** @} */
}
