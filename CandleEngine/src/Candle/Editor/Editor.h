#pragma once

#include "Candle/CandleCore.h"

#include "ImguiLayer.h"
#include "EditorCameraController.h"

#include "Candle/Core/Time.h"
#include "Candle/Layers/LayerStack.h"

#include "Candle/PostProcessing/PostProcessing.h"
#include "Candle/Assets/AssetManager.h"
#include "Candle/ECS/ECS.h"

#include "Candle/Audio/AudioMixer.h"

#include "Candle/Renderer/Renderer.h"
#include "Candle/Renderer/Renderer2D.h"

#include "GUI/GuiAPI.h"

namespace Candle {

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
	};

	class CANDLE_API Editor {

		public:
			static void Init(); /* Initialise the Editor */
			static bool Show() { return _eb.ShowEditor; } /* Return a bool representing if the editor is shown or hidden */
			static void OnUpdate(); /* Called every frame to update the editor camera */
			static void OnEvent(Event& event); /* React to an event */
			static void Display(); /* Called every frame to display the editor */
			static void Clear(); /* Clear imgui context */

			static EditorVariables & Variables() { return _eb; }

			static bool PlayGame() { return _eb.PlayGame; }
			static bool RenderGameView() { return _eb.RenderGameView; }
			static EditorCameraController & Controller() { return _cameraController; }
			static RawCamera & Camera() { return _cameraController.GetCamera(); }
			static void UseOrthograpic(bool state) { _cameraController.UseOrthographic(state); }

		private:
			static Unique<ImGuiLayer> _imguiLayer;
			static EditorVariables _eb;
			static Shared<FrameBuffer> _editorFrameBuffer;
			static Shared<Texture2D> _editorTexture;

			static EditorCameraController _cameraController;

			static void BuildDockSpace();
			static void ShowMainMenu();
			static void ShowMasterWindow();
			static void ShowEditorViewer();
			static void ShowGameViewer();
			static void ShowAssetManagerWindow();
			static void ShowSceneHierarchy();
			static void ShowComponentsOf(Blueprint* bp);
			static void ShowPostProcessingPipeline();
			static void ShowRendererWindow();

	};

}