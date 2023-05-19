#include "Application.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

namespace Applications
{
	Application::Application()
		: lightPos(0.8f, 2.8f, 15.0f)
	{
		window = std::make_unique<Utils::Window>("TU.CG.Lab", 1280, 720);
	}

	void Application::Run()
	{
		auto lastFrame = 0.0f;

		LoadMap();
		Initialize();
		LoadContent();

		while (!window->GetShouldClose())
		{
			const auto currentFrame = window->GetElapsedTime();
			const auto deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			Update(deltaTime);
			Render();

			window->SwapBuffers();
			window->PollEvents();
		}

		UnloadContent();
	}

	void Application::Initialize()
	{
		window->ActivateInputFor(this);

		camera = std::make_unique<Utils::Camera3D>(
			glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f);

		camera->SetMap(map);
		stbi_set_flip_vertically_on_load(true);

		glCullFace(GL_FRONT);

		glEnable(GL_DEPTH_TEST);
	}

	void Application::LoadContent()
	{
		//
		// -- Textures
		//

		boxDiffuseMap = std::make_unique<Graphics::Texture>(
			content.GetTexture("Content/Textures/stone.png"));

		boxSpecularMap = std::make_unique<Graphics::Texture>(
			content.GetTexture("Content/Textures/stone_specular.png"));

		redstoneDiffuseMap = std::make_unique<Graphics::Texture>(
			content.GetTexture("Content/Textures/redstone.png"));

		redstoneSpecularMap = std::make_unique<Graphics::Texture>(
			content.GetTexture("Content/Textures/redstone_specular.png"));

		goldDiffuseMap = std::make_unique<Graphics::Texture>(
			content.GetTexture("Content/Textures/gold.png"));

		goldSpecularMap = std::make_unique<Graphics::Texture>(
			content.GetTexture("Content/Textures/gold_specular.png"));

		////
		// -- Models
		///

		bed = content.GetModel("Content/Models/bed2/bed.obj");

		//
		// --- Buffers
		//

		constexpr float lightVertices[] =
		{
			// positions
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		constexpr float vertices[] =
		{
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		};

		lightVa = std::make_unique<Graphics::VertexArray>();

		auto lightVb = std::make_unique<Graphics::VertexBuffer>(
			lightVertices, sizeof lightVertices);

		lightVb->SetAttributes({
			{"aPos", Graphics::VertexAttributeType::VEC3F},
		});

		lightVa->SetVertexBuffer(std::move(lightVb));

		objectVa = std::make_unique<Graphics::VertexArray>();

		auto objectVb = std::make_unique<Graphics::VertexBuffer>(
			vertices, sizeof vertices);

		objectVb->SetAttributes({
			{"aPos", Graphics::VertexAttributeType::VEC3F},
			{"aNormal", Graphics::VertexAttributeType::VEC3F},
			{"aTexCoords", Graphics::VertexAttributeType::VEC2F},
		});

		objectVa->SetVertexBuffer(std::move(objectVb));

		//
		// --- Shaders
		//

		lightShader = content.GetShader(
			"Content/Shaders/light_box.vert",
			"Content/Shaders/light_box.frag");

		objectShader = content.GetShader(
			"Content/Shaders/lighting.vert",
			"Content/Shaders/lighting.frag");

		objectShader->Use();

		objectShader->SetInt("material.diffuse", 0);
		objectShader->SetInt("material.specular", 1);
		objectShader->SetFloat("material.shininess", 128.0f);
		objectShader->SetVec3f("light.position", lightPos);
		objectShader->SetVec3f("light.ambient", glm::vec3(0.2f));
		objectShader->SetVec3f("light.diffuse", glm::vec3(0.5f));
		objectShader->SetVec3f("light.specular", glm::vec3(1.0f));

		objectShader->Unuse();


		//
		// ------ Shaders for Model
		//

		modelShader = content.GetShader(
			"Content/Shaders/model_loading.vert",
			"Content/Shaders/model_loading.frag");

		lightShader = content.GetShader(
			"Content/Shaders/light_box.vert",
			"Content/Shaders/light_box.frag");

		modelShader->Use();

		modelShader->SetFloat("material.shininess", 128.0f);
		modelShader->SetVec3f("light.position", lightPos);
		modelShader->SetVec3f("light.ambient", glm::vec3(0.2f));
		modelShader->SetVec3f("light.diffuse", glm::vec3(0.5f));
		modelShader->SetVec3f("light.specular", glm::vec3(1.0f));

		modelShader->Unuse();


	}

	void Application::UnloadContent()
	{
		content.Clear();

		objectVa = nullptr;
		objectShader = nullptr;
		lightVa = nullptr;
		lightShader = nullptr;

		bed = nullptr;
		torch = nullptr;
		content.Clear();

	}

	void Application::Update(const float deltaTime)
	{
		if (inputManager.IsKeyDown(Input::Keys::ESCAPE))
			window->SetShouldClose(true);

		camera->Update(deltaTime, inputManager);

		inputManager.ResetState();
	}

	void Application::Render() const
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto model = glm::mat4(1.0f);

		const auto view = camera->GetViewMatrix();

		const auto windowSize = window->GetSize();

		const auto projection = glm::perspective(
			glm::radians(camera->GetZoom()), windowSize.x / windowSize.y, 0.1f, 100.0f);

		const auto normalMatrix = glm::inverseTranspose(glm::mat3(model));

		objectShader->Use();
		objectVa->Bind();

		boxDiffuseMap->BindAndActivate(0);
		boxSpecularMap->BindAndActivate(1);
		redstoneDiffuseMap->BindAndActivate(0);

		objectShader->SetMat4f("model", model);
		objectShader->SetMat4f("view", view);
		objectShader->SetMat4f("projection", projection);
		objectShader->SetMat3f("normal", normalMatrix);
		objectShader->SetVec3f("viewPos", camera->GetPosition());

		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Models
		const auto normal = glm::inverseTranspose(glm::mat3(model));

		modelShader->Use();
		
		model = glm::scale(model, glm::vec3(0.015f));
		model = glm::translate(model, glm::vec3(450.8f, 25.8f, 207.0f));

		modelShader->SetMat4f("model", model);
		modelShader->SetMat4f("view", view);
		modelShader->SetMat4f("projection", projection);
		modelShader->SetMat3f("normal", normal);
		modelShader->SetVec3f("viewPos", camera->GetPosition());
		modelShader->SetVec3f("light.position", lightPos);

		bed->Draw(*modelShader);
		

		//bed->Draw(*modelShader);

		modelShader->Unuse();



		for (int x = 0; x < 32; x++) {
			for (int y = 0; y < 32; y++) {
				for (int z = 0; z < 32; z++) {
					// prazen blok
					if (map[x][y][z] == 0)
						continue;

					// da ne pokazva vutreshnite blokove
					if (x > 0 && x < sizeX - 1 && map[x - 1][y][z] != 0 && map[x + 1][y][z] != 0 &&
						y > 0 && y < sizeY - 1 && map[x][y - 1][z] != 0 && map[x][y + 1][z] != 0 &&
						z > 0 && z < sizeZ - 1 && map[x][y][z - 1] != 0 && map[x][y][z + 1] != 0) {
						continue;
					}

					if (map[x][y][z] == 1) {
						boxDiffuseMap->BindAndActivate(0);
						boxSpecularMap->BindAndActivate(1);
					}

					if (map[x][y][z] == 2) {
						redstoneDiffuseMap->BindAndActivate(0);
						redstoneSpecularMap->BindAndActivate(1);
					}
					if (map[x][y][z] == 3) {
						goldDiffuseMap->BindAndActivate(0);
						goldSpecularMap->BindAndActivate(1);
					}

					auto position = glm::vec3((float)x, (float)z, (float)y);

					objectShader->Use();
					objectVa->Bind();

					model = glm::mat4(1.0f);
					model = glm::translate(model, position);

					objectShader->SetMat4f("model", model);
					objectShader->SetMat4f("view", view);
					objectShader->SetMat4f("projection", projection);
					objectShader->SetMat3f("normal", normalMatrix);
					objectShader->SetVec3f("viewPos", camera->GetPosition());

					glDrawArrays(GL_TRIANGLES, 0, 36);

					objectVa->Unbind();
					objectShader->Unuse();
				}
			}
		}


		objectVa->Unbind();
		objectShader->Unuse();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightShader->Use();
		lightVa->Bind();

		lightShader->SetMat4f("model", model);
		lightShader->SetMat4f("view", view);
		lightShader->SetMat4f("projection", projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightVa->Unbind();
		lightShader->Unuse();
	}

	void Application::LoadMap() {

		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				for (int z = 0; z < sizeZ; z++) {
					map[x][y][z] = 1;
				}
			}
		}

