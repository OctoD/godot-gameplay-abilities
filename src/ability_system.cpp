#include "ability_system.hpp"
#include <godot_cpp/classes/engine.hpp>

using namespace octod::gameplay::abilities;

#pragma region macros

#ifndef ABILITY_PARAMETER_IS_NULL_CHECK
#define ABILITY_PARAMETER_IS_NULL_CHECK(p_parameter) ERR_FAIL_COND_V_MSG(p_parameter.is_null(), ABILITY_PARAMETER_IS_NULL, "The Ability cannot be null.");
#endif

#ifndef CHECK_IF_SHOULD_BE_CALLED
#define CHECK_IF_SHOULD_BE_CALLED(p_method_name, p_starting_value)     \
	bool output = p_starting_value;                                    \
                                                                       \
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, p_method_name)) {         \
		GDVIRTUAL_CALL_PTR(ability, p_method_name, container, output); \
	}                                                                  \
                                                                       \
	return output;
#endif

#ifndef CALL_VIRTUAL_ABILITY_METHOD
#define CALL_VIRTUAL_ABILITY_METHOD(                                                      \
	p_check_method_name,                                                              \
	p_method_name,                                                                    \
	p_error_type,                                                                     \
	p_refusal_type)                                                                   \
                                                                                          \
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, p_check_method_name)) {                      \
		bool output = false;                                                              \
                                                                                          \
		if (!GDVIRTUAL_CALL_PTR(ability, p_check_method_name, container, this, output)) { \
			SET_STATE(p_error_type);                                                      \
			return p_error_type;                                                          \
		}                                                                                 \
                                                                                          \
		if (!output) {                                                                    \
			SET_STATE(p_refusal_type);                                                    \
			return p_refusal_type;                                                        \
		}                                                                                 \
	}                                                                                     \
                                                                                          \
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, p_method_name)) {                            \
		if (!GDVIRTUAL_CALL_PTR(ability, p_method_name, container, this)) {               \
			SET_STATE(p_error_type);                                                      \
			return p_error_type;                                                          \
		}                                                                                 \
	}
#endif
#ifndef ENSURE_ABILITY_EXECUTION
#define ENSURE_ABILITY_EXECUTION(p_error_type)                                     \
	ERR_FAIL_COND_V_MSG(ability == nullptr, p_error_type, "The ability is null."); \
	ERR_FAIL_COND_V_MSG(container == nullptr, p_error_type, "The container is null.");
#endif

#ifndef IS_STATE
#define IS_STATE(p_state) ((state & p_state) == p_state)
#endif

#ifndef SET_STATE
#define SET_STATE(p_state)                   \
	switch (p_state) {                       \
		case ABILITY_EVENT_TYPE_ACTIVATED:   \
			state &= ~ABILITY_STATE_IDLE;    \
			state |= ABILITY_STATE_ACTIVE;   \
			break;							 \
		case ABILITY_EVENT_TYPE_ACTIVATED_COOLDOWN_STARTED:   \
			state &= ~ABILITY_STATE_ACTIVE;    \
			state |= ABILITY_STATE_COOLING_DOWN;   \
			break;							 \
		case ABILITY_EVENT_TYPE_BLOCKED:     \
			state &= ~ABILITY_STATE_ACTIVE;  \
			state &= ~ABILITY_STATE_COOLING_DOWN;  \
			state &= ~ABILITY_STATE_IDLE;    \
			state |= ABILITY_STATE_BLOCKED;  \
			break;                           \
		case ABILITY_EVENT_TYPE_ENDED:       \
			state &= ~ABILITY_STATE_ACTIVE;  \
			state &= ~ABILITY_STATE_COOLING_DOWN;  \
			state |= ABILITY_STATE_IDLE;     \
			break;                           \
		case ABILITY_EVENT_TYPE_GRANTED:     \
			state &= ~ABILITY_STATE_ACTIVE;  \
			state &= ~ABILITY_STATE_BLOCKED; \
			state &= ~ABILITY_STATE_COOLING_DOWN; \
			state |= ABILITY_STATE_IDLE;     \
			state |= ABILITY_STATE_GRANTED;  \
			break;                           \
		case ABILITY_EVENT_TYPE_REVOKED:     \
			state &= ~ABILITY_STATE_ACTIVE;  \
			state &= ~ABILITY_STATE_BLOCKED; \
			state &= ~ABILITY_STATE_COOLING_DOWN; \
			state &= ~ABILITY_STATE_GRANTED; \
			state &= ~ABILITY_STATE_IDLE;    \
			break;                           \
		case ABILITY_EVENT_TYPE_UNBLOCKED:   \
			state &= ~ABILITY_STATE_ACTIVE;  \
			state &= ~ABILITY_STATE_BLOCKED; \
			state |= ABILITY_STATE_IDLE;     \
			break;                           \
		default:                             \
			break;                           \
	}
