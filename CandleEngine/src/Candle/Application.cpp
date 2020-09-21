#include "cdlpch.h"

#include "Application.h"
#include "Core/Input.h"
#include "Core/Time.h"

#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include "Assets/Assets.h"
#include "ECS/ECS.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
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
		Audio::Clear();
	}


	void Application::OnEvent (Event & event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(Application::OnWindowResize));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(Application::OnKeyPressed));

		for ( auto it = _stack.end(); it != _stack.begin();) {
			( *--it )->OnEvent(event);
			if ( event.IsHandled() ) return;
		}

		SceneManagement::OnEvent(event);

	}


	void Application::Run()
	{
		static long frameCount = 0;

		while (_running) {

			Time::BeginFrame();

			if (!_minimized) {

				SceneManagement::OnUpdate();

				RenderCommands::Clear();
				Renderer::BeginScene();
				SceneManagement::OnRender();
				Renderer::EndScene();

					// TODO : Refactor post processing completely
				//PostProcessing::Process(SceneManagement::FinalTexture())
				//GUI::Render();
				//GUI::PostProcess();
				//Renderer::Combine();
				Renderer2D::DrawFrameBuffer(SceneManagement::FinalSceneTexture());

				for ( auto it = _stack.begin(); it != _stack.end(); it++ ) {
					( *it )->OnUpdate();
					( *it )->OnDraw();
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
		return false;
	}



	uint32_t Application::GetWidth() 
	{ 
		return _instance->GetWindow().GetWidth(); 
	}

	uint32_t Application::GetHeight()
	{
		return _instance->GetWindow().GetHeight(); 
	}

	double Application::AspectRatio() 
	{
		return _instance->GetWindow().GetWidth() / (double)_instance->GetWindow().GetHeight(); 
	}

	bool Application::IsFullScreen()
	{
		return _instance->Get()._fullscreened; 
	}

	void Application::Stop()
	{
		_instance->_running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		_stack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		_stack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		_stack.PopLayer(layer);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		_stack.PopOverlay(overlay);
	}
}