#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "IApplication.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/VertexArray.hpp"
#include "Utils/Camera3D.hpp"
#include "Utils/ContentManager.hpp"
#include "Utils/Window.hpp"

#include "Graphics/Model.hpp"
#include "Input/InputManager.hpp"

namespace Applications
{
	class Application : public IApplication
	{
		private:
			glm::vec3 lightPos;
			std::unique_ptr<Graphics::Texture> boxDiffuseMap;
			std::unique_ptr<Graphics::Texture> redstoneDiffuseMap;
			std::unique_ptr<Graphics::Texture> boxSpecularMap;
			std::unique_ptr<Graphics::Texture> redstoneSpecularMap;
			std::unique_ptr<Graphics::Texture> goldDiffuseMap;
			std::unique_ptr<Graphics::Texture> goldSpecularMap;

		protected:
			Input::InputManager inputManager;
			Utils::ContentManager content;

			std::unique_ptr<Utils::Window> window;
			std::unique_ptr<Utils::Camera3D> camera;
			std::unique_ptr<Graphics::ShaderProgram> objectShader;
			std::unique_ptr<Graphics::ShaderProgram> lightShader;
			std::unique_ptr<Graphics::ShaderProgram> modelShader;
			std::unique_ptr<Graphics::VertexArray> objectVa;
			std::unique_ptr<Graphics::VertexArray> lightVa;

			std::unique_ptr<Graphics::Model> bed;
			std::unique_ptr<Graphics::Model> torch;

			int map[32][32][32];
			int sizeX = 32;
			int sizeY = 32;
			int sizeZ = 6;

			void Initialize();
			void LoadContent();
			void UnloadContent();
			void Update(float deltaTime);
			void Render() const;
			void LoadMap();
		public:
			Application();

			void Run();

		Input::InputManager& GetInputManager() override
		{
			return inputManager;
		}
	};
}