#endif

#pragma endregion

#pragma region RuntimeAbility

void RuntimeAbility::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("activate"), &RuntimeAbility::activate);
	ClassDB::bind_method(D_METHOD("block"), &RuntimeAbility::block);
	ClassDB::bind_method(D_METHOD("end"), &RuntimeAbility::end);
	ClassDB::bind_method(D_METHOD("get_ability"), &RuntimeAbility::get_ability);
	ClassDB::bind_method(D_METHOD("get_container"), &RuntimeAbility::get_container);
	ClassDB::bind_method(D_METHOD("get_cooldown"), &RuntimeAbility::get_cooldown);
	ClassDB::bind_method(D_METHOD("get_duration"), &RuntimeAbility::get_duration);
	ClassDB::bind_method(D_METHOD("grant"), &RuntimeAbility::grant);
	ClassDB::bind_method(D_METHOD("is_active"), &RuntimeAbility::is_active);
	ClassDB::bind_method(D_METHOD("is_blocked"), &RuntimeAbility::is_blocked);
	ClassDB::bind_method(D_METHOD("is_cooldown_active"), &RuntimeAbility::is_cooldown_active);
	ClassDB::bind_method(D_METHOD("is_duration_active"), &RuntimeAbility::is_duration_active);
	ClassDB::bind_method(D_METHOD("is_ended"), &RuntimeAbility::is_ended);
	ClassDB::bind_method(D_METHOD("is_granted"), &RuntimeAbility::is_granted);
	ClassDB::bind_method(D_METHOD("revoke"), &RuntimeAbility::revoke);
	ClassDB::bind_method(D_METHOD("set_ability", "ability"), &RuntimeAbility::set_ability);
	ClassDB::bind_method(D_METHOD("set_container", "container"), &RuntimeAbility::set_container);

	/// binds enum constants to godot
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ACTIVATED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ACTIVATED_COOLDOWN_STARTED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ACTIVATED_DURATION_STARTED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_BLOCKED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ENDED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ERROR_ACTIVATING);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ERROR_BLOCKING);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ERROR_ENDING);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ERROR_GRANTING);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_ERROR_REVOKING);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_GRANTED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_DECIDED_TO_BLOCK);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_BLOCKED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_COOLING_DOWN);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_DURATION_ACTIVE);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_REVOKED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK_IS_NOT_GRANTED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_END);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_BLOCKED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_COOLING_DOWN);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_NOT_ACTIVE);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_NOT_GRANTED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_GRANT);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_GRANT_ALREADY_GRANTED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE_ALREADY_REVOKED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_UNBLOCK);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REFUSED_TO_UNBLOCK_IS_NOT_BLOCKED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_REVOKED);
	BIND_ENUM_CONSTANT(ABILITY_EVENT_TYPE_UNBLOCKED);

	/// binds signals to godot
	ADD_SIGNAL(MethodInfo("activated"));
	ADD_SIGNAL(MethodInfo("blocked"));
	ADD_SIGNAL(MethodInfo("cooldown_end"));
	ADD_SIGNAL(MethodInfo("cooldown_start"));
	ADD_SIGNAL(MethodInfo("ended"));
	ADD_SIGNAL(MethodInfo("granted"));
	ADD_SIGNAL(MethodInfo("revoked"));
	ADD_SIGNAL(MethodInfo("unblocked"));
}

void RuntimeAbility::handle_tick(const double p_delta)
{
	if (ability == nullptr || container == nullptr)
	{
		return;
	}

	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, _on_tick))
	{
		GDVIRTUAL_CALL_PTR(ability, _on_tick, p_delta, cooldown_time, container, this);
		return;
	}

	/// if the ability is blocked, it will not be re-activated
	if (should_be_blocked() && block() == ABILITY_EVENT_TYPE_BLOCKED)
	{
		return;
	}

	if (is_active() && is_duration_active())
	{
		duration_time -= p_delta;

		if (!is_duration_active())
		{
			trigger_cooldown();
		}

		return;
	}

	if (IS_STATE(ABILITY_STATE_COOLING_DOWN) && is_cooldown_active())
	{
		cooldown_time -= p_delta;
		return;
	}

	/// if the ability should be ended, it will be ended
	if (should_be_ended())
	{
		end();
	}

	if (!should_be_activated())
	{
		return;
	}

	/// if the ability should be activated, it will be re-activated. This is ideal for continuous abilities.
	activate();
}

