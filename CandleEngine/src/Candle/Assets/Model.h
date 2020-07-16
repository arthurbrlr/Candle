#pragma once

#include "Candle/CandleCore.h"

#include "Candle/Renderer/VertexArray.h"

namespace Candle {

	class Model {

		public:
			Model() {}

			void CreateFromObj(const std::string & filePath);


		private:
			Shared<VertexArray> _vaoID;


	};

}