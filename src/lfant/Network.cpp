/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-19 by Taylor Snead
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

#include <lfant/Network.h>

// External

// Internal

#include <lfant/Console.h>
#include <lfant/Thread.h>

#include <lfant/network/Connection.h>

namespace lfant
{

Network::Network()
{
	//Init();
}

Network::~Network()
{
}

void Network::Init()
{
	Log("Network::Init: Touch.");
}

void Network::Update()
{
	for(auto& con : connections)
	{
		con->Update();
	}
}

Connection* Network::GetConnection(string name)
{
	for(auto& con : connections)
	{
		if(con->name == name)
		{
			return con;
		}
	}
	return nullptr;
}

}

