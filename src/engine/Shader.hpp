/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-10-29 by Taylor Snead
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
#include "stdafx.hpp"

// External

// Internal

namespace sfs
{
/** @addtogroup Engine
 *	@{
 */
/** @addtogroup Rendering
 *	@{
 */

/**
 *	@details
 *
 *	@todo
 *
 */
class Shader
{
public:
	Shader()
	{
	}

	Shader(string name) :
		name(name)
	{
		LoadFile(name);
	}

	void LoadFile(string file);

	operator string()
	{
		return name;
	}

	operator uint32_t()
	{
		return id;
	}

	string name = "";
	uint32_t id = 0;
};

/// @}
/// @}
}
