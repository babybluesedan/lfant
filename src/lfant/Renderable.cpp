/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-02-18 by Taylor Snead
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

#include <lfant/Renderable.h>

// Internal
#include <lfant/Console.h>

// External

namespace lfant
{

Renderable::Renderable()
{
}

Renderable::~Renderable()
{
}

void Renderable::Init()
{
	Component::Init();
//	BeginRender();
}

void Renderable::PostUpdate()
{
	Component::PostUpdate();
	if(loaded)
	{
		Render();
	}
}

void Renderable::Deinit()
{
	Component::Deinit();
	if(loaded)
	{
		EndRender();
	}
}

void Renderable::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}
	loaded = true;
}

void Renderable::EndRender()
{
	loaded = false;
}

}