		for (int x = 1; x < sizeX - 1; x++) {
			for (int y = 1; y < sizeY - 1; y++) {
				for (int z = 1; z < sizeZ - 1; z++) {
					map[x][y][z] = 0;
				}
			}
		}

		map[1][1][1] = 1;
		map[1][2][1] = 1;
		map[1][3][1] = 1;
		map[1][4][1] = 1;
		map[1][5][1] = 1;
		map[1][6][1] = 1;
		map[1][7][1] = 1;
		map[1][8][1] = 1;
		map[1][9][1] = 1;
		map[1][18][1] = 1;
		map[1][19][1] = 1;
		map[1][20][1] = 1;
		map[1][21][1] = 1;
		map[1][22][1] = 1;
		map[1][23][1] = 1;
		map[1][24][1] = 1;
		map[1][25][1] = 1;
		map[1][26][1] = 1;
		map[1][27][1] = 1;
		map[1][28][1] = 1;
		map[1][29][1] = 1;
		map[1][30][1] = 1;
		map[2][1][1] = 1;
		map[2][2][1] = 1;
		map[2][3][1] = 1;
		map[2][4][1] = 1;
		map[2][5][1] = 1;
		map[2][6][1] = 1;
		map[2][7][1] = 1;
		map[2][19][1] = 1;
		map[2][20][1] = 1;
		map[2][21][1] = 1;
		map[2][22][1] = 1;
		map[2][23][1] = 1;
		map[2][24][1] = 1;
		map[2][25][1] = 1;
		map[2][26][1] = 1;
		map[2][27][1] = 1;
		map[2][28][1] = 1;
		map[2][29][1] = 1;
		map[2][30][1] = 1;
		map[3][1][1] = 1;
		map[3][2][1] = 1;
		map[3][3][1] = 1;
		map[3][4][1] = 1;
		map[3][5][1] = 1;
		map[3][22][1] = 1;
		map[3][23][1] = 1;
		map[3][24][1] = 1;
		map[3][25][1] = 1;
		map[3][26][1] = 1;
		map[3][27][1] = 1;
		map[3][28][1] = 1;
		map[3][29][1] = 1;
		map[3][30][1] = 1;
		map[4][1][1] = 1;
		map[4][2][1] = 1;
		map[4][3][1] = 1;
		map[4][4][1] = 1;
		map[4][23][1] = 1;
		map[4][24][1] = 1;
		map[4][25][1] = 1;
		map[4][26][1] = 1;
		map[4][27][1] = 1;
		map[4][28][1] = 1;
		map[4][29][1] = 1;
		map[4][30][1] = 1;
		map[5][1][1] = 1;
		map[5][2][1] = 1;
		map[5][3][1] = 1;
		map[5][23][1] = 1;
		map[5][24][1] = 1;
		map[5][25][1] = 1;
		map[5][26][1] = 1;
		map[5][27][1] = 1;
		map[5][28][1] = 1;
		map[5][29][1] = 1;
		map[5][30][1] = 1;
		map[6][1][1] = 1;
		map[6][2][1] = 1;
		map[6][24][1] = 1;
		map[6][25][1] = 1;
		map[6][26][1] = 1;
		map[6][27][1] = 1;
		map[6][28][1] = 1;
		map[6][29][1] = 1;
		map[6][30][1] = 1;
		map[7][1][1] = 1;
		map[7][25][1] = 1;
		map[7][26][1] = 1;
		map[7][27][1] = 1;
		map[7][28][1] = 1;
		map[7][29][1] = 1;
		map[7][30][1] = 1;
		map[8][1][1] = 1;
		map[8][25][1] = 1;
		map[8][26][1] = 1;
		map[8][27][1] = 1;
		map[8][28][1] = 1;
		map[8][29][1] = 1;
		map[8][30][1] = 1;
		map[9][1][1] = 1;
		map[9][18][1] = 1;
		map[9][19][1] = 1;
		map[9][26][1] = 1;
		map[9][27][1] = 1;
		map[9][28][1] = 1;
		map[9][29][1] = 1;
		map[9][30][1] = 1;
		map[10][15][1] = 1;
		map[10][16][1] = 1;
		map[10][17][1] = 1;
		map[10][18][1] = 1;
		map[10][19][1] = 1;
		map[10][26][1] = 1;
		map[10][27][1] = 1;
		map[10][28][1] = 1;
		map[10][29][1] = 1;
		map[10][30][1] = 1;
		map[11][11][1] = 1;
		map[11][12][1] = 1;
		map[11][13][1] = 1;
		map[11][14][1] = 1;
		map[11][15][1] = 1;
		map[11][16][1] = 1;
		map[11][17][1] = 1;
		map[11][18][1] = 1;
		map[11][19][1] = 1;
		map[11][27][1] = 1;
		map[11][28][1] = 1;
		map[11][29][1] = 1;
		map[11][30][1] = 1;
		map[12][9][1] = 1;
		map[12][10][1] = 1;
		map[12][11][1] = 1;
		map[12][12][1] = 1;
		map[12][13][1] = 1;
		map[12][14][1] = 1;
		map[12][15][1] = 1;
		map[12][16][1] = 1;
		map[12][17][1] = 1;
		map[12][18][1] = 1;
		map[12][19][1] = 1;
		map[12][20][1] = 1;
		map[12][27][1] = 1;
		map[12][28][1] = 1;
		map[12][29][1] = 1;
		map[12][30][1] = 1;
		map[13][8][1] = 1;
		map[13][9][1] = 1;
		map[13][19][1] = 1;
		map[13][20][1] = 1;
		map[13][27][1] = 1;
		map[13][28][1] = 1;
		map[13][29][1] = 1;
		map[13][30][1] = 1;
		map[14][7][1] = 1;
		map[14][8][1] = 1;
		map[14][20][1] = 1;
		map[14][21][1] = 1;
		map[14][28][1] = 1;
		map[14][29][1] = 1;
		map[14][30][1] = 1;
		map[15][7][1] = 1;
		map[15][20][1] = 1;
		map[15][21][1] = 1;
		map[15][28][1] = 1;
		map[15][29][1] = 1;
		map[15][30][1] = 1;
		map[16][7][1] = 1;
		map[16][21][1] = 1;
		map[16][22][1] = 1;
		map[16][29][1] = 1;
		map[16][30][1] = 1;
		map[17][7][1] = 1;
		map[17][8][1] = 1;
		map[17][21][1] = 1;
		map[17][22][1] = 1;
		map[17][29][1] = 1;
		map[17][30][1] = 1;
		map[18][8][1] = 1;
		map[18][9][1] = 1;
		map[18][22][1] = 1;
		map[18][30][1] = 1;
		map[19][1][1] = 1;
		map[19][9][1] = 1;
		map[19][10][1] = 1;
		map[19][11][1] = 1;
		map[19][22][1] = 1;
		map[19][30][1] = 1;
		map[20][1][1] = 1;
		map[20][11][1] = 1;
		map[20][12][1] = 1;
		map[20][13][1] = 1;
		map[20][14][1] = 1;
		map[20][15][1] = 1;
		map[20][16][1] = 1;
		map[20][20][1] = 1;
		map[20][21][1] = 1;
		map[20][22][1] = 1;
		map[20][23][1] = 1;
		map[21][1][1] = 1;
		map[21][16][1] = 1;
		map[21][17][1] = 1;
		map[21][18][1] = 1;
		map[21][19][1] = 1;
		map[21][20][1] = 1;
		map[22][1][1] = 1;
		map[23][1][1] = 1;
		map[23][30][1] = 1;
		map[24][1][1] = 1;
		map[24][2][1] = 1;
		map[24][30][1] = 1;
		map[25][1][1] = 1;
		map[25][2][1] = 1;
		map[25][3][1] = 1;
		map[25][30][1] = 1;
		map[26][1][1] = 1;
		map[26][2][1] = 1;
		map[26][3][1] = 1;
		map[26][4][1] = 1;
		map[26][5][1] = 1;
		map[26][29][1] = 1;
		map[26][30][1] = 1;
		map[27][1][1] = 1;
		map[27][2][1] = 1;
		map[27][3][1] = 1;
		map[27][4][1] = 1;
		map[27][5][1] = 1;
		map[27][6][1] = 1;
		map[27][7][1] = 1;
		map[27][8][1] = 1;
		map[27][29][1] = 1;
		map[27][30][1] = 1;
		map[28][1][1] = 1;
		map[28][2][1] = 1;
		map[28][3][1] = 1;
		map[28][4][1] = 1;
		map[28][5][1] = 1;
		map[28][6][1] = 1;
		map[28][7][1] = 1;
		map[28][8][1] = 1;
		map[28][9][1] = 1;
		map[28][10][1] = 1;
		map[28][28][1] = 1;
		map[28][29][1] = 1;
		map[28][30][1] = 1;
		map[29][1][1] = 1;
		map[29][10][1] = 1;
		map[29][11][1] = 1;
		map[29][12][1] = 1;
		map[29][13][1] = 1;
		map[29][14][1] = 1;
		map[29][15][1] = 1;
		map[29][16][1] = 1;
		map[29][17][1] = 1;
		map[29][18][1] = 1;
		map[29][28][1] = 1;
		map[29][29][1] = 1;
		map[29][30][1] = 1;
		map[30][1][1] = 1;
		map[30][13][1] = 1;
		map[30][14][1] = 1;
		map[30][18][1] = 1;
		map[30][19][1] = 1;
		map[30][24][1] = 1;
		map[30][25][1] = 1;
		map[30][26][1] = 1;
		map[30][27][1] = 1;
		map[30][28][1] = 1;
		map[30][29][1] = 1;
		map[30][30][1] = 1;

