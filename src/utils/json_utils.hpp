#pragma once

// JUST TO TRY MACRO A LIT BIT
#define AFFECT_DATA_TO_JSON(json, obj, field) ((json)[#field] = (obj).field)
#define READ_KEYBIND(json, obj, field) ((obj).field = (json)[#field].get<ebtask::config::KeyBinding>())
#define READ_JSON_DATA(json, obj, field) ((obj).field = (json)[#field])

#define READ_OPTIONAL_JSON_DATA(json, obj, field) \
	if ((json).contains(#field))                  \
	READ_JSON_DATA(json, obj, field)
