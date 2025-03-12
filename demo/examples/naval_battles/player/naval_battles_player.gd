class_name NavalBattlesPlayer
extends ShipBase


func _ready() -> void:
	ability_container.ability_added.connect(print)


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("ability_000"):
		ability_container.try_activate(AbilitiesAsStatesShootLeft.new())
	elif event.is_action_pressed("ability_001"):
		ability_container.try_activate(AbilitiesAsStatesShootRight.new())
	elif event.is_action_pressed("ability_002"):
		ability_container.try_activate(NavalBattlesRepairAbility.new())


func _physics_process(delta: float) -> void:
	accelerate(Input.get_axis("move_backward", "move_forward"))
	steer(Input.get_axis("move_right", "move_left"))
	sail(delta)
