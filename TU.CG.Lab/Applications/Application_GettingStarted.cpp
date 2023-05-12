#include "Application_GettingStarted.hpp"

#include <iostream>
#include <glad/glad.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Applications
{
	Application_GettingStarted::Application_GettingStarted()
		: boxTexture(0), faceTexture(0), redstoneTexture(0)
	{
		window = std::make_unique<Utils::Window>("TU.CG.Lab", 800, 600);
	}

	void Application_GettingStarted::Run()
	{
		auto lastFrame = 0.0f; // Time of last frame

		Initialize();
		LoadContent();

		// Render loop
		// Each iteration is a frame
		while (!window->GetShouldClose())
		{
			const auto currentFrame = window->GetElapsedTime();
			const auto deltaTime = currentFrame - lastFrame; // Time between current frame and last frame
			lastFrame = currentFrame;

			Update(deltaTime);
			Render();

			window->SwapBuffers();
			window->PollEvents();
		}

		UnloadContent();
	}

	void Application_GettingStarted::Initialize()
	{
		window->ActivateInputFor(this);

		camera = std::make_unique<Utils::Camera3D>(
			glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);

		stbi_set_flip_vertically_on_load(true);


		glCullFace(GL_FRONT);

		glEnable(GL_DEPTH_TEST);
	}

	void Application_GettingStarted::LoadContent()
	{
		//
		// ------ Textures
		//

		glGenTextures(1, &boxTexture);
		glBindTexture(GL_TEXTURE_2D, boxTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, channels;
		std::string texturePath = "Content/Textures/stone.jpg";
		auto data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::string errorMessage = "Failed to load texture: ";
			errorMessage.append(texturePath);
			throw std::exception(errorMessage.c_str());
		}

		stbi_image_free(data);


		// epic face
		glGenTextures(1, &faceTexture);
		glBindTexture(GL_TEXTURE_2D, faceTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texturePath = "Content/Textures/awesomeface.png";
		data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::string errorMessage = "Failed to load texture: ";
			errorMessage.append(texturePath);
			throw std::exception(errorMessage.c_str());
		}

		stbi_image_free(data);





		// RedSloune
		glGenTextures(1, &redstoneTexture);
		glBindTexture(GL_TEXTURE_2D, redstoneTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texturePath = "Content/Textures/redstone.jpg";
		data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, data);

			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			std::string errorMessage = "Failed to load texture: ";
			errorMessage.append(texturePath);
			throw std::exception(errorMessage.c_str());
		}

		stbi_image_free(data);



		//
		// ------ Buffers
		//

		constexpr float vertices[] =
		{
			// positions          // texture coordinates
			// TOP
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			// bottom
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			// left
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			// right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 //back
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			//front
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		};

		//constexpr unsigned indices[] =
		//{
		//	0, 1, 3,	// first triangle
		//	1, 2, 3,	// second triangle
		//};

		va = std::make_unique<Graphics::VertexArray>();
		auto vb = std::make_unique<Graphics::VertexBuffer>(vertices, sizeof vertices);

		vb->SetAttributes({
			{"aPos", Graphics::VertexAttributeType::VEC3F},
			{"aTexCoord", Graphics::VertexAttributeType::VEC2F},
		});

		//auto eb = std::make_unique<Graphics::ElementBuffer>(indices, 6);

		va->SetVertexBuffer(std::move(vb));
		//va->SetElementBuffer(std::move(eb));

		//
		// ------ Shaders
		//

		shader = std::make_unique<Graphics::ShaderProgram>(
			"Content/Shaders/getting_started.vert",
			"Content/Shaders/getting_started.frag");

		shader->Use();

		shader->SetInt("boxTexture", 0);
		shader->SetInt("faceTexture", 1);

		shader->Unuse();
	}

	void Application_GettingStarted::UnloadContent()
	{
		glDeleteTextures(1, &boxTexture);
		glDeleteTextures(1, &faceTexture);
		//glDeleteTextures(1, &redstoneOre);

		va = nullptr;
		shader = nullptr;
		window = nullptr;
	}

	void Application_GettingStarted::Update(const float deltaTime)
	{
		if (inputManager.IsKeyDown(Input::Keys::ESCAPE))
			window->SetShouldClose(true);

		camera->Update(deltaTime, inputManager);

		inputManager.ResetState();
	}

	void Application_GettingStarted::Render() const
	{
		glClearColor(0.393f, 0.585f, 0.930f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		constexpr glm::vec3 cubePositions[] =
		{
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(1.0f,  0.0f,  0.0f),
			glm::vec3(1.0f,  1.0f,  0.0f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f),
		};

		// Begin Draw
		shader->Use();

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, boxTexture);
		
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, faceTexture);


		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, redstoneTexture);


		va->Bind();



		

		// map generation
		// x y z
		int map[16][3][16];
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 3; y++) {
				for (int z = 0; z < 3; z++) {
					map[x][y][z] = 1;
				}
			}
		}


		map[0][1][2] = 3;

		// y x z
		// draw
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 3; y++) {
				for (int z = 0; z < 16; z++) {

					auto position = glm::vec3((float)x, (float)z, (float)y);
					if (map[x][y][z] == 1) {
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, boxTexture);

						auto model = glm::mat4(1.0f);

						model = glm::translate(model, position);

						model = glm::rotate(
							model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

						shader->SetMat4f("model", model);

						glDrawArrays(GL_TRIANGLES, 0, 36);

					}

					if (map[x][y][z] == 3) {
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, redstoneTexture);

						auto model = glm::mat4(1.0f);

						model = glm::translate(model, position);

						model = glm::rotate(
							model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

						shader->SetMat4f("model", model);

						glDrawArrays(GL_TRIANGLES, 0, 36);

					}

				}
			}
		}


		const auto view = camera->GetViewMatrix();

		const auto windowSize = window->GetSize();

		const auto projection = glm::perspective(
			glm::radians(camera->GetZoom()), windowSize.x / windowSize.y, 0.1f, 100.0f);

		shader->SetMat4f("view", view);
		shader->SetMat4f("projection", projection);

		// End Draw
		shader->Unuse();
		glBindTexture(GL_TEXTURE_2D, 0);
		va->Unbind();
	}
}
