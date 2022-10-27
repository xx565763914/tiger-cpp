#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

class YamlConfigBase {
private:
  std::string _file;
  YAML::Node _yaml_conf;

public:
  YamlConfigBase(std::string file)
      : _file(file), _yaml_conf(YAML::LoadFile(_file)){};

  YAML::Node &get_config() { return _yaml_conf; };
};