class_name LookAbility extends Ability


const MOUSE_SENSITIVITY := 50.0


func _init(_ability_name = "LookAbility") -> void:
	ability_name = _ability_name


func _can_be_granted(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	return ability_container.get_parent() is CharacterBody3D


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var character = ability_container.get_parent() as CharacterBase

	if character is Player:
		var camera_neck = character.camera_neck
		var last_velocity = Input.get_last_mouse_velocity().normalized()
		var delta = character.get_physics_process_delta_time()
		var x = last_velocity.y * MOUSE_SENSITIVITY * delta
		var y = last_velocity.x * MOUSE_SENSITIVITY * delta
		var max_x = 0.9

		character.rotate_y(deg_to_rad(-y))
		camera_neck.rotate_x(deg_to_rad(-x))
		camera_neck.rotation.x = clamp(camera_neck.rotation.x, -max_x, max_x)


func _should_be_activated(ability_container: AbilityContainer) -> bool:
	var maybeplayer = ability_container.get_parent() as Player

	if maybeplayer && !maybeplayer.is_dead():
		return Input.get_last_mouse_velocity().normalized() > Vector2.ZERO

	return false


func _should_be_blocked(ability_container: AbilityContainer) -> bool:
	var maybeplayer = ability_container.get_parent() as Player

	if maybeplayer && maybeplayer.is_dead():
		return true

	return false
