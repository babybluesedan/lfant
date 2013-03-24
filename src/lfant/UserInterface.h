/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-02-13 by Taylor Snead
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
#include <forward_list>

// Internal
#include <lfant/ptr.h>
#include <lfant/Subsystem.h>

/*
   namespace CEGUI
   {
   class OpenGL3Renderer;
   class Window;
   class WindowManager;
   class System;
   class GUIContext;
   }
 */

/*
   namespace Rocket
   {
   namespace Core
   {

   class Context;
   class ElementDocument;

   }
   }
 */

namespace lfant
{

namespace gui
{
class Renderer;
class System;
class FileSystem;
}

class UserInterface : public Subsystem
{
public:
	UserInterface();
	virtual ~UserInterface();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	virtual void CreateWindow(string fileName);
	virtual void RemoveWindow(string fileName);

	virtual void OnKey(int key, int mode);
	virtual void OnChar(char key);
	virtual void OnMouseButton(int btn, int pressed);
	virtual void OnMouseMove(int x, int y);
	virtual void OnWindowResize(uint width, uint height);

protected:
	/*
	   CEGUI::OpenGL3Renderer* renderer;
	   CEGUI::Window* rootWindow;
	   CEGUI::WindowManager* windowManager;
	   vector<CEGUI::Window*> windows;
	   CEGUI::System* system;
	   CEGUI::GUIContext* context;
	 */

	/*
	   ptr<Rocket::Core::Context> context;
	   ptr<gui::Renderer> renderer;
	   ptr<gui::System> system;
	   ptr<gui::FileSystem> fileSystem;
	   forward_list< ptr<Rocket::Core::ElementDocument> > documents;
	 */

	bool resized = false;
	uvec2 size;
};

}
