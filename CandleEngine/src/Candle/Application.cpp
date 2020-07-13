#include "cdlpch.h"

#include "Application.h"
#include "Core/Input.h"
#include "Core/Time.h"

#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "Editor/Editor.h"
#include "Assets/Assets.h"
#include "ECS/ECS.h"
#include "Renderer/Renderer.h"
#include "PostProcessing/PostProcessing.h"
#include "Audio/AudioMixer.h"
#include "Scenes/SceneManagement.h"


namespace Candle {

	Application* Application::_instance = nullptr;

	Application::Application(const std::string & appName)
	{
		Time::Start();

		CASSERT(!_instance, "Application already exist!");

		if ( _instance != nullptr ) {
			CASSERT(false, "Two applications can't exist at the same time");
		} else {
		_instance = this;
		}

		_window = Unique<Window>(Window::Create(WindowProperties(appName)));
		_window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

		Input::Init();

		Editor::Init();
		Assets::Init();
		SceneManagement::Init();

		ECS::Init();

		Renderer::Init();
		PostProcessing::Init();

		Audio::Init();
	}


	Application::~Application()
	{
		Renderer::Clear();
		Editor::Clear();
		Audio::Clear();
	}


	void Application::OnEvent (Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(Application::OnKeyPressed));

		SceneManagement::OnEvent(event);
		
		if ( Editor::Show() )
			Editor::OnEvent(event);
	}


	void Application::Run()
	{
		static long frameCount = 0;

		while (_running) {

			Time::BeginFrame();

			if (!_minimized) {

				if ( Editor::PlayGame() || !Editor::Show() )
					SceneManagement::OnUpdate();

				if ( Editor::RenderGameView() || !Editor::Show() ) {
					Renderer::BeginScene();
					SceneManagement::OnRender();
					Renderer::EndScene();
				}

					// TODO in the future
				//PostProcessing::Process(SceneManagement::FinalTexture())
				//GUI::Render();
				//GUI::PostProcess();
				//Renderer::Combine();

				
				if (!Editor::Show()) {
					Renderer2D::DrawFrameBuffer(SceneManagement::FinalSceneTexture());
				} else {
					Editor::OnUpdate();
					Editor::Display();
				}
				
			}

			_window->OnUpdate();
			frameCount++;
		}

		CINFO("FPS (Average): {0}", frameCount / Time::Seconds());
	}


	bool Application::OnWindowClose(WindowClosedEvent & e)
	{
		_running = false;
		return true;
	}


	bool Application::OnWindowResize(WindowResizeEvent & e)
	{
		if ( (e.GetHeight() == 0 || e.GetWidth() == 0) && !_minimized) _minimized = true;
		else if ( (e.GetHeight() != 0 && e.GetWidth() != 0) && _minimized) _minimized = false;

		RenderCommands::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return false;
	}


	bool Application::OnKeyPressed(KeyPressedEvent & e)
	{
		if (e.GetKeyCode() == CDL_KEY_F11) {
			Editor::Variables().ShowEditor = !Editor::Variables().ShowEditor;
			return true;
		}


		if ( e.GetKeyCode() == CDL_KEY_ESCAPE ) {
			_running = false;
			return true;
		}

		return false;
	}
}