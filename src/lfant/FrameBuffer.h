/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Object.h>
#include <lfant/Texture.h>
#include <lfant/Rect.h>
#include <lfant/Shader.h>
#include <lfant/Geometry.h>

// External

namespace lfant {

class FrameBuffer : public Object
{
public:
	FrameBuffer(Game* game);
	~FrameBuffer();

	virtual void Init();
	virtual void Deinit();

	virtual Game* GetGame() const;

	void DrawBuffers();

	void Clear();

	void Bind();
	static void Unbind();

	void BindRead();
	static void UnbindRead();

	static FrameBuffer* GetCurrent();

	uint32 GetAttachment(uint32 idx);

	void ResizeViewport();
	void SetRect(URect r) { rect = r; }
	void Resize();

	void AddTexture(string name, Texture::Format form1 = Texture::Format::RGB32F, Texture::Format form2 = Texture::Format::RGB);
	void AddDepthTexture(string name, Texture::Format form1 = Texture::Format::RGB32F, Texture::Format form2 = Texture::Format::RGB);
	void GetTextures(Shader* sh) const;
	Texture* GetTexture(string name) const;

	void BindTextures(Shader* sh = nullptr);

	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();

	URect rect {0,0,0,0};
//	ptr<Shader> shader;

	void DrawBuffer(int idx);
protected:

	ptr<Texture> depthTexture;
	std::shared_ptr<Shader> shader;
	uint32 id = 0;
	uint32 startIndex = 0;
	uint32 depthBuffer = 0;
	bool hasDepth = false;

	std::deque<ptr<Texture>> textures;
	std::vector<uint32> drawBuffers;
	Buffer<vec2> posBuffer;

	Game* game = nullptr;


	static FrameBuffer* current;
private:
};

}
