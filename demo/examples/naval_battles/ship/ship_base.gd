class_name ShipBase
extends RigidBody3D


signal damage_taken()


@export_category("Maneuvering")
@export_group("Base values", "base_")
@export var base_acceleration   := 0.015
@export_group("Max values", "max_")
@export var max_acceleration 	:= 10.0
@export var max_steering		:= 25.0

@export_category("Ships state")
@export var max_anchor_down		:= 1.0
@export var max_health			:= 100.0

@export_category("Ship setup")
@export var ability_container: AbilityContainer = null
@export var ship: Ship							= null


var acceleration	:= 0.0
var anchor_down		:= 0.0
var cannoballs		:= 40
var __health		:= 100.0
var health: float 	= __health:
	get:
		return __health
	set(value):
		if value < __health:
			damage_taken.emit()
		__health = clampf(value, 0.0, max_health)
var is_sunk: bool	:
	get:
		return health <= 0.0
var movement_speed  := 0.0
var wheel_steer		:= 0.0
var wood_planks		:= 10


func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	if is_sunk:
		state.linear_velocity = Vector3.ZERO
		state.apply_force(Vector3.DOWN)
	else:
		state.linear_velocity = global_transform.basis * Vector3(0, 0, acceleration) 
		state.apply_force(Vector3(0.0, deg_to_rad(wheel_steer) * 0.5, 0.0))


func accelerate(p_accelerate: float) -> void:
	acceleration = clampf(acceleration + (p_accelerate * base_acceleration), -0.2, max_acceleration)


func sail(delta: float) -> void:
	rotation.y += deg_to_rad(wheel_steer * delta) * 0.5


func steer(angle: float) -> void:
	wheel_steer += angle
	wheel_steer = clampf(wheel_steer, -max_steering, max_steering)
