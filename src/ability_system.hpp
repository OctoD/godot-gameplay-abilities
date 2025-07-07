/**************************************************************************/
/*  ability_system.hpp                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                        Godot Gameplay Systems                          */
/*              https://github.com/OctoD/godot-gameplay-systems           */
/**************************************************************************/
/* Read the license file in this repo.						              */
/**************************************************************************/

#ifndef OCTOD_GAMEPLAY_ABILITIES_ABILITY_SYSTEM_HPP
#define OCTOD_GAMEPLAY_ABILITIES_ABILITY_SYSTEM_HPP

#include "ability_system.hpp"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

namespace octod::gameplay::abilities
{
	class Ability;
	class AbilityContainer;

#pragma region RuntimeAbility

	/// @brief Let's have fun with bitmasks to handle the ability internal state.
	enum AbilityState : uint8_t
	{
		/// @brief Represents the ability being in no state.
		ABILITY_STATE_IDLE = 1 << 0,
		/// @brief Represents the ability being cooling down.
		ABILITY_STATE_COOLING_DOWN = 1 << 1,
		/// @brief Represents the ability being active.
		ABILITY_STATE_ACTIVE = 1 << 2,
		/// @brief Represents the ability being blocked.
		ABILITY_STATE_BLOCKED = 1 << 4,
		/// @brief Represents the ability being granted.
		ABILITY_STATE_GRANTED = 1 << 6,
	};

	enum AbilityEventType : uint8_t
	{
		/// success events
		/// @brief Represents the ability being activated.
		ABILITY_EVENT_TYPE_ACTIVATED,
		ABILITY_EVENT_TYPE_ACTIVATED_COOLDOWN_STARTED,
		ABILITY_EVENT_TYPE_ACTIVATED_DURATION_STARTED,
		/// @brief Represents the ability being blocked.
		ABILITY_EVENT_TYPE_BLOCKED,
		/// @brief Represents the ability being ended.
		ABILITY_EVENT_TYPE_ENDED,
		/// @brief Represents the ability being granted.
		ABILITY_EVENT_TYPE_GRANTED,
		/// @brief Represents the ability being revoked.
		ABILITY_EVENT_TYPE_REVOKED,
		/// @brief Represents the ability being unblocked.
		ABILITY_EVENT_TYPE_UNBLOCKED,

		/// refusal events
		/// @brief Represents the ability being refused to activate.
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE,
		/// @brief Represents the ability being refused to activate because the ability itself decided to block itself.
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_DECIDED_TO_BLOCK,
		/// @brief Represents the ability being refused to activate because it is blocked.
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_BLOCKED,
		/// @brief Represents the ability being refused to activate because it is cooling down.
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_COOLING_DOWN,
		/// @brief Represents the ability being refused to activate because the duration is active.
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_DURATION_ACTIVE,
		/// @brief Represents the ability being refused to activate because it is revoked.
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_REVOKED,

		/// @brief Represents the ability being refused to block.
		ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK,
		/// @brief Represents the ability being refused to block because it is not granted.
		ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK_IS_NOT_GRANTED,

		/// @brief Represents the ability being refused to end.
		ABILITY_EVENT_TYPE_REFUSED_TO_END,
		/// @brief Represents the ability being refused to end because it is blocked.
		ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_BLOCKED,
		/// @brief Represents the ability being refused to end because it is not active.
		ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_NOT_ACTIVE,
		/// @brief Represents the ability being refused to end because it is cooling down.
		ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_COOLING_DOWN,
		/// @brief Represents the ability being refused to end because it is not granted.
		ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_NOT_GRANTED,

		/// @brief Represents the ability
		/// being refused
		/// to be granted.
		ABILITY_EVENT_TYPE_REFUSED_TO_GRANT,
		/// @brief Represents the ability being refused to grant because it is already granted.
		ABILITY_EVENT_TYPE_REFUSED_TO_GRANT_ALREADY_GRANTED,

		/// @brief Represents the ability being refused to revoke.
		ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE,
		/// @brief Represents the ability being refused to revoke because it is already revoked.
		ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE_ALREADY_REVOKED,

