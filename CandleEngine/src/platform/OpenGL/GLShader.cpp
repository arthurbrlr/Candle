#include "cdlpch.h"
#include "GLShader.h"

#include <fstream>

namespace Candle {

	GLenum ShaderTypeFromString(const std::string & typeString)
	{
		if (typeString == "vertex") return GL_VERTEX_SHADER;
		if (typeString == "fragment" || typeString == "pixel") return GL_FRAGMENT_SHADER;

		CASSERT(false, "Unknown Shader Type");
		return 0;
	}


	GLShader::GLShader(const std::string & shaderPath)
	{
		std::string source = ReadShaderFile(shaderPath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
			
			// Transform "res/shaders/shadername.glsl" into "shadername"
		auto lastSlash = shaderPath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = shaderPath.rfind('.');
		auto count = lastDot == std::string::npos ? shaderPath.size() - lastSlash : lastDot - lastSlash;

		_name = shaderPath.substr(lastSlash, count);
		_shaderPath = shaderPath;

		RegisterUniforms(source);
		Bind();
	}


	GLShader::GLShader(const std::string & shaderCode, const std::string & shaderName)
	{
		std::string source = shaderCode;
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		_name = shaderName;
		_shaderPath = "CDL_SHADERS/" + shaderName;

		RegisterUniforms(source);
		Bind();
	}


	GLShader::GLShader(const std::string & name, const std::string & vertexSource, const std::string & fragmentSource)
	{
		
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSource;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(shaderSources);
		_name = name;

	}


	GLShader::~GLShader()
	{
		glDeleteProgram(_programID);
	}


	std::string GLShader::ReadShaderFile(const std::string & shaderPath)
	{
		std::string filestring;
		std::ifstream shader(shaderPath, std::ios::in | std::ios::binary);

		if (shader) {

			shader.seekg(0, std::ios::end);
			filestring.resize(shader.tellg());
			shader.seekg(0, std::ios::beg);
			shader.read(&filestring[0], filestring.size());
			shader.close();

		}
		else {
			CASSERT(false, "Couldn't open shader file");
		}

		return filestring;
	}


	std::unordered_map<GLenum, std::string> GLShader::PreProcess(const std::string & shaderSource)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* token = "@shader_type";
		size_t tokenLength = strlen(token);
		size_t pos = shaderSource.find(token, 0);
		while (pos != std::string::npos) {

			size_t eol = shaderSource.find_first_of("\r\n", pos);
			CASSERT(eol != std::string::npos, "Syntax error in shader file");
			size_t begin = pos + tokenLength + 1;
			std::string type = shaderSource.substr(begin, eol - begin);
			CASSERT(ShaderTypeFromString(type), "Invalid shader specifier");

			size_t nextLinePos = shaderSource.find_first_not_of("\r\n", eol);
			pos = shaderSource.find(token, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				shaderSource.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? shaderSource.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}


	void GLShader::Compile(const std::unordered_map<GLenum, std::string> & sources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> shaderIDs;

		for (auto& key : sources) {

			GLenum shaderType = key.first;
			std::string shaderCode = key.second;
			const char *source = shaderCode.c_str();
			
			uint32_t shader = glCreateShader(shaderType);
			glShaderSource(shader, 1, &source, NULL);
			glCompileShader(shader);

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);
				CERROR("Shader didn't compile: {0}", infoLog.data());
				return;
			}

			shaderIDs.push_back(shader);
		}

		for (int i = 0; i < shaderIDs.size(); i++) {
			glAttachShader(program, shaderIDs[i]);
		}

		glLinkProgram(program);
		glValidateProgram(program);

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (int i = 0; i < shaderIDs.size(); i++)
				glDeleteShader(shaderIDs[i]);

			CERROR("Shader program didn't link: {0}", infoLog.data());
			return;
		}

		for (int i = 0; i < shaderIDs.size(); i++)
			glDetachShader(program, shaderIDs[i]);

