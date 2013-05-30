/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-01-15 by Taylor Snead
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
#include <lfant/Texture.h>

// External
#include <GL/glew.h>
#include <lfant/lodepng.h>
#include <lfant/CImg.h>

// Internal
#include <lfant/TextureLoader.h>
#include <lfant/util/String.h>
#include <lfant/FileSystem.h>
#include <lfant/Console.h>

namespace lfant
{

void Texture::Load(Properties *prop)
{
	prop->Get("path", path);
	Log("Tex path: '"+path+"'.");
	prop->Get("anisoLevel", anisoLevel);

	LoadFile(path);
}

void Texture::Save(Properties *prop)
{
	Object::Save(prop);

	prop->Set("path", path);
	prop->Set("anisoLevel", anisoLevel);
}

uint32 Texture::GetId()
{
	return id;
}

void Texture::OnDestroy()
{
	glDeleteTextures(1, &id);
}

void Texture::LoadFile(string path, int mode)
{
	if(path == "")
	{
		path = this->path;
	}
	if(mode == 0)
	{
		mode = GL_TEXTURE_2D;
	}

	this->path = game->fileSystem->GetGamePath(path).string();

	cimg_library::CImg<byte> img(this->path.c_str());
	Log("Texture loaded...");
	size.x = img.width();
	size.y = img.height();

	glGenTextures(1, &id);
	glBindTexture(mode, id);
	glTexImage2D(mode, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data());
	glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(mode, 0);
}

void Texture::LoadPNG(int mode)
{
	vector<byte> data;
	uint error = lodepng::decode(data, size.x, size.y, path);
	if(error != 0)
	{
		Log("LoadPNG: Error, ", lodepng_error_text(error));
		return;
	}
	Log("Texture::LoadPNG: Image size: (", size.x, ", ", size.y, ").");
	if(id == 0)
	{
		glGenTextures(1, &this->id);
		Log("Texture::LoadPNG: GL Texture generated");
	}
	glBindTexture(mode, this->id);
	Log("Texture::LoadPNG: GL Texture bound");
	glTexImage2D(mode, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
	Log("Texture::LoadPNG: texture data sent");

	glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	Log("Texture::LoadPNG: texture filtering set");

	glBindTexture(mode, 0);
}

void Texture::LoadJPEG(int mode)
{
}

void Texture::LoadBMP(int mode)
{
	vector<byte> data;
	byte header[54];
	uint dataPos;
	//	uint width, height;
	uint imageSize;
	//	vector<byte> data;

	FILE* file = fopen(path.c_str(), "rb");
	if (!file)
	{
		Log("Image " + path + " could not be loaded");
		return;
	}

	if (fread(header, 1, 54, file) != 54 || (header[0] != 'B' || header[1] != 'M'))
	{
		Log("Image " + path + " is not a correct BMP file");
		return;
	}

	// Grab header data
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	size.x = *(int*)&(header[0x12]);
	size.y = *(int*)&(header[0x16]);

	Log("Read image header");

	// Make up any missing header data
	if (imageSize == 0) imageSize = size.x * size.y * 3;
	if (dataPos == 0) dataPos = 54;

	// Buffer
	data.reserve(imageSize);
	// Read data from file to buffer
	fread(&data[0], 1, imageSize, file);
	// Close file
	fclose(file);

	Log("Read data and closed file");
	if(id == 0)
	{
		glGenTextures(1, &this->id);
	}
	glBindTexture(mode, this->id);
	glTexImage2D(mode, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

	glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(mode, 0);
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

void Texture::LoadDDS(int mode)
{
	unsigned char header[124];

	FILE* fp;

	/* try to open the file */
	fp = fopen(path.c_str(), "rb");
	if (fp == NULL)
		return;

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(fp);
		return;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	size.y = *(unsigned int*)&(header[8]);
	size.x  = *(unsigned int*)&(header[12]);
	unsigned int linearSize  = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);


	unsigned char* buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(mode, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (size.x || size.y); ++level)
	{
		unsigned int tsize = ((this->size.x+3)/4)*((this->size.y+3)/4)*blockSize;
		glCompressedTexImage2D(mode, level, format, size.x, size.y,
							   0, tsize, buffer + offset);

		offset += tsize;
		size /= 2;
	}

	free(buffer);
	id = textureID;
	Log("Finished loading DDS texture");
}

}
