#pragma once

#include "Candle/CandleCore.h"

	/* Primitive types */
#include "Texture.h"
#include "Shader.h"

	/* Derived types */
#include "Texture2D.h"

namespace Candle {

	class CANDLE_API AssetManager {

		public:
			AssetManager() {}
			~AssetManager() {}

			static void Init();
			static void Reload();
			inline static AssetManager& Get() { return *_instance; }


				/* Shaders */
			static void BindShader(const std::string& name) { _instance->BindShaderImpl(name); }
			static Shared<Shader> ReloadShader(const std::string& name) { return _instance->ReloadShaderImpl(name); }
			static Shared<Shader> GetShader(const std::string& name) { return _instance->GetShaderImpl(name); }
			static const std::unordered_map<std::string, Shared<Shader>> GetAllShaders() { return _instance->_shaders; }


				/* Textures 2D */
			static Shared<Texture2D> GetTexture2D(const std::string& name) { return _instance->GetTexture2DImpl(name); }
			static const std::unordered_map<std::string, Shared<Texture2D>> GetAllTexture2D() { return _instance->_textures2D; }
			

		private:
			static AssetManager* _instance;

			std::unordered_map<std::string, Shared<Shader>> _shaders;
			std::string _boundShader;

			std::unordered_map<std::string, Shared<Texture2D>> _textures2D;

			void BindShaderImpl(const std::string& name);
			Shared<Shader> ReloadShaderImpl(const std::string& name);
			Shared<Shader> GetShaderImpl(const std::string& name);

			void LoadAllShaders();
			void LoadShader(const std::string & path, const std::string & name);
			void LoadDefaultShaders();


			Shared<Texture2D> GetTexture2DImpl(const std::string& name);

			void LoadAllTextures();
			void LoadTexture(const std::string & path, const std::string & name);
			void LoadDefaultTextures();
	};

}