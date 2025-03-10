class_name AbilitiesAsStatesMoveAbility
extends Ability

const ABILITY_NAME := "AbilitiesAsStatesMoveAbility"

func _init(_ability_name := ABILITY_NAME) -> void:
	ability_name = _ability_name


func _can_be_activated(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
	var player = ability_container.get_parent() as AbilitiesAsStatesPlayer
	
	if player == null:
		return false
		
	return absf(player.steering) < absf(player.max_steering)
