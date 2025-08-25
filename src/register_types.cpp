/**************************************************************************/
/*  register_types.cpp                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        Godot Gameplay Systems                          */
/*              https://github.com/OctoD/godot-gameplay-systems           */
/**************************************************************************/
/* Read the license file in this repo.						              */
/**************************************************************************/

#include "register_types.h"
#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>

#include "ability_system.hpp"

using namespace godot;

namespace octod::gameplay::abilities
{
	void register_types(ModuleInitializationLevel p_level)
	{
		if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE)
		{
			ClassDB::register_class<Ability>();
			ClassDB::register_class<AbilityContainer>();
			ClassDB::register_runtime_class<RuntimeAbility>();
		}
	}

	void unregister_types(ModuleInitializationLevel p_level)
	{
	}
}


extern "C"{
// Initialization
GDExtensionBool GDE_EXPORT godot_gameplay_abilities_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
{
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(octod::gameplay::abilities::register_types);
	init_obj.register_terminator(octod::gameplay::abilities::unregister_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
