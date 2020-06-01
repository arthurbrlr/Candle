#include "cdlpch.h"
#include "Renderer.h"

#include "Candle/Core/Time.h"
#include "Renderer2D.h"
#include "CameraManagement.h"


namespace Candle {

	void Renderer::Init()
	{
		RenderCommands::Init();
		Renderer2D::Init();
	}

	
	void Renderer::Clear()
	{
		Renderer2D::Clear();
	}


	void Renderer::BeginScene()
	{
		CameraManagement::UpdateView();
	}


	void Renderer::EndScene()
	{
		RenderCommands::Clear();
	}


	void Renderer::SubmitVAO(const Shared<Shader> & shader, const Shared<Texture2D> & texture, const Shared<VertexArray> & vertexArray, glm::mat4 transform)
	{
		texture->Bind();
		shader->Bind();
		
		shader->SetFloat("u_time", (float)Time());
		shader->SetMat4("u_transform", transform);
		shader->SetMat4("u_projectionViewMatrix", CameraManagement::GetViewProjection());
		
		vertexArray->Bind();
		RenderCommands::DrawArray(vertexArray);
	}


	void Renderer::SubmitVAO(const Shared<Shader> & shader, const Shared<VertexArray> & vertexArray, glm::mat4 transform)
	{
		shader->Bind();

		shader->SetFloat("u_time", (float)Time());
		shader->SetFloat4("u_color", glm::vec4(0.2, 0.4, 0.2, 1.0));
		shader->SetMat4("u_transform", transform);
		shader->SetMat4("u_projectionViewMatrix", CameraManagement::GetViewProjection());

		vertexArray->Bind();
		RenderCommands::DrawArray(vertexArray);
	}
}
