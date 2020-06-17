#include "cdlpch.h"
#include "Editor.h"

#include "Candle/Core/Input.h"
#include "Candle/Scenes/SceneManagement.h"

namespace Candle {

	void Editor::Display()
	{
		double t1 = Time::Milliseconds();
			// Do not modify this line
		_imguiLayer->Begin();
		BuildDockSpace();

		ShowMainMenu();
		ShowMasterWindow();
		ShowRendererWindow();

		ShowGameViewer();
		ShowEditorViewer();

		ShowAssetManagerWindow();
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
		if ( ImGui::BeginMainMenuBar() ) {
			if ( ImGui::BeginMenu("File") ) {

				{
					ImGui::MenuItem("(dummy menu)", NULL, false, false);
					if ( ImGui::MenuItem("New") ) {}
					if ( ImGui::MenuItem("Open", "Ctrl+O") ) {}
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
				}

				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu("Edit") ) {
				if ( ImGui::MenuItem("Undo", "CTRL+Z") ) {}
				if ( ImGui::MenuItem("Redo", "CTRL+Y", false, false) ) {}  // Disabled item
				ImGui::Separator();
				if ( ImGui::MenuItem("Cut", "CTRL+X") ) {}
				if ( ImGui::MenuItem("Copy", "CTRL+C") ) {}
				if ( ImGui::MenuItem("Paste", "CTRL+V") ) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
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
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 mouse = ImGui::GetMousePos();
			bool mouseXvalid = mouse.x > windowPos.x && mouse.x < windowPos.x + windowSize.x;
			bool mouseYvalid = mouse.y > windowPos.y && mouse.y < windowPos.y + windowSize.y;
			_eb.MouseInEditorViewport = mouseXvalid && mouseYvalid;

				// Editor view rendering
			_editorFrameBuffer->Bind();
			CameraManagement::UseView(Controller().GetPVMatrix());
			SceneManagement::OnEditorRender();
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
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 mouse = ImGui::GetMousePos();
			bool mouseXvalid = mouse.x > windowPos.x && mouse.x < windowPos.x + windowSize.x;
			bool mouseYvalid = mouse.y > windowPos.y && mouse.y < windowPos.y + windowSize.y;
			_eb.MouseInGameViewport = mouseXvalid && mouseYvalid;
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
			--- ShowAssetManagerWindow
			Display all the currently loaded assets and their related informations
			e.g: Textures, Shaders, etc...
		*/
	void Editor::ShowAssetManagerWindow()
	{
		ImGui::Begin("AssetManager");
		{
			if (ImGui::CollapsingHeader("Shaders Loaded")) {
				for (auto& shader : Candle::AssetManager::GetAllShaders()) {
					if (shader.second == nullptr) continue;
					std::string headerName = "Shader " + std::to_string(shader.second->GetID()) + " : " + shader.second->GetName();
					if (ImGui::TreeNode(headerName.c_str())) {

						std::string buttonname = "Reload: " + shader.second->GetName();
						ImGui::SameLine();					
						if (ImGui::Button(buttonname.c_str())) {
							AssetManager::ReloadShader(shader.first);
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
				for (auto& texture : Candle::AssetManager::GetAllTexture2D()) {
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
		static Blueprint* bpShowed = nullptr;
		int index = 0;

		static bool openPopup = false;
		ImVec2 mpos = ImGui::GetMousePos();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		bool x_valid = mpos.x > windowPos.x && mpos.x < windowPos.x + windowSize.x;
		bool y_valid = mpos.y > windowPos.y && mpos.y < windowPos.y + windowSize.y;
		bool mouseInWindow = x_valid && y_valid;

		/*
		if (ImGui::Button("Save Changes")) {
			CINFO("Saving blueprint data");
		}
		*/

		if (ImGui::Button("Clear Selection")) {
			selected = -1;
			bpShowed = nullptr;
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
			index++;
			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
			if ( !currentBlueprint->GetViewInEditor() ) return;

			std::string blueprintEditor = currentBlueprint->GetName() + " (ID: " + std::to_string(currentBlueprint->GetID()) + ")";

			if ( currentBlueprint->HasChildren() ) {

				if ( selected == index ) node_flags |= ImGuiTreeNodeFlags_Selected;
				bool node_open = ImGui::TreeNodeEx(blueprintEditor.c_str(), node_flags);
				if ( ImGui::IsItemClicked() ) {
					selected = index;
					bpShowed = currentBlueprint;
				}
				if ( node_open ) {
					for ( auto& child : currentBlueprint->GetChilds() ) {
						BuildBlueprintTree(child.second);
					}
					ImGui::TreePop();
				}

			} else {

				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				if ( selected == index ) node_flags |= ImGuiTreeNodeFlags_Selected;
				ImGui::TreeNodeEx(blueprintEditor.c_str(), node_flags);
				if ( ImGui::IsItemClicked()) {
					selected = index;
					bpShowed = currentBlueprint;
				}

			}

		};


		for (auto& bp : BlueprintManager::All()) {
			
			if ( !bp.second->GetViewInEditor() || bp.second->HasParent() ) continue;
			BuildBlueprintTree(&(*bp.second));

		}

		ShowComponentsOf(bpShowed);


		if ( (Input::IsMouseButtonPressed(1) || openPopup) && mouseInWindow) {

			openPopup = true;

			ImGui::OpenPopup("sceneHierarchyPopup");
		}

		if (ImGui::BeginPopup("sceneHierarchyPopup"))
		{
			if (ImGui::MenuItem("New Blueprint")) {
				ECS::New();
			}
			if (selected != -1 && bpShowed != nullptr) if (ImGui::MenuItem("Remove Blueprint")) {
				size_t id = bpShowed->GetID();
				bpShowed = nullptr;
				ECS::Remove(id);
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

			for (ComponentID id = 0; id < MaxComponents; id++) {

				if (!bp->HasComponentOfID(id)) continue;
				auto component = bp->GetComponentOfID(id);

				std::string menuTitle = component->GetName() + " (" + std::to_string(bp->GetID()) + ")";
				if (ImGui::TreeNodeEx(menuTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Checkbox("Active", &component->IsActive());
					component->OnEditor();
					ImGui::TreePop();
					ImGui::Separator();
				}

			}

			for (auto& script : bp->Scripts()) {

				std::string menuTitle = script->GetName() + " (" + std::to_string(bp->GetID()) + ")";
				if (ImGui::TreeNodeEx(menuTitle.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
					script->OnEditor();
					ImGui::TreePop();
					ImGui::Separator();
				}
			}

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("EditorAddComponentPopup");
			}

			if (ImGui::BeginPopup("EditorAddComponentPopup")) {
			
				if (ImGui::MenuItem("Transform", "", false, !bp->HasComponent<Transform>())) bp->AddComponent<Transform>();
				if (ImGui::MenuItem("SpriteRenderer", "", false, !bp->HasComponent<SpriteRenderer>())) bp->AddComponent<SpriteRenderer>();
				if (ImGui::MenuItem("AnimationController", "", false, !bp->HasComponent<AnimationController>())) bp->AddComponent<AnimationController>();
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
			ImGui::Combo("Camera Projection", &currentEditorCamera, editorCameras, IM_ARRAYSIZE(editorCameras));
			_cameraController.UseOrthographic((bool)currentEditorCamera);
			float camPosition[3] = { _cameraController.GetPosition().x, _cameraController.GetPosition().y, _cameraController.GetPosition().z };
			float camRotation[3] = { _cameraController.GetRotation().x, _cameraController.GetRotation().y, _cameraController.GetRotation().z };
			ImGui::InputFloat3("Camera Position", camPosition);
			ImGui::InputFloat3("Camera Rotation", camRotation);

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
	EditorCameraController Editor::_cameraController;


	void Editor::Init()
	{
		_imguiLayer = std::make_unique<ImGuiLayer>();
		_imguiLayer->OnAttach();

		_eb.ContextVersion = std::string(reinterpret_cast<const char*>(Application::Get().GetWindow().GetContext().GetContextVersion()));
		_eb.HardwareVendor = std::string(reinterpret_cast<const char*>(Application::Get().GetWindow().GetContext().GetHardwareVendor()));
		_eb.HardwareRenderer = std::string(reinterpret_cast<const char*>(Application::Get().GetWindow().GetContext().GetRenderer()));

		_cameraController = EditorCameraController(CDL_APP_WIDTH, CDL_APP_HEIGHT);

		_editorFrameBuffer = FrameBuffer::Create({ Texture, DepthBuffer }, CDL_APP_WIDTH, CDL_APP_HEIGHT);
		_editorTexture = Texture2D::Create(_editorFrameBuffer->Get(Texture), CDL_APP_WIDTH, CDL_APP_HEIGHT);
	}


	void Editor::OnUpdate()
	{
		_cameraController.OnUpdate();
		
		// Dynamicly reload the assets
	}


	void Editor::OnEvent(Event& event)
	{
		if (_eb.MouseInEditorViewport )
			_cameraController.OnEvent(event); 
	}


	void Editor::Clear()
	{
		_imguiLayer->OnDetach();
	}


}