#include "FileReader.h"
#include <fstream>
#include <sstream>

FileReader::FileReader(std::filesystem::path path) {
    std::ifstream filehandle;
    filehandle.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    filehandle.open(path);
    std::stringstream instream;
    instream << filehandle.rdbuf();
    filehandle.close();
    content = instream.str();
}

std::string_view FileReader::getContent() {
    return content;
}