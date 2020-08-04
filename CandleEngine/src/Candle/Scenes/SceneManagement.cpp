#include "cdlpch.h"
#include "SceneManagement.h"
#include "EmptyScene.h"

#include "Candle/Application.h"
#include "Candle/Renderer/Renderer.h"
#include "Candle/Renderer/CameraManagement.h"
#include "Candle/PostProcessing/PostProcessing.h"
#include "Candle/ECS/ECS.h"

#include "Candle/Editor/Editor.h"

namespace Candle {

	uint32_t SceneManagement::_currentScene = -1;
	std::unordered_map<uint32_t, Shared<Scene>> SceneManagement::_scenes;
	Shared<Texture2D> SceneManagement::_finalTexture = nullptr;

	void SceneManagement::Init()
	{
		if ( _currentScene == -1 ) {
			AddScene(new EmptyScene());
			LoadScene(_currentScene);
		}
	}


	void SceneManagement::AddScene(Scene* scene)
	{
		Shared<Scene> sceneptr(scene);
		_scenes[scene->GetID()] = std::move(sceneptr);
		_currentScene = scene->GetID();
	}


	void SceneManagement::LoadScene(uint32_t sceneID)
	{
		if (_currentScene != -1) {
			_scenes[_currentScene]->Unload();
		}

		ECS::ClearBlueprints();
		CameraManagement::Init();

		_currentScene = sceneID;
		_scenes[sceneID]->Load();

		Application::SetTitle("Candle - " +_scenes[sceneID]->GetName());
	}


	void SceneManagement::OnUpdate()
	{
		ECS::Update();
		_scenes[_currentScene]->OnUpdate();
	}


	void SceneManagement::OnRender()
	{
		if ( _currentScene == -1 ) return;
		_scenes[_currentScene]->GetFrameBuffer()->Bind();
		// v temp line
		RenderCommands::SetLinesRendering(Editor::Variables().DrawLines);
		RenderCommands::Clear();
		
		RenderScene();

			// v temp line
		RenderCommands::SetLinesRendering(false);
		_scenes[_currentScene]->GetFrameBuffer()->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);


			// Move postprocessing out of here
		_finalTexture = _scenes[_currentScene]->GetTexture();
		
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
		if ( _currentScene == -1 ) return;
		_scenes[_currentScene]->OnEvent(event);
	}


	void SceneManagement::RenderScene()
	{
		Animation* _animptr = new Animation("default", {});
		std::vector<Shared<Blueprint>> bps = BlueprintManager::Get<SpriteRenderer>();

		if ( bps.size() == 0 ) return;

		double t1 = Time::Milliseconds();

		Renderer2D::BeginScene();
		Renderer2D::GetStats()->Reset();

		/* Sprite Rendering */
		for ( auto& bp : bps ) {

			SpriteRenderer& srComp = bp->GetComponent<SpriteRenderer>();

			if ( !srComp.IsActive() ) continue;

			if ( bp->HasComponent<AnimationController>() ) {
				bool animBool = bp->GetComponent<AnimationController>().GetCurrentAnimation(*_animptr);

				if ( animBool ) {
					srComp.SetTextureCoordinates(_animptr->GetKeyframes()[_animptr->GetCurrentKeyFrameIndex()].textureCoordinates);
				}
			}

			if ( bp->HasComponent<Transform>() ) {
				Renderer2D::DrawSprite(srComp, bp->GetComponent<Transform>());
			} else {
				Renderer2D::DrawSprite(srComp);
			}

		}


		// Sprite part test
		SpritePart part1 = { "part1" , {0, 1, 1}, { 0, 1, 0, 1 }, { 0.1, 0.1 } };
		SpritePart part2 = { "part2" , {1, 0, 1}, { 0, 1, 0, 1 }, { 0.2, 0.2 } };
		SpritePart part3 = { "part3" , {0, 0, 1}, { 0, 1, 0, 1 }, { 0.3, 0.1 } };

		std::vector<SpritePart> parts = {
			part1,
			part2,
			part3
		};

		//Renderer2D::DrawPartialSprite(Transform(nullptr), Assets::GetTexture2D("dodo"), parts, false);


		Renderer2D::EndScene();
		Renderer2D::GetStats()->ecsSpriteRenderTime = Time::Milliseconds() - t1;

		delete _animptr;
	}

}