class_name AbilitiesAsStatesShoot
extends Ability

const ABILITY_NAME := "AbilitiesAsStatesShoot"
const CANNOBALL_PROJECTILE = preload("res://examples/naval_battles/cannonball_projectile/cannoball_projectile.tscn")


enum CannonSide {
	LEFT 	= 0,
	RIGHT	= 1,
}

@export_enum("Left", "Right") var cannon_side: int = CannonSide.LEFT


func _init(_ability_name := ABILITY_NAME) -> void:
	ability_name = _ability_name
	
	
func _can_be_activated(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	var maybe_a_ship := ability_container.get_parent() as ShipBase
	
	return maybe_a_ship != null && maybe_a_ship.cannoballs > 0
	
	
func _get_cooldown(_ability_container: AbilityContainer) -> float:
	return 1.5


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var ship = ability_container.get_parent() as ShipBase
	var cannonball = CANNOBALL_PROJECTILE.instantiate()
	var muzzle: Node3D

	ship.get_parent_node_3d().add_child(cannonball)

	if cannon_side == CannonSide.LEFT:
		# yup, it's odd but I do not want to think 
		muzzle = ship.ship.cannon_right.muzzle
	else:
		muzzle = ship.ship.cannon_left.muzzle

	cannonball.shooted_by = ability_container.get_parent()
	cannonball.transform = muzzle.global_transform
	cannonball.velocity = -cannonball.transform.basis.z * cannonball.speed
	
	ship.cannoballs -= 1
