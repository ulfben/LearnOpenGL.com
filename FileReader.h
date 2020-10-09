#pragma once
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string_view>

class FileReader {
    std::string content;
public:
    FileReader(std::filesystem::path path) {
        std::ifstream filehandle;
        filehandle.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        filehandle.open(path);
        std::stringstream instream;
        instream << filehandle.rdbuf();
        filehandle.close();
        content = instream.str();
    }

    std::string_view getContent() {
        return content;
    }
};