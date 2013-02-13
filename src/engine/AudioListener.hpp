/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: Oct 29, 2012 by Taylor Snead
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
#include "Audio.hpp"
#include "Component.hpp"

namespace sfs
{
/** @addtogroup Engine
 *	@{
 */
/** @addtogroup Audio
 *	@{
 */

/**
 *	@details
 *
 *	@todo
 *
 */
class AudioListener: public Component
{
public:
	float gain;
	vec3 velocity;

protected:

private:

};

/// @}
/// @}

} /* namespace sfs */
