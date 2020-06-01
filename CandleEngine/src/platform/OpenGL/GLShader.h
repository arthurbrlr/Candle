#pragma once

#include "cdlpch.h"
#include "Candle/CandleCore.h"
#include "Candle/Assets/Shader.h"

#include <GL/glew.h>

namespace Candle {

	class CANDLE_API GLShader : public Shader {

		public:
			GLShader(const std::string & shaderPath);
			GLShader(const std::string & shaderCode, const std::string & shaderName);
			GLShader(const std::string & name, const std::string & vertexShader, const std::string & fragmentShader);
			virtual ~GLShader();

			void SetInt(const std::string & name, int value)					  		override;
			void SetIntArray(const std::string & name, int * value, unsigned int count) override;
			void SetFloat(const std::string & name, float value)						override;
			void SetFloat2(const std::string & name, const glm::vec2 & value)			override;
			void SetFloat3(const std::string & name, const glm::vec3 & value)			override;
			void SetFloat4(const std::string & name, const glm::vec4 & value)			override;
			void SetMat4(const std::string & name, const glm::mat4 & value)				override;


		private:
			void BindShaderImplementation() override;
			void UnbindShaderImplementation() override;
		
			std::string ReadShaderFile(const std::string & shaderPath);
			std::unordered_map<GLenum, std::string> PreProcess(const std::string & shaderSource);
			void Compile(const std::unordered_map<GLenum, std::string> & sources);
			void RegisterUniforms(const std::string & shaderSource) override;
	};


}