		/// @brief Represents the ability being refused to unblock.
		ABILITY_EVENT_TYPE_REFUSED_TO_UNBLOCK,
		/// @brief Represents the ability being refused to unblock because it is not blocked.
		ABILITY_EVENT_TYPE_REFUSED_TO_UNBLOCK_IS_NOT_BLOCKED,
		/// @brief Represents the ability being refused to unblock because it should be blocked.
		ABILITY_EVENT_TYPE_REFUSED_TO_UNBLOCK_SHOULD_BE_BLOCKED,

		/// error events
		/// @brief Represents an error activating the ability.
		ABILITY_EVENT_TYPE_ERROR_ACTIVATING,
		/// @brief Represents an error blocking the ability.
		ABILITY_EVENT_TYPE_ERROR_BLOCKING,
		/// @brief Represents an error ending the ability.
		ABILITY_EVENT_TYPE_ERROR_ENDING,
		/// @brief Represents an error granting the ability.
		ABILITY_EVENT_TYPE_ERROR_GRANTING,
		/// @brief Represents an error revoking the ability.
		ABILITY_EVENT_TYPE_ERROR_REVOKING,
		/// @brief Represents an error unblocking the ability.
		ABILITY_EVENT_TYPE_ERROR_UNBLOCKING,

		/// more bad errors, these are related to the container itself.
		/// @brief Returned when the ability container did not find the ability.
		ABILITY_NOT_FOUND,
		/// @brief Returned when the ability parameter is null on some method.
		ABILITY_PARAMETER_IS_NULL
	};

	/// @brief Represents an ability that at runtime.
	//reSharper disable once CppClassCanBeFinal
	class RuntimeAbility : public RefCounted
	{
		GDCLASS(RuntimeAbility, RefCounted);
		/// @brief Marks the ability state.
		/// Using a bitmask
		/// to store the state.
		uint8_t state = ABILITY_STATE_IDLE;
		/// @brief The ability resource.
		Ref<Ability> ability;
		/// @brief The ability container which contains, owns and runs the ability.
		AbilityContainer *container;
		/// @brief Some arbitrary data stored on this ability.
		Variant data;
		/// @brief The last tick time. Used by duration.
		double duration_time = 0.0;
		/// @brief The last tick time. Used by cooldown.
		double cooldown_time = 0.0;

	public:
		/// @brief Activates the ability.
		AbilityEventType activate();

		/// @brief Blocks the ability.
		AbilityEventType block();

		/// @brief Ends the ability.
		AbilityEventType end();

		/// @brief Gets the ability.
		/// @return The ability.
		[[nodiscard]] Ref<Ability> get_ability() const;

		/// @brief Gets the ability container.
		/// @return The ability container.
		[[nodiscard]] AbilityContainer *get_container() const;

		/// @brief Gets the ability cooldown.
		/// @return The ability cooldown.
		[[nodiscard]] double get_cooldown() const;

		/// @brief Gets the ability's data.
		/// @return The ability's data
		[[nodiscard]] Variant get_data() const;

		/// @brief Gets the ability duration.
		/// @return The ability duration.
		[[nodiscard]] double get_duration() const;

		/// @brief Grants the ability.
		AbilityEventType grant();

		/// @brief Returns true if the ability is active.
		/// @return True if the ability is active, false otherwise.
		[[nodiscard]] bool is_active() const;

		/// @brief Returns true if the ability is blocked.
		/// @return True if the ability is blocked, false otherwise.
		[[nodiscard]] bool is_blocked() const;

		/// @brief Returns true if the ability is cooling down.
		/// @return True if the ability is cooling down, false otherwise.
		[[nodiscard]] bool is_cooldown_active() const;

		/// @brief Returns true if the ability is duration active.
		/// @return True if the ability is duration active, false otherwise.
		[[nodiscard]] bool is_duration_active() const;

		/// @brief Returns true if the ability is ended.
		/// @return True if the ability is ended, false otherwise.
		[[nodiscard]] bool is_ended() const;

