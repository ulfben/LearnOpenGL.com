#pragma once
#include <filesystem>
#include <string>
#include <string_view>

class FileReader {
    std::string content;
public:
    FileReader(std::filesystem::path path);
    std::string_view getContent();
};