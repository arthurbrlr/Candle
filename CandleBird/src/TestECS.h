#pragma		once

#include <Candle.h>

	// ------------------ Need abstraction --------------

static bool useOrthoCamera = true;


static std::pair<double, double> GetMouseScreenCoordinates()
{
	double screenWidth = CDL_APP_WIDTH;
	double screenHeight = CDL_APP_HEIGHT;

	std::pair<double, double> mousePixelPos = Candle::Input::GetMousePos();
	
	return { 2 * mousePixelPos.first / screenWidth - 1., 1. - 2 * mousePixelPos.second / screenHeight };
}

	// ----------------------------------------------


	// Later, the renderer won't be part of the ECS so the OnRenderUpdate() will have to be ditched
	// of the system manager, and ECS::Render() should be deleted

class RenderSystem : public Candle::System {

	public:
		RenderSystem()
		{
			_animptr = new Candle::Animation("default", {});
		}


		~RenderSystem()
		{
			delete _animptr;
		}


		void OnRenderUpdate() override
		{
			std::vector<Candle::Shared<Candle::Blueprint>> bps = Candle::BlueprintManager::Get<Candle::SpriteRenderer>();

			if (bps.size() == 0) return;

			//static int numberOfQuadsDraw = 0;
			//static int previousNumberOfQuadsDraw = 0;
			double t1 = Time::Milliseconds();

			//previousNumberOfQuadsDraw = Candle::Renderer2D::GetStats()->OpaqueQuads;
			//numberOfQuadsDraw = bps.size();

			Candle::Renderer2D::BeginScene();
			//if ( previousNumberOfQuadsDraw == numberOfQuadsDraw ) goto endScene;
			Candle::Renderer2D::GetStats()->Reset();

				/* Sprite Rendering */
			for (auto& bp : bps) {

				Candle::SpriteRenderer& srComp = bp->GetComponent<Candle::SpriteRenderer>();
				
				if (!srComp.IsActive()) continue;

				if (bp->HasComponent<Candle::AnimationController>()) {
					bool animBool = bp->GetComponent<Candle::AnimationController>().GetCurrentAnimation(*_animptr);

					if (animBool) {
						srComp.SetTextureCoordinates(_animptr->GetKeyframes()[_animptr->GetCurrentKeyFrameIndex()].textureCoordinates);
					}
				}

				if (bp->HasComponent<Candle::Transform>()) {
					Candle::Renderer2D::DrawSprite(srComp, bp->GetComponent<Candle::Transform>());
				} else {
					Candle::Renderer2D::DrawSprite(srComp);
				}
					
			}

			//endScene:

				// Sprite part test
			Candle::SpritePart part1 = { "part1" , {0, 1, 1}, { 0, 1, 0, 1 }, { 0.1, 0.1 } };
			Candle::SpritePart part2 = { "part2" , {1, 0, 1}, { 0, 1, 0, 1 }, { 0.2, 0.2 } };
			Candle::SpritePart part3 = { "part3" , {0, 0, 1}, { 0, 1, 0, 1 }, { 0.3, 0.1 } };

			std::vector<Candle::SpritePart> parts = {
				part1,
				part2,
				part3
			};

			//Candle::Renderer2D::DrawPartialSprite(Transform(), Assets::GetTexture2D("dodo"), parts, true);


			Candle::Renderer2D::EndScene();
			Candle::Renderer2D::GetStats()->ecsSpriteRenderTime = Time::Milliseconds() - t1;
		}


	private:
		Candle::Animation* _animptr;

};


class LineRenderSystem : public Candle::System {

	public:
		LineRenderSystem()
		{

			_lineVao = Candle::VertexArray::Create();
			Candle::BufferLayout layout = {
				{ "position",	Candle::ShaderDataType::vec3f },
				{ "color",		Candle::ShaderDataType::vec4f }
			};
			_lineVbo = Candle::VertexBuffer::Create(14 * sizeof(float));
			_lineVbo->SetLayout(layout);
			_lineVao->AddVertexBuffer(_lineVbo);

			Candle::Shared<Candle::IndexBuffer> ebo = Candle::IndexBuffer::Create(_defaultIndices, 2 * sizeof(uint32_t));
			_lineVao->SetIndexBuffer(ebo);
		}


		void OnRenderUpdate() override
		{

			Candle::Assets::GetShader("line")->Bind();
			Candle::Assets::GetShader("line")->SetMat4("u_viewProjection", Candle::CameraManagement::GetViewProjection());

			for ( int i = 0; i < _lines.size() / 2; i++ ) {

				_defaultVertices[0] = _lines[2 * i].x;
				_defaultVertices[1] = _lines[2 * i].y;
				_defaultVertices[2] = _lines[2 * i].z;


				_defaultVertices[7] = _lines[2 * i + 1].x;
				_defaultVertices[8] = _lines[2 * i + 1].y;
				_defaultVertices[9] = _lines[2 * i + 1].z;

				_lineVbo->SetData(_defaultVertices, 14 * sizeof(float));
				_lineVao->Bind();
				Candle::RenderCommands::DrawLine(_lineVao, 2);
			}

		}
	

		static void AddLine(const glm::vec3& p1, const glm::vec3& p2)
		{
			_lines.push_back(p1);
			_lines.push_back(p2);

			CTRACE("Line from ({0}, {1}, {2}) to ({3}, {4}, {5})", p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
		}

	private:
		Candle::Shared<Candle::VertexArray> _lineVao;
		Candle::Shared<Candle::VertexBuffer> _lineVbo;

		static std::vector<glm::vec3> _lines;

		float _defaultVertices[14] = {
			0., 0., 0., /**/ .7, .5, .5, 1.,
			0., 0., 0., /**/ .5, .5, .7, 1.
		};

		uint32_t _defaultIndices[2] = {
			0, 1
		};

};

std::vector<glm::vec3> LineRenderSystem::_lines;