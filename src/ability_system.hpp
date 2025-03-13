#ifndef OCTOD_GAMEPLAY_ABILITIES_ABILITY_SYSTEM_HPP
#define OCTOD_GAMEPLAY_ABILITIES_ABILITY_SYSTEM_HPP

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
	enum AbilityState : unsigned int
	{
		ABILITY_STATE_NONE,
		ABILITY_STATE_ACTIVE,
		ABILITY_STATE_BLOCKED,
		ABILITY_STATE_GRANTED,
	};

	enum AbilityEventType : unsigned int
	{
		/// success events
		ABILITY_EVENT_TYPE_ACTIVATED,
		ABILITY_EVENT_TYPE_BLOCKED,
		ABILITY_EVENT_TYPE_ENDED,
		ABILITY_EVENT_TYPE_GRANTED,
		ABILITY_EVENT_TYPE_REVOKED,

		/// refusal events
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE,
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_DECIDED_TO_BLOCK,
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_BLOCKED,
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_COOLING_DOWN,
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_DURATION_ACTIVE,
		ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_REVOKED,

		ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK,
		ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK_IS_NOT_ACTIVE,
		ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK_IS_NOT_GRANTED,

		ABILITY_EVENT_TYPE_REFUSED_TO_END,
		ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_BLOCKED,
		ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_NOT_ACTIVE,
		ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_NOT_GRANTED,

		ABILITY_EVENT_TYPE_REFUSED_TO_GRANT,
		ABILITY_EVENT_TYPE_REFUSED_TO_GRANT_ALREADY_GRANTED,

		ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE,
		ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE_ALREADY_REVOKED,

		/// error events
		ABILITY_EVENT_TYPE_ERROR_ACTIVATING,
		ABILITY_EVENT_TYPE_ERROR_BLOCKING,
		ABILITY_EVENT_TYPE_ERROR_ENDING,
		ABILITY_EVENT_TYPE_ERROR_GRANTING,
		ABILITY_EVENT_TYPE_ERROR_REVOKING,

		/// more bad errors, these are related to the container itself.
		ABILITY_NOT_FOUND,
		ABILITY_PARAMETER_IS_NULL
	};

	/// @brief Represents an ability that at runtime.
	class RuntimeAbility : public RefCounted
	{
		GDCLASS(RuntimeAbility, RefCounted);

		/// @brief Marks the ability state. Using a bitmask to store the state.
		int state = ABILITY_STATE_NONE;

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
		/// @brief Returns true if the ability should be activated.
		/// @return True if the ability should be activated, false otherwise.
		[[nodiscard]] bool should_be_activated() const;
		/// @brief Returns true if the ability should be blocked.
		/// @return True if the ability should be blocked, false otherwise.
		[[nodiscard]] bool should_be_blocked() const;
		/// @brief Returns true if the ability should be ended.
		/// @return True if the ability should be ended, false otherwise.
		[[nodiscard]] bool should_be_ended() const;

	protected:
		friend class AbilityContainer;

		/// @brief Binds the methods to godot.
		static void _bind_methods();
		/// @brief The ability resource.
		Ref<Ability> ability;
		/// @brief The ability container which contains, owns and runs the ability.
		AbilityContainer *container;
		/// @brief Activates the ability every x seconds.
		double activate_every = 0.0;
		/// @brief The last tick time. Used by duration.
		double duration_time = 0.0;
		/// @brief The last tick time. Used by cooldown.
		double cooldown_time = 0.0;

		/// @brief Handles the tick.
		/// @param p_delta The delta time.
		void handle_tick(double p_delta);
	};

