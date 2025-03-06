#ifndef GODOT_GAMEPLAY_ABILITIES_REGISTER_TYPES_H
#define GODOT_GAMEPLAY_ABILITIES_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void register_gdextension_types(ModuleInitializationLevel p_level);
void unregister_gdextension_types(ModuleInitializationLevel p_level);

#endif // GODOT_GAMEPLAY_ABILITIES_REGISTER_TYPES_H