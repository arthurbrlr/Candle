#include "cdlpch.h"
#include "SceneManagement.h"
#include "EmptyScene.h"
#include "Candle/ECS/Entity.h"

#include "Candle/Application.h"
#include "Candle/Renderer/Renderer.h"
#include "Candle/Renderer/CameraManagement.h"
#include "Candle/PostProcessing/PostProcessing.h"
#include "Candle/ECS/ECS.h"

#include "Candle/Editor/Editor.h"

namespace Candle {

	Scene* SceneManagement::_currentScene = nullptr;
	std::string SceneManagement::_currentSceneFilePath = "";
	Shared<Texture2D> SceneManagement::_finalTexture = nullptr;

	void SceneManagement::Init()
	{
		CreateEmptyScene();
	}


	void SceneManagement::CreateEmptyScene()
	{
		if ( _currentScene ) {
			_currentScene->Unload();
			delete _currentScene;
		}

		_currentScene = new EmptyScene();
		( (EmptyScene*)_currentScene )->Load();
		_currentSceneFilePath = "";

		Application::SetTitle("Candle - default");
	}


	void SceneManagement::LoadScene(const std::string& scenePath)
	{
		if ( _currentScene ) {
			delete _currentScene;
		}

		CameraManagement::Init();

		_currentSceneFilePath = scenePath;
		_currentScene = new Scene();
		_currentScene->Load(scenePath);

		std::string sceneName = scenePath.substr(scenePath.find_last_of('\\') + 1, scenePath.size());
		std::string applicationTitle = "Candle - " + sceneName;
		Application::SetTitle(applicationTitle);
	}


	void SceneManagement::SaveCurrentScene(const std::string& scenePath)
	{
		if ( _currentScene && scenePath.size() == 0 ) {
			_currentScene->Save(_currentSceneFilePath);
		} else {
			_currentScene->Save(scenePath);
			_currentSceneFilePath = scenePath;

			std::string sceneName = scenePath.substr(scenePath.find_last_of('\\') + 1, scenePath.size());
			std::string applicationTitle = "Candle - " + sceneName;
			Application::SetTitle(applicationTitle);
		}
	}


	void SceneManagement::Reload()
	{
		if ( _currentSceneFilePath.size() == 0 ) {
			CERROR("Error: can't reload modified empty scene");
			return;
		}

		_currentScene->_sceneRegistery.Clear();
		_currentScene->Load(_currentSceneFilePath);
	}


	void SceneManagement::OnUpdate()
	{
		ECS::Update();
		_currentScene->OnUpdate();
	}


	void SceneManagement::OnRender()
	{
		if ( !_currentScene ) return;
		_currentScene->GetFrameBuffer()->Bind();
		// v temp line
		RenderCommands::SetLinesRendering(Editor::Variables().DrawLines);
		RenderCommands::Clear();
		
		RenderScene();

			// v temp line
		RenderCommands::SetLinesRendering(false);
		_currentScene->GetFrameBuffer()->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);


			// Move postprocessing out of here
		_finalTexture = _currentScene->GetTexture();
		
		if ( PostProcessing::Use() ) {
			PostProcessing::SetRenderViewport(glm::vec4(0, 0, CDL_APP_WIDTH, CDL_APP_HEIGHT));
			PostProcessing::Process(_finalTexture);
			_finalTexture = PostProcessing::GetFinalTexture();
		}

	}


	void SceneManagement::OnEditorRender()
	{
		RenderCommands::SetLinesRendering(Editor::Variables().DrawLines);
		RenderCommands::Clear();

		RenderScene();

		RenderCommands::SetLinesRendering(false);
	}


	void SceneManagement::OnEvent(Event& event)
	{
		if ( !_currentScene ) return;
		_currentScene->OnEvent(event);
	}


	void SceneManagement::RenderScene()
	{
		static Animation* _animptr = new Animation("default", {});
		auto& sprites = _currentScene->_sceneRegistery.View<SpriteRenderer>();

		if ( sprites.size() == 0 ) return;

		double t1 = Time::Milliseconds();

		Renderer2D::BeginScene();
		Renderer2D::GetStats()->Reset();

		/* Sprite Rendering */
		for ( auto& sprite : sprites ) {

			SpriteRenderer* srComp = (SpriteRenderer*)sprite.second;
			if ( !srComp->IsActive() ) continue;

			Entity currentEntity = Entity{ _currentScene, sprite.first };
			if ( currentEntity.HasComponent<AnimationController>() ) {
				bool animBool = currentEntity.GetComponent<AnimationController>().GetCurrentAnimation(*_animptr);

				if ( animBool ) {
					srComp->SetTextureCoordinates(_animptr->GetKeyframes()[_animptr->GetCurrentKeyFrameIndex()].textureCoordinates);
				}
			}

			if ( currentEntity.HasComponent<Transform>() ) {
				Renderer2D::DrawSprite(*srComp, currentEntity.GetComponent<Transform>());
			} else {
				Renderer2D::DrawSprite(*srComp);
			}

		}


			// Sprite part test
		/*
		SpritePart part1 = { "part1" , {0, 1, 1}, { 0, 1, 0, 1 }, { 0.1, 0.1 } };
		SpritePart part2 = { "part2" , {1, 0, 1}, { 0, 1, 0, 1 }, { 0.2, 0.2 } };
		SpritePart part3 = { "part3" , {0, 0, 1}, { 0, 1, 0, 1 }, { 0.3, 0.1 } };

		std::vector<SpritePart> parts = {
			part1,
			part2,
			part3
		};

		Transform transform;
		Renderer2D::DrawPartialSprite(transform, Assets::GetTexture2D("dodo"), parts, false);
		*/

		Renderer2D::EndScene();
		Renderer2D::GetStats()->ecsSpriteRenderTime = Time::Milliseconds() - t1;

	}

}