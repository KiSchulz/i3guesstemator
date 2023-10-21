/*
 Copyright (c) 09.12.22 15:45 Kilian Schulz

 See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_ELEMENTGENERATOR_H
#define I3GUESSTEMATOR_ELEMENTGENERATOR_H

#include "element.h"
#include "yaml-cpp/yaml.h"

class ElementGenerator {
protected:
    const std::string generatorName;
    const YAML::Node generatorConfig;
    std::string prefix;
public:
    ElementGenerator(std::string_view _generatorName, const YAML::Node &_config) : generatorName(_generatorName),
                                                                                   generatorConfig(
                                                                                           _config[_generatorName]) {
        if (!generatorConfig.IsDefined()) {
            return;
        }
        const YAML::Node &prefixNode = generatorConfig["prefix"];
        if (!prefixNode.IsDefined()) {
            prefix = "";
            return;
        }
        prefix = prefixNode.as<std::string>();
    }


    template<typename T>
    static std::unique_ptr<ElementGenerator> createElement(std::string_view name, const YAML::Node &config) {
        return std::make_unique<T>(name, config);
    }

    virtual ~ElementGenerator() = default;

    virtual Element getElement() = 0;
};

#endif //I3GUESSTEMATOR_ELEMENTGENERATOR_H
