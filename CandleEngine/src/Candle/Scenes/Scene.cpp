#include "cdlpch.h"
#include "Scene.h"

#include "Candle/Application.h"

#include "Candle/ECS/Entity.h"
#include "Candle/ECS/ECS.h"
#include "Candle/ECS/Components/EngineComponents.h"

#include "Candle/Core/FileSystem.h"
#include "Candle/Assets/Assets.h"

namespace Candle {

	Scene::Scene()
	{
		_sceneID = sceneID++;
		_sceneBuffer = Candle::FrameBuffer::Create({ Candle::FrameBufferType::Texture, Candle::FrameBufferType::DepthBuffer }, CDL_APP_WIDTH, CDL_APP_HEIGHT);
		_sceneBuffer->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);

		_sceneTexture = Candle::Texture2D::Create(_sceneBuffer->Get(Candle::FrameBufferType::Texture), CDL_APP_WIDTH, CDL_APP_HEIGHT);
	}


	void Scene::Load(const std::string& scenefilePath)
	{
		_sceneID = sceneID++;
		_sceneBuffer = Candle::FrameBuffer::Create({ Candle::FrameBufferType::Texture, Candle::FrameBufferType::DepthBuffer }, CDL_APP_WIDTH, CDL_APP_HEIGHT);
		_sceneBuffer->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);

		_sceneTexture = Candle::Texture2D::Create(_sceneBuffer->Get(Candle::FrameBufferType::Texture), CDL_APP_WIDTH, CDL_APP_HEIGHT);

		std::fstream sceneFile(scenefilePath);
		std::string line;

		while ( std::getline(sceneFile, line) ) {

			if ( line.find("Entity") != std::string::npos ) {

				Entity newEntity;
				std::getline(sceneFile, line);

				while ( !line.empty() ) {

					if ( line.find("EntityTagComponent") != std::string::npos ) {
						
						std::getline(sceneFile, line);
						std::string name = line.substr(line.find_first_of(':') + 1, line.size());

						std::getline(sceneFile, line);
						std::string uuid = line.substr(line.find_first_of(':') + 1, line.size());

						newEntity = Entity{ this, name, uuid };
					}

					if ( line.find("HierarchyComponent") != std::string::npos ) {

						if ( newEntity.HasComponent<HierarchyComponent>() ) {
							newEntity.GetComponent<HierarchyComponent>().Deserialize(sceneFile);
						} else {
							newEntity.AddComponent<HierarchyComponent>().Deserialize(sceneFile);
						}
					}

					if ( line.find("SpriteRenderer") != std::string::npos ) {
						newEntity.AddComponent<SpriteRenderer>().Deserialize(sceneFile);
					}

					if ( line.find("Transform") != std::string::npos ) {
						newEntity.AddComponent<Transform>().Deserialize(sceneFile);
					}

					if ( line.find("CameraHandler") != std::string::npos ) {
						newEntity.AddComponent<CameraHandler>().Deserialize(sceneFile);
					}

					if ( line.find("ScriptComponent") != std::string::npos ) {
						newEntity.AddComponent<ScriptComponent>().Deserialize(sceneFile);
					}

					std::getline(sceneFile, line);
				}

			}

		}
	}


	void Scene::Unload(const std::string& sceneFilePath)
	{
		Save(sceneFilePath);
		_sceneRegistery.Clear();
	}


	void Scene::Save(const std::string& sceneFilePath)
	{
		if ( sceneFilePath.size() == 0 ) {
			CERROR("Couldn't save scene to file : no path provided");
			return;
		}

		try {

			std::fstream file(sceneFilePath, std::ios::out);

			for ( auto [uuid, nativeEntity] : _sceneRegistery.ViewEntities() ) {

				Entity currentEntity = { this, nativeEntity };
				file << "Entity:" << std::endl;
				for ( auto component : _sceneRegistery.View(nativeEntity) ) {

					CINFO("Serialization of : {0}-{1}", currentEntity.GetUUID(), typeid( *component ).name());
					Component* componentPtr = (Component*)component;
					if ( !componentPtr->IsSerializable() ) continue;
					file << "\tComponent:" << componentPtr->GetComponentName() << std::endl;
					componentPtr->Serialize(file);
				}

				file << std::endl;

			}

			for ( auto texture : Assets::GetAllTexture2D() ) {

				file << "Asset:" << std::endl;
				file << "\ttype:texture2D" << std::endl;
				file << "\t" << texture.first << std::endl;

				file << std::endl;
			}

		} catch ( std::exception e ) {
			CERROR("Error while saving scene to path {1} : {2}", sceneFilePath, e.what());
		}
	}

}