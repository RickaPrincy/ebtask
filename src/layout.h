#ifndef __EBTASK_KEY__
#define __EBTASK_KEY__

#include <string>
#include <tuple>
#include <vector>

#include "json/json.hpp"
#include "utils/utils.h"

class Key
{
public:
	int _code{ 1 };
	std::string _normal{}, _alt{}, _altgr{}, _shift{}, _capslock{};
	KeyStatus _status{ KeyStatus::RELEASED };
	Key(){};
};

// Key &get_key(int code);
void save_key(const Key& key);
void load_keys();
std::tuple<std::string, nlohmann::json> get_key_config();

#endif	// !__EBTASK_KEY__
