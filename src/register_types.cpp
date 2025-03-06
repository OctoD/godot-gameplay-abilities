#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "ability_system.hpp"

using namespace godot;

void register_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<ggas::Ability>();
		ClassDB::register_class<ggas::AbilityContainer>();
		ClassDB::register_runtime_class<ggas::RuntimeAbility>();
	}
}

void unregister_gdextension_types(ModuleInitializationLevel p_level)
{
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		// wabungus
	}
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT godot_gameplay_abilities_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
{
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(register_gdextension_types);
	init_obj.register_terminator(unregister_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}