bool RuntimeAbility::trigger_cooldown()
{
	if (const double cooldown = get_cooldown(); !Math::is_zero_approx(cooldown))
	{
		if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, _can_activate_cooldown))
		{
			bool should_activate_cooldown = false;

			GDVIRTUAL_CALL_PTR(ability, _can_activate_cooldown, container, this, should_activate_cooldown);

			if (should_activate_cooldown)
			{
				cooldown_time = cooldown;
			}
		} else
		{
			cooldown_time = cooldown;
		}

		SET_STATE(ABILITY_EVENT_TYPE_ACTIVATED_COOLDOWN_STARTED)

		emit_signal("cooldown_start");
		return true;
	}

	return false;
}

bool RuntimeAbility::trigger_duration()
{
	if (const double duration = get_duration(); !Math::is_zero_approx(duration))
	{
		duration_time = duration;
		return true;
	}

	return false;
}

void RuntimeAbility::try_reset_cooldown()
{
	bool should_reset_cooldown = !Math::is_zero_approx(cooldown_time);

	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, _should_reset_cooldown))
	{
		GDVIRTUAL_CALL_PTR(ability, _should_reset_cooldown, container, should_reset_cooldown);
	}

	if (should_reset_cooldown)
	{
		cooldown_time = 0.0;
		emit_signal("cooldown_end");
	}
}

void RuntimeAbility::try_reset_duration()
{
	bool should_reset_duration = true;

	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, _should_reset_duration))
	{
		GDVIRTUAL_CALL_PTR(ability, _should_reset_duration, container, should_reset_duration);
	}

	if (should_reset_duration)
	{
		duration_time = 0.0;
	}
}

AbilityEventType RuntimeAbility::activate()
{
	ENSURE_ABILITY_EXECUTION(ABILITY_EVENT_TYPE_ERROR_ACTIVATING);

	WARN_PRINT(IS_STATE(ABILITY_STATE_BLOCKED) ? "blocked" : "not blocked");

	WARN_PRINT(String::num(state));

	if (is_revoked())
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_REVOKED;
	}

	if (is_blocked())
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_BLOCKED;
	}

	if (should_be_blocked() && block() == ABILITY_EVENT_TYPE_BLOCKED)
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_DECIDED_TO_BLOCK;
	}

	if (is_duration_active())
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_DURATION_ACTIVE;
	}

	if (is_cooldown_active())
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE_IS_COOLING_DOWN;
	}

	CALL_VIRTUAL_ABILITY_METHOD(_can_be_activated, _on_activate, ABILITY_EVENT_TYPE_ERROR_ACTIVATING, ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE);

	SET_STATE(ABILITY_EVENT_TYPE_ACTIVATED);

	emit_signal("activated");

	if (trigger_duration())
	{
		return ABILITY_EVENT_TYPE_ACTIVATED_DURATION_STARTED;
	}

	if (trigger_cooldown())
	{
		return ABILITY_EVENT_TYPE_ACTIVATED_COOLDOWN_STARTED;
	}

	return ABILITY_EVENT_TYPE_ACTIVATED;
}

AbilityEventType RuntimeAbility::block()
{
	ENSURE_ABILITY_EXECUTION(ABILITY_EVENT_TYPE_ERROR_BLOCKING);

	if (!IS_STATE(ABILITY_STATE_GRANTED))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK_IS_NOT_GRANTED;
	}

	CALL_VIRTUAL_ABILITY_METHOD(_can_be_blocked, _on_block, ABILITY_EVENT_TYPE_ERROR_BLOCKING, ABILITY_EVENT_TYPE_REFUSED_TO_BLOCK);

	SET_STATE(ABILITY_EVENT_TYPE_BLOCKED);

	emit_signal("blocked");

	try_reset_cooldown();
	try_reset_duration();

	return ABILITY_EVENT_TYPE_BLOCKED;
}

AbilityEventType RuntimeAbility::end()
{
	ENSURE_ABILITY_EXECUTION(ABILITY_EVENT_TYPE_ERROR_ENDING);

	if (IS_STATE(ABILITY_STATE_BLOCKED))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_BLOCKED;
	}

	if (!IS_STATE(ABILITY_STATE_ACTIVE) && !IS_STATE(ABILITY_STATE_COOLING_DOWN))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_COOLING_DOWN;
	}

	if (IS_STATE(ABILITY_STATE_COOLING_DOWN) && !Math::is_zero_approx(cooldown_time))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_END;
	}

	if (!IS_STATE(ABILITY_STATE_GRANTED))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_END_IS_NOT_GRANTED;
	}

	CALL_VIRTUAL_ABILITY_METHOD(_can_be_ended, _on_end, ABILITY_EVENT_TYPE_ERROR_ENDING, ABILITY_EVENT_TYPE_REFUSED_TO_END);

	SET_STATE(ABILITY_EVENT_TYPE_ENDED);

	emit_signal("ended");

	try_reset_cooldown();
	try_reset_duration();

	return ABILITY_EVENT_TYPE_ENDED;
}