		map[1][1][2] = 1;
		map[1][2][2] = 1;
		map[1][3][2] = 1;
		map[1][4][2] = 1;
		map[1][5][2] = 1;
		map[1][6][2] = 1;
		map[1][7][2] = 1;
		map[1][8][2] = 1;
		map[1][9][2] = 1;
		map[1][19][2] = 1;
		map[1][20][2] = 1;
		map[1][21][2] = 1;
		map[1][22][2] = 1;
		map[1][23][2] = 1;
		map[1][24][2] = 1;
		map[1][25][2] = 1;
		map[1][26][2] = 1;
		map[1][27][2] = 1;
		map[1][28][2] = 1;
		map[1][29][2] = 1;
		map[1][30][2] = 1;
		map[2][1][2] = 1;
		map[2][2][2] = 1;
		map[2][3][2] = 1;
		map[2][4][2] = 1;
		map[2][5][2] = 1;
		map[2][6][2] = 1;
		map[2][7][2] = 1;
		map[2][21][2] = 1;
		map[2][22][2] = 1;
		map[2][23][2] = 1;
		map[2][24][2] = 1;
		map[2][25][2] = 1;
		map[2][26][2] = 1;
		map[2][27][2] = 1;
		map[2][28][2] = 1;
		map[2][29][2] = 1;
		map[2][30][2] = 1;
		map[3][1][2] = 1;
		map[3][2][2] = 1;
		map[3][3][2] = 1;
		map[3][23][2] = 2;
		map[3][24][2] = 2;
		map[3][25][2] = 1;
		map[3][26][2] = 1;
		map[3][27][2] = 1;
		map[3][28][2] = 1;
		map[3][29][2] = 1;
		map[3][30][2] = 1;
		map[4][1][2] = 1;
		map[4][2][2] = 1;
		map[4][3][2] = 1;
		map[4][24][2] = 2;
		map[4][25][2] = 1;
		map[4][26][2] = 1;
		map[4][27][2] = 1;
		map[4][28][2] = 1;
		map[4][29][2] = 1;
		map[4][30][2] = 1;
		map[5][1][2] = 1;
		map[5][2][2] = 1;
		map[5][24][2] = 2;
		map[5][25][2] = 2;
		map[5][26][2] = 1;
		map[5][27][2] = 1;
		map[5][28][2] = 1;
		map[5][29][2] = 1;
		map[5][30][2] = 1;
		map[6][1][2] = 1;
		map[6][25][2] = 2;
		map[6][26][2] = 1;
		map[6][27][2] = 1;
		map[6][28][2] = 1;
		map[6][29][2] = 1;
		map[6][30][2] = 1;
		map[7][1][2] = 1;
		map[7][25][2] = 2;
		map[7][26][2] = 2;
		map[7][27][2] = 1;
		map[7][28][2] = 1;
		map[7][29][2] = 1;
		map[7][30][2] = 1;
		map[8][1][2] = 1;
		map[8][26][2] = 2;
		map[8][27][2] = 1;
		map[8][28][2] = 1;
		map[8][29][2] = 1;
		map[8][30][2] = 1;
		map[9][1][2] = 1;
		map[9][18][2] = 1;
		map[9][19][2] = 1;
		map[9][26][2] = 2;
		map[9][27][2] = 2;
		map[9][28][2] = 1;
		map[9][29][2] = 1;
		map[9][30][2] = 1;
		map[10][15][2] = 1;
		map[10][16][2] = 1;
		map[10][17][2] = 1;
		map[10][18][2] = 1;
		map[10][19][2] = 1;
		map[10][27][2] = 2;
		map[10][28][2] = 1;
		map[10][29][2] = 1;
		map[10][30][2] = 1;
		map[11][11][2] = 3;
		map[11][12][2] = 3;
		map[11][13][2] = 3;
		map[11][14][2] = 3;
		map[11][15][2] = 1;
		map[11][16][2] = 1;
		map[11][17][2] = 1;
		map[11][18][2] = 1;
		map[11][19][2] = 1;
		map[11][27][2] = 1;
		map[11][28][2] = 1;
		map[11][29][2] = 1;
		map[11][30][2] = 1;
		map[12][9][2] = 1;
		map[12][10][2] = 1;
		map[12][11][2] = 1;
		map[12][12][2] = 1;
		map[12][13][2] = 1;
		map[12][14][2] = 1;
		map[12][15][2] = 1;
		map[12][16][2] = 1;
		map[12][17][2] = 1;
		map[12][18][2] = 1;
		map[12][19][2] = 1;
		map[12][20][2] = 1;
		map[12][28][2] = 1;
		map[12][29][2] = 1;
		map[12][30][2] = 1;
		map[13][9][2] = 1;
		map[13][19][2] = 1;
		map[13][20][2] = 1;
		map[13][28][2] = 1;
		map[13][29][2] = 1;
		map[13][30][2] = 1;
		map[14][8][2] = 1;
		map[14][9][2] = 1;
		map[14][10][2] = 1;
		map[14][20][2] = 1;
		map[14][21][2] = 1;
		map[14][29][2] = 1;
		map[14][30][2] = 1;
		map[15][8][2] = 1;
		map[15][9][2] = 1;
		map[15][10][2] = 1;
		map[15][20][2] = 1;
		map[15][21][2] = 1;
		map[15][29][2] = 1;
		map[15][30][2] = 1;
		map[16][8][2] = 1;
		map[16][9][2] = 1;
		map[16][10][2] = 1;
		map[16][21][2] = 1;
		map[16][22][2] = 1;
		map[16][30][2] = 1;
		map[17][8][2] = 1;
		map[17][9][2] = 1;
		map[17][10][2] = 1;
		map[17][21][2] = 1;
		map[17][22][2] = 3;
		map[17][30][2] = 1;
		map[18][9][2] = 1;
		map[18][10][2] = 1;
		map[18][11][2] = 1;
		map[18][12][2] = 1;
		map[18][22][2] = 3;
		map[18][30][2] = 1;
		map[19][10][2] = 1;
		map[19][11][2] = 1;
		map[19][12][2] = 1;
		map[19][13][2] = 1;
		map[19][14][2] = 1;
		map[19][15][2] = 1;
		map[19][16][2] = 1;
		map[19][22][2] = 1;
		map[20][12][2] = 1;
		map[20][13][2] = 1;
		map[20][14][2] = 1;
		map[20][15][2] = 1;
		map[20][16][2] = 1;
		map[20][17][2] = 1;
		map[20][18][2] = 1;
		map[20][19][2] = 1;
		map[20][20][2] = 1;
		map[20][21][2] = 1;
		map[20][22][2] = 1;
		map[20][23][2] = 1;
		map[21][18][2] = 1;
		map[21][19][2] = 1;
		map[21][20][2] = 1;
		map[22][1][2] = 1;
		map[23][1][2] = 1;
		map[23][30][2] = 1;
		map[24][1][2] = 1;
		map[24][30][2] = 1;
		map[25][1][2] = 1;
		map[25][2][2] = 1;
		map[25][3][2] = 1;
		map[25][30][2] = 1;
		map[26][1][2] = 1;
		map[26][2][2] = 1;
		map[26][3][2] = 1;
		map[26][4][2] = 1;
		map[26][5][2] = 1;
		map[26][29][2] = 1;
		map[26][30][2] = 1;
		map[27][1][2] = 1;
		map[27][2][2] = 1;
		map[27][3][2] = 1;
		map[27][4][2] = 1;
		map[27][5][2] = 1;
		map[27][6][2] = 4;
		map[27][7][2] = 4;
		map[27][8][2] = 4;
		map[27][29][2] = 1;
		map[27][30][2] = 1;
		map[28][1][2] = 1;
		map[28][2][2] = 1;
		map[28][3][2] = 1;
		map[28][4][2] = 1;
		map[28][5][2] = 1;
		map[28][6][2] = 1;
		map[28][7][2] = 1;
		map[28][8][2] = 1;
		map[28][9][2] = 1;
		map[28][10][2] = 1;
		map[28][29][2] = 1;
		map[28][30][2] = 1;
		map[29][1][2] = 1;
		map[29][10][2] = 1;
		map[29][11][2] = 1;
		map[29][12][2] = 1;
		map[29][13][2] = 1;
		map[29][14][2] = 1;
		map[29][15][2] = 1;
		map[29][16][2] = 1;
		map[29][17][2] = 1;
		map[29][28][2] = 1;
		map[29][29][2] = 1;
		map[29][30][2] = 1;
		map[30][1][2] = 1;
		map[30][13][2] = 1;
		map[30][14][2] = 1;
		map[30][18][2] = 1;
		map[30][19][2] = 1;
		map[30][24][2] = 1;
		map[30][25][2] = 1;
		map[30][26][2] = 1;
		map[30][27][2] = 1;
		map[30][28][2] = 1;
		map[30][29][2] = 1;
		map[30][30][2] = 1;


