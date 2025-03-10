class_name AbilitiesAsStatesShoot
extends Ability

const ABILITY_NAME := "AbilitiesAsStatesShoot"
const CANNOBALL_PROJECTILE = preload("res://examples/abilities_as_states/cannonball_projectile/cannoball_projectile.tscn")


enum CannonSide {
	LEFT 	= 0,
	RIGHT	= 1,
}

@export_enum("Left", "Right") var cannon_side: int = CannonSide.LEFT


func _init(_ability_name := ABILITY_NAME) -> void:
	ability_name = _ability_name
	
	
func _can_be_activated(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> bool:
	var maybe_a_ship := ability_container.get_parent() as ShipBase
	
	if maybe_a_ship == null:
		return false
		
	return maybe_a_ship.cannoballs > 0


func _on_activate(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
	var ship = ability_container.get_parent() as ShipBase
	var cannonball = CANNOBALL_PROJECTILE.instantiate()
	var muzzle: Node3D

	ship.get_parent_node_3d().add_child(cannonball)
	
	if cannon_side == CannonSide.LEFT:
		muzzle = ship.ship.cannon_left
	else:
		muzzle = ship.ship.cannon_right

	cannonball.global_position = muzzle.global_position
	cannonball.shoot_at(ship.rotation)
	
	ship.cannoballs -= 1
