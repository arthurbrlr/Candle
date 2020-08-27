#include "cdlpch.h"
#include "SerializeUtility.h"

#include "Utility/StringUtility.h"

    /* Getters */

void GetSerializedString(std::fstream& file, std::string& value)
{
    std::string line;
    std::getline(file, line);
    value = line.substr(line.find_first_of(':') + 1, line.size());
}

void GetSerializedVector(std::fstream& file, std::vector<std::string>& vector)
{
    std::string line;
    std::getline(file, line);
    std::string valueString = line.substr(line.find_first_of(':') + 1, line.size());
    vector = Candle::StringUtility::Split(valueString, ',');
}

void GetSerializedFloat(std::fstream& file, float& value)
{
    std::string line;
    std::getline(file, line);
    std::string serializedValueString = line.substr(line.find_first_of(':') + 1, line.size());
    value = std::stof(serializedValueString);
}

void GetSerializedFloat2(std::fstream& file, glm::vec2& value)
{
    std::vector<std::string> values;
    GetSerializedVector(file, values);
    value = glm::vec2{ std::stof(values[0]), std::stof(values[1]) };
}

void GetSerializedFloat3(std::fstream& file, glm::vec3& value)
{
    std::vector<std::string> values;
    GetSerializedVector(file, values);
    value = glm::vec3{ std::stof(values[0]), std::stof(values[1]), std::stof(values[2]) };
}

void GetSerializedFloat4(std::fstream& file, glm::vec4& value)
{
    std::vector<std::string> values;
    GetSerializedVector(file, values);
    value = glm::vec4{ std::stof(values[0]), std::stof(values[1]), std::stof(values[2]), std::stof(values[3]) };
}

void GetSerializedInt(std::fstream& file, int& value)
{
    std::string line;
    std::getline(file, line);
    std::string serializedValueString = line.substr(line.find_first_of(':') + 1, line.size());
    value = std::stoi(serializedValueString);
}

void GetSerializedInt2(std::fstream& file, glm::vec2& value)
{
    std::vector<std::string> values;
    GetSerializedVector(file, values);
    value = glm::vec2{ std::stoi(values[0]), std::stoi(values[1]) };
}

void GetSerializedInt3(std::fstream& file, glm::vec3& value)
{
    std::vector<std::string> values;
    GetSerializedVector(file, values);
    value = glm::vec3{ std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]) };
}

void GetSerializedInt4(std::fstream& file, glm::vec4& value)
{
    std::vector<std::string> values;
    GetSerializedVector(file, values);
    value = glm::vec4{ std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]), std::stoi(values[3]) };
}


    /* Writers */

void SerializeString(std::fstream& file, const std::string& valueName, const std::string& value)
{
    file << "\t\t" << valueName << ":" << value << std::endl;
}

void SerializeFloat(std::fstream& file, const std::string& valueName, float value)
{
    file << "\t\t" << valueName << ":" << value << std::endl;
}

void SerializeFloat2(std::fstream& file, const std::string& valueName, glm::vec2& value)
{
    file << "\t\t" << valueName << ":"
         << value.x << ","
         << value.y << std::endl;
}

void SerializeFloat3(std::fstream& file, const std::string& valueName, glm::vec3& value)
{
    file << "\t\t" << valueName << ":"
         << value.x << ","
         << value.y << ","
         << value.z << std::endl;
}

void SerializeFloat4(std::fstream& file, const std::string& valueName, glm::vec4& value)
{
    file << "\t\t" << valueName << ":"
         << value.x << ","
         << value.y << ","
         << value.z << ","
         << value.w << std::endl;
}

void SerializeInt(std::fstream& file, const std::string& valueName, int value)
{
    file << "\t\t" << valueName << ":" << value << std::endl;
}

void SerializeInt2(std::fstream& file, const std::string& valueName, glm::vec2& value)
{
    file << "\t\t" << valueName << ":"
         << value.x << ","
         << value.y << std::endl;
}

void SerializeInt3(std::fstream& file, const std::string& valueName, glm::vec3& value)
{
    file << "\t\t" << valueName << ":"
         << value.x << ","
         << value.y << ","
         << value.z << std::endl;
}

void SerializeInt4(std::fstream& file, const std::string& valueName, glm::vec4& value)
{
    file << "\t\t" << valueName << ":"
         << value.x << ","
         << value.y << ","
         << value.z << ","
         << value.w << std::endl;
}
