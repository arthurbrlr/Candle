#include "cdlpch.h"
#include "Editor.h"

#include "Candle/Core/Input.h"
#include "Candle/Scenes/EmptyScene.h"
#include "Candle/Scenes/SceneManagement.h"
#include "Candle/Renderer/CameraManagement.h"

namespace Candle {

	void Editor::Display()
	{
		double t1 = Time::Milliseconds();
			// Do not modify this line
		_eb.ReadInputs = true;
		_imguiLayer->Begin();
		BuildDockSpace();

		ShowMainMenu();
		ShowMasterWindow();
		ShowRendererWindow();

		_eb.ReadInputs = false;
		ShowGameViewer();
		_eb.ReadInputs = true;
		ShowEditorViewer();

		ShowAssetsWindow();
		ShowSceneHierarchy();
		ShowPostProcessingPipeline();
			   		 	  	  	 
		ECS::OnEditor();
		SceneManagement::CurrentScene()->OnEditor();

		static bool demo = true;
		//ImGui::ShowDemoWindow(&demo);


			// Do not modify this line
		double t2 = Time::Milliseconds();
		ImGui::Begin("EditorTime");
		ImGui::Text("%.5f", t2 - t1);
		ImGui::End();
		_imguiLayer->End();
		_eb.ReadInputs = false;
	}