		/// @brief Returns true if the ability is granted.
		/// @return True if the ability is granted, false otherwise.
		[[nodiscard]] bool is_granted() const;

		/// @brief Returns true if the ability is revoked.
		/// @return True if the ability is revoked, false otherwise.
		[[nodiscard]] bool is_revoked() const;

		/// @brief Revokes the ability.
		AbilityEventType revoke();

		/// @brief Sets the ability.
		/// @param p_ability The ability.
		void set_ability(const Ref<Ability> &p_ability);

		/// @brief Sets the ability container.
		/// @param p_container The ability container.
		void set_container(AbilityContainer *p_container);

		/// @brief Sets the ability's data.
		/// @param p_data the ability's data.
		void set_data(const Variant &p_data);

		/// @brief Returns true if the ability should be activated.
		/// @return True if the ability should be activated, false otherwise.
		[[nodiscard]] bool should_be_activated() const;

		/// @brief Returns true if the ability should be blocked.
		/// @return True if the ability should be blocked, false otherwise.
		[[nodiscard]] bool should_be_blocked() const;

		/// @brief Returns true if the ability should be ended.
		/// @return True if the ability should be ended, false otherwise.
		[[nodiscard]] bool should_be_ended() const;

		/// @brief Unblocks the ability.
		[[nodiscard]] AbilityEventType unblock();

	protected:
		friend class AbilityContainer;

		/// @brief Binds the methods to godot.
		static void _bind_methods();

		/// @brief Handles the tick.
		/// @param p_delta The delta time.
		void handle_tick(double p_delta);

		/// @brief Triggers the ability cooldown.
		bool trigger_cooldown();

		/// @brief Triggers the ability duration.
		bool trigger_duration();

		/// @brief Tries to reset the ability cooldown.
		void try_reset_cooldown();

		/// @brief Tries to reset the ability duration.
		void try_reset_duration();
	};

#pragma endregion
#pragma region AbilityContainer

	/// @brief Represents a container of abilities.
	// ReSharper disable once CppClassCanBeFinal
	class AbilityContainer : public Node
	{
		GDCLASS(AbilityContainer, Node)
		/// @brief The runtime abilities.
		Dictionary runtime_abilities = Dictionary();
		TypedArray<Ability> initial_abilities = TypedArray<Ability>();

	protected:
		/// @brief Binds the methods to godot.
		static void _bind_methods();

		[[nodiscard]] Ref<RuntimeAbility> build_runtime_ability(const Ref<Ability> &p_ability) const;

		/// @brief Called when the node receives a notification.
		// ReSharper disable once CppHidingFunction
		void _notification(int p_what);

		/// @brief Handles the active ability signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_active_ability(const Ref<RuntimeAbility> &p_runtime_ability);

		/// @brief Handles the blocked ability signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_blocked_ability(const Ref<RuntimeAbility> &p_runtime_ability);

		/// @brief Handles the ended ability signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_ended_ability(const Ref<RuntimeAbility> &p_runtime_ability);

		/// @brief Handles the granted ability signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_granted_ability(const Ref<RuntimeAbility> &p_runtime_ability);

		/// @brief Handles the revoked ability signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_revoked_ability(const Ref<RuntimeAbility> &p_runtime_ability);

		/// @brief Handles the cooldown end signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_cooldown_end(const Ref<RuntimeAbility> &p_runtime_ability);

		/// @brief Handles the cooldown start signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_cooldown_start(const Ref<RuntimeAbility> &p_runtime_ability);

		/// @brief Handles the ability unblock signal.
		/// @param p_runtime_ability The runtime ability.
		void _on_unblocked_ability(const Ref<RuntimeAbility> &p_runtime_ability);

