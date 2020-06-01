#include "cdlpch.h"
#include "GLLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Util/stb_image.h"


namespace RenderEngine {

	unsigned int Loader::CreateVAO()
	{
		unsigned int vao; glGenVertexArrays(1, &vao);
		_vaos.push_back(vao);
		BindVAO(vao);
		return vao;
	}

	void Loader::BindVAO(unsigned int vaoID)
	{
		glBindVertexArray(vaoID);
	}

	unsigned int Loader::BindIndicesBuffer(const std::vector<unsigned int> & indices, GLenum mode)
	{
		unsigned int vbo; glGenVertexArrays(1, &vbo);
		_vbos.push_back(vbo);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		if (indices.size() == 0) std::cout << "Error : indices is size " << indices.size() << std::endl;
		else glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], mode);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return vbo;
	}

	unsigned int Loader::StoreDataInAttributeList(int                        attributeNumber,
		int                        coordinateSize,
		GLenum                     mode,
		const std::vector<float> & data)
	{
		unsigned int vbo; glGenVertexArrays(1, &vbo);
		_vbos.push_back(vbo);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		if (data.size() == 0) std::cout << "Error : data is size " << data.size() << std::endl;
		else glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], mode);
		glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, coordinateSize * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vbo;
	}

	void Loader::UnbindVAO()
	{
		glBindVertexArray(0);
	}

	unsigned int Loader::LoadToVAO(const std::vector<float> & vertices)
	{
		unsigned int vao = CreateVAO();
		StoreDataInAttributeList(0, 2, GL_STATIC_DRAW, vertices);
		UnbindVAO();
		return vao;
	}


	unsigned int Loader::LoadToVAO(const std::vector<float> & vertices, const std::vector<float> & uvs)
	{
		unsigned int vao = CreateVAO();
		StoreDataInAttributeList(0, 2, GL_STATIC_DRAW, vertices);
		StoreDataInAttributeList(1, 2, GL_STATIC_DRAW, uvs);
		UnbindVAO();
		return vao;
	}


	Models::RawModel Loader::LoaddToVAO(const std::vector<float> & vertices)
	{
		unsigned int vao = CreateVAO();
		StoreDataInAttributeList(0, 3, GL_STATIC_DRAW, vertices);
		UnbindVAO();
		return Models::RawModel(vao, vertices.size());
	}


	Models::RawModel Loader::LoadToVAO(const std::vector<float>          & vertices,
		const std::vector<float>          & uvs,
		const std::vector<float>          & normals,
		const std::vector<unsigned int>   & indices)
	{
		unsigned int vaoID = CreateVAO();
		unsigned int vboID = BindIndicesBuffer(indices, GL_STATIC_DRAW);
		StoreDataInAttributeList(0, 3, GL_STATIC_DRAW, vertices);
		StoreDataInAttributeList(1, 2, GL_STATIC_DRAW, uvs);
		StoreDataInAttributeList(2, 3, GL_STATIC_DRAW, normals);
		UnbindVAO();
		return Models::RawModel(vaoID, vboID, (long)indices.size());
	}

	std::vector<unsigned int> Loader::LoadToBuffers(const std::vector<unsigned int> & indices,
		const std::vector<float>        & vertices,
		const std::vector<float>        & textureCoord,
		const std::vector<float>        & normals,
		const std::vector<float>        & colors,
		GLenum                            indicesMode,
		GLenum                            verticesMode)
	{
		std::vector<unsigned int> buffersIndices;
		buffersIndices.resize(6);
		buffersIndices[0] = CreateVAO();
		buffersIndices[1] = BindIndicesBuffer(indices, indicesMode);
		buffersIndices[2] = StoreDataInAttributeList(0, 3, verticesMode, vertices);
		buffersIndices[3] = StoreDataInAttributeList(1, 2, GL_STATIC_DRAW, textureCoord);
		buffersIndices[4] = StoreDataInAttributeList(2, 3, GL_STATIC_DRAW, normals);
		buffersIndices[5] = StoreDataInAttributeList(3, 4, GL_STATIC_DRAW, colors);
		UnbindVAO();
		return buffersIndices;
	}

	void Loader::ChangeIndicesBuffer(const std::vector<unsigned int> & newIndices,
		unsigned int vaoToChange,
		unsigned int vboToChange)
	{
		BindVAO(vaoToChange);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboToChange);
		if (newIndices.size() == 0) std::cout << "Error : data is size " << newIndices.size() << std::endl;
		else glBufferData(GL_ELEMENT_ARRAY_BUFFER, newIndices.size() * sizeof(unsigned int), &newIndices[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		UnbindVAO();
	}

	void Loader::ChangeDataInAttributeList(const std::vector<float> & data,
		int attributeNumber,
		int coordinateSize,
		unsigned int vaoToChange,
		unsigned int vboToChange)
	{
		BindVAO(vaoToChange);
		glBindBuffer(GL_ARRAY_BUFFER, vboToChange);
		if (data.size() == 0) std::cout << "Error : data is size " << data.size() << std::endl;
		else glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, coordinateSize * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		UnbindVAO();
	}

	void Loader::BindVertexAttributes()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	}

	void Loader::UnbindVertexAttributes()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glBindVertexArray(0);
	}


	unsigned int Loader::LoadTexture(const std::string & path, bool isTransparent)
	{

		if (_textures[path]) return _textures[path];

		/* Texture binding */
		unsigned int texture;
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		/* Texture loading */
		int nrChannels, width, height;
		stbi_set_flip_vertically_on_load(false);
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (data) {
			if (isTransparent) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else {
			std::cout << "ERROR::TEXTURE::LOADING: Couldn't load " << path << std::endl;
		}
		stbi_image_free(data);

		_textures[path] = texture;
		return texture;
	}

	Models::RawModel Loader::LoadObjModel(std::string filePath) {

		bool isSizeSet = false;

		/* Loading the obj file and exception handling */
		std::ifstream modelFile;
		modelFile.open(filePath);
		if (!modelFile.is_open()) {
			std::cout << "ERROR: ObjLoader couldn't open " << filePath << " model." << std::endl;
			return Models::RawModel();
		}

		/* Setting up the arrays used when file reading */
		std::vector<glm::vec2> readUvs;
		std::vector<glm::vec3> readNormals;
		std::vector<glm::vec3> readPositions;

		/* Setting up the returned arrays */
		std::vector<float> positions, normals, uvs;
		std::vector<unsigned int> indices;

		/* File Reading */
		std::string line;

		printf("\nReading %s", filePath.c_str());

		while (!modelFile.eof()) {
			std::getline(modelFile, line);
			std::vector<std::string> lineSplited = Util::Split(line, ' ');

			if (lineSplited.size() > 0) {
				if (lineSplited[0] == "v") {
					/* Read the raw positions */
					readPositions.push_back(glm::vec3(std::stof(lineSplited[1]), std::stof(lineSplited[2]), std::stof(lineSplited[3])));
				}
				else if (lineSplited[0] == "vt") {
					/* Read the raw uvs */
					readUvs.push_back(glm::vec2(std::stof(lineSplited[1]), std::stof(lineSplited[2])));
				}
				else if (lineSplited[0] == "vn") {
					/* Read the raw normals */
					readNormals.push_back(glm::vec3(std::stof(lineSplited[1]), std::stof(lineSplited[2]), std::stof(lineSplited[3])));
				}
				else if (lineSplited[0] == "f") {

					/* Set size for returned vectors */
					if (!isSizeSet) {
						long size = readPositions.size();
						uvs.resize(size * 2);
						normals.resize(size * 3);
						isSizeSet = true;
					}

					/*
						Read all the vertices on the line "f 77/2/4 89/5/3 109/67/32 "
						Meaning the first vertex of the face has position 77, uv 2 and normal 4
						And so on
					*/

					for (int i = 1; i < 4; i++) {
						std::vector<std::string> vertex_data = Util::Split(lineSplited[i], '/');
						ProcessVertexData(vertex_data, indices, uvs, normals, readUvs, readNormals);
					}
				}
			}
		}

		modelFile.close();

		/* SZet up the positions vector */
		for (int i = 0; i < readPositions.size(); i++) {
			positions.push_back(readPositions[i].x);
			positions.push_back(readPositions[i].y);
			positions.push_back(readPositions[i].z);
		}

		printf("\nObjLoader: Number Of Vertices : %d", readPositions.size());
		printf("\nObjLoader: Position size : %d", positions.size());
		printf("\nObjLoader: UV size : %d", uvs.size());
		printf("\nObjLoader: Normal size : %d", normals.size());
		printf("\nObjLoader: Indices size : %d", indices.size());
		printf("\n");

		return CDL_LOADER.LoadToVAO(positions, uvs, normals, indices);
	}


	void Loader::ProcessVertexData(std::vector<std::string>    vertex_data,
		std::vector<unsigned int> & indices,
		std::vector<float>        & uvs,
		std::vector<float>        & normals,
		std::vector<glm::vec2>    & readUvs,
		std::vector<glm::vec3>    & readNormals) {

		/* Processing indices data */
		int currentVertexIndex = std::stoi(vertex_data[0]) - 1;
		indices.push_back(currentVertexIndex);

		/* Processing uv data */
		glm::vec2 currentUv = readUvs.at(std::stoi(vertex_data[1]) - 1);
		uvs[currentVertexIndex * 2] = currentUv.x;
		uvs[currentVertexIndex * 2 + 1] = 1. - currentUv.y;

		/* Processing normals data */
		glm::vec3 currentNormal = readNormals.at(std::stoi(vertex_data[2]) - 1);
		normals[currentVertexIndex * 3] = currentNormal.x;
		normals[currentVertexIndex * 3 + 1] = currentNormal.y;
		normals[currentVertexIndex * 3 + 2] = currentNormal.z;
	}


	void Loader::CleanUp()
	{
		for (unsigned int i : _vaos) {
			glDeleteVertexArrays(1, &i);
		}

		for (unsigned int i : _vbos) {
			glDeleteBuffers(1, &i);
		}
	}
}
