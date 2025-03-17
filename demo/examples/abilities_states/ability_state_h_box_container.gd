@tool
extends HBoxContainer


enum States {
	ACTIVATED,
	BLOCKED,
	GRANTED,
	COOLING_DOWN
}


@export_enum("Activated", "Blocked", "Granted", "Cooling down") var state_to_display: int 	= States.ACTIVATED
@export var value																			:= false:
	get:
		return state_state_label.text == "✅"
	set(value):
		if value:
			state_state_label.text = "✅"
		else:
			state_state_label.text = "❌"


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
	state_name_label.text = _get_state_label(state_to_display)