Ref<Ability> RuntimeAbility::get_ability() const
{
	return ability;
}

AbilityContainer *RuntimeAbility::get_container() const
{
	return container;
}

double RuntimeAbility::get_cooldown() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, _get_cooldown))
	{
		if (double cooldown = 0.0; GDVIRTUAL_CALL_PTR(ability, _get_cooldown, container, cooldown))
		{
			return cooldown;
		}
	}

	return 0.0;
}

double RuntimeAbility::get_duration() const
{
	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, _get_duration))
	{
		if (double duration = 0.0; GDVIRTUAL_CALL_PTR(ability, _get_duration, container, duration))
		{
			return duration;
		}
	}

	return 0.0;
}

AbilityEventType RuntimeAbility::grant()
{
	ENSURE_ABILITY_EXECUTION(ABILITY_EVENT_TYPE_ERROR_GRANTING);

	if (IS_STATE(ABILITY_STATE_GRANTED))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_GRANT_ALREADY_GRANTED;
	}

	CALL_VIRTUAL_ABILITY_METHOD(_can_be_granted, _on_grant, ABILITY_EVENT_TYPE_ERROR_GRANTING, ABILITY_EVENT_TYPE_REFUSED_TO_GRANT);

	SET_STATE(ABILITY_EVENT_TYPE_GRANTED);

	emit_signal("granted");

	return ABILITY_EVENT_TYPE_GRANTED;
}

bool RuntimeAbility::is_active() const
{
	return IS_STATE(ABILITY_STATE_ACTIVE);
}

bool RuntimeAbility::is_blocked() const
{
	return IS_STATE(ABILITY_STATE_BLOCKED);
}

bool RuntimeAbility::is_cooldown_active() const
{
	return !Math::is_zero_approx(cooldown_time);
}

bool RuntimeAbility::is_duration_active() const
{
	return !Math::is_zero_approx(duration_time);
}

bool RuntimeAbility::is_ended() const
{
	return !IS_STATE(ABILITY_STATE_ACTIVE) && !IS_STATE(ABILITY_STATE_BLOCKED) && IS_STATE(ABILITY_STATE_GRANTED);
}

bool RuntimeAbility::is_granted() const
{
	return IS_STATE(ABILITY_STATE_GRANTED);
}

bool RuntimeAbility::is_revoked() const
{
	return !IS_STATE(ABILITY_STATE_GRANTED);
}

AbilityEventType RuntimeAbility::revoke()
{
	ENSURE_ABILITY_EXECUTION(ABILITY_EVENT_TYPE_ERROR_REVOKING);

	if (!IS_STATE(ABILITY_STATE_GRANTED))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE_ALREADY_REVOKED;
	}

	CALL_VIRTUAL_ABILITY_METHOD(_can_be_revoked, _on_revoke, ABILITY_EVENT_TYPE_ERROR_REVOKING, ABILITY_EVENT_TYPE_REFUSED_TO_REVOKE);

	SET_STATE(ABILITY_EVENT_TYPE_REVOKED);

	if (!Math::is_zero_approx(cooldown_time))
	{
		emit_signal("cooldown_end");
	}

	cooldown_time = 0.0;
	duration_time = 0.0;

	emit_signal("revoked");

	return ABILITY_EVENT_TYPE_REVOKED;
}

void RuntimeAbility::set_ability(const Ref<Ability> &p_ability)
{
	ability = p_ability;
	state = ABILITY_STATE_IDLE;
}

void RuntimeAbility::set_container(AbilityContainer *p_container)
{
	container = p_container;
	cooldown_time = 0.0;
}

bool RuntimeAbility::should_be_activated() const
{
	CHECK_IF_SHOULD_BE_CALLED(_should_be_activated, false);
}

bool RuntimeAbility::should_be_blocked() const
{
	CHECK_IF_SHOULD_BE_CALLED(_should_be_blocked, false);
}

bool RuntimeAbility::should_be_ended() const
{
	CHECK_IF_SHOULD_BE_CALLED(_should_be_ended, true);
}

