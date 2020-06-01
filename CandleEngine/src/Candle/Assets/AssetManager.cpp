#include "cdlpch.h"
#include "AssetManager.h"
#include "Candle/Renderer/Renderer2D.h"

#include "Res/EngineShaders.h"

namespace Candle {


	/*
		
		This class neeeds refactor: one unique templated function LoadAllAssetsOfType<AssetType>
		-> Paths are stored in an external file with the following format:

		--- pathdefs.cdl ---

		@Assets=textures
		res/textures

		@Assets=shaders
		res/shaders

		etc...

	*/

	AssetManager* AssetManager::_instance = new AssetManager();


	void AssetManager::Init()
	{
		_instance->LoadAllShaders();
		_instance->LoadAllTextures();
	}


	void AssetManager::Reload()
	{
		Get()._textures2D.clear();
		Get()._shaders.clear();

		_instance->LoadAllShaders();
		_instance->LoadAllTextures();
	}



		/* --- Shaders --- */

	void AssetManager::BindShaderImpl(const std::string & name)
	{
		if (_boundShader != "")
			_shaders[_boundShader]->Unbind();

		_boundShader = name;
		_shaders[_boundShader]->Bind();
	}


	Shared<Shader> AssetManager::ReloadShaderImpl(const std::string & name)
	{
		Unique<Shader> shader = Shader::Create(_shaders[name]->GetPath());
		_shaders.erase(name);
		_shaders.emplace(name, std::move(shader));

			// temp line
		if (name == "texture")
			Renderer2D::OnTextureShaderReload();
			// temporary, generalise this shit

		return _shaders[name];
	}


	Shared<Shader> AssetManager::GetShaderImpl(const std::string & name)
	{
		if (_shaders.find(name) == _shaders.end()) return _shaders["texture"];
		return _shaders[name];
	}


	Shared<Texture2D> AssetManager::GetTexture2DImpl(const std::string & name)
	{
		if (_textures2D.find(name) == _textures2D.end()) return _textures2D["CDL_TEXTURE_DEFAULT"];
		return _textures2D[name];
	}



			/* --- Privates --- */

	void AssetManager::LoadAllShaders()
	{
		std::string parentFolder = "res/shaders";
		int i = 0;

		try {
			for (const auto& entry : std::filesystem::directory_iterator(parentFolder)) {
				LoadShader(entry.path().string(), entry.path().stem().string());
			}
		} catch (std::exception e) {
			CINFO("No shaders found");
		}

		LoadDefaultShaders();
	}


	void AssetManager::LoadShader(const std::string & path, const std::string & name)
	{
		Unique<Shader> shader = Shader::Create(path);
		//if (_shaders.find(name) != _shaders.end()) return;
		_shaders.emplace(name, std::move(shader));
	}


	void AssetManager::LoadDefaultShaders()
	{
		Unique<Shader> textureShader = Shader::Create(CDL_SHADER_Texture, "CDL_SHADER_TEXTURE");
		_shaders.emplace("CDL_SHADER_TEXTURE", std::move(textureShader));
		
		Unique<Shader> fullscreenShader = Shader::Create(CDL_SHADER_Fullscreen, "CDL_SHADER_FSQuad");
		_shaders.emplace("CDL_SHADER_FSQuad", std::move(fullscreenShader));
	}




	void AssetManager::LoadAllTextures()
	{
		std::string parentFolder = "res/textures";
		int i = 0;

		try {
			for (const auto& entry : std::filesystem::directory_iterator(parentFolder)) {
				if (entry.path().extension().string() != ".png" && entry.path().extension().string() != ".PNG") continue;
				LoadTexture(entry.path().string(), entry.path().stem().string());
			}
		} catch (std::exception e) {
			CINFO("No Textures found");
		}

		LoadDefaultTextures();
	}


	void AssetManager::LoadTexture(const std::string & path, const std::string & name)
	{
		Unique<Texture2D> texture = Texture2D::Create(path);
		//if (_textures2D.find(name) != _textures2D.end()) return;
		_textures2D.emplace(name, std::move(texture));
	}


	void AssetManager::LoadDefaultTextures()
	{
		Unique<Texture2D> whiteTexture = Texture2D::Create(1, 1, 3);
		unsigned int whiteTextureData = 0xffffff;
		whiteTexture->SetData(&whiteTextureData, 3);
		_textures2D.emplace("CDL_TEXTURE_WHITE", std::move(whiteTexture));
		sizeof(unsigned int);
		Unique<Texture2D> defaultTexture = Texture2D::Create(1, 1, 3);
		unsigned int defaultTextureData = 0xff00f8;
		defaultTexture->SetData(&defaultTextureData, 3);
		_textures2D.emplace("CDL_TEXTURE_DEFAULT", std::move(defaultTexture));		
	}


}