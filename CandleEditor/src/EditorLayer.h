#pragma once

#include "Candle/CandleCore.h"

#include "ImguiLayer.h"
#include "EditorCamera.h"

#include "Candle/Core/Time.h"
#include "Candle/Layers/LayerStack.h"

#include "Candle/PostProcessing/PostProcessing.h"
#include "Candle/Assets/Assets.h"
#include "Candle/ECS/ECS.h"

#include "Candle/Audio/AudioMixer.h"

#include "Candle/Renderer/Renderer.h"
#include "Candle/Renderer/Renderer2D.h"

#include "GUI/GuiAPI.h"

struct EditorVariables {
	std::string ContextVersion;
	std::string HardwareVendor;
	std::string HardwareRenderer;

	float TimeFactor = 1.0;

	bool ShowEditor = true;

	bool DrawTextures = true;
	bool DrawLines = false;

	bool MouseInGameViewport = false;
	bool MouseInEditorViewport = false;

	bool PlayGame = false;
	bool RenderGameView = true;
	bool FullScreen = false;
	bool ReadInputs = false;

	bool wasSceneModified = false;
};

class EditorLayer : public Candle::Layer {

public:
	void OnAttach() override; /* Initialise the Editor */
	void OnDetach() override; /* Clear imgui context */
	void OnUpdate() override; /* Called every frame to update the editor camera */
	void OnDraw() override; /* Called every frame to display the editor */
	void OnEvent(Candle::Event& event) override; /* React to an event */

	bool Show() { return _eb.ShowEditor; } /* Return a bool representing if the editor is shown or hidden */

	EditorVariables& Variables() { return _eb; }

	bool PlayGame() { return _eb.PlayGame; }
	bool RenderGameView() { return _eb.RenderGameView; }
	EditorCamera& Controller() { return _cameraController; }
	Candle:: RawCamera& Camera() { return _cameraController.GetCamera(); }
	void UseOrthograpic(bool state) { _cameraController.UseOrthographic(state); }

private:
	Candle::Unique<Candle::ImGuiLayer> _imguiLayer = nullptr;
	Candle::Shared<Candle::FrameBuffer> _editorFrameBuffer = nullptr;
	Candle::Shared<Candle::Texture2D> _editorTexture = nullptr;

	EditorVariables _eb;
	EditorCamera _cameraController;

	void BuildDockSpace();
	void ShowMainMenu();
	void ShowSettingsWindow(bool* showSettingsWindow);
	void ShowEditorViewer();
	void ShowGameViewer();
	void ShowTextEditor(bool* showTextEditor);
	void ShowAssetsWindow();
	void ShowSceneHierarchy();
	void ShowComponentsOf(Candle::Entity& entity);
	void ShowPostProcessingPipeline();
	void ShowRendererWindow(bool* showRendererWindow);
	void ShowInformationToolTip(const std::string& description);
};