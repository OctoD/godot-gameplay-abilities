class_name NavalBattlesRepairAbility
extends Ability

const ABILITY_NAME := "NavalBattlesRepairAbility"

func _init(_ability_name = ABILITY_NAME) -> void:
	ability_name = _ability_name


func _can_be_activated(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	var maybe_ship = ability_container.get_parent() as ShipBase

	return maybe_ship != null && maybe_ship.wood_planks > 0


func _get_cooldown(_ability_container: AbilityContainer) -> float:
	return 1.0


func _get_duration(_ability_container: AbilityContainer) -> float:
	return 2.0


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var ship = ability_container.get_parent() as ShipBase

	ship.wood_planks -= 1	
	ship.health += 15.0
