class_name Enemy extends CharacterBase


var aggro_direction: Vector3 = Vector3.ZERO


func _ready() -> void:
	super._ready()
	
	attribute_container.attribute_changed.connect(func (_a, _b, _c): 
		if is_dead():
			print(_a, _b, _c)
			queue_free()
	)


func _handle_physics_process(_delta: float) -> void:
	if ability_container.has_ability(MoveAbility.ABILITY_NAME):
		ability_container.try_activate(MoveAbility.ABILITY_NAME)
