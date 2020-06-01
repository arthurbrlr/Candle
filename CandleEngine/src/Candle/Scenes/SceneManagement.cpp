#include "cdlpch.h"
#include "SceneManagement.h"

#include "Candle/Application.h"
#include "Candle/Renderer/Renderer.h"
#include "Candle/Renderer/CameraManagement.h"
#include "Candle/PostProcessing/PostProcessing.h"
#include "Candle/ECS/ECS.h"

#include "Candle/Debug/Editor.h"

namespace Candle {

	unsigned int SceneManagement::_currentScene = -1;
	std::unordered_map<unsigned int, Shared<Scene>> SceneManagement::_scenes;
	Shared<Texture2D> SceneManagement::_finalTexture = nullptr;

	void SceneManagement::Init()
	{
	}


	void SceneManagement::AddScene(Scene* scene)
	{
		Shared<Scene> sceneptr(scene);
		_scenes[scene->GetID()] = std::move(sceneptr);
		_currentScene = scene->GetID();
	}


	void SceneManagement::LoadScene(unsigned int sceneID)
	{
		if (_currentScene != -1) {
			_scenes[_currentScene]->Unload();
		}

		ECS::ClearBlueprints();
		CameraManagement::Init();

		_currentScene = sceneID;
		_scenes[sceneID]->Load();
	}


	void SceneManagement::OnUpdate()
	{
		ECS::Update();
		_scenes[_currentScene]->OnUpdate();
	}


	void SceneManagement::OnRender()
	{
		_scenes[_currentScene]->GetFrameBuffer()->Bind();
			// v temp line
		RenderCommands::SetLinesRendering(Editor::Variables().DrawLines);
		RenderCommands::Clear();
		ECS::Render();
			// v temp line
		RenderCommands::SetLinesRendering(false);
		_scenes[_currentScene]->GetFrameBuffer()->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);


			// Move postprocessing out of here
		_finalTexture = _scenes[_currentScene]->GetTexture();
		PostProcessing::SetRenderViewport(glm::vec4(0, 0, CDL_APP_WIDTH, CDL_APP_HEIGHT));
		
		if ( PostProcessing::GetFinalNodeID() != -1 ) {
			PostProcessing::Process(_finalTexture);
			_finalTexture = PostProcessing::GetFinalTexture();
		}

	}


	void SceneManagement::OnEditorRender()
	{
		RenderCommands::SetLinesRendering(Editor::Variables().DrawLines);
		RenderCommands::Clear();
		ECS::Render();
		RenderCommands::SetLinesRendering(false);
	}


	void SceneManagement::OnEvent(Event& event)
	{
		_scenes[_currentScene]->OnEvent(event);
	}

}