AbilityEventType RuntimeAbility::unblock()
{
	ENSURE_ABILITY_EXECUTION(ABILITY_EVENT_TYPE_ERROR_UNBLOCKING);

	if (!IS_STATE(ABILITY_STATE_BLOCKED))
	{
		return ABILITY_EVENT_TYPE_REFUSED_TO_UNBLOCK_IS_NOT_BLOCKED;
	}

	if (GDVIRTUAL_IS_OVERRIDDEN_PTR(ability, _should_be_blocked))
	{
		bool should_be_blocked = false;

		if (!GDVIRTUAL_CALL_PTR(ability, _should_be_blocked, container, should_be_blocked))
		{
			return ABILITY_EVENT_TYPE_ERROR_UNBLOCKING;
		}

		if (should_be_blocked)
		{
			return ABILITY_EVENT_TYPE_REFUSED_TO_UNBLOCK_SHOULD_BE_BLOCKED;
		}
	}

	SET_STATE(ABILITY_EVENT_TYPE_UNBLOCKED)

	emit_signal("unblocked");

	return ABILITY_EVENT_TYPE_UNBLOCKED;
}

#pragma endregion

#pragma region AbilityContainer

void AbilityContainer::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("add_ability", "ability"), &AbilityContainer::add_ability);
	ClassDB::bind_method(D_METHOD("find_ability", "predicate"), &AbilityContainer::find_ability);
	ClassDB::bind_method(D_METHOD("get_initial_abilities"), &AbilityContainer::get_initial_abilities);
	ClassDB::bind_method(D_METHOD("get_runtime_ability", "ability_name_or_instance"), &AbilityContainer::get_runtime_ability);
	ClassDB::bind_method(D_METHOD("get_runtime_abilities"), &AbilityContainer::get_runtime_abilities);
	ClassDB::bind_method(D_METHOD("has_ability", "ability_name_or_instance"), &AbilityContainer::has_ability);
	ClassDB::bind_method(D_METHOD("is_ability_active", "ability_name_or_instance"), &AbilityContainer::is_ability_active);
	ClassDB::bind_method(D_METHOD("is_ability_blocked", "ability_name_or_instance"), &AbilityContainer::is_ability_blocked);
	ClassDB::bind_method(D_METHOD("is_ability_cooldown_active", "ability_name_or_instance"), &AbilityContainer::is_ability_cooldown_active);
	ClassDB::bind_method(D_METHOD("is_ability_ended", "ability_name_or_instance"), &AbilityContainer::is_ability_ended);
	ClassDB::bind_method(D_METHOD("is_ability_granted", "ability_name_or_instance"), &AbilityContainer::is_ability_granted);
	ClassDB::bind_method(D_METHOD("remove_ability", "ability"), &AbilityContainer::remove_ability);
	ClassDB::bind_method(D_METHOD("set_initial_abilities", "abilities"), &AbilityContainer::set_initial_abilities);
	ClassDB::bind_method(D_METHOD("try_activate", "ability_or_ability_name"), &AbilityContainer::try_activate);
	ClassDB::bind_method(D_METHOD("try_block", "ability_or_ability_name"), &AbilityContainer::try_block);
	ClassDB::bind_method(D_METHOD("try_end", "ability_or_ability_name"), &AbilityContainer::try_end);
	ClassDB::bind_method(D_METHOD("try_grant", "ability_or_ability_name"), &AbilityContainer::try_grant);
	ClassDB::bind_method(D_METHOD("try_revoke", "ability_or_ability_name"), &AbilityContainer::try_revoke);
	ClassDB::bind_method(D_METHOD("try_unblock", "ability_or_ability_name"), &AbilityContainer::try_unblock);

	/// binds virtual methods to godot
	GDVIRTUAL_BIND(_build_runtime_ability, "ability_resource");
	GDVIRTUAL_BIND(_process, "delta");
	GDVIRTUAL_BIND(_try_activate, "ability_or_ability_name");
	GDVIRTUAL_BIND(_try_block, "ability_or_ability_name");
	GDVIRTUAL_BIND(_try_end, "ability_or_ability_name");
	GDVIRTUAL_BIND(_try_grant, "ability_or_ability_name");
	GDVIRTUAL_BIND(_try_revoke, "ability_or_ability_name");
	GDVIRTUAL_BIND(_try_unblock, "ability_or_ability_name");

	/// Registering signals handlers
	ClassDB::bind_method(D_METHOD("_on_active_ability", "runtime_ability"), &AbilityContainer::_on_active_ability);
	ClassDB::bind_method(D_METHOD("_on_blocked_ability", "runtime_ability"), &AbilityContainer::_on_blocked_ability);
	ClassDB::bind_method(D_METHOD("_on_ended_ability", "runtime_ability"), &AbilityContainer::_on_ended_ability);
	ClassDB::bind_method(D_METHOD("_on_granted_ability", "runtime_ability"), &AbilityContainer::_on_granted_ability);
	ClassDB::bind_method(D_METHOD("_on_revoked_ability", "runtime_ability"), &AbilityContainer::_on_revoked_ability);
	ClassDB::bind_method(D_METHOD("_on_cooldown_end", "runtime_ability"), &AbilityContainer::_on_cooldown_end);
	ClassDB::bind_method(D_METHOD("_on_cooldown_start", "runtime_ability"), &AbilityContainer::_on_cooldown_start);
	ClassDB::bind_method(D_METHOD("_on_unblocked_ability", "runtime_ability"), &AbilityContainer::_on_unblocked_ability);

	/// binding properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "abilities", PROPERTY_HINT_RESOURCE_TYPE, "24/17:Ability"), "set_initial_abilities", "get_initial_abilities");

	/// Binds enum constants
	BIND_ENUM_CONSTANT(ABILITY_NOT_FOUND)
	BIND_ENUM_CONSTANT(ABILITY_PARAMETER_IS_NULL)

	/// binds signals to godot
	ADD_SIGNAL(MethodInfo("ability_activated", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("ability_added", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("ability_blocked", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("ability_ended", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("ability_granted", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("ability_removed", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("ability_revoked", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("ability_unblocked", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("cooldown_end", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
	ADD_SIGNAL(MethodInfo("cooldown_start", PropertyInfo(Variant::OBJECT, "ability", PROPERTY_HINT_RESOURCE_TYPE, "Ability")));
}

Ref<RuntimeAbility> AbilityContainer::build_runtime_ability(const Ref<Ability> &p_ability) const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_build_runtime_ability))
	{
		if (Ref<RuntimeAbility> runtime_ability; GDVIRTUAL_CALL(_build_runtime_ability, p_ability, runtime_ability))
		{
			return runtime_ability;
		}
	}

	return {};
}

void AbilityContainer::_notification(const int p_what)
{
	if (p_what == NOTIFICATION_READY)
	{
		for (int i = 0; i < initial_abilities.size(); i++)
		{
			if (Ref<Ability> ability = initial_abilities[i]; ability.is_valid() && !ability.is_null())
			{
				add_ability(ability);
			}
		}

		set_physics_process(true);
	}

	if (p_what == NOTIFICATION_PHYSICS_PROCESS)
	{
		TypedArray<RuntimeAbility> values = runtime_abilities.values();

		for (int i = 0; i < values.size(); i++)
		{
			if (Ref<RuntimeAbility> runtime_ability = values[i]; runtime_ability.is_valid() && !runtime_ability.is_null() && runtime_ability->is_granted())
			{
				runtime_ability->handle_tick(get_physics_process_delta_time());
			}
		}

		GDVIRTUAL_CALL(_process, get_physics_process_delta_time());
	}
}

void AbilityContainer::_on_active_ability(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("ability_activated", p_runtime_ability->get_ability());
}

void AbilityContainer::_on_blocked_ability(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("ability_blocked", p_runtime_ability->get_ability());
}

void AbilityContainer::_on_ended_ability(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("ability_ended", p_runtime_ability->get_ability());
}

void AbilityContainer::_on_granted_ability(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("ability_granted", p_runtime_ability->get_ability());
}

void AbilityContainer::_on_revoked_ability(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("ability_revoked", p_runtime_ability->get_ability());
}

void AbilityContainer::_on_cooldown_end(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("cooldown_end", p_runtime_ability->get_ability());
}

void AbilityContainer::_on_cooldown_start(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("cooldown_start", p_runtime_ability->get_ability());
}

void AbilityContainer::_on_unblocked_ability(const Ref<RuntimeAbility> &p_runtime_ability)
{
	emit_signal("ability_unblocked", p_runtime_ability->get_ability());
}

bool AbilityContainer::add_ability(const Ref<Ability> &p_ability)
{
	ERR_FAIL_COND_V_MSG(p_ability.is_null(), false, "The Ability cannot be null.");
	ERR_FAIL_COND_V_MSG(!p_ability.is_valid(), false, "The Ability Ref cannot be invalid.");

	if (!has_ability(p_ability))
	{
		const Ref runtime_ability = memnew(RuntimeAbility);

		runtime_ability->set_ability(p_ability);
		runtime_ability->set_container(this);

		runtime_abilities[p_ability->get_ability_name()] = runtime_ability;

		runtime_ability->connect("activated", Callable::create(this, "_on_active_ability").bind(runtime_ability));
		runtime_ability->connect("blocked", Callable::create(this, "_on_blocked_ability").bind(runtime_ability));
		runtime_ability->connect("ended", Callable::create(this, "_on_ended_ability").bind(runtime_ability));
		runtime_ability->connect("granted", Callable::create(this, "_on_granted_ability").bind(runtime_ability));
		runtime_ability->connect("revoked", Callable::create(this, "_on_revoked_ability").bind(runtime_ability));
		runtime_ability->connect("cooldown_end", Callable::create(this, "_on_cooldown_end").bind(runtime_ability));
		runtime_ability->connect("cooldown_start", Callable::create(this, "_on_cooldown_start").bind(runtime_ability));
		runtime_ability->connect("unblocked", Callable::create(this, "_on_unblocked_ability").bind(runtime_ability));

		emit_signal("ability_added", p_ability);

		if (try_grant(p_ability) == ABILITY_EVENT_TYPE_GRANTED && runtime_ability->should_be_activated() && try_activate(p_ability) == ABILITY_EVENT_TYPE_ACTIVATED)
		{
			return true;
		}
	}

	return false;
}

Ref<RuntimeAbility> AbilityContainer::get_runtime_ability(const Variant &p_variant) const
{
	if (p_variant.get_type() == Variant::OBJECT)
	{
		if (const Ref<Ability> ability = p_variant; ability.is_valid() && !ability.is_null())
		{
			return runtime_abilities[ability->get_ability_name()];
		}
	}

	return runtime_abilities[p_variant];
}

TypedArray<RuntimeAbility> AbilityContainer::get_runtime_abilities() const
{
	return runtime_abilities.values();
}

TypedArray<Ability> AbilityContainer::get_initial_abilities() const
{
	return initial_abilities;
}

Ref<RuntimeAbility> AbilityContainer::find_ability(const Callable &p_predicate) const
{
	TypedArray<RuntimeAbility> values = runtime_abilities.values();

	for (int i = 0; i < values.size(); i++)
	{
		if (Ref<RuntimeAbility> runtime_ability = values[i]; runtime_ability.is_valid() && !runtime_ability.is_null() && p_predicate.call(runtime_ability, i))
		{
			return runtime_ability;
		}
	}

	return {};
}

bool AbilityContainer::has_ability(const Variant &p_variant) const
{
	if (p_variant.get_type() == Variant::OBJECT)
	{
		const Ref<Ability> ability = p_variant;
		return ability.is_valid() && !ability.is_null() && runtime_abilities.has(ability->get_ability_name());
	}

	if (p_variant.get_type() == Variant::STRING)
	{
		return runtime_abilities.has(p_variant);
	}

	return runtime_abilities.has(p_variant);
}

bool AbilityContainer::is_ability_active(const Variant &p_variant) const
{
	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_variant);
	return runtime_ability.is_valid() && !runtime_ability.is_null() && runtime_ability->is_active();
}

bool AbilityContainer::is_ability_blocked(const Variant &p_variant) const
{
	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_variant);
	return runtime_ability.is_valid() && !runtime_ability.is_null() && runtime_ability->is_blocked();
}

bool AbilityContainer::is_ability_cooldown_active(const Variant &p_variant) const
{
	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_variant);
	return runtime_ability.is_valid() && !runtime_ability.is_null() && runtime_ability->is_cooldown_active();
}

bool AbilityContainer::is_ability_ended(const Variant &p_variant) const
{
	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_variant);
	return runtime_ability.is_valid() && !runtime_ability.is_null() && runtime_ability->is_ended();
}

bool AbilityContainer::is_ability_granted(const Variant &p_variant) const
{
	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_variant);
	return runtime_ability.is_valid() && !runtime_ability.is_null() && runtime_ability->is_granted();
}

bool AbilityContainer::remove_ability(const Ref<Ability> &p_ability)
{
	if (has_ability(p_ability))
	{
		const Ref<RuntimeAbility> runtime_ability = runtime_abilities[p_ability->get_ability_name()];

		if (runtime_ability->is_active())
		{
			runtime_ability->end();
		}

		runtime_ability->revoke();

		runtime_abilities.erase(p_ability->get_ability_name());

		emit_signal("ability_removed", p_ability);
		return true;
	}

	return false;
}

void AbilityContainer::set_initial_abilities(const TypedArray<Ability> &p_abilities)
{
	initial_abilities = p_abilities;
}

AbilityEventType AbilityContainer::try_activate(const Variant &p_ability_or_ability_name) const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_try_activate))
	{
		if (int event_type = ABILITY_NOT_FOUND; GDVIRTUAL_CALL(_try_activate, p_ability_or_ability_name, event_type))
		{
			return static_cast<AbilityEventType>(event_type);
		}

		return ABILITY_EVENT_TYPE_ERROR_ACTIVATING;
	}

	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_ability_or_ability_name);

	ABILITY_PARAMETER_IS_NULL_CHECK(runtime_ability);

	return runtime_ability->activate();
}