		/*
			--- BuildDockSpace
			Setup of the editor dockspace
		*/
	void Editor::BuildDockSpace()
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
	void Editor::ShowMainMenu()
	{
		static char newSceneNameBuffer[32];
		static bool createNewSceneModal = false;

		if ( ImGui::BeginMainMenuBar() ) {
			if ( ImGui::BeginMenu("Scene") ) {
				if ( ImGui::BeginMenu("Open Scene File", false) ) {
					bool endMenu = false;
					/*
					for ( const auto& entry : std::filesystem::recursive_directory_iterator("D:/_code/Candle/CandleBird/res") ) {

						ImGui::MenuItem(entry.path().string().c_str());


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
					if ( endMenu ) {
						CINFO("End Menu");
						ImGui::EndMenu();
					}
					*/
					ImGui::EndMenu();
				}

				if ( ImGui::MenuItem("New Scene") ) {
					//ImGui::OpenPopup("Create New Scene");
					createNewSceneModal = true;
					memset(newSceneNameBuffer, ' ', 32);
				}

				if ( ImGui::BeginMenu("Open Scene") ) {
					for ( auto scene : SceneManagement::AllScenes() ) {
						if ( ImGui::MenuItem(scene.second->GetName().c_str(), "", false, scene.first != SceneManagement::CurrentScene()->GetID()) ) {
							SceneManagement::LoadScene(scene.first);
						}
					}
					ImGui::EndMenu();
				}

				/*
				if ( ImGui::BeginMenu("Open Recent") ) {
					ImGui::MenuItem("fish_hat.c");
					ImGui::MenuItem("fish_hat.inl");
					ImGui::MenuItem("fish_hat.h");
					if ( ImGui::BeginMenu("More..") ) {
						ImGui::MenuItem("Hello");
						ImGui::MenuItem("Sailor");
						if ( ImGui::BeginMenu("Recurse..") ) {
							//ShowExampleMenuFile();
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if ( ImGui::MenuItem("Save", "Ctrl+S") ) {}
				if ( ImGui::MenuItem("Save As..") ) {}
				*/
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

		if ( createNewSceneModal ) ImGui::OpenPopup("Create New Scene");
		if ( ImGui::BeginPopupModal("Create New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize) ) {
			ImGui::InputText("Name", newSceneNameBuffer, 32);
			if ( ( ImGui::Button("Create") || Input::IsKeyPressed(CDL_KEY_SPACE) ) && newSceneNameBuffer ) {
				Scene* newScene = new EmptyScene();
				newScene->SetName(newSceneNameBuffer);
				uint32_t newSceneID = newScene->GetID();
				SceneManagement::AddScene(newScene);
				SceneManagement::LoadScene(newSceneID);
				createNewSceneModal = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}


		/*
			--- ShowMasterWindow
			Display general informations about the engine and hardware
			e.g: FrameTime, GPU used, etc...
		*/
	void Editor::ShowMasterWindow()
	{
		ImGui::Begin("Master");
		{
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

			ImGui::Spacing();

			ImGui::Checkbox("Play Game", &_eb.PlayGame);
			ImGui::SameLine();
			if ( ImGui::Button("Reload Scene") ) {
				SceneManagement::LoadScene(SceneManagement::CurrentScene()->GetID());
			}

			ImGui::Spacing();
			bool previous = _eb.FullScreen;
			ImGui::Checkbox("Full Screen", &_eb.FullScreen);
			bool current = _eb.FullScreen;

			if (previous ^ current) Application::SetFullScreen(_eb.FullScreen);


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
	}


		/*
			--- ShowEditorViewer
			Render the scene to the editor viewer using the Editor camera
		*/
	void Editor::ShowEditorViewer()
	{
		static bool show = true;
		if (ImGui::Begin("Scene Viewer", &show, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImVec2 windowSize = ImGui::GetWindowSize();
			_eb.MouseInEditorViewport = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();

				// Editor view rendering
			_editorFrameBuffer->Bind();
			CameraManagement::UseView(Controller().GetPVMatrix());
			SceneManagement::OnEditorRender();
			if ( !Controller().UseOrthographic() ) {
				Renderer2D::BeginScene();
				Renderer2D::DrawQuad(Controller().GetTargetTransform().GetPosition(), { .1, .1 });
				Renderer2D::EndScene();
			}
			CameraManagement::Reset();
			_editorFrameBuffer->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);

			double aspectRatio = Application::AspectRatio();
			ImVec2 viewerSize = ImVec2(windowSize.x * 0.99f, windowSize.x * 0.99f / (float)aspectRatio);
			ImVec2 cursorPos = ImVec2((windowSize.x - viewerSize.x) * 0.5f, (windowSize.y - viewerSize.y) * 0.5f);
			ImGui::SetCursorPos(cursorPos);
			ImGui::Image((void*)(intptr_t)_editorTexture->GetID(), viewerSize, ImVec2(0, 1), ImVec2(1, 0));
		} else {
			_eb.MouseInEditorViewport = false;
		}
		ImGui::End();
	}


		/*
			--- ShowGameViewer
			Render the scene to the game viewer using the camera active in the game scen
		*/
	void Editor::ShowGameViewer()
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
			ImGui::Image((void*)(intptr_t)SceneManagement::FinalSceneTexture()->GetID(), viewerSize, ImVec2(0, 1), ImVec2(1, 0));

		} else {
			_eb.MouseInGameViewport = false;
			_eb.RenderGameView = false;
		}
		ImGui::End();
	}

		
		/*
			--- ShowAssetsWindow
			Display all the currently loaded assets and their related informations
			e.g: Textures, Shaders, etc...
		*/
	void Editor::ShowAssetsWindow()
	{
		ImGui::Begin("Assets");
		{
			if (ImGui::CollapsingHeader("Shaders Loaded")) {
				for (auto& shader : Candle::Assets::GetAllShaders()) {
					if (shader.second == nullptr) continue;
					std::string headerName = "Shader " + std::to_string(shader.second->GetID()) + " : " + shader.second->GetName();
					if (ImGui::TreeNode(headerName.c_str())) {

						std::string buttonname = "Reload: " + shader.second->GetName();
						ImGui::SameLine();					
						if (ImGui::Button(buttonname.c_str())) {
							Assets::ReloadShader(shader.first);
						}

						if (ImGui::TreeNode("Uniforms")) {
							ImGui::Columns(5, "uniform_columns"); // 5-ways, with border
							ImGui::Separator();
							ImGui::Text("Name"); ImGui::NextColumn();
							ImGui::Text("Type"); ImGui::NextColumn();
							ImGui::Text("Length"); ImGui::NextColumn();
							ImGui::Text("Rate"); ImGui::NextColumn();
							ImGui::Text("Location"); ImGui::NextColumn();
							ImGui::Separator();

							for (  auto& uniform : shader.second->GetUniforms()) {
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

						char* shaderCode = (char*)shader.second->GetCode().c_str();
						ImGui::InputTextMultiline("Shader Code", shaderCode, IM_ARRAYSIZE(shaderCode));
					}

				}
			}

			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Textures Loaded")) {
				for (auto& texture : Candle::Assets::GetAllTexture2D()) {
					if (texture.second == nullptr) continue;
					if (ImGui::TreeNode(texture.first.c_str())) {
						ImGui::Text("Texture ID: %d", texture.second->GetID());
						ImGui::Text("Number of References: %d", texture.second.use_count());
						ImGui::Text("Resolution: %d x %d", texture.second->GetWidth(), texture.second->GetHeight());
						ImGui::Text("Channels: %d", texture.second->NbChannels());
						ImGui::Image((void*)(intptr_t)texture.second->GetID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0));
						ImGui::TreePop();
					}
				}
			}

		}
		ImGui::End();
	}


		/*
			--- ShowSceneHierarchy
			Display the scene tree given by the ECS
			Only Blueprints and their childrens are displayed
		*/
	void Editor::ShowSceneHierarchy()
	{
		ImGui::Begin("Scene Hierarchy");
		static int selected = -1;
		static size_t bpSelected;
		bpSelected = -1;
		int index = 0;

		static bool openPopup = false;
		bool windowFocusAndHovered = ImGui::IsWindowHovered();
		/*
		if (ImGui::Button("Save Changes")) {
			CINFO("Saving blueprint data");
		}
		*/

		if (ImGui::Button("Clear Selection")) {
			selected = -1;
			bpSelected = -1;
		}

	/*
		for ( auto& scene : SceneManagement::AllScenes() ) {
			if ( ImGui::TreeNode(std::to_string(scene.second->GetID()).c_str()) ) {



				ImGui::TreePop();
			}
		}
	*/
		std::function<void(Blueprint*)> BuildBlueprintTree;
		BuildBlueprintTree = [&index, &BuildBlueprintTree](Blueprint* currentBlueprint) -> void {
			if ( currentBlueprint == nullptr ) return;
			index++;
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
			if ( !currentBlueprint->GetViewInEditor() ) return;

			std::string blueprintEditor = currentBlueprint->GetName();

			if ( currentBlueprint->HasChildren() ) {

				if ( selected == index ) {
					node_flags |= ImGuiTreeNodeFlags_Selected;
					bpSelected = currentBlueprint->GetID();
				}
				bool node_open = ImGui::TreeNodeEx(blueprintEditor.c_str(), node_flags);
				if ( ImGui::IsItemClicked() ) selected = index;
				if ( node_open ) {
					for ( auto& child : currentBlueprint->GetChilds() ) {
						BuildBlueprintTree(child.second);
					}
					ImGui::TreePop();
				}

			} else {

				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				if ( selected == index ) {
					node_flags |= ImGuiTreeNodeFlags_Selected;
					bpSelected = currentBlueprint->GetID();
				}
				ImGui::TreeNodeEx(blueprintEditor.c_str(), node_flags);
				if ( ImGui::IsItemClicked()) selected = index;

			}

		};


		for (auto& bp : BlueprintManager::All()) {
			
			if ( bp.second == nullptr || !bp.second->GetViewInEditor() || bp.second->HasParent() ) continue;
			BuildBlueprintTree(&(*bp.second));

		}
		
		Blueprint* bpShowed = nullptr;
		if ( bpSelected != -1 ) bpShowed = &(*BlueprintManager::All()[bpSelected]);

		if (Input::IsKeyPressed(CDL_KEY_LEFT_CONTROL) && Input::OnKeyDown(CDL_KEY_D) ) {
			CTRACE("Control D -> Copy Entity");
		}

		ShowComponentsOf(bpShowed);


		if ( (Input::IsMouseButtonPressed(1) || openPopup) && windowFocusAndHovered ) {

			openPopup = true;

			ImGui::OpenPopup("sceneHierarchyPopup");
		}

		if (ImGui::BeginPopup("sceneHierarchyPopup"))
		{
			if (ImGui::MenuItem("New Blueprint")) {
				ECS::New();
			}
			if (selected != -1 && bpSelected != -1 ) if (ImGui::MenuItem("Remove Blueprint")) {
				ECS::Remove(bpSelected);
			}
			ImGui::EndPopup();
		}
		
		if ( (Input::IsMouseButtonPressed(0) || Input::IsMouseButtonPressed(2)) && openPopup) {
			openPopup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::End();

	}


		/*
			--- ShowComponentsOf (Called from ::ShowSceneHierarchy)
			Display informations about the scripts and components attached to the selected Blueprint
			
			@input Shared<Blueprint> : selected blueprint
		*/
	void Editor::ShowComponentsOf(Blueprint* bp)
	{
		static char * bpName;
		ImGui::Begin("Blueprint Properties");

		if (bp != nullptr) {

			bpName = (char*)bp->GetName().c_str();
			ImGui::InputText("Name", bpName, IM_ARRAYSIZE(bpName));
			ImGui::SameLine();
			bool isAlive = bp->IsAlive();
			ImGui::Checkbox("alive", &isAlive);
			if ( !isAlive ) bp->Destroy();
			std::string bpid = "Entity ID : " + std::to_string(bp->GetID());
			ImGui::Text(bpid.c_str());

			for (ComponentID id = 0; id < MaxComponents; id++) {

				if (!bp->HasComponentOfID(id)) continue;
				auto component = bp->GetComponentOfID(id);

				std::string menuTitle = component->GetName();
				if (ImGui::TreeNodeEx(menuTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Checkbox("Active", &component->IsActive());
					component->OnEditor();
					ImGui::TreePop();
				}

				std::string alertBoxTitle = "Remove Component " + component->GetName();
				if ( ImGui::ArrowButton(alertBoxTitle.c_str(), ImGuiDir_Down) )
					ImGui::OpenPopup(alertBoxTitle.c_str());

				if ( ImGui::BeginPopupModal(alertBoxTitle.c_str(), NULL, ImGuiWindowFlags_NoResize) ) {
					if ( ImGui::Button("Remove") ) {
						CTRACE("Delete component {0}", component->GetName());
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if ( ImGui::Button("Cancel") ) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndMenu();
				}

				ImGui::Separator();

			}

			for (auto& script : bp->Scripts()) {

				std::string menuTitle = script->GetName();
				if (ImGui::TreeNodeEx(menuTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					script->OnEditor();
					ImGui::TreePop();
					ImGui::Separator();
				}
			}

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("EditorAddComponentPopup");
			}

			if ( ImGui::Button("Add Script") ) {
				ImGui::OpenPopup("EditorAddScriptPopup");
			}

			if (ImGui::BeginPopup("EditorAddComponentPopup")) {
				if (ImGui::MenuItem("Transform", "", false, !bp->HasComponent<Transform>())) bp->AddComponent<Transform>();
				if (ImGui::MenuItem("SpriteRenderer", "", false, !bp->HasComponent<SpriteRenderer>())) bp->AddComponent<SpriteRenderer>();
				if (ImGui::MenuItem("AnimationController", "", false, !bp->HasComponent<AnimationController>())) bp->AddComponent<AnimationController>();
				if (ImGui::MenuItem("CameraHandler", "", false, !bp->HasComponent<CameraHandler>())) bp->AddComponent<CameraHandler>();
				ImGui::EndPopup();
			}

			if ( ImGui::BeginPopup("EditorAddScriptPopup") ) {

				auto& scripts = ScriptManager::Get().GetScripts();
				auto& scriptsNames = ScriptManager::Get().GetScriptsNames();

				for ( int i = 0; i < scripts.size(); i++) {
					if ( ImGui::MenuItem(scriptsNames[i].c_str(), "") ) {
						Script* _ptr = (*scripts[i])();
						bp->AddScript(_ptr);
					}
				}
				ImGui::EndPopup();
			}

		} else {
			std::string space = " ";
			bpName = (char*)space.c_str();
		}

		ImGui::End();
	}



		/*
			--- ShowPostProcessingPipeline
			Display informations about the post processing pipeline
		*/
	void Editor::ShowPostProcessingPipeline()
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
	void Editor::ShowRendererWindow()
	{
		ImGui::Begin("Renderer");
		{
			static int currentEditorCamera = (int)_cameraController.UseOrthographic();
			const char* editorCameras[2] = { "Perspective", "Orthographic" };

			ImGui::Combo("Editor Camera", &currentEditorCamera, editorCameras, IM_ARRAYSIZE(editorCameras));
			_cameraController.UseOrthographic((bool)currentEditorCamera);

			if ( ImGui::Button("Reset Camera View") ) {
				_cameraController.ResetView();
			}

			ImGui::SameLine();
			ShowInformationToolTip("Right click to move (orthographic) / rotate (perspective) the camera, use the arrow keys to move in 3D!");


			ImGui::Spacing();
			ImGui::Checkbox("Draw Wireframe", &_eb.DrawLines);
			ImGui::Checkbox("Draw Textures", &_eb.DrawTextures);

			static bool depthTestEnable = true;
			ImGui::Checkbox("Depth Test", &depthTestEnable);
			RenderCommands::SetDepthTesting(depthTestEnable);

			static bool blendingEnable = false;
			ImGui::Checkbox("Blending", &blendingEnable);
			RenderCommands::SetAlphaBlending(blendingEnable);

			ImGui::Spacing();

			ImGui::Text("ECS Sprite Render Time: %.5f", Renderer2D::GetStats()->ecsSpriteRenderTime);
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


	Unique<ImGuiLayer> Editor::_imguiLayer = nullptr;
	EditorVariables Editor::_eb;
	Shared<FrameBuffer> Editor::_editorFrameBuffer = nullptr;
	Shared<Texture2D> Editor::_editorTexture = nullptr;
	EditorCamera Editor::_cameraController;


	void Editor::Init()
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


	void Editor::OnUpdate()
	{
		_eb.ReadInputs = true;
		_cameraController.OnUpdate();
		_eb.ReadInputs = false;
		
		// Dynamicly reload the assets
	}


	void Editor::OnEvent(Event& event)
	{
		if ( _eb.MouseInEditorViewport ) {
			_eb.ReadInputs = true;
			_cameraController.OnEvent(event); 
			_eb.ReadInputs = false;
		}
	}


	void Editor::Clear()
	{
		_imguiLayer->OnDetach();
	}


	void Editor::ShowInformationToolTip(const std::string& description)
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


}