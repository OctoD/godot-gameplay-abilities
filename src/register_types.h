/**************************************************************************/
/*  register_types.h                                                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        Godot Gameplay Systems                          */
/*              https://github.com/OctoD/godot-gameplay-systems           */
/**************************************************************************/
/* Read the license file in this repo.						              */
/**************************************************************************/

#ifndef OCTOD_GAMEPLAY_ABILITIES_REGISTER_TYPES_H
#define OCTOD_GAMEPLAY_ABILITIES_REGISTER_TYPES_H

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

namespace octod::gameplay::abilities
{
	/// @brief Registers the types.
	void register_types(ModuleInitializationLevel p_level);

	/// @brief Unregisters the types.
	void unregister_types(ModuleInitializationLevel p_level);
}

#endif // OCTOD_GAMEPLAY_ABILITIES_REGISTER_TYPES_H