		map[1][1][3] = 1;
		map[1][2][3] = 1;
		map[1][3][3] = 1;
		map[1][4][3] = 1;
		map[1][5][3] = 1;
		map[1][6][3] = 1;
		map[1][7][3] = 1;
		map[1][8][3] = 1;
		map[1][9][3] = 1;
		map[1][19][3] = 1;
		map[1][20][3] = 1;
		map[1][21][3] = 1;
		map[1][22][3] = 1;
		map[1][23][3] = 1;
		map[1][24][3] = 1;
		map[1][25][3] = 1;
		map[1][26][3] = 1;
		map[1][27][3] = 1;
		map[1][28][3] = 1;
		map[1][29][3] = 1;
		map[1][30][3] = 1;
		map[2][1][3] = 1;
		map[2][2][3] = 1;
		map[2][3][3] = 1;
		map[2][4][3] = 1;
		map[2][5][3] = 1;
		map[2][6][3] = 1;
		map[2][7][3] = 1;
		map[2][23][3] = 1;
		map[2][24][3] = 1;
		map[2][25][3] = 1;
		map[2][26][3] = 1;
		map[2][27][3] = 1;
		map[2][28][3] = 1;
		map[2][29][3] = 1;
		map[2][30][3] = 1;
		map[3][1][3] = 1;
		map[3][2][3] = 1;
		map[3][3][3] = 1;
		map[3][4][3] = 1;
		map[3][5][3] = 1;
		map[3][24][3] = 2;
		map[3][25][3] = 1;
		map[3][26][3] = 1;
		map[3][27][3] = 1;
		map[3][28][3] = 1;
		map[3][29][3] = 1;
		map[3][30][3] = 1;
		map[4][1][3] = 1;
		map[4][2][3] = 1;
		map[4][3][3] = 1;
		map[4][4][3] = 1;
		map[4][5][3] = 1;
		map[4][25][3] = 2;
		map[4][26][3] = 1;
		map[4][27][3] = 1;
		map[4][28][3] = 1;
		map[4][29][3] = 1;
		map[4][30][3] = 1;
		map[5][1][3] = 1;
		map[5][2][3] = 1;
		map[5][3][3] = 1;
		map[5][4][3] = 1;
		map[5][5][3] = 1;
		map[5][25][3] = 2;
		map[5][26][3] = 1;
		map[5][27][3] = 1;
		map[5][28][3] = 1;
		map[5][29][3] = 1;
		map[5][30][3] = 1;
		map[6][1][3] = 1;
		map[6][2][3] = 1;
		map[6][3][3] = 1;
		map[6][4][3] = 1;
		map[6][26][3] = 2;
		map[6][27][3] = 1;
		map[6][28][3] = 1;
		map[6][29][3] = 1;
		map[6][30][3] = 1;
		map[7][1][3] = 1;
		map[7][2][3] = 1;
		map[7][3][3] = 1;
		map[7][4][3] = 1;
		map[7][26][3] = 2;
		map[7][27][3] = 1;
		map[7][28][3] = 1;
		map[7][29][3] = 1;
		map[7][30][3] = 1;
		map[8][1][3] = 1;
		map[8][2][3] = 1;
		map[8][3][3] = 1;
		map[8][27][3] = 2;
		map[8][28][3] = 1;
		map[8][29][3] = 1;
		map[8][30][3] = 1;
		map[9][1][3] = 1;
		map[9][2][3] = 1;
		map[9][18][3] = 1;
		map[9][19][3] = 1;
		map[9][27][3] = 2;
		map[9][28][3] = 1;
		map[9][29][3] = 1;
		map[9][30][3] = 1;
		map[10][1][3] = 1;
		map[10][2][3] = 4;
		map[10][15][3] = 1;
		map[10][16][3] = 1;
		map[10][17][3] = 1;
		map[10][18][3] = 1;
		map[10][19][3] = 1;
		map[10][28][3] = 2;
		map[10][29][3] = 1;
		map[10][30][3] = 1;
		map[11][1][3] = 1;
		map[11][2][3] = 4;
		map[11][11][3] = 3;
		map[11][12][3] = 3;
		map[11][13][3] = 1;
		map[11][14][3] = 1;
		map[11][15][3] = 1;
		map[11][16][3] = 1;
		map[11][17][3] = 1;
		map[11][18][3] = 1;
		map[11][19][3] = 1;
		map[11][28][3] = 1;
		map[11][29][3] = 1;
		map[11][30][3] = 1;
		map[12][1][3] = 1;
		map[12][2][3] = 4;
		map[12][9][3] = 1;
		map[12][10][3] = 1;
		map[12][11][3] = 1;
		map[12][12][3] = 1;
		map[12][13][3] = 1;
		map[12][14][3] = 1;
		map[12][15][3] = 1;
		map[12][16][3] = 1;
		map[12][17][3] = 1;
		map[12][18][3] = 1;
		map[12][19][3] = 1;
		map[12][20][3] = 1;
		map[12][29][3] = 1;
		map[12][30][3] = 1;
		map[13][1][3] = 1;
		map[13][2][3] = 4;
		map[13][9][3] = 1;
		map[13][19][3] = 1;
		map[13][20][3] = 1;
		map[13][29][3] = 1;
		map[13][30][3] = 1;
		map[14][1][3] = 1;
		map[14][8][3] = 1;
		map[14][9][3] = 1;
		map[14][10][3] = 1;
		map[14][20][3] = 1;
		map[14][21][3] = 1;
		map[14][30][3] = 1;
		map[15][1][3] = 1;
		map[15][8][3] = 1;
		map[15][9][3] = 1;
		map[15][10][3] = 1;
		map[15][20][3] = 1;
		map[15][21][3] = 1;
		map[15][30][3] = 1;
		map[16][8][3] = 1;
		map[16][9][3] = 1;
		map[16][10][3] = 1;
		map[16][21][3] = 1;
		map[16][22][3] = 1;
		map[16][30][3] = 1;
		map[17][9][3] = 1;
		map[17][10][3] = 1;
		map[17][21][3] = 1;
		map[17][22][3] = 1;
		map[17][30][3] = 1;
		map[18][11][3] = 1;
		map[18][12][3] = 1;
		map[18][22][3] = 3;
		map[18][30][3] = 1;
		map[19][1][3] = 1;
		map[19][12][3] = 1;
		map[19][13][3] = 1;
		map[19][14][3] = 1;
		map[19][15][3] = 1;
		map[19][16][3] = 1;
		map[19][22][3] = 1;
		map[20][1][3] = 1;
		map[20][12][3] = 1;
		map[20][13][3] = 1;
		map[20][14][3] = 1;
		map[20][15][3] = 1;
		map[20][16][3] = 1;
		map[20][17][3] = 1;
		map[20][18][3] = 1;
		map[20][19][3] = 1;
		map[20][20][3] = 1;
		map[20][21][3] = 1;
		map[20][22][3] = 1;
		map[20][23][3] = 1;
		map[21][1][3] = 1;
		map[21][18][3] = 1;
		map[21][19][3] = 1;
		map[21][20][3] = 1;
		map[22][1][3] = 1;
		map[22][2][3] = 1;
		map[23][1][3] = 1;
		map[23][2][3] = 1;
		map[23][30][3] = 1;
		map[24][1][3] = 1;
		map[24][2][3] = 1;
		map[24][30][3] = 1;
		map[25][1][3] = 1;
		map[25][2][3] = 1;
		map[25][3][3] = 1;
		map[25][30][3] = 1;
		map[26][1][3] = 1;
		map[26][2][3] = 1;
		map[26][3][3] = 1;
		map[26][4][3] = 1;
		map[26][5][3] = 1;
		map[26][29][3] = 5;
		map[26][30][3] = 1;
		map[27][1][3] = 1;
		map[27][2][3] = 1;
		map[27][3][3] = 1;
		map[27][4][3] = 1;
		map[27][5][3] = 1;
		map[27][6][3] = 4;
		map[27][7][3] = 4;
		map[27][8][3] = 4;
		map[27][29][3] = 5;
		map[27][30][3] = 1;
		map[28][1][3] = 1;
		map[28][2][3] = 1;
		map[28][3][3] = 1;
		map[28][4][3] = 1;
		map[28][5][3] = 1;
		map[28][6][3] = 1;
		map[28][7][3] = 1;
		map[28][8][3] = 1;
		map[28][9][3] = 1;
		map[28][10][3] = 1;
		map[28][29][3] = 1;
		map[28][30][3] = 1;
		map[29][1][3] = 1;
		map[29][10][3] = 1;
		map[29][11][3] = 1;
		map[29][12][3] = 1;
		map[29][13][3] = 1;
		map[29][14][3] = 1;
		map[29][15][3] = 1;
		map[29][16][3] = 1;
		map[29][17][3] = 1;
		map[29][28][3] = 1;
		map[29][29][3] = 1;
		map[29][30][3] = 1;
		map[30][1][3] = 1;
		map[30][13][3] = 1;
		map[30][14][3] = 1;
		map[30][18][3] = 1;
		map[30][19][3] = 1;
		map[30][24][3] = 1;
		map[30][25][3] = 1;
		map[30][26][3] = 1;
		map[30][27][3] = 1;
		map[30][28][3] = 1;
		map[30][29][3] = 1;
		map[30][30][3] = 1;