AbilityEventType AbilityContainer::try_block(const Variant &p_ability_or_ability_name) const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_try_block))
	{
		if (int event_type = ABILITY_NOT_FOUND; GDVIRTUAL_CALL(_try_block, p_ability_or_ability_name, event_type))
		{
			return static_cast<AbilityEventType>(event_type);
		}

		return ABILITY_EVENT_TYPE_ERROR_BLOCKING;
	}

	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_ability_or_ability_name);

	ABILITY_PARAMETER_IS_NULL_CHECK(runtime_ability);

	return runtime_ability->block();
}

AbilityEventType AbilityContainer::try_end(const Variant &p_ability_or_ability_name) const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_try_end))
	{
		if (int event_type = ABILITY_NOT_FOUND; GDVIRTUAL_CALL(_try_end, p_ability_or_ability_name, event_type))
		{
			return static_cast<AbilityEventType>(event_type);
		}

		return ABILITY_EVENT_TYPE_ERROR_ENDING;
	}

	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_ability_or_ability_name);

	ABILITY_PARAMETER_IS_NULL_CHECK(runtime_ability);

	return runtime_ability->end();
}

AbilityEventType AbilityContainer::try_grant(const Variant &p_ability_or_ability_name) const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_try_grant))
	{
		if (int event_type = ABILITY_NOT_FOUND; GDVIRTUAL_CALL(_try_grant, p_ability_or_ability_name, event_type))
		{
			return static_cast<AbilityEventType>(event_type);
		}

		return ABILITY_EVENT_TYPE_ERROR_GRANTING;
	}

	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_ability_or_ability_name);

	ABILITY_PARAMETER_IS_NULL_CHECK(runtime_ability);

	return runtime_ability->grant();
}

