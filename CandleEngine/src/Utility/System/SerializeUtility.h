#pragma once

#include <string>
#include <fstream>

#include "glm/glm.hpp"

        /* Getters */

void GetSerializedString    (std::fstream& file, std::string&   value);
void GetSerializedFloat     (std::fstream& file, float&         value);
void GetSerializedFloat2    (std::fstream& file, glm::vec2&     value);
void GetSerializedFloat3    (std::fstream& file, glm::vec3&     value);
void GetSerializedFloat4    (std::fstream& file, glm::vec4&     value);
void GetSerializedInt       (std::fstream& file, int&           value);
void GetSerializedInt2      (std::fstream& file, glm::vec2&     value);
void GetSerializedInt3      (std::fstream& file, glm::vec3&     value);
void GetSerializedInt4      (std::fstream& file, glm::vec4&     value);


    /* Writers */

void SerializeString (std::fstream& file, const std::string& valueName, const std::string&  value);
void SerializeFloat  (std::fstream& file, const std::string& valueName, float               value);
void SerializeFloat2 (std::fstream& file, const std::string& valueName, glm::vec2&          value);
void SerializeFloat3 (std::fstream& file, const std::string& valueName, glm::vec3&          value);
void SerializeFloat4 (std::fstream& file, const std::string& valueName, glm::vec4&          value);
void SerializeInt    (std::fstream& file, const std::string& valueName, int                 value);
void SerializeInt2   (std::fstream& file, const std::string& valueName, glm::vec2&          value);
void SerializeInt3   (std::fstream& file, const std::string& valueName, glm::vec3&          value);
void SerializeInt4   (std::fstream& file, const std::string& valueName, glm::vec4&          value);