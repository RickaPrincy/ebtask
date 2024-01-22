#ifndef __EBTASK_KEY__
#define __EBTASK_KEY__

#include <string>
#include <vector>

#include "utils/utils.h"

class Key
{
public:
	int _code{ 1 };
	std::string _normal{}, _alt{}, _altgr{}, _shift{}, _capslock{};
	short _status{ RELEASED };
	Key(){};
};

// Key &get_key(int code);

#endif	// !__EBTASK_KEY__
