class_name Player extends CharacterBase


@onready var camera_3d: Camera3D = %Camera3D
@onready var camera_neck: Node3D = %CameraNeck
@onready var muzzle: Node3D = %Muzzle
@onready var hud = %Hud


func _ready() -> void:
	super._ready()
	hud.setup_abilities(ability_container)
	hud.setup_attributes(attribute_container)


func _handle_physics_process(_delta: float) -> void:
	ability_container.try_activate("MoveAbility") 
	ability_container.try_activate("LookAbility") 

	if Input.is_action_just_pressed("action_jump"):
		ability_container.try_activate(JumpAbility.ABILITY_NAME)

	if Input.is_action_just_pressed("ability_000"):
		ability_container.try_activate(FireProjectileAbility.ABILITY_NAME)

	if Input.is_action_just_pressed("ability_001"):
		ability_container.try_activate(DashAbility.ABILITY_NAME)
		
	if Input.is_action_just_pressed("ability_002"):
		ability_container.try_activate(EnergyShieldAbility.ABILITY_NAME)
		
