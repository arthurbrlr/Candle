#pragma once

#include "Candle/CandleCore.h"

namespace Candle {

	enum ShaderUniformType {
		Undefined = -1,
		Float, Int,
		Vec2, Vec3, Vec4,
		Mat3, Mat4,
		Sampler
	};


	enum ShaderUniformRefreshRate {
		Dynamic, Static
	};

		// Lookup Table : Translation from GLSL type to ShaderUniformType
	static std::map<std::string, ShaderUniformType> uniformTypeTable = {
		{ "int",		ShaderUniformType::Int},
		{ "float",		ShaderUniformType::Float},
		{ "vec2",		ShaderUniformType::Vec2},
		{ "vec3",		ShaderUniformType::Vec3},
		{ "vec4",		ShaderUniformType::Vec4},
		{ "mat3",		ShaderUniformType::Mat3},
		{ "mat4",		ShaderUniformType::Mat4},
		{ "sampler2D",	ShaderUniformType::Sampler}
	};
	
		// Lookup Table : Translation from ShaderUniformType to std::string (debug)
	static std::map<ShaderUniformType, std::string> uniformStringsTable = {
		{ ShaderUniformType::Undefined, "Undefined" },
		{ ShaderUniformType::Float,		"Float" },
		{ ShaderUniformType::Int,		"Int" },
		{ ShaderUniformType::Vec2,		"Vec2" },
		{ ShaderUniformType::Vec3,		"Vec3" },
		{ ShaderUniformType::Vec4,		"Vec4" },
		{ ShaderUniformType::Mat3,		"Mat3" },
		{ ShaderUniformType::Mat4,		"Mat4" },
		{ ShaderUniformType::Sampler,	"Sampler" }
	};


		// Looku Table : Translation from ShaderUniformRefreshRate to std::string (debug)
	static std::map<ShaderUniformRefreshRate, std::string> uniformRefreshTable = {
		{ShaderUniformRefreshRate::Dynamic,	"Dynamic"},
		{ShaderUniformRefreshRate::Static,	"Static"}
	};


	static std::map<ShaderUniformType, ShaderUniformRefreshRate> uniformTypeRefreshTable = {
		{ ShaderUniformType::Undefined, ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Float,		ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Int,		ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Vec2,		ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Vec3,		ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Vec4,		ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Mat3,		ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Mat4,		ShaderUniformRefreshRate::Dynamic },
		{ ShaderUniformType::Sampler,	ShaderUniformRefreshRate::Static }
	};

	static ShaderUniformType ShaderUniformTypeFromGLSLKey(const std::string & uniformKey)
	{
		return uniformTypeTable[uniformKey];
	}


	static std::string ShaderUniformStringFromType(ShaderUniformType type)
	{
		return uniformStringsTable[type];
	}


	static std::string ShaderUniformRefreshStringFromRate(ShaderUniformRefreshRate rate)
	{
		return uniformRefreshTable[rate];
	}


	static ShaderUniformRefreshRate ShaderUniformRefreshRateFromType(ShaderUniformType type)
	{
		return uniformTypeRefreshTable[type];
	}



	struct ShaderUniform {

		ShaderUniformType type = ShaderUniformType::Undefined;
		ShaderUniformRefreshRate rate = ShaderUniformRefreshRate::Dynamic; 
		int length = -1;
		int location;
		bool initialized = false;
	};

}