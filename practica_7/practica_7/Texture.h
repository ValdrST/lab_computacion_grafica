#pragma once
#include<glew.h>
#include "stb_image.h"


class Texture
{
public:
	Texture();
	Texture(const char* FileLoc);
	void LoadTexture();
	void UseTexture();
	void ClearTexture();
	~Texture();
private: 
	GLuint textureID;
	int width, height, bitDepth;
	const char *fileLocation;

};

