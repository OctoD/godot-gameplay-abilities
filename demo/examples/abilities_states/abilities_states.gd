extends VBoxContainer


@onready var ability_container: AbilityContainer = %AbilityContainer
@onready var ability_cooldown_spin_box: SpinBox = $HBoxContainer/AbilityCooldownSpinBox
@onready var activate_button: Button = %ActivateButton
@onready var block_button: Button = %BlockButton
@onready var grant_button: Button = %GrantButton


@export var ability_cooldown 	:= 0.0:
	get:
		return ability_cooldown_spin_box.value
	set(value):
		if value != null:
			ability_cooldown_spin_box.value = value


var should_be_blocked := false


func _ready() -> void:
	ability_cooldown_spin_box.value = ability_cooldown
	
	activate_button.pressed.connect(func ():
		var ability: RuntimeAbility = get_ability()
		
		if ability.is_active():
			if ability_container.try_end(ability.get_ability()) == RuntimeAbility.ABILITY_EVENT_TYPE_ENDED:
				activate_button.text = "Activate ability"
		else:
			if ability_container.try_activate(ability.get_ability()) == RuntimeAbility.ABILITY_EVENT_TYPE_ACTIVATED:
				activate_button.text = "End ability"
	)
	
	block_button.pressed.connect(func ():
		var ability: RuntimeAbility = get_ability()
		
		should_be_blocked = !should_be_blocked
		
		if ability.is_blocked():
			should_be_blocked = false
			block_button.text = "Block ability"
		else:
			should_be_blocked = true
			if ability_container.try_block(ability.get_ability()) == RuntimeAbility.ABILITY_EVENT_TYPE_BLOCKED:
				block_button.text = "Unblock ability"
	)

	grant_button.pressed.connect(func ():
		var ability: RuntimeAbility = get_ability()

		if ability.is_granted():
			if ability_container.try_revoke(ability.get_ability()) == RuntimeAbility.ABILITY_EVENT_TYPE_REVOKED:
				grant_button.text = "Grant ability"
		else:
			if ability_container.try_grant(ability.get_ability()) == RuntimeAbility.ABILITY_EVENT_TYPE_GRANTED:
				grant_button.text = "Revoke ability"
	)


func get_ability() -> RuntimeAbility:
	return ability_container.get_ability(AbilitiesStatesTestAbility.ABILITY_NAME)
