#pragma once

#include<iostream>
#include<string>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture {
private:
	GLuint id;
	int width;
	int height;
	unsigned int type;
	GLint textureUnit;

public:

	Texture(const char *fileName, GLenum type, GLint textureUnit) 
	{
		
		this->type = type;
		this->textureUnit = textureUnit;

		int channels;
		unsigned char* image = stbi_load(fileName, &this->width, &this->height, &channels, 0);

		glGenTextures(1, &this->id);
		glBindTexture(type, this->id);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (image)
		{
			glTexImage2D(
				type, 0, GL_RGB, this->width, this->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image);

			glGenerateMipmap(type);
		}
		else
		{
			std::string errorMessage = "Failed to load texture: ";
			errorMessage.append(fileName);
			throw std::exception(errorMessage.c_str());
		}


		glActiveTexture(0);
		glBindTexture(type, 0);
		stbi_image_free(image);
	}

	~Texture() {
		glDeleteTextures(1, &this->id);
	}

	GLuint GetId() const 
	{
		return this->id;
	}

	void Bind() 
	{
		glActiveTexture(GL_TEXTURE0 + this->textureUnit);
		glBindTexture(this->type, this->id);
	}

	void Unbind() 
	{
		glActiveTexture(0);
		glBindTexture(this->type, this->id);
	}

	GLint GetTextureUnit() const 
	{
		return this->textureUnit;
	}
};