AbilityEventType AbilityContainer::try_revoke(const Variant &p_ability_or_ability_name) const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_try_revoke))
	{
		if (int event_type = ABILITY_NOT_FOUND; GDVIRTUAL_CALL(_try_revoke, p_ability_or_ability_name, event_type))
		{
			return static_cast<AbilityEventType>(event_type);
		}

		return ABILITY_EVENT_TYPE_ERROR_REVOKING;
	}

	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_ability_or_ability_name);

	ABILITY_PARAMETER_IS_NULL_CHECK(runtime_ability);

	return runtime_ability->revoke();
}

AbilityEventType AbilityContainer::try_unblock(const Variant &p_ability_or_ability_name) const
{
	if (GDVIRTUAL_IS_OVERRIDDEN(_try_unblock))
	{
		if (int event_type = ABILITY_NOT_FOUND; GDVIRTUAL_CALL(_try_unblock, p_ability_or_ability_name, event_type))
		{
			return static_cast<AbilityEventType>(event_type);
		}

		return ABILITY_EVENT_TYPE_ERROR_UNBLOCKING;
	}

	const Ref<RuntimeAbility> runtime_ability = get_runtime_ability(p_ability_or_ability_name);

	ABILITY_PARAMETER_IS_NULL_CHECK(runtime_ability);

	return runtime_ability->unblock();
}

