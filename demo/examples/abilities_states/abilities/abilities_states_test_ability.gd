class_name AbilitiesStatesTestAbility
extends Ability

const ABILITY_NAME := "AbilitiesStatesTestAbility"

func _init(_ability_name = ABILITY_NAME) -> void:
	ability_name = _ability_name


func _get_cooldown(ability_container: AbilityContainer) -> float:
	return ability_container.get_parent().ability_cooldown


func _should_be_blocked(ability_container: AbilityContainer) -> bool:
	return ability_container.get_parent().should_be_blocked
