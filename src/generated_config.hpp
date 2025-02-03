#pragma once

#define EBTASK_VERSION "0.2.0"
#define EBTASK_PATH_ENV_NAME "EBTASK_PATH"

#include <string>

static const std::string DEFAULT_DEVNODE_PATH = "/dev/input/event3";
static const std::string DEFAULT_CONFIG_FILE_NAME = "ebtask";
static const std::string CONFIG_SUFFIX = ".config.json";
static const std::string LAYOUT_CONFIG_SUFFIX = ".layout.json";
