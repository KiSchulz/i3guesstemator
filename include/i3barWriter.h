#ifndef I3GUESSTEMATOR_I3BARWRITER_H
#define I3GUESSTEMATOR_I3BARWRITER_H

#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

#include "writer.h"
#include "elementGenerator.h"
#include "element.h"

class I3barWriter : public Writer {
public:
    I3barWriter() {
        //init the connection to i3Bar
        std::cout << "{ \"version\": 1 }\n[" << std::endl;
    }

    //update all elements and output them to i3bar
    void update() override {
        //start the update
        std::cout << "[\n";

        //print every element except the last one because of formatting
        for (std::size_t i = 0; i < elementGenerators.size() - 1; i++) {
            std::cout << elementToString(elementGenerators[i]->getElement()) << ",\n";
        }
        //print the last element without a colon at the end
        std::cout << elementToString(elementGenerators.back()->getElement()) << "\n";

        //end of the update
        std::cout << "]," << std::endl;
    }

    void pushBack(const std::shared_ptr<ElementGenerator> &elementGenerator) override {
        elementGenerators.push_back(elementGenerator);
    }

private:
    static std::string elementToString(const Element &element) {
        std::stringstream ss;
        ss << "{\n";

        ss << R"("full_text": ")" << element.text << "\"\n";
        //TODO add all the other options

        ss << "}";
        return ss.str();
    }
};

#endif //I3GUESSTEMATOR_I3BARWRITER_H