		map[1][8][4] = 1;
		map[1][9][4] = 1;
		map[1][10][4] = 1;
		map[1][11][4] = 1;
		map[1][12][4] = 1;
		map[1][13][4] = 1;
		map[1][14][4] = 1;
		map[1][15][4] = 1;
		map[1][19][4] = 1;
		map[1][20][4] = 1;
		map[1][21][4] = 1;
		map[1][22][4] = 1;
		map[1][23][4] = 1;
		map[1][24][4] = 1;
		map[1][25][4] = 1;
		map[1][26][4] = 1;
		map[1][27][4] = 1;
		map[1][28][4] = 1;
		map[2][8][4] = 1;
		map[2][9][4] = 1;
		map[2][10][4] = 1;
		map[2][11][4] = 1;
		map[2][12][4] = 1;
		map[2][13][4] = 1;
		map[2][14][4] = 1;
		map[2][21][4] = 1;
		map[2][22][4] = 1;
		map[2][23][4] = 1;
		map[2][24][4] = 1;
		map[2][25][4] = 1;
		map[2][26][4] = 1;
		map[2][27][4] = 1;
		map[2][28][4] = 1;
		map[2][29][4] = 1;
		map[3][5][4] = 1;
		map[3][6][4] = 1;
		map[3][7][4] = 1;
		map[3][8][4] = 1;
		map[3][9][4] = 1;
		map[3][10][4] = 1;
		map[3][11][4] = 1;
		map[3][12][4] = 1;
		map[3][13][4] = 1;
		map[3][22][4] = 1;
		map[3][23][4] = 1;
		map[3][24][4] = 1;
		map[3][25][4] = 1;
		map[3][26][4] = 1;
		map[3][27][4] = 1;
		map[3][28][4] = 1;
		map[3][29][4] = 1;
		map[4][4][4] = 1;
		map[4][5][4] = 1;
		map[4][6][4] = 1;
		map[4][7][4] = 1;
		map[4][8][4] = 1;
		map[4][9][4] = 1;
		map[4][10][4] = 1;
		map[4][23][4] = 1;
		map[4][24][4] = 1;
		map[4][25][4] = 1;
		map[4][26][4] = 1;
		map[4][27][4] = 1;
		map[4][28][4] = 1;
		map[4][29][4] = 1;
		map[4][30][4] = 1;
		map[5][3][4] = 1;
		map[5][4][4] = 1;
		map[5][5][4] = 1;
		map[5][6][4] = 1;
		map[5][7][4] = 1;
		map[5][8][4] = 1;
		map[5][9][4] = 1;
		map[5][25][4] = 1;
		map[5][26][4] = 1;
		map[5][27][4] = 1;
		map[5][28][4] = 1;
		map[5][29][4] = 1;
		map[5][30][4] = 1;
		map[6][3][4] = 1;
		map[6][4][4] = 1;
		map[6][5][4] = 1;
		map[6][6][4] = 1;
		map[6][7][4] = 1;
		map[6][8][4] = 1;
		map[6][25][4] = 1;
		map[6][26][4] = 1;
		map[6][27][4] = 1;
		map[6][28][4] = 1;
		map[6][29][4] = 1;
		map[6][30][4] = 1;
		map[7][3][4] = 1;
		map[7][4][4] = 1;
		map[7][5][4] = 1;
		map[7][6][4] = 1;
		map[7][7][4] = 1;
		map[7][13][4] = 2;
		map[7][14][4] = 2;
		map[7][15][4] = 1;
		map[7][26][4] = 1;
		map[7][27][4] = 1;
		map[7][28][4] = 1;
		map[7][29][4] = 1;
		map[7][30][4] = 1;
		map[8][2][4] = 1;
		map[8][3][4] = 1;
		map[8][4][4] = 1;
		map[8][5][4] = 1;
		map[8][6][4] = 1;
		map[8][7][4] = 1;
		map[8][12][4] = 2;
		map[8][13][4] = 2;
		map[8][14][4] = 1;
		map[8][15][4] = 1;
		map[8][16][4] = 1;
		map[8][27][4] = 1;
		map[8][28][4] = 1;
		map[8][29][4] = 1;
		map[8][30][4] = 1;
		map[9][2][4] = 1;
		map[9][3][4] = 1;
		map[9][4][4] = 1;
		map[9][5][4] = 1;
		map[9][6][4] = 1;
		map[9][10][4] = 2;
		map[9][11][4] = 2;
		map[9][12][4] = 2;
		map[9][13][4] = 1;
		map[9][14][4] = 1;
		map[9][15][4] = 1;
		map[9][16][4] = 1;
		map[9][17][4] = 1;
		map[9][18][4] = 1;
		map[9][19][4] = 1;
		map[9][27][4] = 1;
		map[9][28][4] = 1;
		map[9][29][4] = 1;
		map[9][30][4] = 1;
		map[10][1][4] = 1;
		map[10][2][4] = 1;
		map[10][3][4] = 1;
		map[10][4][4] = 1;
		map[10][5][4] = 1;
		map[10][10][4] = 2;
		map[10][11][4] = 1;
		map[10][12][4] = 1;
		map[10][13][4] = 1;
		map[10][14][4] = 1;
		map[10][15][4] = 1;
		map[10][16][4] = 1;
		map[10][17][4] = 1;
		map[10][18][4] = 1;
		map[10][19][4] = 1;
		map[10][27][4] = 1;
		map[10][28][4] = 1;
		map[10][29][4] = 1;
		map[10][30][4] = 1;
		map[11][1][4] = 1;
		map[11][2][4] = 1;
		map[11][3][4] = 1;
		map[11][4][4] = 1;
		map[11][10][4] = 1;
		map[11][11][4] = 1;
		map[11][12][4] = 1;
		map[11][13][4] = 1;
		map[11][14][4] = 1;
		map[11][15][4] = 1;
		map[11][16][4] = 1;
		map[11][17][4] = 1;
		map[11][18][4] = 1;
		map[11][19][4] = 1;
		map[11][20][4] = 1;
		map[11][27][4] = 1;
		map[11][28][4] = 1;
		map[11][29][4] = 1;
		map[11][30][4] = 1;
		map[12][1][4] = 1;
		map[12][2][4] = 1;
		map[12][3][4] = 1;
		map[12][9][4] = 1;
		map[12][10][4] = 1;
		map[12][11][4] = 1;
		map[12][12][4] = 1;
		map[12][13][4] = 1;
		map[12][14][4] = 1;
		map[12][15][4] = 1;
		map[12][16][4] = 1;
		map[12][17][4] = 1;
		map[12][18][4] = 1;
		map[12][19][4] = 1;
		map[12][20][4] = 1;
		map[12][28][4] = 1;
		map[12][29][4] = 1;
		map[12][30][4] = 1;
		map[13][1][4] = 1;
		map[13][2][4] = 1;
		map[13][3][4] = 1;
		map[13][9][4] = 1;
		map[13][10][4] = 1;
		map[13][11][4] = 1;
		map[13][12][4] = 1;
		map[13][13][4] = 1;
		map[13][14][4] = 1;
		map[13][15][4] = 1;
		map[13][16][4] = 1;
		map[13][19][4] = 1;
		map[13][20][4] = 1;
		map[13][21][4] = 1;
		map[13][28][4] = 1;
		map[13][29][4] = 1;
		map[13][30][4] = 1;
		map[14][1][4] = 1;
		map[14][2][4] = 1;
		map[14][3][4] = 1;
		map[14][8][4] = 1;
		map[14][9][4] = 1;
		map[14][10][4] = 1;
		map[14][11][4] = 1;
		map[14][12][4] = 1;
		map[14][13][4] = 1;
		map[14][14][4] = 1;
		map[14][20][4] = 1;
		map[14][21][4] = 1;
		map[14][28][4] = 1;
		map[14][29][4] = 1;
		map[14][30][4] = 1;
		map[15][1][4] = 1;
		map[15][2][4] = 1;
		map[15][8][4] = 1;
		map[15][9][4] = 1;
		map[15][10][4] = 1;
		map[15][11][4] = 1;
		map[15][12][4] = 1;
		map[15][13][4] = 1;
		map[15][20][4] = 1;
		map[15][21][4] = 1;
		map[15][22][4] = 1;
		map[15][29][4] = 1;
		map[15][30][4] = 1;
		map[16][1][4] = 1;
		map[16][2][4] = 1;
		map[16][8][4] = 1;
		map[16][9][4] = 1;
		map[16][10][4] = 1;
		map[16][11][4] = 1;
		map[16][12][4] = 1;
		map[16][21][4] = 1;
		map[16][22][4] = 1;
		map[16][29][4] = 1;
		map[16][30][4] = 1;
		map[17][1][4] = 1;
		map[17][9][4] = 1;
		map[17][10][4] = 1;
		map[17][21][4] = 1;
		map[17][22][4] = 1;
		map[17][30][4] = 1;
		map[18][1][4] = 1;
		map[18][10][4] = 1;
		map[18][11][4] = 1;
		map[18][12][4] = 1;
		map[18][22][4] = 1;
		map[18][23][4] = 1;
		map[18][30][4] = 1;
		map[19][1][4] = 1;
		map[19][10][4] = 1;
		map[19][11][4] = 1;
		map[19][12][4] = 1;
		map[19][13][4] = 1;
		map[19][14][4] = 1;
		map[19][15][4] = 1;
		map[19][16][4] = 1;
		map[19][22][4] = 1;
		map[19][23][4] = 1;
		map[19][30][4] = 1;
		map[20][1][4] = 1;
		map[20][11][4] = 1;
		map[20][12][4] = 1;
		map[20][13][4] = 1;
		map[20][14][4] = 1;
		map[20][15][4] = 1;
		map[20][16][4] = 1;
		map[20][17][4] = 1;
		map[20][18][4] = 1;
		map[20][19][4] = 1;
		map[20][20][4] = 1;
		map[20][21][4] = 1;
		map[20][22][4] = 1;
		map[20][23][4] = 1;
		map[20][30][4] = 3;
		map[21][1][4] = 1;
		map[21][2][4] = 1;
		map[21][13][4] = 1;
		map[21][14][4] = 1;
		map[21][15][4] = 1;
		map[21][16][4] = 1;
		map[21][17][4] = 1;
		map[21][18][4] = 1;
		map[21][19][4] = 1;
		map[21][20][4] = 1;
		map[21][21][4] = 1;
		map[21][22][4] = 1;
		map[21][30][4] = 3;
		map[22][1][4] = 1;
		map[22][2][4] = 1;
		map[22][15][4] = 1;
		map[22][16][4] = 1;
		map[22][17][4] = 1;
		map[22][18][4] = 1;
		map[22][19][4] = 1;
		map[22][30][4] = 1;
		map[23][1][4] = 1;
		map[23][2][4] = 1;
		map[23][3][4] = 1;
		map[23][30][4] = 1;
		map[24][1][4] = 1;
		map[24][2][4] = 1;
		map[24][3][4] = 1;
		map[24][30][4] = 1;
		map[25][1][4] = 1;
		map[25][2][4] = 1;
		map[25][3][4] = 1;
		map[25][4][4] = 1;
		map[25][29][4] = 1;
		map[25][30][4] = 1;
		map[26][1][4] = 1;
		map[26][2][4] = 1;
		map[26][3][4] = 1;
		map[26][4][4] = 1;
		map[26][5][4] = 1;
		map[26][29][4] = 1;
		map[26][30][4] = 1;
		map[27][1][4] = 1;
		map[27][2][4] = 1;
		map[27][3][4] = 1;
		map[27][4][4] = 1;
		map[27][5][4] = 1;
		map[27][6][4] = 4;
		map[27][7][4] = 4;
		map[27][8][4] = 4;
		map[27][29][4] = 1;
		map[27][30][4] = 1;
		map[28][1][4] = 1;
		map[28][2][4] = 1;
		map[28][3][4] = 1;
		map[28][4][4] = 1;
		map[28][5][4] = 1;
		map[28][6][4] = 1;
		map[28][7][4] = 1;
		map[28][8][4] = 1;
		map[28][9][4] = 1;
		map[28][10][4] = 1;
		map[28][28][4] = 1;
		map[28][29][4] = 1;
		map[28][30][4] = 1;
		map[29][1][4] = 1;
		map[29][10][4] = 1;
		map[29][11][4] = 1;
		map[29][12][4] = 1;
		map[29][13][4] = 1;
		map[29][14][4] = 1;
		map[29][15][4] = 1;
		map[29][16][4] = 1;
		map[29][17][4] = 1;
		map[29][24][4] = 1;
		map[29][25][4] = 1;
		map[29][26][4] = 1;
		map[29][27][4] = 1;
		map[29][28][4] = 1;
		map[29][29][4] = 1;
		map[29][30][4] = 1;
		map[30][1][4] = 1;
		map[30][13][4] = 1;
		map[30][14][4] = 1;
		map[30][18][4] = 1;
		map[30][19][4] = 1;
		map[30][20][4] = 1;
		map[30][21][4] = 1;
		map[30][22][4] = 1;
		map[30][23][4] = 1;
		map[30][24][4] = 1;
		map[30][25][4] = 1;
		map[30][26][4] = 1;
		map[30][27][4] = 1;
		map[30][28][4] = 1;
		map[30][29][4] = 1;
		map[30][30][4] = 1;