	public:
		/// @brief Returns an instance of a RuntimeAbility. Override this method if you want to use an extended class of RuntimeAbility.
		GDVIRTUAL1RC(Ref<RuntimeAbility>, _build_runtime_ability, Ref<Resource>); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method which handles the _physics_process
		GDVIRTUAL1(_physics_process, double); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method called to activate an ability.
		GDVIRTUAL1RC(int, _try_activate, Variant); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method called to block an ability.
		GDVIRTUAL1RC(int, _try_block, Variant); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method called to end an ability.
		GDVIRTUAL1RC(int, _try_end, Variant); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method called to grant an ability.
		GDVIRTUAL1RC(int, _try_grant, Variant); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method called to revoke an ability.
		GDVIRTUAL1RC(int, _try_revoke, Variant); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method called to try to unblock an ability.
		GDVIRTUAL1RC(int, _try_unblock, Variant); // NOLINT(*-unnecessary-value-param)

		/// @brief Adds an ability to the container.
		/// @param p_ability The ability to add.
		/// @return True if the ability was added, false otherwise.
		bool add_ability(const Ref<Ability> &p_ability);

		/// @brief Finds an ability in the container by calling a predicate.
		/// @param p_predicate The predicate to find the ability.
		/// @return The ability
		[[nodiscard]] Ref<RuntimeAbility> find_ability(const Callable &p_predicate) const;

		/// @brief Gets the runtime abilities.
		/// @return The runtime abilities.
		[[nodiscard]] TypedArray<RuntimeAbility> get_runtime_abilities() const;

		/// @brief Gets the initial abilities.
		/// @return The initial abilities.
		[[nodiscard]] TypedArray<Ability> get_initial_abilities() const;

		/// @brief Gets an ability.
		/// @param p_variant The ability to get.
		/// @return The ability.
		[[nodiscard]] Ref<RuntimeAbility> get_runtime_ability(const Variant &p_variant) const;

		/// @brief Checks if the container has an ability.
		/// @param p_variant The ability to check.
		/// @return True if the container has the ability, false otherwise.
		[[nodiscard]] bool has_ability(const Variant &p_variant) const;

		/// @brief Checks if the ability is active.
		/// @param p_variant The ability to check.
		/// @return True if the ability is active, false otherwise.
		[[nodiscard]] bool is_ability_active(const Variant &p_variant) const;

		/// @brief Checks if the ability is blocked.
		/// @param p_variant The ability to check.
		/// @return True if the ability is blocked, false otherwise.
		[[nodiscard]] bool is_ability_blocked(const Variant &p_variant) const;

		/// @brief Checks if the ability cooldown is active.
		/// @param p_variant The ability to check.
		/// @return True if the ability cooldown is active, false otherwise.
		[[nodiscard]] bool is_ability_cooldown_active(const Variant &p_variant) const;

		/// @brief Checks if the ability is ended.
		/// @param p_variant The ability to check.
		/// @return True if the ability is ended, false otherwise.
		[[nodiscard]] bool is_ability_ended(const Variant &p_variant) const;

		/// @brief Checks if the ability is granted.
		/// @param p_variant The ability to check.
		/// @return True if the ability is granted, false otherwise.
		[[nodiscard]] bool is_ability_granted(const Variant &p_variant) const;

		/// @brief Removes an ability from the container.
		/// @param p_ability The ability to remove.
		/// @return True if the ability was removed, false otherwise.
		bool remove_ability(const Ref<Ability> &p_ability);

		/// @brief Sets the abilities in the container.
		/// @param p_abilities The abilities to set.
		void set_initial_abilities(const TypedArray<Ability> &p_abilities);

		/// @brief Activates an ability in the container.
		/// @param p_ability_or_ability_name The ability to activate.
		/// @return The resulting AbilityEventType.
		[[nodiscard]] AbilityEventType try_activate(const Variant &p_ability_or_ability_name) const;

		/// @brief Blocks an ability in the container.
		/// @param p_ability_or_ability_name The ability to block.
		/// @return The resulting AbilityEventType.
		[[nodiscard]] AbilityEventType try_block(const Variant &p_ability_or_ability_name) const;

		/// @brief Ends an ability in the container.
		/// @param p_ability_or_ability_name The ability to end.
		/// @return The resulting AbilityEventType.
		[[nodiscard]] AbilityEventType try_end(const Variant &p_ability_or_ability_name) const;

