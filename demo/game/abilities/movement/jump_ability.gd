# honest disclaimer: 
# 	I usually do the movement part in a node which handles the moveable body _physics_process.
# 	This is just to show and test the functionalities of the addon.

class_name JumpAbility extends Ability


const ABILITY_NAME = "JumpAbility"


@export var stamina_consumption := 2.0
@export var debug_enabled := false


func _init(_ability_name = ABILITY_NAME) -> void:
	ability_name = _ability_name


func _can_be_activated(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	var owner = ability_container.get_parent() as Player

	if debug_enabled: print("Asking my friends if I can jump, wait a sec")

	if owner == null:
		if debug_enabled: print("Wrong owner I think.")
		return false

	if owner.is_dead():
		if debug_enabled: print("Sorry, I am dead atm, I don't want to jump anymore.")
		return false

	if !ability_container.get_parent().is_on_floor():
		if debug_enabled: print("I am already flying Jack.")
		return false
		
	var stamina = owner.attribute_container.get_attribute_value_by_name(StaminaAttribute.ATTRIBUTE_NAME)
	return stamina >= stamina_consumption


func _can_be_granted(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> bool:
	if ability_container.get_parent() is not Player:
		# yes, zombies cannot jump. Checkmate.
		if debug_enabled: print("Nice try zombie, you cannot jump.")
		return false

	return ability_container.get_parent() is Player 


func _on_activate(ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	var player = ability_container.get_parent() as Player
	var jump_height = player.attribute_container.get_attribute_buffed_value_by_name(JumpHeightAttribute.ATTRIBUTE_NAME)
	var stamina_debuff = AttributeBuff.new()
	
	stamina_debuff.attribute_name = StaminaAttribute.ATTRIBUTE_NAME
	stamina_debuff.operation = AttributeOperation.subtract(stamina_consumption)
	
	player.attribute_container.apply_buff(stamina_debuff)

	player.velocity.y = jump_height

	if debug_enabled: print("Weeeeeee")


func _should_be_ended(ability_container: AbilityContainer) -> bool:
	var player = ability_container.get_parent() as Player

	if debug_enabled: print("Lemme check if I landed.")

	var landed = player and player.is_on_floor()
	
	if landed:
		if debug_enabled: print("Yup, I landed")
		
	return landed


func _on_end(_ability_container: AbilityContainer, _runtime_ability: RuntimeAbility) -> void:
	if debug_enabled: print("Boing, landed.")