		_programID = program;
	}


	void GLShader::RegisterUniforms(const std::string & shaderSource)
	{
		const char* token = "uniform";
		size_t tokenLength = strlen(token);
		size_t pos = shaderSource.find(token, 0);

		while (pos != std::string::npos) {
			ShaderUniform shaderUniformObject;

			size_t eol = shaderSource.find_first_of("\r\n", pos);
			CASSERT(eol != std::string::npos, "Syntax error in shader file");

				// Beginning of the line
			size_t begin = pos + tokenLength + 1;
			std::string uniform = shaderSource.substr(begin, eol - begin);
		
				// Find the space between uniform and type
				//		  v
				// uniform float u_time;
			size_t spacePos = uniform.find_first_of(" ", 0);
				// uniform key is the GLSL type (float, int, mat4, etc...)
			std::string uniformKey = uniform.substr(0, spacePos);
			
				// uniformEnd is the place of the coma
			size_t uniformEnd = uniform.find_first_of(";", 0);
				// samplerEnd is finite if the uniform is a sampler of the format "uniform sampler2D sampler[32]"
			size_t uniformSamplerEnd = uniform.find_first_of("[", 0);
				// take the min of these sizes
			size_t uniformNameEnd = std::min(uniformEnd, uniformSamplerEnd);
				// isolate the name of the uniform
			std::string uniformName = uniform.substr(spacePos + 1, uniformNameEnd - spacePos - 1);

				// Convert the glsl key to a ShaderUniformType
			shaderUniformObject.type = ShaderUniformTypeFromGLSLKey(uniformKey);

				// If it is a sampler
			if (shaderUniformObject.type == ShaderUniformType::Sampler) {
					// Check for left and right brackets
				size_t samplerLeftBrackPos = uniform.find_first_of("[");
				size_t samplerRightBrackPos = uniform.find_first_of("]");
					
					// get the size of the sampler
				// if it's superior to 3, then it means that there were no brackets
				std::string samplerSize = uniform.substr(samplerLeftBrackPos + 1, samplerRightBrackPos - samplerLeftBrackPos - 1);

				if (samplerSize.size() < 3) {
					shaderUniformObject.length = std::stoi(samplerSize);
				} else {
					shaderUniformObject.length = 0;
				}
			}
				// Get the location of the uniform in the shader program
			shaderUniformObject.location = glGetUniformLocation(_programID, uniformName.c_str());
				// Get the refresh type of the shader
			shaderUniformObject.rate = ShaderUniformRefreshRateFromType(shaderUniformObject.type);

				// [NOTE]
			// UpdateUniforms () { for (auto& uniform: uniforms) if (uniform.rate == Dynamic) SetUniformValue(uniform, "key" ?) 
			// (key here is sort of a Candle KeyCode like : CDLTIME for Candle::Time()

				// Adds the uniform to the table
			_uniforms[uniformName] = shaderUniformObject;

				// Next line
			size_t nextLinePos = shaderSource.find_first_not_of("\r\n", eol);
			pos = shaderSource.find(token, nextLinePos);
		}
	}


	void GLShader::BindShaderImplementation()
	{
		glUseProgram(_programID);
	}


	void GLShader::UnbindShaderImplementation()
	{
		glUseProgram(0);
	}


	void GLShader::SetInt(const std::string & name, int value)
	{
		glUniform1i(glGetUniformLocation(_programID, name.c_str()), value);
	}
	
	
	void GLShader::SetIntArray(const std::string & name, int * value, uint32_t count)
	{
		glUniform1iv(glGetUniformLocation(_programID, name.c_str()), count, value);
	}

	
	void GLShader::SetFloat(const std::string & name, float value)
	{
		glUniform1f(glGetUniformLocation(_programID, name.c_str()), value);
	}


	void GLShader::SetFloat2(const std::string & name, const glm::vec2 & value)
	{
		glUniform2f(glGetUniformLocation(_programID, name.c_str()), value.x, value.y);
	}


	void GLShader::SetFloat3(const std::string & name, const glm::vec3 & value)
	{
		glUniform3f(glGetUniformLocation(_programID, name.c_str()), value.x, value.y, value.z);
	}


	void GLShader::SetFloat4(const std::string & name, const glm::vec4 & value)
	{
		glUniform4f(glGetUniformLocation(_programID, name.c_str()), value.x, value.y, value.z, value.w);
	}


	void GLShader::SetMat4(const std::string & name, const glm::mat4 & value)
	{
		glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

}