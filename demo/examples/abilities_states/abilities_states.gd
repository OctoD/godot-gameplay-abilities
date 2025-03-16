extends VBoxContainer


@onready var ability_container: AbilityContainer = %AbilityContainer
@onready var ability_cooldown_spin_box: SpinBox = $HBoxContainer/AbilityCooldownSpinBox
@onready var activate_button: Button = %ActivateButton
@onready var block_button: Button = %BlockButton
@onready var grant_button: Button = %GrantButton

@onready var activated_ability_state_h_box_container: HBoxContainer = %ActivatedAbilityStateHBoxContainer
@onready var blocked_ability_state_h_box_container: HBoxContainer = %BlockedAbilityStateHBoxContainer
@onready var granted_ability_state_h_box_container: HBoxContainer = %GrantedAbilityStateHBoxContainer
@onready var cooldown_ability_state_h_box_container: HBoxContainer = %CooldownAbilityStateHBoxContainer


@export var ability_cooldown 	:= 0.0:
	get:
		return ability_cooldown_spin_box.value
	set(value):
		if value != null:
			ability_cooldown_spin_box.value = value


func _ready() -> void:
	ability_container.ability_activated.connect(handle_state_changed("ability_activated"))
	ability_container.ability_blocked.connect(handle_state_changed("ability_blocked"))
	ability_container.ability_ended.connect(handle_state_changed("ability_ended"))
	ability_container.ability_granted.connect(handle_state_changed("ability_granted"))
	ability_container.ability_revoked.connect(handle_state_changed("ability_revoked"))
	ability_container.ability_unblocked.connect(handle_state_changed("ability_unblocked"))
	ability_container.cooldown_end.connect(handle_state_changed("cooldown_end"))
	ability_container.cooldown_start.connect(handle_state_changed("cooldown_start"))
	
	ability_cooldown_spin_box.value = ability_cooldown
	
	handle_state_changed().call(get_ability().get_ability())
	
	activate_button.pressed.connect(func ():
		var ability: RuntimeAbility = get_ability()
		
		if ability.is_active():
			ability_container.try_end(ability.get_ability())
		else:
			ability_container.try_activate(ability.get_ability())
	)
	
	block_button.pressed.connect(func ():
		var ability: RuntimeAbility = get_ability()

		if ability.is_blocked():
			ability_container.try_unblock(ability.get_ability())
		else:
			ability_container.try_block(ability.get_ability())
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
	
	
func handle_state_changed(signal_name: String = "") -> Callable:
	return func (ability: Ability) -> void:
		if (signal_name != ""):
			print("""
got signal {0}

ability_container.is_ability_active(ability)			:{1}
ability_container.is_ability_blocked(ability)			:{2}
ability_container.is_ability_granted(ability)			:{3}
ability_container.is_ability_cooldown_active(ability)	:{4}
			""".format({
				0: signal_name,
				1: ability_container.is_ability_active(ability),
				2: ability_container.is_ability_blocked(ability),
				3: ability_container.is_ability_granted(ability),
				4: ability_container.is_ability_cooldown_active(ability),
			})
			)
		
		if ability_container.is_ability_active(ability):
			activate_button.text = "End ability"
		else:
			activate_button.text = "Activate ability"
			
		if ability_container.is_ability_blocked(ability):
			block_button.text = "Unblock ability"
		else:
			block_button.text = "Block ability"
			
		if ability_container.is_ability_granted(ability):
			grant_button.text = "Revoke ability"
		else:
			grant_button.text = "Grant ability"
			
		activated_ability_state_h_box_container.value 	= ability_container.is_ability_active(ability)
		blocked_ability_state_h_box_container.value 	= ability_container.is_ability_blocked(ability)
		granted_ability_state_h_box_container.value 	= ability_container.is_ability_granted(ability)
		cooldown_ability_state_h_box_container.value 	= ability_container.is_ability_cooldown_active(ability)
