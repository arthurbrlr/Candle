#include "../EditorLayer.h"

#include "Candle/Core/Input.h"
#include "Candle/Scenes/SceneManagement.h"
#include "Candle/Renderer/CameraManagement.h"

using namespace Candle;

glm::vec3 CursorPositionInSceneFromViewport(const glm::vec2& panelSize, const glm::vec2& panelPosition, const glm::vec3& cameraPosition, double zoomLevel)
{
	static double logging_cooldown = 2.0;
	static double cooldown_current = 2.0;

	cooldown_current -= Time::FixedDeltaTime();
	if ( cooldown_current < 0 ) {
		CDL_INFO("**********************");
		CDL_INFO("Panel Size: ( {0}, {1})", panelSize.x, panelSize.y);
		CDL_INFO("Panel Position: ( {0}, {1})", panelPosition.x, panelPosition.y);
		CDL_INFO("Mouse Position: ( {0}, {1})", Input::GetMouseX(), Input::GetMouseY());
		CDL_INFO("Zoom level: {0}", zoomLevel);

		cooldown_current = logging_cooldown;
	}

	double x = Input::GetMouseX() - panelPosition.x;
	x = ( x / panelSize.x ) * 2.0 - 1.0;
	x *= Application::AspectRatio() * zoomLevel;
	x += cameraPosition.x;

	double y = Input::GetMouseY() - panelPosition.y;
	y = 1.0 - ( y / panelSize.y ) * 2.0;
	y *= zoomLevel;
	y += cameraPosition.y;

	return { x, y, 0 };
}

	/*
		--- ShowEditorViewer
		Render the scene to the editor viewer using the Editor camera
	*/
void EditorLayer::ShowEditorViewer()
{
	static bool show = true;
	static bool showStatsWindow = false;

	if ( ImGui::Begin("Scene Viewer", &show, ImGuiWindowFlags_MenuBar) ) {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 1, 1, 1, 1 });

		// Menu
		if ( ImGui::BeginMenuBar() ) {

			if ( ImGui::BeginMenu("Settings") ) {
				if ( ImGui::MenuItem("Placeholder", "") ) {
				}

				ImGui::Separator();
				if ( ImGui::MenuItem("Placeholder 2", "") ) {
				}

				ImGui::EndMenu();
			}

			if ( ImGui::BeginMenu("Debug") ) {

				if ( ImGui::BeginMenu("Editor Camera") ) {

					if ( ImGui::MenuItem("Perspective", "", !_cameraController.UseOrthographic()) ) {
						_cameraController.UseOrthographic(false);
					} else if ( ImGui::MenuItem("Orthographic", "", _cameraController.UseOrthographic()) ) {
						_cameraController.UseOrthographic(true);
					}

					if ( ImGui::Button("Reset Camera View") ) {
						_cameraController.ResetView();
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if ( ImGui::MenuItem("Draw Wireframe", "", _eb.DrawLines) ) {
					_eb.DrawLines = !_eb.DrawLines;
				}

				if ( ImGui::MenuItem("Draw Textures", "", _eb.DrawTextures) ) {
					_eb.DrawTextures = !_eb.DrawTextures;
				}

				static bool depthTestEnable = true;
				if ( ImGui::MenuItem("Depth Test", "", depthTestEnable) ) {
					depthTestEnable = !depthTestEnable;
					RenderCommands::SetDepthTesting(depthTestEnable);
				}

				static bool blendingEnable = false;
				if ( ImGui::MenuItem("Blending", "", blendingEnable) ) {
					blendingEnable = !blendingEnable;
					RenderCommands::SetAlphaBlending(blendingEnable);
				}

				ImGui::Separator();

				if ( ImGui::MenuItem("Stats") ) {
					showStatsWindow = true;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}


		// Editor render
		ImVec2 windowSize = ImGui::GetWindowSize();
		_eb.MouseInEditorViewport = ImGui::IsWindowFocused() && ImGui::IsWindowHovered();

		double aspectRatio = Application::AspectRatio();
		ImVec2 viewerSize = ImVec2(windowSize.x * 0.99f, windowSize.x * 0.99f / (float)aspectRatio);
		ImVec2 cursorPos = ImVec2(( windowSize.x - viewerSize.x ) * 0.5f, ( windowSize.y - viewerSize.y ) * 0.5f);
		ImGui::SetCursorPos(cursorPos);
		ImVec2 cursorPosInScreen = ImGui::GetCursorScreenPos();


		if ( _eb.MouseInEditorViewport ) {
			static Entity cursorEntity;
			static bool set = false;
			if ( !set ) {
				cursorEntity = ECS::New("cursor_follower");
				set = true;
				cursorEntity.AddComponent<Transform>();
				cursorEntity.AddComponent<SpriteRenderer>();
			}

			cursorEntity.GetComponent<Transform>().SetPosition(
				CursorPositionInSceneFromViewport( { viewerSize.x, viewerSize.y }, { cursorPosInScreen.x, cursorPosInScreen.y }, _cameraController.GetPosition(), _cameraController.GetZoom() )
			);
		}

		// Editor view rendering
		_editorFrameBuffer->Bind();
		RenderCommands::Clear();
		CameraManagement::UseView(Controller().GetPVMatrix());

		Renderer2D::BeginScene();
		SceneManagement::OnRender(false);
		if ( !Controller().UseOrthographic() ) {
			Renderer2D::DrawQuad(Controller().GetTargetTransform().GetPosition(), { .1, .1 });
		}
		Renderer2D::EndScene();

		CameraManagement::Reset();
		_editorFrameBuffer->Unbind(CDL_APP_WIDTH, CDL_APP_HEIGHT);

		ImGui::Image((void*)(intptr_t)_editorTexture->GetID(), viewerSize, ImVec2(0, 1), ImVec2(1, 0));


		// Editor view raycasting


		ImGui::PopStyleColor();
	} else {
		_eb.MouseInEditorViewport = false;
	}
	ImGui::End();

	if ( showStatsWindow ) ShowRendererWindow(&showStatsWindow);
}