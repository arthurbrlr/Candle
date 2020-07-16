#pragma once

#include "Candle/CandleCore.h"
#include "ShaderUniforms.h"

namespace Candle {

	class Shader {

		public:
			Shader() {}
			virtual ~Shader() {}

			void Bind();
			void Unbind();

			virtual void SetInt(const std::string & name, int value) = 0;
			virtual void SetIntArray(const std::string & name, int * value, uint32_t count) = 0;
			virtual void SetFloat(const std::string & name, float value) = 0;
			virtual void SetFloat2(const std::string & name, const glm::vec2 & value) = 0;
			virtual void SetFloat3(const std::string & name, const glm::vec3 & value) = 0;
			virtual void SetFloat4(const std::string & name, const glm::vec4 & value) = 0;
			virtual void SetMat4(const std::string & name, const glm::mat4 & value) = 0;

			inline const uint32_t GetID() { return _programID; }
			inline const std::string & GetName() { return _name; }
			inline const std::string & GetPath() { return _shaderPath; }
			inline const std::string & GetCode() { return _shaderCode; }
			inline const std::unordered_map<std::string, ShaderUniform> & GetUniforms() { return _uniforms; }

			static Unique<Shader> Create(const std::string & shaderPath);
			static Unique<Shader> Create(const std::string & shaderCode, const std::string & shaderName);
			static Unique<Shader> Create(const std::string & name, const std::string & vertexShader, const std::string & fragmentShader);

		protected:
			uint32_t _programID = 0;
			std::string _name;
			std::string _shaderPath;
			std::string _shaderCode;
			std::unordered_map<std::string, ShaderUniform> _uniforms;

			virtual void RegisterUniforms(const std::string & shaderSource) = 0;

			virtual void BindShaderImplementation() = 0;
			virtual void UnbindShaderImplementation() = 0;
			void InitializeUniforms();
			void ResetUniforms();

	};

}