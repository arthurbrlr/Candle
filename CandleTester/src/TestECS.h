#pragma		once

#include <Candle.h>

	// ------------------ Need abstraction --------------

static bool useOrthoCamera = true;


static std::pair<double, double> GetMouseScreenCoordinates()
{
	double screenWidth = Candle::CDL_APP_WIDTH;
	double screenHeight = Candle::CDL_APP_HEIGHT;

	std::pair<double, double> mousePixelPos = Candle::Input::GetMousePos();
	
	return { 2 * mousePixelPos.first / screenWidth - 1., 1. - 2 * mousePixelPos.second / screenHeight };
}


/*
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
			0., 0., 0.,  .7, .5, .5, 1.,
			0., 0., 0.,  .5, .5, .7, 1.
		};

		uint32_t _defaultIndices[2] = {
			0, 1
		};

};

std::vector<glm::vec3> LineRenderSystem::_lines;
*/
