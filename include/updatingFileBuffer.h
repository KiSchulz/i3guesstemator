/*
 Copyright (c) 09.12.22 15:41 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_UPDATINGFILEBUFFER_H
#define I3GUESSTEMATOR_UPDATINGFILEBUFFER_H

#include <string>
#include <fstream>
#include <streambuf>
#include <filesystem>
#include <utility>

class UpdatingFileBuffer {
    const std::filesystem::path path;
    std::ifstream fileStream;
    std::string content;

public:
    explicit UpdatingFileBuffer(std::filesystem::path  _path) : path(std::move(_path)) {
        updateContent();
    }

    [[nodiscard]] std::string_view getContent() {
        updateContent();
        return std::string_view{content};
    }

private:
    void updateContent() {
        if (fileStream.is_open()) {
            fileStream.clear();
            fileStream.seekg(0, std::ios::beg);
        } else {
            fileStream.open(path);
        }

        if (fileStream.fail()) {
            std::stringstream ss;
            ss << "Failed to read from " << path << "!";
            throw std::runtime_error(ss.str());
        }

        content.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
    }
};

#endif //I3GUESSTEMATOR_UPDATINGFILEBUFFER_H