		/// @brief Grants an ability to the container.
		/// @param p_ability_or_ability_name The ability to grant.
		[[nodiscard]] AbilityEventType try_grant(const Variant &p_ability_or_ability_name) const;

		/// @brief Revokes an ability from the container.
		/// @param p_ability_or_ability_name The ability to revoke.
		/// @return The resulting AbilityEventType.
		[[nodiscard]] AbilityEventType try_revoke(const Variant &p_ability_or_ability_name) const;

		/// @brief Unblocks an ability in the container.
		/// @param p_ability_or_ability_name The ability to unblock.
		/// @return The resulting AbilityEventType.
		[[nodiscard]] AbilityEventType try_unblock(const Variant &p_ability_or_ability_name) const;
	};

#pragma endregion

#pragma region Ability

	// ReSharper disable once CppClassCanBeFinal
	class Ability : public Resource
	{
		GDCLASS(Ability, Resource)
		/// @brief The ability name.
		StringName ability_name;

	public:
		/// @brief Called when the ability container checks if the ability cooldown can be activated.
		GDVIRTUAL2RC(bool, _can_activate_cooldown, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be activated.
		GDVIRTUAL2RC(bool, _can_be_activated, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be blocked.
		GDVIRTUAL2RC(bool, _can_be_blocked, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks
		///		   if the ability can be canceled.
		GDVIRTUAL2RC(bool, _can_be_granted, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be ended.
		GDVIRTUAL2RC(bool, _can_be_revoked, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be granted.
		GDVIRTUAL2RC(bool, _can_be_ended, AbilityContainer *, RuntimeAbility *);
		/// @brief Gets the ability cooldown.
		GDVIRTUAL1RC(double, _get_cooldown, AbilityContainer *);
		/// @brief Gets the ability duration.
		GDVIRTUAL1RC(double, _get_duration, AbilityContainer *);
		/// @brief Gets the ability's initial data.
		GDVIRTUAL1RC(Variant, _get_initial_data, AbilityContainer *);
		/// @brief Called when the ability is activated.
		/// @param The ability container.
		GDVIRTUAL2(_on_activate, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability is blocked.
		/// @param The ability container.
		GDVIRTUAL2(_on_block, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability is ended.
		/// @param The ability container.
		GDVIRTUAL2(_on_end, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability is granted.
		/// @param The ability container.
		GDVIRTUAL2(_on_grant, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability is revoked.
		/// @param The ability container.
		GDVIRTUAL2(_on_revoke, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability is ticked.
		/// @note If you override this method,
		///		  you will have
		///		  to deal by hand with automatic blocking/unblocking,
		///		  cooldown and duration,
		///		  etc.
		/// @param The ability container.
		GDVIRTUAL4(_on_tick, double, double, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability should be activated.
		GDVIRTUAL1RC(bool, _should_be_activated, AbilityContainer *);
		/// @brief Called when the ability container checks if the ability should be blocked.
		GDVIRTUAL1RC(bool, _should_be_blocked, AbilityContainer *);
		/// @brief Called when the ability container checks if the ability should be granted.
		GDVIRTUAL1RC(bool, _should_be_ended, AbilityContainer *);
		/// @brief Called when the ability container checks if the ability cooldown should be reset when blocked.
		GDVIRTUAL1RC(bool, _should_reset_cooldown, AbilityContainer *);
		/// @brief Called when the ability container checks if the ability duration should be reset when blocked.
		GDVIRTUAL1RC(bool, _should_reset_duration, AbilityContainer *);

		/// @brief Gets the ability name.
		/// @return The ability name.
		[[nodiscard]] StringName get_ability_name() const;

		/// @brief Sets the ability name.
		/// @param p_ability_name The ability name.
		void set_ability_name(const StringName &p_ability_name);

	protected:
		/// @brief Binds the methods to godot.
		static void _bind_methods();
	};

#pragma endregion
} //namespace octod::gameplay::abilities

VARIANT_ENUM_CAST(octod::gameplay::abilities::AbilityEventType);

#endif
