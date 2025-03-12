@tool
extends HBoxContainer


enum States {
	ACTIVATED,
	BLOCKED,
	GRANTED,
	COOLING_DOWN
}


@export var ability_container: AbilityContainer = null
@export_enum("Activated", "Blocked", "Granted", "Cooling down") var state_to_display: int = States.ACTIVATED


@onready var state_name_label: Label = %StateNameLabel
@onready var state_state_label: Label = %StateStateLabel


func _get_state_label(state: int) -> String:
	match state:
		States.ACTIVATED: return "active"
		States.BLOCKED: return "blocked"
		States.GRANTED: return "granted"
		States.COOLING_DOWN: return "cooling down"
		_: return ""


func _ready() -> void:
	ability_container.ability_activated.connect(make_handler(state_to_display))
	ability_container.ability_blocked.connect(make_handler(state_to_display))
	ability_container.ability_granted.connect(make_handler(state_to_display))
	ability_container.ability_revoked.connect(make_handler(state_to_display))
	ability_container.ability_ended.connect(make_handler(state_to_display))
	_set_state_value(_get_state_label(state_to_display), false)


func _set_state_value(state_name: String, state_value: bool) -> void:
	state_name_label.text = state_name
	if state_value:
		state_state_label.text = "✅"
	else:
		state_state_label.text = "❌"


func make_handler(state: int) -> Callable:
	return func handler(ability: Ability) -> void:
		var runtime_ability = ability_container.get_ability(ability)

		match state:
			States.ACTIVATED: _set_state_value(_get_state_label(state), runtime_ability.is_active()) 
			States.BLOCKED: _set_state_value(_get_state_label(state), runtime_ability.is_blocked()) 
			States.GRANTED: _set_state_value(_get_state_label(state), runtime_ability.is_granted()) 
			States.COOLING_DOWN: _set_state_value(_get_state_label(state), runtime_ability.is_cooldown_active()) 
