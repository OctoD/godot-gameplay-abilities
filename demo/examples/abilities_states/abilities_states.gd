extends VBoxContainer


@onready var ability_container: AbilityContainer = %AbilityContainer
@onready var ability_cooldown_spin_box: SpinBox = $HBoxContainer/AbilityCooldownSpinBox
@onready var activate_button: Button = %ActivateButton
@onready var grant_button: Button = %GrantButton


@export var ability_cooldown 	:= 0.0:
	get:
		return ability_cooldown_spin_box.value
	set(value):
		if value != null:
			ability_cooldown_spin_box.value = value


func _ready() -> void:
	ability_cooldown_spin_box.value = ability_cooldown
	
	activate_button.pressed.connect(func ():
		var ability: RuntimeAbility = get_ability()
		
		if ability.is_active():
			ability_container.try_end(ability.get_ability())
		else:
			ability_container.try_activate(ability.get_ability())
	)

	grant_button.pressed.connect(func ():
		var ability: RuntimeAbility = get_ability()

		if ability.is_granted():
			ability_container.try_revoke(ability.get_ability())
		else:
			ability_container.try_grant(ability.get_ability())
	)


func get_ability() -> RuntimeAbility:
	return ability_container.get_ability(AbilitiesStatesTestAbility.ABILITY_NAME)
