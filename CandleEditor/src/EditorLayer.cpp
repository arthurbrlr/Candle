#include "EditorLayer.h"

#include "Candle/Core/FileSystem.h"
#include "Candle/Core/Input.h"
#include "Candle/ECS/Script.h"
#include "Candle/Scenes/EmptyScene.h"
#include "Candle/Scenes/SceneManagement.h"
#include "Candle/Renderer/CameraManagement.h"

using namespace Candle;

void EditorLayer::OnDraw()
{
	//double t1 = Time::Milliseconds();
		// Do not modify this line
	_eb.ReadInputs = true;
	_imguiLayer->Begin();
	BuildDockSpace();

	ShowMainMenu();

	_eb.ReadInputs = false;
	ShowGameViewer();
	_eb.ReadInputs = true;
	ShowEditorViewer();

	ShowAssetsWindow();
	ShowSceneHierarchy();
	ShowPostProcessingPipeline();
			   		 	  	  	 
	ECS::OnEditor();
	if ( SceneManagement::CurrentScene() ) SceneManagement::CurrentScene()->OnEditor();

	static bool sceneWasChanged = false;
	if ( sceneWasChanged ^ _eb.wasSceneModified ) {
		Application::SetChangesState(_eb.wasSceneModified);
		sceneWasChanged = _eb.wasSceneModified;
	}

		// Do not modify this line
	//double t2 = Time::Milliseconds();
	//ImGui::Begin("EditorTime");
	//ImGui::Text("%.5f", t2 - t1);
	//ImGui::End();
	_imguiLayer->End();
	_eb.ReadInputs = false;
}

	/*
		--- BuildDockSpace
		Setup of the editor dockspace
	*/