		map[1][8][4] = 1;
		map[1][9][4] = 1;
		map[1][10][4] = 1;
		map[1][11][4] = 1;
		map[1][12][4] = 1;
		map[1][13][4] = 1;
		map[1][14][4] = 1;
		map[1][15][4] = 1;
		map[1][19][4] = 1;
		map[1][20][4] = 1;
		map[1][21][4] = 1;
		map[1][22][4] = 1;
		map[1][23][4] = 1;
		map[1][24][4] = 1;
		map[1][25][4] = 1;
		map[1][26][4] = 1;
		map[1][27][4] = 1;
		map[1][28][4] = 1;
		map[2][8][4] = 1;
		map[2][9][4] = 1;
		map[2][10][4] = 1;
		map[2][11][4] = 1;
		map[2][12][4] = 1;
		map[2][13][4] = 1;
		map[2][14][4] = 1;
		map[2][21][4] = 1;
		map[2][22][4] = 1;
		map[2][23][4] = 1;
		map[2][24][4] = 1;
		map[2][25][4] = 1;
		map[2][26][4] = 1;
		map[2][27][4] = 1;
		map[2][28][4] = 1;
		map[2][29][4] = 1;
		map[3][5][4] = 1;
		map[3][6][4] = 1;
		map[3][7][4] = 1;
		map[3][8][4] = 1;
		map[3][9][4] = 1;
		map[3][10][4] = 1;
		map[3][11][4] = 1;
		map[3][12][4] = 1;
		map[3][13][4] = 1;
		map[3][22][4] = 1;
		map[3][23][4] = 1;
		map[3][24][4] = 1;
		map[3][25][4] = 1;
		map[3][26][4] = 1;
		map[3][27][4] = 1;
		map[3][28][4] = 1;
		map[3][29][4] = 1;
		map[4][4][4] = 1;
		map[4][5][4] = 1;
		map[4][6][4] = 1;
		map[4][7][4] = 1;
		map[4][8][4] = 1;
		map[4][9][4] = 1;
		map[4][10][4] = 1;
		map[4][23][4] = 1;
		map[4][24][4] = 1;
		map[4][25][4] = 1;
		map[4][26][4] = 1;
		map[4][27][4] = 1;
		map[4][28][4] = 1;
		map[4][29][4] = 1;
		map[4][30][4] = 1;
		map[5][3][4] = 1;
		map[5][4][4] = 1;
		map[5][5][4] = 1;
		map[5][6][4] = 1;
		map[5][7][4] = 1;
		map[5][8][4] = 1;
		map[5][9][4] = 1;
		map[5][25][4] = 1;
		map[5][26][4] = 1;
		map[5][27][4] = 1;
		map[5][28][4] = 1;
		map[5][29][4] = 1;
		map[5][30][4] = 1;
		map[6][3][4] = 1;
		map[6][4][4] = 1;
		map[6][5][4] = 1;
		map[6][6][4] = 1;
		map[6][7][4] = 1;
		map[6][8][4] = 1;
		map[6][25][4] = 1;
		map[6][26][4] = 1;
		map[6][27][4] = 1;
		map[6][28][4] = 1;
		map[6][29][4] = 1;
		map[6][30][4] = 1;
		map[7][3][4] = 1;
		map[7][4][4] = 1;
		map[7][5][4] = 1;
		map[7][6][4] = 1;
		map[7][7][4] = 1;
		map[7][13][4] = 2;
		map[7][14][4] = 2;
		map[7][15][4] = 1;
		map[7][26][4] = 1;
		map[7][27][4] = 1;
		map[7][28][4] = 1;
		map[7][29][4] = 1;
		map[7][30][4] = 1;
		map[8][2][4] = 1;
		map[8][3][4] = 1;
		map[8][4][4] = 1;
		map[8][5][4] = 1;
		map[8][6][4] = 1;
		map[8][7][4] = 1;
		map[8][12][4] = 2;
		map[8][13][4] = 2;
		map[8][14][4] = 1;
		map[8][15][4] = 1;
		map[8][16][4] = 1;
		map[8][27][4] = 1;
		map[8][28][4] = 1;
		map[8][29][4] = 1;
		map[8][30][4] = 1;
		map[9][2][4] = 1;
		map[9][3][4] = 1;
		map[9][4][4] = 1;
		map[9][5][4] = 1;
		map[9][6][4] = 1;
		map[9][10][4] = 2;
		map[9][11][4] = 2;
		map[9][12][4] = 2;
		map[9][13][4] = 1;
		map[9][14][4] = 1;
		map[9][15][4] = 1;
		map[9][16][4] = 1;
		map[9][17][4] = 1;
		map[9][18][4] = 1;
		map[9][19][4] = 1;
		map[9][27][4] = 1;
		map[9][28][4] = 1;
		map[9][29][4] = 1;
		map[9][30][4] = 1;
		map[10][1][4] = 1;
		map[10][2][4] = 1;
		map[10][3][4] = 1;
		map[10][4][4] = 1;
		map[10][5][4] = 1;
		map[10][10][4] = 2;
		map[10][11][4] = 1;
		map[10][12][4] = 1;
		map[10][13][4] = 1;
		map[10][14][4] = 1;
		map[10][15][4] = 1;
		map[10][16][4] = 1;
		map[10][17][4] = 1;
		map[10][18][4] = 1;
		map[10][19][4] = 1;
		map[10][27][4] = 1;
		map[10][28][4] = 1;
		map[10][29][4] = 1;
		map[10][30][4] = 1;
		map[11][1][4] = 1;
		map[11][2][4] = 1;
		map[11][3][4] = 1;
		map[11][4][4] = 1;
		map[11][10][4] = 1;
		map[11][11][4] = 1;
		map[11][12][4] = 1;
		map[11][13][4] = 1;
		map[11][14][4] = 1;
		map[11][15][4] = 1;
		map[11][16][4] = 1;
		map[11][17][4] = 1;
		map[11][18][4] = 1;
		map[11][19][4] = 1;
		map[11][20][4] = 1;
		map[11][27][4] = 1;
		map[11][28][4] = 1;
		map[11][29][4] = 1;
		map[11][30][4] = 1;
		map[12][1][4] = 1;
		map[12][2][4] = 1;
		map[12][3][4] = 1;
		map[12][9][4] = 1;
		map[12][10][4] = 1;
		map[12][11][4] = 1;
		map[12][12][4] = 1;
		map[12][13][4] = 1;
		map[12][14][4] = 1;
		map[12][15][4] = 1;
		map[12][16][4] = 1;
		map[12][17][4] = 1;
		map[12][18][4] = 1;
		map[12][19][4] = 1;
		map[12][20][4] = 1;
		map[12][28][4] = 1;
		map[12][29][4] = 1;
		map[12][30][4] = 1;
		map[13][1][4] = 1;
		map[13][2][4] = 1;
		map[13][3][4] = 1;
		map[13][9][4] = 1;
		map[13][10][4] = 1;
		map[13][11][4] = 1;
		map[13][12][4] = 1;
		map[13][13][4] = 1;
		map[13][14][4] = 1;
		map[13][15][4] = 1;
		map[13][16][4] = 1;
		map[13][19][4] = 1;
		map[13][20][4] = 1;
		map[13][21][4] = 1;
		map[13][28][4] = 1;
		map[13][29][4] = 1;
		map[13][30][4] = 1;
		map[14][1][4] = 1;
		map[14][2][4] = 1;
		map[14][3][4] = 1;
		map[14][8][4] = 1;
		map[14][9][4] = 1;
		map[14][10][4] = 1;
		map[14][11][4] = 1;
		map[14][12][4] = 1;
		map[14][13][4] = 1;
		map[14][14][4] = 1;
		map[14][20][4] = 1;
		map[14][21][4] = 1;
		map[14][28][4] = 1;
		map[14][29][4] = 1;
		map[14][30][4] = 1;
		map[15][1][4] = 1;
		map[15][2][4] = 1;
		map[15][8][4] = 1;
		map[15][9][4] = 1;
		map[15][10][4] = 1;
		map[15][11][4] = 1;
		map[15][12][4] = 1;
		map[15][13][4] = 1;
		map[15][20][4] = 1;
		map[15][21][4] = 1;
		map[15][22][4] = 1;
		map[15][29][4] = 1;
		map[15][30][4] = 1;
		map[16][1][4] = 1;
		map[16][2][4] = 1;
		map[16][8][4] = 1;
		map[16][9][4] = 1;
		map[16][10][4] = 1;
		map[16][11][4] = 1;
		map[16][12][4] = 1;
		map[16][21][4] = 1;
		map[16][22][4] = 1;
		map[16][29][4] = 1;
		map[16][30][4] = 1;
		map[17][1][4] = 1;
		map[17][9][4] = 1;
		map[17][10][4] = 1;
		map[17][21][4] = 1;
		map[17][22][4] = 1;
		map[17][30][4] = 1;
		map[18][1][4] = 1;
		map[18][10][4] = 1;
		map[18][11][4] = 1;
		map[18][12][4] = 1;
		map[18][22][4] = 1;
		map[18][23][4] = 1;
		map[18][30][4] = 1;
		map[19][1][4] = 1;
		map[19][10][4] = 1;
		map[19][11][4] = 1;
		map[19][12][4] = 1;
		map[19][13][4] = 1;
		map[19][14][4] = 1;
		map[19][15][4] = 1;
		map[19][16][4] = 1;
		map[19][22][4] = 1;
		map[19][23][4] = 1;
		map[19][30][4] = 1;
		map[20][1][4] = 1;
		map[20][11][4] = 1;
		map[20][12][4] = 1;
		map[20][13][4] = 1;
		map[20][14][4] = 1;
		map[20][15][4] = 1;
		map[20][16][4] = 1;
		map[20][17][4] = 1;
		map[20][18][4] = 1;
		map[20][19][4] = 1;
		map[20][20][4] = 1;
		map[20][21][4] = 1;
		map[20][22][4] = 1;
		map[20][23][4] = 1;
		map[20][30][4] = 3;
		map[21][1][4] = 1;
		map[21][2][4] = 1;
		map[21][13][4] = 1;
		map[21][14][4] = 1;
		map[21][15][4] = 1;
		map[21][16][4] = 1;
		map[21][17][4] = 1;
		map[21][18][4] = 1;
		map[21][19][4] = 1;
		map[21][20][4] = 1;
		map[21][21][4] = 1;
		map[21][22][4] = 1;
		map[21][30][4] = 3;
		map[22][1][4] = 1;
		map[22][2][4] = 1;
		map[22][15][4] = 1;
		map[22][16][4] = 1;
		map[22][17][4] = 1;
		map[22][18][4] = 1;
		map[22][19][4] = 1;
		map[22][30][4] = 1;
		map[23][1][4] = 1;
		map[23][2][4] = 1;
		map[23][3][4] = 1;
		map[23][30][4] = 1;
		map[24][1][4] = 1;
		map[24][2][4] = 1;
		map[24][3][4] = 1;
		map[24][30][4] = 1;
		map[25][1][4] = 1;
		map[25][2][4] = 1;
		map[25][3][4] = 1;
		map[25][4][4] = 1;
		map[25][29][4] = 1;
		map[25][30][4] = 1;
		map[26][1][4] = 1;
		map[26][2][4] = 1;
		map[26][3][4] = 1;
		map[26][4][4] = 1;
		map[26][5][4] = 1;
		map[26][29][4] = 1;
		map[26][30][4] = 1;
		map[27][1][4] = 1;
		map[27][2][4] = 1;
		map[27][3][4] = 1;
		map[27][4][4] = 1;
		map[27][5][4] = 1;
		map[27][6][4] = 4;
		map[27][7][4] = 4;
		map[27][8][4] = 4;
		map[27][29][4] = 1;
		map[27][30][4] = 1;
		map[28][1][4] = 1;
		map[28][2][4] = 1;
		map[28][3][4] = 1;
		map[28][4][4] = 1;
		map[28][5][4] = 1;
		map[28][6][4] = 1;
		map[28][7][4] = 1;
		map[28][8][4] = 1;
		map[28][9][4] = 1;
		map[28][10][4] = 1;
		map[28][28][4] = 1;
		map[28][29][4] = 1;
		map[28][30][4] = 1;
		map[29][1][4] = 1;
		map[29][10][4] = 1;
		map[29][11][4] = 1;
		map[29][12][4] = 1;
		map[29][13][4] = 1;
		map[29][14][4] = 1;
		map[29][15][4] = 1;
		map[29][16][4] = 1;
		map[29][17][4] = 1;
		map[29][24][4] = 1;
		map[29][25][4] = 1;
		map[29][26][4] = 1;
		map[29][27][4] = 1;
		map[29][28][4] = 1;
		map[29][29][4] = 1;
		map[29][30][4] = 1;
		map[30][1][4] = 1;
		map[30][13][4] = 1;
		map[30][14][4] = 1;
		map[30][18][4] = 1;
		map[30][19][4] = 1;
		map[30][20][4] = 1;
		map[30][21][4] = 1;
		map[30][22][4] = 1;
		map[30][23][4] = 1;
		map[30][24][4] = 1;
		map[30][25][4] = 1;
		map[30][26][4] = 1;
		map[30][27][4] = 1;
		map[30][28][4] = 1;
		map[30][29][4] = 1;
		map[30][30][4] = 1;
	}
}
