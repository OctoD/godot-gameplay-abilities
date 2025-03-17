extends VBoxContainer


@onready var ability_container: AbilityContainer = %AbilityContainer
@onready var activate_button: Button = %ActivateButton
@onready var block_button: Button = %BlockButton
@onready var grant_button: Button = %GrantButton
@onready var ability_cooldown_spin_box: SpinBox = %AbilityCooldownSpinBox
@onready var duration_spin_box: SpinBox = %DurationSpinBox

@onready var activated_ability_state_h_box_container: HBoxContainer = %ActivatedAbilityStateHBoxContainer
@onready var blocked_ability_state_h_box_container: HBoxContainer = %BlockedAbilityStateHBoxContainer
@onready var granted_ability_state_h_box_container: HBoxContainer = %GrantedAbilityStateHBoxContainer
@onready var cooldown_ability_state_h_box_container: HBoxContainer = %CooldownAbilityStateHBoxContainer
@onready var end_button: Button = %EndButton
@onready var unblock_button: Button = %UnblockButton
@onready var revoke_button: Button = %RevokeButton


func _ready() -> void:
	ability_container.ability_activated.connect(handle_state_changed("ability_activated"))
	ability_container.ability_blocked.connect(handle_state_changed("ability_blocked"))
	ability_container.ability_ended.connect(handle_state_changed("ability_ended"))
	ability_container.ability_granted.connect(handle_state_changed("ability_granted"))
	ability_container.ability_revoked.connect(handle_state_changed("ability_revoked"))
	ability_container.ability_unblocked.connect(handle_state_changed("ability_unblocked"))
	ability_container.cooldown_end.connect(handle_state_changed("cooldown_end"))
	ability_container.cooldown_start.connect(handle_state_changed("cooldown_start"))
	
	ability_cooldown_spin_box.value = 0.0
	
	handle_state_changed().call(get_ability())
	
	activate_button.pressed.connect(ability_container.try_activate.bind(get_ability()))
	block_button.pressed.connect(ability_container.try_block.bind(get_ability()))
	grant_button.pressed.connect(ability_container.try_grant.bind(get_ability()))
	end_button.pressed.connect(ability_container.try_end.bind(get_ability()))
	unblock_button.pressed.connect(ability_container.try_unblock.bind(get_ability()))
	revoke_button.pressed.connect(ability_container.try_revoke.bind(get_ability()))


func get_ability() -> Ability:
	return ability_container.get_runtime_ability(AbilitiesStatesTestAbility.ABILITY_NAME).get_ability()
	
	
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
			
		activated_ability_state_h_box_container.value 	= ability_container.is_ability_active(ability)
		blocked_ability_state_h_box_container.value 	= ability_container.is_ability_blocked(ability)
		granted_ability_state_h_box_container.value 	= ability_container.is_ability_granted(ability)
		cooldown_ability_state_h_box_container.value 	= ability_container.is_ability_cooldown_active(ability)
