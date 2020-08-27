#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>

namespace Candle {

	class StringUtility {
    public:
        static std::vector<std::string> Split(const std::string& line, char delimiter)
        {
            std::vector<std::string> tokens, returnedToken;
            std::string token;
            std::istringstream tokenStream(line);
            while ( std::getline(tokenStream, token, delimiter) ) {
                tokens.push_back(token);
            }

            for ( int i = 0; i < tokens.size(); i++ ) {
                if ( !tokens[i].empty() ) {
                    returnedToken.push_back(tokens[i]);
                }
            }

            return returnedToken;
        }



	};

}