void EditorLayer::BuildDockSpace()
{
	// DockSpace
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiIO& io = ImGui::GetIO();
	if ( io.ConfigFlags & ImGuiConfigFlags_DockingEnable ) {
		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
}


	/*
		--- ShowMainMenu
	*/
void EditorLayer::ShowMainMenu()
{
	static char newSceneNameBuffer[32]{ ' ' };
	static bool createNewSceneModal = false;
	static bool closeApplicationRequest = false;
		
	static bool showSettingsWindow = false;
	static bool showTextEditor = false;


	auto saveCurrentScene = [&]() {
		_eb.wasSceneModified = false;

		std::string sceneFilePath;
		if ( SceneManagement::HasNoFilePath() && FileSystem::SaveFile(sceneFilePath, L"scene") ) {
			SceneManagement::SaveCurrentScene(sceneFilePath);
		} else {
			SceneManagement::SaveCurrentScene();
		}
	};


	auto openNewScene = [&]() {
		if ( _eb.wasSceneModified ) {
			saveCurrentScene();
		}

		std::string sceneFileToLoad;
		if ( FileSystem::OpenFile(sceneFileToLoad, L"scene", L"Scene Files\0*.scene\0") ) {
			SceneManagement::LoadScene(sceneFileToLoad);
			_eb.wasSceneModified = false;
		}
	};

		// Shortcuts
	if ( Input::IsKeyPressed(CDL_KEY_LEFT_CONTROL) && Input::OnKeyDown(CDL_KEY_S) ) saveCurrentScene();
	if ( Input::IsKeyPressed(CDL_KEY_LEFT_CONTROL) && Input::OnKeyDown(CDL_KEY_A) ) closeApplicationRequest = true;

		// Menu
	if ( ImGui::BeginMainMenuBar() ) {
		if ( ImGui::BeginMenu("File") ) {
			if ( ImGui::MenuItem("Save", "Ctrl + S") ) {
				saveCurrentScene();
			}

			ImGui::Separator();
			if ( ImGui::MenuItem("Settings") ) {
				showSettingsWindow = true;
			}

			ImGui::Separator();
			if ( ImGui::MenuItem("Close", "Ctrl + Q") ) {
				closeApplicationRequest = true;
			}

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu("Scene") ) {

			if ( ImGui::MenuItem("New Scene") ) {
				//createNewSceneModal = true;
				//memset(newSceneNameBuffer, '\0', 32);
				SceneManagement::CreateEmptyScene();
				_eb.wasSceneModified = false;
			}

			if ( ImGui::MenuItem("Open Scene File") ) {
				openNewScene();
			}

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu("Window") ) {

			if ( ImGui::MenuItem("Text Editor") )
				showTextEditor = true;

			ImGui::Separator();

			static bool state = false;
			if ( ImGui::MenuItem("Full Sceen", "", &_eb.FullScreen) ) {
				state = !state;
				Application::SetFullScreen(_eb.FullScreen);
			}

			ImGui::EndMenu();
		}

		/*
		if ( ImGui::BeginMenu("Edit") ) {
			if ( ImGui::MenuItem("Undo", "CTRL+Z") ) {}
			if ( ImGui::MenuItem("Redo", "CTRL+Y", false, false) ) {}  // Disabled item
			ImGui::Separator();
			if ( ImGui::MenuItem("Cut", "CTRL+X") ) {}
			if ( ImGui::MenuItem("Copy", "CTRL+C") ) {}
			if ( ImGui::MenuItem("Paste", "CTRL+V") ) {}
			ImGui::EndMenu();
		}
		*/
		ImGui::EndMainMenuBar();
	}

		/* Close application popup */
	if ( closeApplicationRequest ) ImGui::OpenPopup("Close Candle Editor");

	if ( ImGui::BeginPopupModal("Close Candle Editor", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize) ) {

		if ( SceneManagement::HasNoFilePath() && _eb.wasSceneModified && ImGui::Button("Save to a file") ) {
			std::string sceneFilePath;
			if ( FileSystem::SaveFile(sceneFilePath, L"scene") ) {
				SceneManagement::CurrentScene()->Unload(sceneFilePath);
				Application::Stop();
			}
		} else if ( !SceneManagement::HasNoFilePath() && _eb.wasSceneModified && ImGui::Button("Save") ) {
			SceneManagement::SaveCurrentScene();
			Application::Stop();
		} 
				
		ImGui::SameLine();
		if ( ImGui::Button("Discard") ) {
			Application::Stop();
		}


		ImGui::SameLine();
		if ( ImGui::Button("Cancel") || Input::OnKeyDown(CDL_KEY_ESCAPE) ) {
			closeApplicationRequest = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	/*
	if ( createNewSceneModal ) ImGui::OpenPopup("Create New Scene");
	if ( ImGui::BeginPopupModal("Create New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize) ) {
		ImGui::InputText("Name", newSceneNameBuffer, 32);
		if ( ( ImGui::Button("Create") || Input::OnKeyDown(CDL_KEY_ENTER) ) && newSceneNameBuffer[0] != '\0') {
	
			_eb.wasSceneModified = false;
			SceneManagement::CreateEmptyScene();
			SceneManagement::CurrentScene()->SetName(newSceneNameBuffer);
			createNewSceneModal = false;

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if ( ImGui::Button("Cancel") || Input::OnKeyDown(CDL_KEY_ESCAPE) ) {
			createNewSceneModal = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	*/


	if (showSettingsWindow) ShowSettingsWindow(&showSettingsWindow);
	if (showTextEditor)		ShowTextEditor(&showTextEditor);

}


	/*
		--- ShowMasterWindow
		Display general informations about the engine and hardware
		e.g: FrameTime, GPU used, etc...
	*/
void EditorLayer::ShowSettingsWindow(bool* showSettingsWindow)
{
	if (ImGui::Begin("Settings", showSettingsWindow) )
	{
		if ( ImGui::BeginTabBar("SettingsTabBar") ) {
				
			if ( ImGui::BeginTabItem("Infos") ) {
				std::string execPath;
				FileSystem::GetExecutablePath(execPath);
				execPath = "Path to executable: " + execPath;
				ImGui::Text(execPath.c_str());

				ImGui::TextColored(ImVec4(0.6f, 0.85f, 0.65f, 1.0f), "Candle Engine v0.1.6");

				ImGui::Spacing();

				ImGui::TextColored(ImVec4(0.6f, 0.85f, 0.65f, 1.0f), _eb.ContextVersion.c_str());
				ImGui::TextColored(ImVec4(0.6f, 0.85f, 0.65f, 1.0f), _eb.HardwareVendor.c_str());
				ImGui::TextColored(ImVec4(0.6f, 0.85f, 0.65f, 1.0f), _eb.HardwareRenderer.c_str());

				ImGui::Spacing();

				ImGui::Text("Frame time: %3f ms", Candle::Time::FixedDeltaTime() * 1000.f);
				ImGui::Text("Scaled Delta time: %3f ms", Candle::Time::DeltaTime() * 1000.f);
				ImGui::SliderFloat("Time Factor", &_eb.TimeFactor, 0, 10);
				Time::TimeScale(_eb.TimeFactor);

				ImGui::EndTabItem();
			}

			if ( ImGui::BeginTabItem("Key Bindings") ) {
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::Spacing();
		ImGui::Separator();

		ImGui::Text("These things will move");
		ImGui::Spacing();

		ImGui::Checkbox("Play Game", &_eb.PlayGame);
		ImGui::SameLine();

		/*
		if (ImGui::Button("Play")) 
			Audio::Play("finalFantasy");
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
			Audio::Pause("finalFantasy");
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
			Audio::Stop("finalFantasy");
		static float pitchFF = 1.f;
		ImGui::SliderFloat("Pitch", &pitchFF, 0.1f, 5.0f);
		*/

		ImGui::Spacing();
		ImGui::Text("Blueprint update time : %.3f", ecsStats.bpManagerUpdateTime);
		ImGui::Text("Script update time : %.3f", ecsStats.scriptManagerUpdateTime);
		ImGui::Text("System update time : %.3f", ecsStats.systemManagerUpdateTime);

	}
	ImGui::End();

	ImGui::ShowDemoWindow(showSettingsWindow);
}


	/*
		--- ShowGameViewer
		Render the scene to the game viewer using the camera active in the game scen
	*/
void EditorLayer::ShowGameViewer()
{
	if ( ImGui::Begin("Game Viewer") ) {
		ImVec2 windowSize = ImGui::GetWindowSize();
		bool windowFocusAndHovered = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();
		_eb.MouseInGameViewport = windowFocusAndHovered;
		_eb.RenderGameView = true;

		double aspectRatio = Application::AspectRatio();
		ImVec2 viewerSize = ImVec2(windowSize.x * 0.99f, windowSize.x * 0.99f / (float)aspectRatio);
		ImVec2 cursorPos = ImVec2(( windowSize.x - viewerSize.x ) * 0.5f, ( windowSize.y - viewerSize.y ) * 0.5f);
		ImGui::SetCursorPos(cursorPos);
		Shared<Texture2D> currentScene = SceneManagement::FinalSceneTexture();
		if ( currentScene ) {
			ImGui::Image((void*)(intptr_t)currentScene->GetID(), viewerSize, ImVec2(0, 1), ImVec2(1, 0));
		}
	} else {
		_eb.MouseInGameViewport = false;
		_eb.RenderGameView = false;
	}
	ImGui::End();
}


/*
	--- ShowTextEditor
*/
void EditorLayer::ShowTextEditor(bool* showTextEditor)
{
	static std::string fileOpenedPath = "";

	if ( ImGui::Begin("Text Editor", showTextEditor, ImGuiWindowFlags_MenuBar) ) {

		if ( ImGui::BeginMenuBar() ) {

			if ( ImGui::BeginMenu("File") ) {
				if ( ImGui::MenuItem("Open", "") && FileSystem::OpenFile(fileOpenedPath, L"scene", L"Scene Files\0*.scene\0All Files\0*.*") ) {
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if ( fileOpenedPath.size() != 0 ) {
			std::ifstream file(fileOpenedPath);
			std::string line;
			while ( std::getline(file, line) ) {
				if ( line.find("Entity:") != std::string::npos 
					|| line.find("Asset:") != std::string::npos) ImGui::TextColored(ImVec4{0.2, 0.8, 0.5, 1.0}, line.c_str());
				else if ( line.find("Component:") != std::string::npos ) ImGui::TextColored(ImVec4{ 0.1, 0.6, 0.2, 1.0 }, line.c_str());
				else ImGui::Text(line.c_str());
			}
		}

	}
	ImGui::End();
}

		
	/*
		--- ShowAssetsWindow
		Display all the currently loaded assets and their related informations
		e.g: Textures, Shaders, etc...
	*/
void EditorLayer::ShowAssetsWindow()
{
	if ( ImGui::Begin("Assets") ) {

		static int selectedAssetType = 0;
		static const char* assetComboText = "Textures\0Shaders\0";
		ImGui::Combo("View", &selectedAssetType, assetComboText);
		ImGui::Separator();


			// Textures display
		if ( selectedAssetType == 0 ) {

			ImVec2 buttonSize = { 40, 40 };
			ImGuiStyle& style = ImGui::GetStyle();
			float windowVisble = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			int textureCount = Candle::Assets::GetAllTexture2D().size();
			int index = 0;

			for ( auto& [textureName, texture] : Candle::Assets::GetAllTexture2D() ) {
				if ( texture == nullptr ) continue;

				ImGui::ImageButton((void*)(intptr_t)texture->GetID(), buttonSize, ImVec2(0, 1), ImVec2(1, 0));

				if ( ImGui::IsItemHovered() ) {
					ImGui::BeginTooltip();

					ImGui::Text("Texture ID: %d", texture->GetID());
					ImGui::Text("Number of References: %d", texture.use_count());
					ImGui::Text("Resolution: %d x %d", texture->GetWidth(), texture->GetHeight());
					ImGui::Text("Channels: %d", texture->NbChannels());

					float textureAspectRatio = (float)texture->GetWidth() / (float)texture->GetHeight();
					ImGui::Image((void*)(intptr_t)texture->GetID(), ImVec2(128, 128 / textureAspectRatio), ImVec2(0, 1), ImVec2(1, 0));

					ImGui::EndTooltip();
				}

				if ( ImGui::BeginDragDropSource() ) {
					ImGui::SetDragDropPayload("Assign texture", &texture, sizeof(Shared<Texture2D>));
					ImGui::EndDragDropSource();
				}

				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x + buttonSize.x; // Expected position if next button was on same line
				if ( index + 1 < textureCount + 1 && next_button_x2 < windowVisble )
					ImGui::SameLine();

				index++;
			}

			if ( ImGui::Button("+", buttonSize) ) {
				std::string newTexturePath;
				FileSystem::OpenFile(newTexturePath, L"image", L"PNG Files\0*.png\0JPEG Files\0*.jpeg\0");
				Assets::LoadTexture(newTexturePath, newTexturePath);
			}
		}


			// Shader display
		if ( selectedAssetType == 1 ) {
			for ( auto& shader : Candle::Assets::GetAllShaders() ) {
				if ( shader.second == nullptr ) continue;
				std::string headerName = "Shader " + std::to_string(shader.second->GetID()) + " : " + shader.second->GetName();
				if ( ImGui::TreeNode(headerName.c_str()) ) {

					std::string buttonname = "Reload: " + shader.second->GetName();
					ImGui::SameLine();
					if ( ImGui::Button(buttonname.c_str()) ) {
						Assets::ReloadShader(shader.first);
					}

					if ( ImGui::TreeNode("Uniforms") ) {
						ImGui::Columns(5, "uniform_columns"); // 5-ways, with border
						ImGui::Separator();
						ImGui::Text("Name"); ImGui::NextColumn();
						ImGui::Text("Type"); ImGui::NextColumn();
						ImGui::Text("Length"); ImGui::NextColumn();
						ImGui::Text("Rate"); ImGui::NextColumn();
						ImGui::Text("Location"); ImGui::NextColumn();
						ImGui::Separator();

						for ( auto& uniform : shader.second->GetUniforms() ) {
							ImGui::Text(uniform.first.c_str()); ImGui::NextColumn();
							ImGui::Text(ShaderUniformStringFromType(uniform.second.type).c_str()); ImGui::NextColumn();
							ImGui::Text(std::to_string(uniform.second.length).c_str()); ImGui::NextColumn();
							ImGui::Text(ShaderUniformRefreshStringFromRate(uniform.second.rate).c_str()); ImGui::NextColumn();
							ImGui::Text(std::to_string(uniform.second.location).c_str()); ImGui::NextColumn();
						}

						ImGui::Columns(1);
						ImGui::Separator();
						ImGui::TreePop();
					}

					ImGui::TreePop();

					char* shaderCode = (char*)( shader.second->GetCode().c_str() );
					ImGui::Text(shaderCode);
				}

			}
		}
	}

	ImGui::End();
}


	/*
		--- ShowComponentsOf (Called from ::ShowSceneHierarchy)
		Display informations about the scripts and components attached to the selected Blueprint
			
		@input Shared<Blueprint> : selected blueprint
	*/
void EditorLayer::ShowComponentsOf(Entity& entity)
{
	if (ImGui::Begin("Entity Components")) {

		ImGui::Text("Active: ");
		ImGui::SameLine();
		bool isAlive = true;
		ImGui::Checkbox("alive", &isAlive);

		for ( auto comp : entity.GetAllComponents() ) {

			if ( typeid( *comp ) == typeid(HierarchyComponent) ) continue;

			std::string menuTitle = ( (Component*)comp )->GetComponentName();
			if ( ImGui::CollapsingHeader(menuTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen) ) {
				( (Component*)comp )->OnEditor();

				if ( typeid( *comp ) == typeid( EntityTagComponent ) ) continue;
				std::string alertBoxTitle = "Remove " + ( (Component*)comp )->GetComponentName();
				if ( ImGui::ArrowButton(alertBoxTitle.c_str(), ImGuiDir_Down) )
					ImGui::OpenPopup(alertBoxTitle.c_str());

				if ( ImGui::BeginPopupModal(alertBoxTitle.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove) ) {
					if ( ImGui::Button("Remove") ) {
						entity.RemoveComponent(comp);
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if ( ImGui::Button("Cancel") ) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndMenu();
				}
			}

			ImGui::Separator();
		}
		/*
		for (auto& script : bp->Scripts()) {

			std::string menuTitle = script->GetName();
			if (ImGui::TreeNodeEx(menuTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
				script->OnEditor();
				ImGui::TreePop();
				ImGui::Separator();
			}
		}
		*/

		if (ImGui::Button("Add Component")) {
			ImGui::OpenPopup("EditorAddComponentPopup");
		}

		if ( ImGui::Button("Add Script") ) {
			ImGui::OpenPopup("EditorAddScriptPopup");
		}

		if (ImGui::BeginPopup("EditorAddComponentPopup")) {

				// Essentials
			if ( ImGui::MenuItem("Transform", "", false, !entity.HasComponent<Transform>()) ) { 
				entity.AddComponent<Transform>(); 
				_eb.wasSceneModified = true;
			}

			if ( ImGui::MenuItem("CameraHandler", "", false, !entity.HasComponent<CameraHandler>()) ) {
				entity.AddComponent<CameraHandler>();
				_eb.wasSceneModified = true;
			}

				// Renderer 2D
			ImGui::Separator();
			if ( ImGui::MenuItem("SpriteRenderer", "", false, !entity.HasComponent<SpriteRenderer>()) ) {
				entity.AddComponent<SpriteRenderer>();
				_eb.wasSceneModified = true;
			}

				// Physics 2D
			ImGui::Separator();
			if ( ImGui::MenuItem("DebugPointCollider", "", false, !entity.HasComponent<DebugPointCollider>()) ) {
				entity.AddComponent<DebugPointCollider>();
				_eb.wasSceneModified = true;
			}

			if ( ImGui::MenuItem("CircleCollider", "", false, !entity.HasComponent<CircleCollider>()) ) {
				entity.AddComponent<CircleCollider>();
				_eb.wasSceneModified = true;
			}

			if ( ImGui::MenuItem("BoxCollider", "", false, !entity.HasComponent<BoxCollider>()) ) {
				entity.AddComponent<BoxCollider>();
				_eb.wasSceneModified = true;
			}

			if ( ImGui::MenuItem("AABB Collider", "", false, !entity.HasComponent<AABB>()) ) {
				entity.AddComponent<AABB>();
				_eb.wasSceneModified = true;
			}

				// Animations
			ImGui::Separator();
			if ( ImGui::MenuItem("AnimationController", "", false, !entity.HasComponent<AnimationController>()) ) {
				entity.AddComponent<AnimationController>();
				_eb.wasSceneModified = true;
			}

			ImGui::EndPopup();
		}
			
		if ( ImGui::BeginPopup("EditorAddScriptPopup") ) {
			for ( auto& [name, script] : EditorScriptRegistery::Get().Scripts() ) {
				if ( ImGui::MenuItem(name.c_str(), "", false) ) {
					Script* _ptr = (*script)();
					entity.AddScript(_ptr);
					_eb.wasSceneModified = true;
				}
			}
			ImGui::EndPopup();
		}
	}

	ImGui::End();
}


	/*
		--- ShowPostProcessingPipeline
		Display informations about the post processing pipeline
	*/
void EditorLayer::ShowPostProcessingPipeline()
{
	ImGui::Begin("PostProcessing Pipeline");
	{

		std::unordered_map<uint32_t, Shared<PostProcessingNode>> pipeline = PostProcessing::GetPipeline();

		float sizeX = 150;
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 entry = { pos.x, pos.y + 25 };
		ImVec2 out = { pos.x + sizeX, pos.y + 25 };
		int i = 0;

		uint32_t finalNodeId = PostProcessing::GetFinalNodeID();
			
		if (Gui::GuiNode(ImGui::GetMousePos(), entry, out, 30, "Entry", finalNodeId == -1 ? IM_COL32(100, 200, 100, 255) : IM_COL32(50, 70, 120, 255)) == GuiElementState::R_CLICK) {
			PostProcessing::SetFinalNode(-1);
		}

		for (auto& it : pipeline) {

			ImVec2 offset = { sizeX + 5, 0.f};
				
			ImVec2 nodeEntPos = { out.x + 20.f, out.y };
			ImVec2 nodeOutPos = { out.x + sizeX, out.y };

			ImGui::GetWindowDrawList()->AddLine(out, nodeEntPos, ImGui::GetColorU32(ImGuiCol_Button), 2.0f);

			ImU32 color;
				
			uint32_t nodeID = it.second->GetID();
			if (nodeID == finalNodeId) color = IM_COL32(100, 200, 100, 255);
			else if (it.second->IsUsed() && ( nodeID < finalNodeId && finalNodeId != -1) ) color = IM_COL32(50, 70, 120, 255);
			else color = IM_COL32(30, 30, 30, 255);

			GuiElementState state = Gui::GuiNode(ImGui::GetMousePos(), nodeEntPos, nodeOutPos, 40, it.second->GetName(), color);
			if (state == GuiElementState::L_CLICK) {
				if ( nodeID != finalNodeId ) {
					it.second->SetState(!it.second->IsUsed());
				}
			} else if ( state == GuiElementState::R_CLICK ) {
				PostProcessing::SetFinalNode(it.second->GetID());
			}
			i++;

			out = nodeOutPos;
		}

	}
	ImGui::End();
}


	/*
		--- ShowRendererWindow
		Display informations about the renderer
	*/
void EditorLayer::ShowRendererWindow(bool* showRendererWindow)
{
	if (ImGui::Begin("Renderer", showRendererWindow) ) {

		ShowInformationToolTip("Right click to move (orthographic) / rotate (perspective) the camera, use the arrow keys to move in 3D!");

		ImGui::Spacing();

		ImGui::Text("Scene Rendering Time: %.5f", Renderer2D::GetStats()->renderingTime);
		ImGui::Text("Debug Rendering Time: %.5f", Renderer2D::GetStats()->debugRenderingTime);
		ImGui::Text("DrawCalls: %d", Renderer2D::GetStats()->DrawCalls);
		ImGui::Text("Opaque: %d", Renderer2D::GetStats()->OpaqueQuads);
		ImGui::Text("Transparent: %d", Renderer2D::GetStats()->TransparentQuads);

		/*
		if (ImGui::CollapsingHeader("PostProcessing")) {
			ImGui::Checkbox("Use Postprocessing", &_eb.PostProcessing);
		}
		*/

	}
	ImGui::End();
}


void EditorLayer::OnAttach()
{
	_imguiLayer = std::make_unique<ImGuiLayer>();
	_imguiLayer->OnAttach();

	_eb.ContextVersion = std::string(reinterpret_cast<const char*>(Application::Get().GetWindow().GetContext().GetContextVersion()));
	_eb.HardwareVendor = std::string(reinterpret_cast<const char*>(Application::Get().GetWindow().GetContext().GetHardwareVendor()));
	_eb.HardwareRenderer = std::string(reinterpret_cast<const char*>(Application::Get().GetWindow().GetContext().GetRenderer()));

	_cameraController = EditorCamera(CDL_APP_WIDTH, CDL_APP_HEIGHT);

	_editorFrameBuffer = FrameBuffer::Create({ Texture, DepthBuffer }, CDL_APP_WIDTH, CDL_APP_HEIGHT);
	_editorTexture = Texture2D::Create(_editorFrameBuffer->Get(Texture), CDL_APP_WIDTH, CDL_APP_HEIGHT);
}


void EditorLayer::OnUpdate()
{
	_eb.ReadInputs = true;
	_cameraController.OnUpdate();
	_eb.ReadInputs = false;
		
	// Dynamicly reload the assets
}


void EditorLayer::OnEvent(Event& event)
{
	if ( _eb.MouseInEditorViewport ) {
		_eb.ReadInputs = true;
		_cameraController.OnEvent(event); 
		_eb.ReadInputs = false;
	}
}


void EditorLayer::OnDetach()
{
	_imguiLayer->OnDetach();
}


void EditorLayer::ShowInformationToolTip(const std::string& description)
{
	ImGui::TextDisabled("(?)");
	if ( ImGui::IsItemHovered() ) {
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(description.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}


/*
					static char pBuf[256] = { 0 };
					static size_t len = 256;
					static std::string executablePath;

					if ( pBuf[0] == 0 ) {
#ifdef _WIN64
						int bytes = GetModuleFileName(NULL, pBuf, len);
#else
						int bytes = MIN(readlink("/proc/self/exe", pBuf, len), len - 1);
						if ( bytes >= 0 )
							pBuf[bytes] = '\0';
						CTRACE(pBuf);
#endif
						executablePath = std::string(pBuf);
						size_t lastBackSlash = executablePath.find_last_of('\\');
						executablePath = executablePath.substr(0, lastBackSlash);
					}

					try {
						for ( const auto& entry : std::filesystem::recursive_directory_iterator(executablePath) ) {

								// Erase executablePath from currentPath
							std::string currentPath = entry.path().string();
							size_t pos = currentPath.find(executablePath);

							if ( pos != std::string::npos ) {
								currentPath.erase(pos, executablePath.length() + 1);
							}

							if ( !entry.is_directory() ) {
								if ( ImGui::MenuItem(currentPath.c_str()) ) {
									Assets::LoadTexture(entry.path().string(), currentPath);
								}
							}
							

							if ( entry.is_directory() ) {
								CINFO("Directory {0}", entry.path().stem().string().c_str());
								if ( endMenu ) {
									CINFO("End Menu");
									ImGui::EndMenu();
									endMenu = false;
									CINFO("End menu = false");
								}
								ImGui::BeginMenu(entry.path().stem().string().c_str());
								endMenu = true;
								CINFO("End menu = true");
							} else {
								ImGui::MenuItem(entry.path().string().c_str());
							}
							
						}
					} catch ( std::exception e ) {
						CERROR(e.what());
					}


*/