#include "cdlpch.h"
#include "Shader.h"

#include "Candle/Renderer/Renderer.h"
#include "platform/OpenGL/GLShader.h"

namespace Candle {

	Unique<Shader> Shader::Create(const std::string & shaderPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
		{
			return std::make_unique<GLShader>(shaderPath);
			break;
		}
		case RendererAPI::API::None:
		{
			CASSERT(false, "No render api choosen");
			break;
		}
		}

		CASSERT(false, "No render api choosen");
		return nullptr;
	}


	Unique<Shader> Shader::Create(const std::string & shaderCode, const std::string & shaderName)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_unique<GLShader>(shaderCode, shaderName);
				break;
			}
			case RendererAPI::API::None:
			{
				CASSERT(false, "No render api choosen");
				break;
			}
		}

		CASSERT(false, "No render api choosen");
		return nullptr;
	}


	Unique<Shader> Shader::Create(const std::string & name, const std::string & vertexShader, const std::string & fragmentShader)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:
			{
				return std::make_unique<GLShader>(name, vertexShader, fragmentShader);
				break;
			}
			case RendererAPI::API::None:
			{
				CASSERT(false, "No render api choosen");
				break;
			}
		}

		CASSERT(false, "No render api choosen");
		return nullptr;
	}



	void Shader::Bind()
	{
		if (this == nullptr) return;
		BindShaderImplementation();
		InitializeUniforms();
	}


	void Shader::Unbind()
	{
		if (this == nullptr) return;
		UnbindShaderImplementation();
		ResetUniforms();
	}


	void Shader::InitializeUniforms()
	{
		if (this == nullptr) return;
		for (auto& uniform : _uniforms) {

			if (uniform.second.rate == ShaderUniformRefreshRate::Static && !uniform.second.initialized) {

				if (uniform.second.type == ShaderUniformType::Sampler) {

					int* samplers = new int[uniform.second.length];
					for (int i = 0; i < uniform.second.length; i++) {
						samplers[i] = i;
					}
					SetIntArray(uniform.first, samplers, uniform.second.length);
					uniform.second.initialized = true;
				}

			}

		}
	}


	void Shader::ResetUniforms()
	{
		if (this == nullptr) return;
		for (auto& uniform : _uniforms) {

			if (uniform.second.rate == ShaderUniformRefreshRate::Dynamic)
				uniform.second.initialized = false;
		}
	}

}