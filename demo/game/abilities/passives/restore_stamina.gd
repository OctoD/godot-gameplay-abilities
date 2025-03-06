class_name RestoreStaminaAbility extends PassiveAbility

const ABILITY_NAME = "RestoreStaminaAbility"

func _init(_ability_name = ABILITY_NAME):
	ability_name = _ability_name


func _get_cooldown(_ability_container: AbilityContainer) -> float:
	return 0.2


func _can_be_activated(ability_container: AbilityContainer, _ability: RuntimeAbility) -> bool:
	var character = ability_container.get_parent() as CharacterBase

	if character:
		return !character.is_dead()
	return false
	
	
func _can_be_ended(_ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	return false


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var character = ability_container.get_parent() as CharacterBase
	var buff = AttributeBuff.new()

	buff.attribute_name = StaminaAttribute.ATTRIBUTE_NAME
	buff.buff_name = "IncrementStamina"
	buff.operation = AttributeOperation.add(1.0)

	character.attribute_container.apply_buff(buff)


func _should_be_activated(ability_container: AbilityContainer) -> bool:
	var character = ability_container.get_parent() as CharacterBase

	if character:
		var stamina = character.attribute_container.get_attribute_by_name(StaminaAttribute.ATTRIBUTE_NAME)
		return stamina.get_constrained_value()

	return false
	
	
func _should_be_ended(ability_container: AbilityContainer) -> bool:
	var character = ability_container.get_parent() as CharacterBase
	if character:
		return character.is_dead()
	return true
