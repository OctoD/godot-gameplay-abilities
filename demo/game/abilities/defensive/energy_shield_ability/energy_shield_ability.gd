class_name EnergyShieldAbility extends AbilityWithIcon

const ABILITY_NAME := "EnergyShieldAbility"
const ICON = preload("res://game/abilities/defensive/energy_shield_ability/energy_shield_ability.png")
const BASE_COOLDOWN = 30.0
const BASE_DURATION = 5.0
const THE_ENERGY_SHIELD = preload("res://game/abilities/defensive/energy_shield_ability/the_energy_shield.tscn")

func _init(_ability_name = ABILITY_NAME, _icon = ICON):
	ability_name = _ability_name
	icon = _icon
	

func _can_be_activated(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	var p = ability_container.get_parent()

	if p is CharacterBase:
		return !p.is_dead()
		
	return false


func _get_cooldown(ability_container: AbilityContainer) -> float:
	var character_base = ability_container.get_parent() as CharacterBase
	
	if character_base:
		var intelligence = character_base.attribute_container.get_attribute_buffed_value_by_name(IntelligenceAttribute.ATTRIBUTE_NAME)
		return clampf(BASE_COOLDOWN - intelligence, 0.0, BASE_COOLDOWN)

	return BASE_COOLDOWN


func _get_duration(ability_container: AbilityContainer) -> float:
	var character_base = ability_container.get_parent() as CharacterBase
	
	if character_base:
		var intelligence = character_base.attribute_container.get_attribute_buffed_value_by_name(IntelligenceAttribute.ATTRIBUTE_NAME)
		return BASE_DURATION + intelligence

	return BASE_DURATION


func _on_activate(ability_container: AbilityContainer, runtime_ability: RuntimeAbility) -> void:
	var c = ability_container.get_parent() as CharacterBase
	var i = THE_ENERGY_SHIELD.instantiate()

	c.add_child(i)

	i.animation_player.play("activated")
	
	runtime_ability.ended.connect(func ():
		i.animation_player.play("deactivated")	
	)
