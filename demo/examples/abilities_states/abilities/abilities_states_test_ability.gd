class_name AbilitiesStatesTestAbility
extends Ability

const ABILITY_NAME := "AbilitiesStatesTestAbility"


func _init(_ability_name = ABILITY_NAME) -> void:
	ability_name = _ability_name


func _get_cooldown(ability_container: AbilityContainer) -> float:
	return ability_container.get_parent().ability_cooldown_spin_box.value


func _get_duration(ability_container: AbilityContainer) -> float:
	return ability_container.get_parent().duration_spin_box.value
