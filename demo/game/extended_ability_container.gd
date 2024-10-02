class_name ExtendedAbilityContainer extends AbilityContainer


func _try_activate(ability_or_ability_name: Variant) -> int:
	# override this and other abilities if you need special or custom behavior, multiplayer support and more

	var ability = get_ability(ability_or_ability_name)
	
	if ability:
		var result = ability.activate()
	
		match result:
			RuntimeAbility.ABILITY_EVENT_TYPE_ACTIVATED:
				return ABILITY_ACTIVATED
			RuntimeAbility.ABILITY_EVENT_TYPE_ERROR_ACTIVATING:
				return ABILITY_ERROR_ACTIVATING
			RuntimeAbility.ABILITY_EVENT_TYPE_REFUSED_TO_ACTIVATE:
				return ABILITY_REFUSED_TO_ACTIVATE
	
	return ABILITY_NOT_FOUND


func _try_grant(ability_or_ability_name: Variant) -> int:
	var ability = get_ability(ability_or_ability_name)

	if ability:
		var result =  ability.grant()

		match result:
			RuntimeAbility.ABILITY_EVENT_TYPE_GRANTED:
				print("Ability " + ability.get_ability().ability_name + " granted to " + get_parent().name)
				return ABILITY_GRANTED
			RuntimeAbility.ABILITY_EVENT_TYPE_ERROR_GRANTING:
				return ABILITY_ERROR_GRANTING
			RuntimeAbility.ABILITY_EVENT_TYPE_REFUSED_TO_GRANT:
				return ABILITY_REFUSED_TO_GRANT

	return ABILITY_NOT_FOUND
