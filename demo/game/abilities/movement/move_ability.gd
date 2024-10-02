class_name MoveAbility extends Ability


const ABILITY_NAME = "MoveAbility"


func _init(_ability_name = ABILITY_NAME) -> void:
	ability_name = _ability_name


func _can_be_activated(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	return ability_container.get_parent() is CharacterBase and !ability_container.get_parent().is_dead() and ability_container.get_parent().is_on_floor()


func _can_be_granted(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	return ability_container.get_parent() is CharacterBase # we want to move only characters.


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var character = ability_container.get_parent() as CharacterBase
	var movement_speed = character.attribute_container.get_attribute_buffed_value_by_name(MovementSpeedAttribute.ATTRIBUTE_NAME)

	if character is Player:
		var input_direction = Input.get_vector("move_left", "move_right", "move_forward", "move_backward")
		var direction: Vector3 = (character.transform.basis * Vector3(input_direction.x, 0, input_direction.y)).normalized()

		if direction:
			character.velocity.x = -direction.x * movement_speed
			character.velocity.z = -direction.z * movement_speed
		else:
			character.velocity.x = -move_toward(character.velocity.x, 0, movement_speed)
			character.velocity.z = -move_toward(character.velocity.z, 0, movement_speed)
	elif character is Enemy:
		character.velocity = -character.transform.basis.z.normalized() * movement_speed


func _should_be_activated(_ability_container: AbilityContainer) -> bool:
	var character: CharacterBase = _ability_container.get_parent()
	
	if character == null:
		return false
	
	return !character.is_dead()
