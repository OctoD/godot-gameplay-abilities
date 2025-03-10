class_name AbilitiesAsStatesPlayer
extends ShipBase


@onready var ability_container: AbilityContainer = $AbilityContainer


func _physics_process(delta: float) -> void:
	accelerate(Input.get_axis("move_backward", "move_forward"))
	steer(Input.get_axis("move_right", "move_left"))
	sail(delta)

	if Input.is_action_just_pressed("ability_000"):
		ability_container.try_activate(AbilitiesAsStatesShootLeft.new())
	elif  Input.is_action_just_pressed("ability_001"):
		ability_container.try_activate(AbilitiesAsStatesShootRight.new())
