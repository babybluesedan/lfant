/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-08-08 by Taylor Snead
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

#include <lfant/FileSystem.h>

// External
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>

// Internal

#include <lfant/Game.h>
#include <lfant/Settings.h>

#include <lfant/Console.h>

namespace lfant
{

FileSystem::FileSystem() :
	gameFolder(absolute(path("../..")).string())
{
}

FileSystem::~FileSystem()
{
}

void FileSystem::Init()
{
	path p("../../ssystem.cfg");
	if(exists(p))
	{
		// Load settings from system.cfg
	}
	else
	{
		// Use default settings
		gameFolder = "../..";
#	if WINDOWS
		const string home = getenv("USERPROFILE");
#	elif UNIX
		const string home = getenv("HOME");
#	endif
		userFolder = home + "/Documents/My Games/" + game->settings->GetValue("general.orgname") + "/" + game->settings->GetValue("general.gamename");
		game->settings->LoadSettings();

		// Save these to system.cfg
	}
}

string FileSystem::ConvertPath(string curr)
{
	for(auto& chr : curr)
	{
		// Supported grouping characters here.
		if(chr == '\\' || chr == ':' || chr == '/')
		{
			// Convert all grouping chars to forward slashes for directory management.
#		if UNIX
			chr = '/';
#		elif WINDOWS
			chr = '\\';
#		endif
		}
	}
	return curr;
}

path FileSystem::GetGamePath(string name)
{
	path result(gameFolder + "/assets/" + name);

	if(exists(result))
	{
		return result;
	}
	Log("FileSystem::GetGameFile: File not found \'"+result.string()+"\'.");
//	game->Exit();
	return "";
}

path FileSystem::GetUserPath(string name)
{
	path result(ConvertPath(userFolder + "/" + name));

	if(exists(result))
	{
		return result;
	}
	Log("FileSystem::GetGameFile: File not found \'"+result.string()+"\'.");
	return "";
}

vector<path> FileSystem::GetGameFiles(string dir)
{
	vector<path> result;
	if(exists(dir) && is_directory(dir))
	{
		copy(directory_iterator(dir), directory_iterator(), back_inserter(result));
	}
	return result;
}

}
