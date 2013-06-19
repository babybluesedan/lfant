/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013, by LazyFox Studios
*	Created: 2012-08-13 by Taylor Snead
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
#include <lfant/stdafx.h>

// Internal
#include <lfant/Component.h>

// External

namespace lfant {

class Animation : public Component
{
public:

	class Clip
	{
		string name = "";
		float length = 0.0f;
		Range<uint16_t> frames {0,1};
	};

	Animation();
	~Animation();

	void Save(Properties* prop);
	void Load(Properties* prop);

	void Init();

	void Play(string anim = "");
	void Stop(string anim = "");

	Clip* GetClip(string name);

protected:
	bool playAutomatically = false;
	deque<Clip> clips;

private:
};

}