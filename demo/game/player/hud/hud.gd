extends MarginContainer


@onready var health: ProgressBar = %Health
@onready var stamina_bar: ProgressBar = %JumpBar
@onready var ability_000 = %Ability000
@onready var ability_001 = %Ability001
@onready var ability_002 = %Ability002
@onready var ability_003 = %Ability003
@onready var ability_bar: HBoxContainer = %AbilityBar


@onready var abilities_slots: Array[AbilityControl] = []


func _ready() -> void:
	for child in ability_bar.get_children():
		if child is AbilityControl:
			abilities_slots.append(child)
			
			
func setup_abilities(ability_container: AbilityContainer) -> void:
	for runtime_ability in ability_container.get_abilities():
		for x in abilities_slots:
			if x.has_ability(runtime_ability):
				x.bind_runtime_ability(runtime_ability)


func setup_attributes(attribute_container: AttributeContainer) -> void:
	attribute_container.attribute_changed.connect(func (attribute: RuntimeAttribute, _old_value: float, new_value: float) -> void:
		if attribute.attribute.attribute_name == HealthAttribute.ATTRIBUTE_NAME:
			health.value = new_value
		elif attribute.attribute.attribute_name == StaminaAttribute.ATTRIBUTE_NAME:
			stamina_bar.value = new_value
	)
	
	var health_attribute = attribute_container.get_attribute_by_name(HealthAttribute.ATTRIBUTE_NAME)
	var max_health_attribute = attribute_container.get_attribute_by_name(MaxHealthAttribute.ATTRIBUTE_NAME)
	var stamina_attribute = attribute_container.get_attribute_by_name(StaminaAttribute.ATTRIBUTE_NAME)
	
	if health_attribute:
		health.min_value = 0.0
		health.value = health_attribute.get_buffed_value()
	if max_health_attribute:
		health.max_value = max_health_attribute.get_buffed_value()
	if stamina_attribute:
		stamina_bar.min_value = 0.0
		stamina_bar.max_value = 100.0
		stamina_bar.value = stamina_attribute.get_buffed_value()
