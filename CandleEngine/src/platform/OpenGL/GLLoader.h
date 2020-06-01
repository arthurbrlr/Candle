#pragma once

#include "cdlpch.h"
#include "Candle/CandleCore.h"

#include "Models/RawModel.h"

#include <GL/glew.h>

namespace RenderEngine {

	class CANDLE_API Loader {
		private:
			std::vector<unsigned int> _vaos, _vbos;
			std::map<std::string, unsigned int> _textures;

			static Loader* instance;
			Loader() {};

			unsigned int	CreateVAO();
			void			BindVAO(unsigned int vaoID);
			unsigned int	BindIndicesBuffer(const std::vector<unsigned int> & indices, GLenum mode);

			unsigned int	StoreDataInAttributeList(int attributeNumber,
				int coordinateSize,
				GLenum mode,
				const std::vector<float> & data);

			void			ProcessVertexData(std::vector<std::string>    vertex_data,
				std::vector<unsigned int> & indices,
				std::vector<float>        & uvs,
				std::vector<float>        & normals,
				std::vector<glm::vec2>    & readUvs,
				std::vector<glm::vec3>    & readNormals);

			void			UnbindVAO();

		public:
			Loader(Loader const&) = delete;
			void operator=(Loader const&) = delete;
			static Loader& GetInstance() {
				static Loader instance;
				return instance;
			};

			unsigned int				LoadToVAO(const std::vector<float> & vertices);

			unsigned int				LoadToVAO(const std::vector<float> & vertices, const std::vector<float> & uvs);

			Models::RawModel			LoaddToVAO(const std::vector<float>		   & vertices);

			Models::RawModel			LoadToVAO(const std::vector<float>          & vertices,
				const std::vector<float>          & uvs,
				const std::vector<float>          & normals,
				const std::vector<unsigned int>   & indices);

			std::vector<unsigned int>	LoadToBuffers(const std::vector<unsigned int>   & indices,
													  const std::vector<float> & vertices,
													  const std::vector<float> & textureCoord,
													  const std::vector<float> & normals,
													  const std::vector<float> & colors,
													  GLenum indicesMode,
													  GLenum verticesMode);

			void						ChangeIndicesBuffer(const std::vector<unsigned int> & newIndices,
															unsigned int vaoToChange,
															unsigned int vboToChange);

			void						ChangeDataInAttributeList(const std::vector<float> & data, 
																  int						 attributeNumber, 
																  int						 coordinateSize,
																  unsigned int				 vaoToChange,
																  unsigned int				 vboToChange);

			void						BindVertexAttributes();
			void						UnbindVertexAttributes();

			unsigned int				LoadTexture(const std::string & path, bool isTransparent);
			Models::RawModel			LoadObjModel(std::string filePath);

			void						CleanUp();
	};
}