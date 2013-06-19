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

#include <lfant/ParticleSystem.h>

// Internal
#include <lfant/Particle.h>
#include <lfant/Random.h>
#include <lfant/Console.h>
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Renderer.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Thread.h>
#include <lfant/Input.h>

// External
#include <GL/glew.h>

namespace lfant {

IMPLEMENT_COMP(ParticleSystem)

ParticleSystem::ParticleSystem()
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init()
{
	material->texture->LoadFile("textures/Default.png");
	BeginRender();
}

void ParticleSystem::Update()
{
	if(paused)
	{
		return;
	}

	if(rate > 0.0f)
	{
		toEmit += rate * game->time->deltaTime;
	//	Log("ParticleSystem updating, toEmit = ", toEmit);
		if(toEmit >= 1.0f)
		{
			Emit(floor(toEmit));
			toEmit -= floor(toEmit);
		}
	}

	for(uint i = 0; i < particles.size(); ++i)
	{
		if(!particles[i])
		{
			particles.erase(particles.begin()+i);
			--i;
			continue;
		}
		if(particles[i]->active)
		{
			particles[i]->Update();

			if(particles[i]->lifetime <= 0.0f)
			{
				Recycle(particles[i]);
			}

			vertexBuffer[i] = particles[i]->position;
			colorBuffer[i] = particles[i]->GetColor();
			sizeBuffer[i] = particles[i]->GetSize();

			if(gravity != vec3(0))
			{
				particles[i]->ApplyForce(gravity);
			}
		}
	}

	for(uint i = 0; i < bursts.size(); ++i)
	{
		if(bursts[i].current <= 0.0f)
		{
			Emit(bursts[i].particles);
			bursts[i].current = bursts[i].time;
		}
		bursts[i].current -= game->time->deltaTime;
	}

	if(game->input->GetButtonDown("ShowFPS"))
	{
		Log("Current particle count: ", GetCount());
	}

}

void ParticleSystem::PostUpdate()
{
	Render();
}

void ParticleSystem::OnDestroy()
{
	EndRender();
}

void ParticleSystem::UpdatePosition(Particle* pt)
{
	for(uint i = 0; i < particles.size(); ++i)
	{
		if(particles[i] == pt)
		{
			vertexBuffer[i] = pt->position;
			colorBuffer[i] = pt->color.start;
			sizeBuffer[i] = pt->GetSize();
		}
	}
}

void ParticleSystem::Load(Properties *prop)
{
	Component::Load(prop);

	for(auto& pb : prop->GetChildren("burst"))
	{
		bursts.emplace_front();
		Burst& burst = bursts[0];
		pb->Get("time", burst.time);
		pb->Get("particles", burst.particles);
		burst.current = burst.time;
	}

	prop->Get("lifetime", lifetime);
	prop->Get("color", color);
	prop->Get("size", size);
	prop->Get("velocity", velocity);

	Log("Loaded velocity: ", lexical_cast<string>(velocity));
//	thread::Sleep(1500);

	prop->Get("radius", radius);
	prop->Get("angle", angle);
//	prop->Get("timeScale", timeScale);
	prop->Get("rate", rate);
	prop->Get("maxParticles", maxParticles);
	prop->Get("dimensions", dimensions);
	prop->Get("gravity", gravity);
	prop->Get("pausable", pausable);
	prop->Get("paused", paused);
	prop->Get("looping", looping);
	prop->Get("inheritTransform", inheritTransform);

	emitterType = (EmitterType)prop->Get<byte>("type");

	material->LoadFile(prop->Get("material"));
}

void ParticleSystem::Save(Properties *prop)
{
	Component::Save(prop);

	/*
	prop->Set("lifetime", lifetime);
	prop->Set("color", color);
	prop->Set("size", size);
	prop->Set("velocity", velocity);

	prop->Set("radius", radius);
	prop->Set("timeScale", timeScale);
	prop->Set("rate", rate);
	prop->Set("maxParticles", maxParticles);
	prop->Set("dimensions", dimensions);
	prop->Set("gravity", gravity);
	prop->Set("pausable", pausable);
	prop->Set("paused", paused);
	prop->Set("looping", looping);
	//	prop->Set("material", material->path);
	*/
}

void ParticleSystem::Emit(uint32_t amount)
{
//	Log("Emitting ", amount, " particles");
	for(uint32_t i = 0; i < amount; ++i)
	{
		Emit();
	}
}

void ParticleSystem::Emit(Particle* pt)
{
	if(!pt)
	{
		if(recycle.size() > 0)
		{
		//	Log("Recycling old particle");
			pt = recycle[0];
			recycle.pop_front();
		}
		else
		{
			if(GetCount() >= maxParticles)
			{
				return;
			}
		//	Log("Creating new particle");
			particles.push_back(pt = new Particle);
			vertexBuffer.push_back(vec3(0));
			colorBuffer.push_back(vec4(1));
			sizeBuffer.push_back(1);
		}
	}

//	pt->system = this;
	pt->SetSizeRange(random::Range(size.start.min, size.start.max), random::Range(size.end.min, size.end.max));
//	pt->SetGravity(gravity);
	pt->color.start = random::Range(color.start.min, color.start.max);
	pt->color.end = random::Range(color.end.min, color.end.max);
	pt->lifetime = random::Range(lifetime.start, lifetime.end);
	pt->Activate(true);
	GenerateVelocity(pt);
	pt->Init();
}

void ParticleSystem::Recycle(Particle* pt)
{
	/*
	if(recycle.size() > glm::ceil(maxParticles/10))
	{
		delete pt;
		vertexBuffer.pop_back();
		colorBuffer.pop_back();
		sizeBuffer.pop_back();
		return;
	}
	*/
	recycle.push_back(pt);
	pt->active = false;
	pt->position = vec3(0);
}

void ParticleSystem::GenerateVelocity(Particle* pt)
{
//	float speed = random::Range(this->speed.start.min, this->speed.start.max);
	vec3 dir;
	vec3 pos;

	switch(emitterType)
	{
	case EmitterType::Cone:
	{
		// Cone emitter
	//	pos = vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius));
		pos = vec3(0);
		dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(-0.1f, 1.0f), random::Range(-angle / 90, angle / 90));
		while(dir == vec3(0.0f))
		{
			// This is to prevent ever getting a 0 direction and generating an empty velocity.
			dir = vec3(random::Range(-angle / 90, angle / 90), random::Range(-0.1f, 1.0f), random::Range(-angle / 90, angle / 90));
		}
		break;
	}
	case EmitterType::Sphere:
	{
		// Sphere emitter
		dir = vec3(random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f), random::Range(-1.0f, 1.0f));
		pos = dir * random::Range(0.0f, radius);
		break;
	}
	case EmitterType::Box:
	{
		// Straight direction emitter
		// Cube volume, radius being half the side length
		pos = vec3(random::Range(-radius, radius), random::Range(-radius, radius), random::Range(-radius, radius));
		dir = vec3(0, 1, 0);
		break;
	}
	case EmitterType::Point:
	{
		// Point emitter
		pos = vec3(0);
		dir = owner->transform->GetDirection();
		break;
	}
	}

	// Square area
