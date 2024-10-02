class_name FireProjectileAbility extends AbilityWithIcon


const ABILITY_NAME = "FireBolt"
const FIRE_PROJECTILE_ABILITY_BULLET = preload("res://game/abilities/attack/fire_projectile_ability/fire_projectile_ability_bullet.tscn")
const ICON = preload("res://game/abilities/attack/fire_projectile_ability/icon.png")


func _init(_ability_name = ABILITY_NAME, _icon = ICON) -> void:
	ability_name = _ability_name
	icon = _icon


func _get_cooldown(ability_container: AbilityContainer) -> float:
	var character = ability_container.get_parent() as CharacterBase

	if character == null:
		return 0.0

	return character.attribute_container.get_attribute_buffed_value_by_name(FireRateAttribute.ATTRIBUTE_NAME)


func _can_be_granted(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	var character = ability_container.get_parent() as CharacterBase
	
	if character == null:
		return false
	
	return !character.is_dead()


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	_spawn(ability_container.get_parent())


func _spawn(character: CharacterBase) -> void:
	if character == null:
		return
	
	var instance = FIRE_PROJECTILE_ABILITY_BULLET.instantiate()

	GameState.spawn_projectile(instance)

	if instance is Node3D:
		instance.transform = character.muzzle.global_transform
	else:
		# destroy the bullet, since we do not handle other ones
		instance.queue_free()