#pragma endregion
#pragma region AbilityContainer

	/// @brief Represents a container of abilities.
	class AbilityContainer : public Node
	{
		GDCLASS(AbilityContainer, Node)

	protected:
		/// @brief Binds the methods to godot.
		static void _bind_methods();
		/// @brief The runtime abilities.
		Dictionary runtime_abilities = Dictionary();
		TypedArray<Ability> initial_abilities = TypedArray<Ability>();

		[[nodiscard]] Ref<RuntimeAbility> build_runtime_ability(const Ref<Ability> &p_ability) const;

		/// @brief Called when the node receives a notification.
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

	public:
		/// @brief Returns an instance of a RuntimeAbility. Override this method if you want to use an extended class of RuntimeAbility.
		GDVIRTUAL1RC(Ref<RuntimeAbility>, _build_runtime_ability, Ref<Resource>); // NOLINT(*-unnecessary-value-param)
		/// @brief The virtual method which handles the _process
		GDVIRTUAL1(_process, double); // NOLINT(*-unnecessary-value-param)
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
		[[nodiscard]] TypedArray<RuntimeAbility> get_abilities() const;
		/// @brief Gets the initial abilities.
		/// @return The initial abilities.
		[[nodiscard]] TypedArray<Ability> get_initial_abilities() const;
		/// @brief Gets an ability.
		/// @param p_variant The ability to get.
		/// @return The ability.
		[[nodiscard]] Ref<RuntimeAbility> get_ability(const Variant &p_variant) const;
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
		/// @return True if the ability was activated, false otherwise.
		[[nodiscard]] AbilityEventType try_activate(const Variant &p_ability_or_ability_name) const;
		/// @brief Blocks an ability in the container.
		/// @param p_ability_or_ability_name The ability to block.
		/// @return True if the ability was blocked, false otherwise.
		[[nodiscard]] AbilityEventType try_block(const Variant &p_ability_or_ability_name) const;
		/// @brief Ends an ability in the container.
		/// @param p_ability_or_ability_name The ability to end.
		/// @return True if the ability was ended, false otherwise.
		[[nodiscard]] AbilityEventType try_end(const Variant &p_ability_or_ability_name) const;
		/// @brief Grants an ability to the container.
		/// @param p_ability_or_ability_name The ability to grant.
		[[nodiscard]] AbilityEventType try_grant(const Variant &p_ability_or_ability_name) const;
		/// @brief Revokes an ability from the container.
		/// @param p_ability_or_ability_name The ability to revoke.
		/// @return True if the ability was revoked, false otherwise.
		[[nodiscard]] AbilityEventType try_revoke(const Variant &p_ability_or_ability_name) const;
	};

#pragma endregion
#pragma region Ability

	class Ability : public Resource
	{
		GDCLASS(Ability, Resource)

	protected:
		/// @brief Binds the methods to godot.
		static void _bind_methods();
		/// @brief The ability name.
		StringName ability_name;

	public:
		/// @brief Called when the ability container checks if the ability cooldown can be activated.
		GDVIRTUAL2RC(bool, _can_activate_cooldown, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be activated.
		GDVIRTUAL2RC(bool, _can_be_activated, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be blocked.
		GDVIRTUAL2RC(bool, _can_be_blocked, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be cancelled.
		GDVIRTUAL2RC(bool, _can_be_granted, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be ended.
		GDVIRTUAL2RC(bool, _can_be_revoked, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability can be granted.
		GDVIRTUAL2RC(bool, _can_be_ended, AbilityContainer *, RuntimeAbility *);
		/// @brief Gets the ability cooldown.
		GDVIRTUAL1RC(double, _get_cooldown, AbilityContainer *);
		/// @brief Gets the ability duration.
		GDVIRTUAL1RC(double, _get_duration, AbilityContainer *);
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
		/// @param The ability container.
		GDVIRTUAL4(_on_tick, double, double, AbilityContainer *, RuntimeAbility *);
		/// @brief Called when the ability container checks if the ability should be activated.
		GDVIRTUAL1RC(bool, _should_be_activated, AbilityContainer *);
		/// @brief Called when the ability container checks if the ability should be blocked.
		GDVIRTUAL1RC(bool, _should_be_blocked, AbilityContainer *);
		/// @brief Called when the ability container checks if the ability should be granted.
		GDVIRTUAL1RC(bool, _should_be_ended, AbilityContainer *);

		/// @brief Gets the ability name.
		/// @return The ability name.
		[[nodiscard]] StringName get_ability_name() const;
		/// @brief Sets the ability name.
		/// @param p_ability_name The ability name.
		void set_ability_name(const StringName &p_ability_name);
	};

#pragma endregion
} //namespace octod::gameplay::abilities

VARIANT_ENUM_CAST(octod::gameplay::abilities::AbilityEventType);

#endif