//	pt->transform->SetPosition(vec3(random::Range(-radius, radius), 0.0f, random::Range(-radius, radius)));

	if(!inheritTransform)
	{
		pos += owner->transform->GetWorldPosition();
	}
//	Log("Particle position: ", lexical_cast<string>(pos));
	pt->SetPosition(pos);
//	pt->velocity = dir * speed;

	// Beginning and ending velocity range, for an interpolating velocity.
//	pt->velRange.start = random::Range(velocity.start.min, velocity.start.max);
//	pt->velRange.end = random::Range(velocity.end.min, velocity.end.max);

	pt->velocity.start = dir * random::Range(velocity.start.min, velocity.start.max);
	pt->velocity.end = dir * random::Range(velocity.end.min, velocity.end.max);
}

uint32_t ParticleSystem::GetCount()
{
	return particles.size();
}


void ParticleSystem::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

//	if(!material->shader->GetId())
	{
		material->shader->LoadFile("shaders/particles/Diffuse.vert", "shaders/particles/Diffuse.frag"/*, "shaders/particles/Diffuse.geom"*/);
	}

//	if(!material->texture->GetId())
	{
//		Log("Manually loading texture.");
//		material->texture->LoadFile();
	}

	if(material->shader->GetId())
	{
		Log("Adding uniforms..");
		material->shader->AddUniform("MVP");
		material->shader->AddUniform("viewportWidth");
		material->shader->AddUniform("cameraPosition");
		material->shader->AddUniform("systemPosition");
		material->shader->AddUniform("textureSampler");
	}

	CreateBuffer(vertexBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	CreateBuffer(colorBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
	CreateBuffer(sizeBuffer, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
//	Mesh::CreateBuffer(normalBuffer, GL_ARRAY_BUFFER);
//	Mesh::CreateBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

	loaded = true;
}

void ParticleSystem::Render()
{
	if(!material->shader->GetId() || !material->texture->GetId())
	{
		return;
	}

	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
//	glEnable(GL_GEOMETRY_PROGRAM_POINT_SIZE);

	glBindVertexArray(vertexArray);

	glUseProgram(material->shader->GetId());

	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView() * owner->transform->GetMatrix();
	material->shader->SetUniform("MVP", mvp);
	material->shader->SetUniform("cameraPosition", game->scene->mainCamera->owner->transform->GetWorldPosition());
	material->shader->SetUniform("viewportWidth", game->renderer->GetResolution().x);

	if(inheritTransform)
	{
		material->shader->SetUniform("systemPosition", owner->transform->GetWorldPosition());
	}
	else
	{
	//	Log("Rendering particle independent from system.");
		material->shader->SetUniform("systemPosition", vec3(0));
	}

//	material->texture->Bind();
//	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
//	material->shader->SetUniform("textureSampler", material->texture);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertexBuffer.size(), &vertexBuffer[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * colorBuffer.size(), &colorBuffer[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sizeBuffer.size(), &sizeBuffer[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glDrawArrays(GL_POINTS, 0, vertexBuffer.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);

	material->texture->Unbind();

	glBindVertexArray(0);

	glUseProgram(0);

	glDisable(GL_POINT_SPRITE);
	glDisable(GL_PROGRAM_POINT_SIZE);
}

void ParticleSystem::EndRender()
{
	vertexBuffer.Destroy();
//	uvBuffer.Destroy();
	normalBuffer.Destroy();
	indexBuffer.Destroy();
	material->texture->Destroy();
	glDeleteVertexArrays(1, &vertexArray);
	loaded = false;
}

}
