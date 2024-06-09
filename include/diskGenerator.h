/*
 * Copyright (c) 6/8/24 9:04 PM Kilian Schulz
 *
 * See file LICENSE for information
 */

#ifndef I3GUESSTEMATOR_DISKINFO_H
#define I3GUESSTEMATOR_DISKINFO_H

#include <cstring>
#include <tuple>

#include "elementGenerator.h"
#include "sys/statvfs.h"
#include "yaml-cpp/yaml.h"

class DiskGenerator : public ElementGenerator {
  std::vector<std::tuple<std::string, struct statvfs>> subElVec;
  bool highDiskUsage;

  std::vector<std::tuple<std::string, struct statvfs>> initPath() {
    if (generatorConfig.IsDefined()) {
      const YAML::Node &pathsNode = generatorConfig["paths"];
      if (pathsNode.IsDefined()) {
        std::vector<std::tuple<std::string, struct statvfs>> ret;
        for (const auto &path : pathsNode.as<std::vector<std::string>>()) {
          ret.push_back({path, {}});
        }
        return ret;
      }
    }
    return {{"/", {}}};
  }

  void updateSubElements() {
    for (auto &[path, stats] : subElVec) {
      if (statvfs(path.c_str(), &stats)) {
        throw std::runtime_error("Failed to call statvfs() for path:" + path + "errno(" + std::to_string(errno) +
                                 "): " + strerror(errno));
      }
    }
  }

  void printSubElement(std::stringstream &ss, const std::tuple<std::string, struct statvfs> &se) {
    const auto &[path, stats] = se;

    auto total = (double)(stats.f_bsize * stats.f_blocks);
    auto free = (double)(stats.f_bsize * stats.f_bavail);
    double usage = ((total - free) / total) * 100.f;

    ss << prefix;
    ss << path << std::format(": {:05.2f}% (free: {:03.0f}GiB)", usage, free / (1 << 30));

    highDiskUsage |= usage >= 80;
  }

public:
  explicit DiskGenerator(std::string_view name, const YAML::Node &config)
      : ElementGenerator(name, config), subElVec(initPath()), highDiskUsage(false) {}

  Element getElement() override {
    highDiskUsage = false;

    updateSubElements();

    std::stringstream ss;

    printSubElement(ss, subElVec[0]);

    for (auto iter = ++subElVec.begin(); iter != subElVec.end(); iter++) {
      // deduplicate filesystems with the same fs-id, useful if a filesystem might not be mounted all the time
      if (std::find_if(subElVec.begin(), iter,
                       [iter](auto a) { return std::get<1>(a).f_fsid == std::get<1>(*iter).f_fsid; }) != iter) {
        continue;
      }
      ss << " ";
      printSubElement(ss, *iter);
    }

    return Element{ss.str(), (highDiskUsage ? -1 : 0)};
  }
};

#endif // I3GUESSTEMATOR_DISKINFO_H
