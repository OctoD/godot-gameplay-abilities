class_name DashAbility extends AbilityWithIcon

const ABILITY_NAME = "DashAbility"
const BASE_COOLDOWN = 10.0
const DASH_ABILITY_ICON = preload("res://game/abilities/movement/dash_ability/dash_ability_icon.png")
const DASH_ABILITY_EFFECT = preload("res://game/abilities/movement/dash_ability/dash_ability_effect.tscn")

@export var stamina_consumption := 20.0
@export var cooldown 	:= 10.0
@export var dash_speed := 20.0
@export var dash_jump  := 4.0

func _init(_ability_name = ABILITY_NAME, _icon = DASH_ABILITY_ICON):
	ability_name = _ability_name
	icon = _icon


func _get_cooldown(_ability_container: AbilityContainer) -> float:
	return cooldown


func _can_be_activated(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	var character = ability_container.get_parent() as CharacterBase
	
	if character:
		var stamina = character.attribute_container.get_attribute_value_by_name(StaminaAttribute.ATTRIBUTE_NAME)
		return stamina >= stamina_consumption && ability_container.get_parent().is_on_floor() && character.velocity != Vector3.ZERO

	return ability_container.get_parent().is_on_floor()


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var character = ability_container.get_parent() as CharacterBase

	if character:
		var buff = AttributeBuff.new()
		var debuff = AttributeBuff.new()
		
		buff.attribute_name = MovementSpeedAttribute.ATTRIBUTE_NAME
		buff.operation = AttributeOperation.add(dash_speed)
		buff.duration = 3.0
		buff.transient = true
		debuff.attribute_name = StaminaAttribute.ATTRIBUTE_NAME
		debuff.operation = AttributeOperation.subtract(stamina_consumption)

		var graphical_effect = DASH_ABILITY_EFFECT.instantiate()
		
		character.attribute_container.apply_buff(buff)
		character.attribute_container.apply_buff(debuff)

		character.velocity.y = dash_jump

		character.add_child(graphical_effect)
