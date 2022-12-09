/*
 Copyright (c) 09.12.22 15:41 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_UPDATINGFILEBUFFER_H
#define I3GUESSTEMATOR_UPDATINGFILEBUFFER_H

#include <string>
#include <fstream>
#include <streambuf>

class UpdatingFileBuffer {
    const std::string path;
    std::ifstream fileStream;
    std::string content;

public:
    explicit UpdatingFileBuffer(std::string_view path) : path(path) {
    }

    std::string_view getContent() {
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

        content.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
    }
};

#endif //I3GUESSTEMATOR_UPDATINGFILEBUFFER_H
