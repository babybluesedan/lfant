/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-17 by Taylor Snead
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
#include <lfant/stdafx.h>

// External
#include <deque>

// Internal
#include <lfant/Range.h>
#include <lfant/Shader.h>
#include <lfant/Vertex.h>

#include <lfant/Subsystem.h>

struct GLFWwindow;

namespace lfant
{
class Mesh;
class Sprite;
class Light;
class ParticleSystem;
class Material;
class FrameBuffer;
class Entity;
class Light;

/**	@addtogroup Game
 *	@{
 */
/**	@addtogroup Rendering
 *	@{
 */

/**	This class handles the base Rendering, it should be called by the GameStart class.
 *		This class currently handles OpenGL via GLFW and GLEW, but
 *		it can be configured to use DirectX, if one so desires.
 *		The OpenGL version variable is held here, and a settings
 *		class should change them for use, then reload the rendering
 *		system.
 */
class Renderer : public Subsystem
{
	friend class Game;
	friend class Light;
public:
	Renderer();
	~Renderer();

	void Load(Properties *prop);
	void Save(Properties *prop);

	void Init();
	void PreUpdate();
	void Update();
	void OnDestroy();

	/**
	 *	Opens a new window, using member settings.
	 *	@param close Whether or not to close the current window. Leave at true unless you really know what you're doing.
	 */
	bool OpenWindow();

	// Static callbacks
	static void OnCloseWindow(GLFWwindow* win);
	static void OnSetResolution(GLFWwindow* win, int x, int y);
//	static void OnError(uint source, uint type, uint id, uint severity, int length, const char* message, void* user);
	static void OnError(int error, const char* msg);

	/// Sets the version property
	void SetVersion(byte major, byte minor);
	void SetVersion(Range<int> ver);

	void SetRendering(bool render);


	// Shaders
	Shader* GetShader(string name);
	void AddShader(Shader* shader);

	/**
	 *	Hides, shows, or toggles the mouse cursor.
	 */
	void HideMouse(bool hide = true);

	void AddLight(Light* light);
	void RemoveLight(Light* light);

	void IndexArray(vector<vec3>& arr, vector<uint32_t>& idx);

	void SetResolution(ivec2 res);
	ivec2 GetResolution();

	void SetWindowTitle(string title);
	void SetWindowPos(ivec2 pos);

	GLFWwindow* GetWindowHandle() { return window; }

	/// The multiplier for anti-aliasing. 2, 4, 8.
	int fsaa = 4;

	/// Determines whether this OpenGL context is full-screen or not.
	bool fullscreen = false;

	/// Whether or not the frame-rate should try to sync with the monitor refresh rate (60 hz/fps)
	bool vsync = true;

	/// The amount of texture interpolation to use
	byte texInterp;

	/// Whether or not to render.
	bool render = true;

	/// The OpenGL version to use, 0 is Primary, 1 is Secondary (eg. {4, 3} = OpenGL 4.3).
	Range<int> version = { 3, 2 };

	float gamma = 2.2f;

	int motionBlur = 5;

	ptr<FrameBuffer> frameBuffer;
	ptr<FrameBuffer> lightBuffer;

protected:

	/// Whether or not to hide the mouse cursor.
	bool hideMouse = false;
	bool windowResizable = false;

	deque< ptr<Shader> > shaders;

	GLFWwindow* window;

	deque<Light*> lights;

public:

private:
	ivec2 resolution = ivec2(1280, 720);
	ivec2 windowPos = ivec2(0);
	string windowTitle = "lfant";
};

/// @}
/// @}
}
