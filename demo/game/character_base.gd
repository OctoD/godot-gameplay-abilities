class_name CharacterBase extends CharacterBody3D


signal dead()


@export var attribute_set: AttributeSet
@export var abilities: Array[Ability]
@export var debug_ability_events := false


var ability_container: ExtendedAbilityContainer
var attribute_container: AttributeContainer
var look_direction: Vector3 = Vector3.ZERO


func _bind_attribute_container_signals() -> void:
	attribute_container.attribute_changed.connect(func (_attribute, _old_value, _new_value):
		if is_dead():
			dead.emit()
	)


func _physics_process(delta: float) -> void:
	if not is_on_floor():
		velocity += get_gravity() * delta
	if has_method("_handle_physics_process"):
		call("_handle_physics_process", delta)

	move_and_slide()


func _ready() -> void:
	ability_container = ExtendedAbilityContainer.new()
	attribute_container = AttributeContainer.new()

	attribute_container.attribute_set = attribute_set

	ability_container.ability_blocked.connect(func (ability):
		if debug_ability_events:
			print(ability.ability_name + " blocked")
	)

	ability_container.ability_ended.connect(func (ability):
		if debug_ability_events:
			print(ability.ability_name + " ended")
	)
	
	ability_container.cooldown_end.connect(func (ability):
		if debug_ability_events:
			print(ability.ability_name + " cooldown ended for " + name)
	)
	
	ability_container.cooldown_start.connect(func (ability):
		if debug_ability_events:
			print(ability.ability_name + " cooldown started for " + name)
	)
	
	add_child(ability_container)
	add_child(attribute_container)

	for ability in abilities:
		ability_container.add_ability(ability)

	attribute_container.setup()

	_bind_attribute_container_signals()


func is_dead() -> bool:
	var health = attribute_container.get_attribute_value_by_name(HealthAttribute.ATTRIBUTE_NAME)
	return health <= 0.0
