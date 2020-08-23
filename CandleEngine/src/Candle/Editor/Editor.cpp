#include "cdlpch.h"
#include "Editor.h"

#include "Candle/Core/Input.h"
#include "Candle/ECS/Script.h"
#include "Candle/Scenes/EmptyScene.h"
#include "Candle/Scenes/SceneManagement.h"
#include "Candle/Renderer/CameraManagement.h"

namespace Candle {

	void Editor::Display()
	{
		//double t1 = Time::Milliseconds();
			// Do not modify this line
		_eb.ReadInputs = true;
		_imguiLayer->Begin();
		BuildDockSpace();

		ShowMainMenu();
		ShowRendererWindow();

		_eb.ReadInputs = false;
		ShowGameViewer();
		_eb.ReadInputs = true;
		ShowEditorViewer();

		ShowAssetsWindow();
		ShowSceneHierarchy();
		ShowPostProcessingPipeline();
			   		 	  	  	 
		ECS::OnEditor();
		if ( SceneManagement::CurrentScene() ) SceneManagement::CurrentScene()->OnEditor();

		static bool demo = true;
		ImGui::ShowDemoWindow(&demo);


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
		static char newSceneNameBuffer[32]{ ' ' };
		static bool createNewSceneModal = false;
		static bool closeApplicationRequest = false;
		static bool showSettingsWindow = false;

		if ( ImGui::BeginMainMenuBar() ) {
			if ( ImGui::BeginMenu("File") ) {
				if ( ImGui::MenuItem("Settings") ) {
					showSettingsWindow = true;
				}
				ImGui::Separator();

				if ( ImGui::MenuItem("Close") ) {
					closeApplicationRequest = true;
				}
				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu("Scene") ) {
				if ( ImGui::BeginMenu("Open Scene File") ) {
					bool endMenu = false;


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
							/*
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
							*/
						}
						/*
						if ( endMenu ) {
							CINFO("End Menu");
							ImGui::EndMenu();
						}
						*/
					} catch ( std::exception e ) {
						CERROR(e.what());
					}

					ImGui::EndMenu();
				}

				if ( ImGui::MenuItem("New Scene") ) {
					//ImGui::OpenPopup("Create New Scene");
					createNewSceneModal = true;
					memset(newSceneNameBuffer, ' ', 32);
				}

				if ( ImGui::BeginMenu("Open Scene") ) {
					for ( auto scene : SceneManagement::AllScenes() ) {
						if ( !scene.second ) continue;
						bool enabled = true;
						if ( SceneManagement::CurrentScene() ) enabled = SceneManagement::CurrentScene()->GetID() != scene.first;
						if ( ImGui::MenuItem(scene.second->GetName().c_str(), "", false, enabled) ) {
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

			/* Close application popup */
		if ( closeApplicationRequest ) ImGui::OpenPopup("Close Candle Editor");
		if ( ImGui::BeginPopupModal("Close Candle Editor", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize) ) {
			ImGui::Text("Close Application ?", newSceneNameBuffer, 32);
			if ( ImGui::Button("Confirm") || Input::OnKeyDown(CDL_KEY_ENTER) ) {
				Application::Stop();
			}
			ImGui::SameLine();
			if ( ImGui::Button("Cancel") || Input::OnKeyDown(CDL_KEY_ESCAPE) ) {
				closeApplicationRequest = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		if ( createNewSceneModal ) ImGui::OpenPopup("Create New Scene");
		if ( ImGui::BeginPopupModal("Create New Scene", NULL, ImGuiWindowFlags_AlwaysAutoResize) ) {
			ImGui::InputText("Name", newSceneNameBuffer, 32);
			if ( ( ImGui::Button("Create") || Input::OnKeyDown(CDL_KEY_ENTER) ) && newSceneNameBuffer[0] != ' ') {
				std::string newSceneName(newSceneNameBuffer);
				bool valid = true;

				for ( auto& scene : SceneManagement::AllScenes() ) {
					if ( !scene.second ) continue;
					if ( scene.second->GetName() == newSceneName ) {
						CDL_INFO("Invalid scene name!");
						valid = false;
					}
				}

				if ( valid ) {
					Scene* newScene = new EmptyScene();
					newScene->SetName(newSceneNameBuffer);
					uint32_t newSceneID = newScene->GetID();
					SceneManagement::AddScene(newScene);
					SceneManagement::LoadScene(newSceneID);
				}
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


		if (showSettingsWindow) ShowSettingsWindow(&showSettingsWindow);

	}


		/*
			--- ShowMasterWindow
			Display general informations about the engine and hardware
			e.g: FrameTime, GPU used, etc...
		*/
	void Editor::ShowSettingsWindow(bool* show)
	{
		if (ImGui::Begin("Settings", show) )
		{
			if ( ImGui::BeginTabBar("SettingsTabBar") ) {
				if ( ImGui::BeginTabItem("Infos") ) {
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

				ImGui::EndTabBar();
			}

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


				// Editor view raycasting



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
			--- ShowAssetsWindow
			Display all the currently loaded assets and their related informations
			e.g: Textures, Shaders, etc...
		*/
	void Editor::ShowAssetsWindow()
	{
		ImGui::Begin("Assets");

		//if ( ImGui::Button("Reload all assets") ) {
		//	Assets::Reload();
		//}

		{
			if (ImGui::CollapsingHeader("Shaders")) {
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

						char* shaderCode = (char*)(shader.second->GetCode().c_str());
						ImGui::Text(shaderCode);
					}

				}
			}

			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Textures")) {
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

		static Burst::Entity hoveredEntity = -1;
		//hoveredEntity = -1;

		static Burst::Entity selectedEntity;
		selectedEntity = -1;

		int index = 0;

		static bool openHierarchyPopup = false;
		static bool openEntityPopup = false;

		bool windowFocusAndHovered = ImGui::IsWindowHovered();
		
		if (ImGui::Button("Clear Selection")) {
			selected = -1;
			selectedEntity = -1;
		}

		std::function<void(Entity&)> BuildEntityHierarchy;
		BuildEntityHierarchy = [&index, &BuildEntityHierarchy](Entity& currentEntity) -> void {
			
			index++;
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

			std::string entityName = currentEntity.GetComponent<EntityTagComponent>().GetName();
			bool hasHierarchy = currentEntity.HasComponent<HierarchyComponent>();
			if ( hasHierarchy && currentEntity.GetComponent<HierarchyComponent>().HasChilds() ) {

				if ( selected == index ) {
					node_flags |= ImGuiTreeNodeFlags_Selected;
					selectedEntity = currentEntity.GetNativeEntity();
				}
				
				bool node_open = ImGui::TreeNodeEx(entityName.c_str(), node_flags);
				if ( ImGui::IsItemHovered() ) hoveredEntity = currentEntity.GetNativeEntity();
				if ( ImGui::IsItemClicked() ) selected = index;

				if ( node_open ) {
					for ( auto& child : currentEntity.GetComponent<HierarchyComponent>().GetChilds() ) {
						Entity childEntity = Entity{ SceneManagement::CurrentScene().get(), child };
						BuildEntityHierarchy(childEntity);
					}
					ImGui::TreePop();
				}

			} else {

				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				if ( selected == index ) {
					node_flags |= ImGuiTreeNodeFlags_Selected;
					selectedEntity = currentEntity.GetNativeEntity();
				}
				ImGui::TreeNodeEx(entityName.c_str(), node_flags);
				if ( ImGui::IsItemClicked() ) selected = index;
				if ( ImGui::IsItemHovered() ) hoveredEntity = currentEntity.GetNativeEntity();

			}

		};


		for (auto& entity : ECS::ViewAllEntities() ) {
			Entity currentEntity = Entity{ SceneManagement::CurrentScene().get(), entity.second };
			bool hasHierarchy = currentEntity.HasComponent<HierarchyComponent>();
			if ( !hasHierarchy || ( hasHierarchy && !currentEntity.GetComponent<HierarchyComponent>().HasParent() ) )
				BuildEntityHierarchy(currentEntity);
		}
		
		if ( selectedEntity != -1 ) {
			Entity entityToShow = ECS::ViewEntity(selectedEntity);
			ShowComponentsOf(entityToShow);
		} else {
			ImGui::Begin("Entity Components");
			ImGui::End();
		}


		if ( Input::IsKeyPressed(CDL_KEY_LEFT_CONTROL) && Input::OnKeyDown(CDL_KEY_D) ) {
			CTRACE("Control D -> Copy Entity");
		}


		if ( (Input::IsMouseButtonPressed(1) || openHierarchyPopup) && windowFocusAndHovered ) {
			openHierarchyPopup = true;
			ImGui::OpenPopup("popup_hierarchy");
		}

		if (ImGui::BeginPopup("popup_hierarchy")) {

			if (ImGui::MenuItem("New Entity")) {
				ECS::New("new_entity");
			}

			if (selected != -1 && selectedEntity != -1 ) if (ImGui::MenuItem("Delete Entity")) {
				ECS::Remove(selectedEntity);
			}
			ImGui::EndPopup();
		}

		if ( (Input::IsMouseButtonPressed(0) || Input::IsMouseButtonPressed(2)) && openHierarchyPopup ) {
			openHierarchyPopup = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::End();

	}


		/*
			--- ShowComponentsOf (Called from ::ShowSceneHierarchy)
			Display informations about the scripts and components attached to the selected Blueprint
			
			@input Shared<Blueprint> : selected blueprint
		*/
	void Editor::ShowComponentsOf(Entity& entity)
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

					std::string alertBoxTitle = "Remove " + ( (Component*)comp )->GetComponentName();
					if ( ImGui::ArrowButton(alertBoxTitle.c_str(), ImGuiDir_Down) )
						ImGui::OpenPopup(alertBoxTitle.c_str());

					if ( ImGui::BeginPopupModal(alertBoxTitle.c_str(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove) ) {
						if ( ImGui::Button("Remove") ) {
							CTRACE("Delete component {0}", ( (Component*)comp )->GetComponentName());
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
				if (ImGui::MenuItem("Transform", "", false, !entity.HasComponent<Transform>())) entity.AddComponent<Transform>();
				if (ImGui::MenuItem("CameraHandler", "", false, !entity.HasComponent<CameraHandler>())) entity.AddComponent<CameraHandler>();

					// Renderer 2D
				ImGui::Separator();
				if (ImGui::MenuItem("SpriteRenderer", "", false, !entity.HasComponent<SpriteRenderer>())) entity.AddComponent<SpriteRenderer>();

					// Animations
				ImGui::Separator();
				if (ImGui::MenuItem("AnimationController", "", false, !entity.HasComponent<AnimationController>())) entity.AddComponent<AnimationController>();

				ImGui::EndPopup();
			}
			
			if ( ImGui::BeginPopup("EditorAddScriptPopup") ) {
				for ( auto& [name, script] : EditorScriptRegistery::Get().Scripts() ) {
					if ( ImGui::MenuItem(name.c_str(), "", false) ) {
						Script* _ptr = (*script)();
						entity.AddScript(_ptr);
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