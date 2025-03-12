class_name NavalBattlesSinkAbility
extends Ability

const ABILITY_NAME := "NavalBattlesSinkAbility"

func _init(_ability_name = ABILITY_NAME) -> void:
	ability_name = _ability_name



func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var ship = ability_container.get_parent() as ShipBase
	ship.collision_layer = 0
	ship.collision_mask  = 0


func _should_be_activated(ability_container: AbilityContainer) -> bool:
	var maybe_ship = ability_container.get_parent() as ShipBase
	return maybe_ship != null && maybe_ship.is_sunk
