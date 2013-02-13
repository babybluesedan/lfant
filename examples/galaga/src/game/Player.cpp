/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-01-11 by Taylor Snead
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

#include "Player.hpp"

// External

// Internal
#include "Galaga.hpp"
#include "Input.hpp"
#include "Console.hpp"
#include "Time.hpp"
#include "Sprite.hpp"
#include "Entity.hpp"
#include "Renderer.hpp"
#include "Settings.hpp"

using namespace sfs;

void Player::Init()
{
//	owner->AddComponent<Sprite>();
	float f = game->settings->GetValue("player.lookspeed").f();
	if(f != 0.0f)
	{
		lookSpeed = f;
		Log("Looking at ", f);
	}
	f = game->settings->GetValue("player.movementSpeed").f();
	if(f != 0.0f)
	{
		movementSpeed = f;
		Log("Moving at ", f);
	}
}

void Player::Update()
{
	//ivec2 mousePos = game->input->GetMousePos();
	//ivec2 screenRes = game->renderer->GetResolution();
	//transform->rotation += degrees(vec3(lookSpeed * float(screenRes.y/2-mousePos.y), 0, lookSpeed * float(screenRes.x/2-mousePos.x))) * (float)game->time->deltaTime;
	//game->input->SetMousePos(screenRes.x/2, screenRes.y/2);

	if (game->input->GetButtonDown("ShowLoc"))
	{
		Log("Player position: ", transform->position().x, ", ", transform->position().y, ", ", transform->position().z);
	}
	if (game->input->GetButtonDown("ShowRot"))
	{
		Log("Player rotation: ", transform->rotation().x, ", ", transform->rotation().y, ", ", transform->rotation().z);
	}
	float hor = game->input->GetAxis("Horizontal");
	if (hor != 0.0f)
	{
		transform->position += transform->right * float(hor * game->time->deltaTime * movementSpeed);
	}
	float ver = game->input->GetAxis("Vertical");
	if (ver != 0.0f)
	{
		transform->position += float(game->time->deltaTime) * transform->direction * float(ver * movementSpeed);
	}
	float hrot = game->input->GetAxis("HRotation");
	if (hrot != 0.0f)
	{
		transform->rotation += degrees(vec3(0, 0, -hrot*lookSpeed*game->time->deltaTime));
	}
	float vrot = game->input->GetAxis("VRotation");
	if (vrot != 0.0f)
	{
		transform->rotation += degrees(vec3(vrot * lookSpeed * game->time->deltaTime, 0, 0));
	}
	if (game->input->GetButtonDown("Fire"))
	{
		Log("Player fired!");
	}
	if (game->input->GetButtonDown("TesterSetVar"))
	{
		game->console->Input("Tester = 62.76f");
	}
	if (game->input->GetButtonDown("TesterGetVar"))
	{
		game->console->Input("Tester");
	}
	if (game->input->GetButtonDown("TesterHelpMe"))
	{
		game->console->Input("help Tester");
	}
	if (game->input->GetButtonDown("Exit"))
	{
		Log("Calling exit function");
		game->console->Input("Exit()");
	}
}