#pragma endregion

#pragma region Ability

void Ability::_bind_methods()
{
	/// binds methods to godot
	ClassDB::bind_method(D_METHOD("get_ability_name"), &Ability::get_ability_name);
	ClassDB::bind_method(D_METHOD("set_ability_name", "ability_name"), &Ability::set_ability_name);

	/// binds properties to godot
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "ability_name"), "set_ability_name", "get_ability_name");

	/// binds virtual methods to godot
	GDVIRTUAL_BIND(_can_activate_cooldown, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_can_be_activated, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_can_be_blocked, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_can_be_ended, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_can_be_granted, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_can_be_revoked, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_get_cooldown, "ability_container");
	GDVIRTUAL_BIND(_get_duration, "ability_container");
	GDVIRTUAL_BIND(_on_activate, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_on_block, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_on_end, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_on_grant, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_on_revoke, "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_on_tick, "delta", "tick_time", "ability_container", "runtime_ability");
	GDVIRTUAL_BIND(_should_be_activated, "ability_container");
	GDVIRTUAL_BIND(_should_be_blocked, "ability_container");
	GDVIRTUAL_BIND(_should_be_ended, "ability_container");
	GDVIRTUAL_BIND(_should_reset_cooldown, "ability_container");
	GDVIRTUAL_BIND(_should_reset_duration, "ability_container");
}

StringName Ability::get_ability_name() const
{
	return ability_name;
}

void Ability::set_ability_name(const StringName &p_ability_name)
{
	ability_name = p_ability_name;
}

#pragma endregion
