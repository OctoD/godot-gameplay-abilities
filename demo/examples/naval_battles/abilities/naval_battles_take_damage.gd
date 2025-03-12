class_name NavalBattlesTakeDamage
extends Ability


const ABILITY_NAME := "NavalBattlesTakeDamage"

func _init(_ability_name = ABILITY_NAME):
	ability_name = _ability_name


func _can_be_activated(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	var maybe_ship = ability_container.get_parent() as ShipBase
	
	return maybe_ship != null && !maybe_ship.is_sunk
	
	
func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var ship = ability_container.get_parent() as ShipBase
	
	ship.health -= 12.5
